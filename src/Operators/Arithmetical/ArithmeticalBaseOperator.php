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

namespace Zephir\Operators\Arithmetical;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception as ZephirException;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;
use Zephir\Types\Types;

use function is_numeric;

/**
 * This is the base operator for commutative, associative and distributive
 * arithmetic operators
 */
class ArithmeticalBaseOperator extends AbstractOperator
{
    protected bool $literalOnly = true;

    /**
     * The types a zval result of `+`, `-` or `*` can hold: an int or a float,
     * or an object from an overloaded operand such as GMP.
     */
    protected array $zvalResultTypes = [Types::T_LONG, Types::T_DOUBLE, Types::T_OBJECT];

    /**
     * Compiles `+`, `-` or `*`.
     *
     * Two native numbers stay a C expression: an int for two integers, a
     * double when either is a double. A bool is the integer 0 or 1. Any other
     * operand, a zval local or a string, null or array literal, is left to
     * PHP's own operator, which coerces it or throws the TypeError, and the
     * native side is boxed for it.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws Exception
     * @throws CompilerException
     *
     * @see https://github.com/zephir-lang/zephir/issues/2676
     * @see https://github.com/zephir-lang/zephir/issues/2677
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $this->checkLeft($expression, CompilerException::class, $expression);
        $this->checkRight($expression, CompilerException::class, $expression);

        // Check for constant folding optimizations
        if ($optimized = $this->optimizeConstantFolding($expression, $compilationContext)) {
            return $optimized;
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly(true);
        $right = $rightExpr->compile($compilationContext);

        [$leftOperand, $rightOperand] = $this->classifiedOperands($left, $right, $expression, $compilationContext);

        if (self::KIND_ZVAL === $leftOperand['kind'] || self::KIND_ZVAL === $rightOperand['kind']) {
            return $this->zvalOperation($leftOperand, $rightOperand, $expression, $compilationContext);
        }

        if (self::KIND_LONG === $leftOperand['kind'] && self::KIND_LONG === $rightOperand['kind']) {
            return new CompiledExpression(
                'int',
                '(' . $leftOperand['code'] . ' ' . $this->operator . ' ' . $rightOperand['code'] . ')',
                $expression
            );
        }

        return new CompiledExpression(
            'double',
            '(' . $this->doubleCode($leftOperand) . ' ' . $this->operator . ' ' . $this->doubleCode($rightOperand) . ')',
            $expression
        );
    }

    /**
     * This tries to perform arithmetical operations.
     *
     * Probably gcc/clang will optimize them without this optimization
     *
     * @see https://en.wikipedia.org/wiki/Constant_folding
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return bool|CompiledExpression
     */
    public function optimizeConstantFolding(array $expression, CompilationContext $compilationContext)
    {
        if ('int' != $expression['left']['type'] && 'double' != $expression['left']['type']) {
            return false;
        }

        if ($compilationContext->config->get('constant-folding', 'optimizations')) {
            if ('int' == $expression['left']['type'] && 'int' == $expression['right']['type']) {
                $left  = self::literalToNumber($expression['left']['value']);
                $right = self::literalToNumber($expression['right']['value']);
                switch ($this->operator) {
                    case '+':
                        return new CompiledExpression('int', $left + $right, $expression);

                    case '-':
                        return new CompiledExpression('int', $left - $right, $expression);

                    case '*':
                        return new CompiledExpression('int', $left * $right, $expression);
                }
            }

            if (('double' == $expression['left']['type'] && 'double' == $expression['right']['type']) || ('double' == $expression['left']['type'] && 'int' == $expression['right']['type']) || ('int' == $expression['left']['type'] && 'double' == $expression['right']['type'])) {
                $left  = self::literalToNumber($expression['left']['value']);
                $right = self::literalToNumber($expression['right']['value']);
                switch ($this->operator) {
                    case '+':
                        return new CompiledExpression('double', $left + $right, $expression);

                    case '-':
                        return new CompiledExpression('double', $left - $right, $expression);

                    case '*':
                        return new CompiledExpression('double', $left * $right, $expression);
                }
            }
        }

        return false;
    }

    /**
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return array
     * @throws ReflectionException
     * @throws ZephirException
     */
    protected function preCompileChecks(
        array $expression,
        CompilationContext $compilationContext
    ): array {
        $this->checkLeft($expression);
        $this->checkRight($expression);

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $left = $leftExpr->compile($compilationContext);

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly(true);
        $right = $rightExpr->compile($compilationContext);

        $compilationContext->headersManager->add('kernel/operators');
        return [$left, $right];
    }

    /**
     * `+`, `-` or `*` through PHP's own operator, with the native side boxed.
     *
     * @throws CompilerException
     */
    private function zvalOperation(
        array $left,
        array $right,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $left  = $this->boxedOperand($left, $compilationContext);
        $right = $this->boxedOperand($right, $compilationContext);

        $compilationContext->headersManager->add('kernel/operators');

        return $this->zvalResult(
            $this->zvalOperator,
            $left,
            $right,
            $expression,
            $compilationContext,
            $this->zvalResultTypes
        );
    }

    /**
     * Reads a native operand as a C double.
     */
    private function doubleCode(array $operand): string
    {
        return self::KIND_DOUBLE === $operand['kind'] ? $operand['code'] : '(double) (' . $operand['code'] . ')';
    }

    /**
     * A native `zephir_safe_div_*`/`zephir_safe_mod_*` result. Those throw
     * DivisionByZeroError for a zero divisor, so the result goes through a
     * temporary and the throw stops the method there, as in PHP. It stays
     * inline when the divisor is a non-zero literal, or when it is returned
     * straight away and nothing runs after it anyway.
     */
    protected function nativeResult(
        string $type,
        string $helper,
        array $left,
        array $right,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $call = $helper . '(' . $left['code'] . ', ' . $right['code'] . ')';
        $isReturned = $this->expecting && 'return_value' === $this->expectingVariable?->getName();
        if ($isReturned || (is_numeric($right['code']) && 0.0 !== (float) $right['code'])) {
            return new CompiledExpression($type, $call, $expression);
        }

        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
            'int' === $type ? 'long' : 'double',
            $compilationContext
        );
        $compilationContext->codePrinter->output($tempVariable->getName() . ' = ' . $call . ';');
        $compilationContext->emitExceptionCheck();

        return new CompiledExpression($type, $tempVariable->getName(), $expression);
    }
}
