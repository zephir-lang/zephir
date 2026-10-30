<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Operators\Bitwise;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;
use Zephir\Types\Types;

use function in_array;

/**
 * Generates PHP's `&`, `|` and `^`, and through ShiftBaseOperator `<<` and `>>`.
 *
 * PHP reads both operands of a bitwise operator as integers
 * (zendi_try_get_long() in Zend/zend_operators.c), so two native operands stay
 * a C integer expression: a bool is 0 or 1, and a double is converted the way
 * the engine converts it, with its deprecation. Any other operand, a zval
 * local or a string or null literal, is left to PHP's own operator with the
 * native side boxed: it throws the TypeError, warns for a leading-numeric
 * string, works byte by byte on two strings and lets an overloaded object
 * such as GMP answer.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2753
 */
class BitwiseBaseOperator extends AbstractOperator
{
    protected bool $literalOnly = true;

    /**
     * Whether PHP's VM may run the operator with its operands swapped.
     */
    protected bool $commutative = true;

    /**
     * PHP's own operator, called when an operand is a zval.
     */
    protected string $zvalOperator;

    /**
     * The types a zval result can hold: an int, a string from two strings, or
     * an object from an overloaded operand such as GMP.
     */
    protected array $zvalResultTypes = [Types::T_LONG, Types::T_STRING, Types::T_OBJECT];

    /**
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws CompilerException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $this->checkLeft($expression, CompilerException::class, $expression);
        $this->checkRight($expression, CompilerException::class, $expression);

        $optimized = $this->optimizeConstantFolding($expression, $compilationContext);
        if (null !== $optimized) {
            return $optimized;
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly(true);
        $right = $rightExpr->compile($compilationContext);

        [$leftOperand, $rightOperand] = $this->classifiedOperands($left, $right, $expression, $compilationContext);

        /**
         * PHP's VM runs `&`, `|` and `^` with their operands swapped when the
         * left one has the lower operand type (SPEC(COMMUTATIVE) in
         * Zend/zend_vm_def.h), so `null | $a` is run as `$a | null`. That order
         * shows in the TypeError and in which operand is converted, and
         * diagnosed, first; the value is the same. A compound assignment such
         * as `$this->p |= $a` is a different opcode and keeps its order.
         */
        if (
            $this->commutative
            && !isset($expression['compound-assignment'])
            && self::phpOperandType($expression['left']) < self::phpOperandType($expression['right'])
        ) {
            [$leftOperand, $rightOperand] = [$rightOperand, $leftOperand];
        }

        if (self::KIND_ZVAL === $leftOperand['kind'] || self::KIND_ZVAL === $rightOperand['kind']) {
            $leftOperand  = $this->boxedOperand($leftOperand, $compilationContext);
            $rightOperand = $this->boxedOperand($rightOperand, $compilationContext);
            $compilationContext->headersManager->add('kernel/operators');

            return $this->zvalResult(
                $this->zvalOperator,
                $leftOperand,
                $rightOperand,
                $expression,
                $compilationContext,
                $this->zvalResultTypes
            );
        }

        return $this->nativeOperation(
            $this->integerOperandCode($leftOperand, $compilationContext),
            $this->integerOperandCode($rightOperand, $compilationContext),
            $expression,
            $compilationContext
        );
    }

    /**
     * The operand type PHP compiles an operand node to, by the values of
     * Zend/zend_compile.h: IS_CONST 1, IS_TMP_VAR 2, IS_VAR 4 (a call result)
     * and IS_CV 8 (a variable). Parentheses do not change it.
     */
    private static function phpOperandType(array $node): int
    {
        return match ($node['type']) {
            'int', 'uint', 'long', 'ulong', 'double', 'bool', 'null',
            'string', 'istring', 'char', 'uchar', 'constant' => 1,
            'fcall', 'mcall', 'scall', 'new'                  => 4,
            'variable'                                        => 8,
            'list'                                            => self::phpOperandType($node['left']),
            default                                           => 2,
        };
    }

    /**
     * The operator applied to two operands already converted to C integers.
     * An operand is the C code, or the value of a folded literal.
     */
    protected function nativeOperation(
        int|string $left,
        int|string $right,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        return new CompiledExpression('int', '(' . $left . ' ' . $this->operator . ' ' . $right . ')', $expression);
    }

    /**
     * Folds two number literals, as PHP's compiler does.
     *
     * PHP folds neither a float that loses precision as an integer, which it
     * deprecates each time the operation runs, nor a negative shift count,
     * which throws ArithmeticError at runtime; both are left to the runtime.
     *
     * @see https://en.wikipedia.org/wiki/Constant_folding
     */
    public function optimizeConstantFolding(
        array $expression,
        CompilationContext $compilationContext
    ): ?CompiledExpression {
        if (!$compilationContext->config->get('constant-folding', 'optimizations')) {
            return null;
        }

        $literals = ['int', 'uint', 'long', 'ulong', 'double'];
        if (
            !in_array($expression['left']['type'], $literals, true)
            || !in_array($expression['right']['type'], $literals, true)
        ) {
            return null;
        }

        $left  = self::literalToNumber($expression['left']['value']);
        $right = self::literalToNumber($expression['right']['value']);
        if (!self::isLongCompatible($left) || !self::isLongCompatible($right)) {
            return null;
        }

        $left  = (int) $left;
        $right = (int) $right;
        if (('<<' === $this->operator || '>>' === $this->operator) && $right < 0) {
            return null;
        }

        $result = match ($this->operator) {
            '&'     => $left & $right,
            '|'     => $left | $right,
            '^'     => $left ^ $right,
            '<<'    => $left << $right,
            '>>'    => $left >> $right,
            default => null,
        };
        if (null === $result) {
            return null;
        }

        /**
         * `-9223372036854775808` is not a C literal but the negation of one too
         * wide for any signed type, which C compilers warn about.
         */
        return new CompiledExpression('int', PHP_INT_MIN === $result ? 'ZEND_LONG_MIN' : $result, $expression);
    }
}
