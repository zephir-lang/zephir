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
use Zephir\Name;
use Zephir\Operators\AbstractOperator;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

use function is_numeric;

/**
 * This is the base operator for commutative, associative and distributive
 * arithmetic operators
 */
class ArithmeticalBaseOperator extends AbstractOperator
{
    /**
     * The C kind an operand is read as by the operators that classify their
     * operands (`/` and `%`).
     */
    protected const KIND_LONG   = 'long';
    protected const KIND_DOUBLE = 'double';
    protected const KIND_ZVAL   = 'zval';

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
     * Both operands classified as a C integer, a C double or a zval. Against
     * a zval a bool stays a bool, so a TypeError names it as PHP does.
     *
     * @return array{0: array, 1: array}
     *
     * @throws CompilerException
     */
    protected function classifiedOperands(
        CompiledExpression $left,
        CompiledExpression $right,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        $leftOperand  = $this->operand($left, $expression, $compilationContext);
        $rightOperand = $this->operand($right, $expression, $compilationContext);
        if (self::KIND_ZVAL === $leftOperand['kind'] && isset($rightOperand['bool'])) {
            $rightOperand = $this->boxedBool($rightOperand['bool'], $compilationContext);
        }
        if (self::KIND_ZVAL === $rightOperand['kind'] && isset($leftOperand['bool'])) {
            $leftOperand = $this->boxedBool($leftOperand['bool'], $compilationContext);
        }

        return [$leftOperand, $rightOperand];
    }

    /**
     * Classifies one compiled operand as a C integer, a C double or a zval,
     * and returns the C code that reads it as that kind. A string or null
     * literal is boxed, since PHP coerces it or throws for it at runtime.
     *
     * @return array{kind: string, code: string, variable: ?Variable}
     *
     * @throws CompilerException
     */
    protected function operand(
        CompiledExpression $operand,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        switch ($operand->getType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
                return ['kind' => self::KIND_LONG, 'code' => $operand->getCode(), 'variable' => null];

            case Types::T_CHAR:
            case Types::T_UCHAR:
                return ['kind' => self::KIND_LONG, 'code' => $operand->getCharCode(), 'variable' => null];

            case Types::T_BOOL:
                return $this->boolOperand($operand->getBooleanCode());

            case Types::T_DOUBLE:
                return ['kind' => self::KIND_DOUBLE, 'code' => $operand->getCode(), 'variable' => null];

            case Types::T_STRING:
                $boxed = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignString(
                    $boxed,
                    Name::addSlashes($operand->getCode()),
                    $compilationContext
                );

                return $this->zvalOperand($boxed, $compilationContext);

            case Types::T_NULL:
                $boxed = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignNull($boxed, $compilationContext);

                return $this->zvalOperand($boxed, $compilationContext);

            case Types::T_ARRAY:
            case Types::T_VARIABLE:
                $variable = $compilationContext->symbolTable->getVariableForRead(
                    $operand->getCode(),
                    $compilationContext,
                    $expression
                );

                return $this->variableOperand($variable, $expression, $compilationContext);

            default:
                throw new CompilerException(
                    'Cannot operate ' . $operand->getType() . ' with the ' . $this->operator . ' operator',
                    $expression
                );
        }
    }

    /**
     * A typed string or array local is already a zval; a char is its byte.
     *
     * @return array{kind: string, code: string, variable: ?Variable}
     *
     * @throws CompilerException
     */
    protected function variableOperand(
        Variable $variable,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        switch ($variable->getType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
            case Types::T_CHAR:
            case Types::T_UCHAR:
                return ['kind' => self::KIND_LONG, 'code' => $variable->getName(), 'variable' => null];

            case Types::T_BOOL:
                return $this->boolOperand($variable->getName());

            case Types::T_DOUBLE:
                return ['kind' => self::KIND_DOUBLE, 'code' => $variable->getName(), 'variable' => null];

            case Types::T_VARIABLE:
            case Types::T_MIXED:
            case Types::T_STRING:
            case Types::T_ARRAY:
                return $this->zvalOperand($variable, $compilationContext);

            default:
                throw new CompilerException(
                    'Cannot operate ' . $variable->getType() . ' variables with the ' . $this->operator . ' operator',
                    $expression
                );
        }
    }

    /**
     * @return array{kind: string, code: string, variable: Variable}
     */
    protected function zvalOperand(Variable $variable, CompilationContext $compilationContext): array
    {
        return [
            'kind'     => self::KIND_ZVAL,
            'code'     => $compilationContext->backend->getVariableCode($variable),
            'variable' => $variable,
        ];
    }

    /**
     * A bool is the integer 0 or 1; `bool` keeps its C code for boxing.
     *
     * @return array{kind: string, code: string, variable: ?Variable, bool: string}
     */
    protected function boolOperand(string $code): array
    {
        return ['kind' => self::KIND_LONG, 'code' => '(zend_long) ' . $code, 'variable' => null, 'bool' => $code];
    }

    protected function boxedBool(string $code, CompilationContext $compilationContext): array
    {
        $boxed = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext);
        $compilationContext->backend->assignBool($boxed, $code, $compilationContext);

        return $this->zvalOperand($boxed, $compilationContext);
    }

    /**
     * A native operand copied into a temporary zval for PHP's own operator.
     *
     * @return array{kind: string, code: string, variable: Variable}
     */
    protected function boxedOperand(array $operand, CompilationContext $compilationContext): array
    {
        if (self::KIND_ZVAL === $operand['kind']) {
            return $operand;
        }

        if (isset($operand['bool'])) {
            return $this->boxedBool($operand['bool'], $compilationContext);
        }

        $boxed = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext);
        if (self::KIND_DOUBLE === $operand['kind']) {
            $compilationContext->backend->assignDouble($boxed, $operand['code'], $compilationContext);
        } else {
            $compilationContext->backend->assignLong($boxed, $operand['code'], $compilationContext);
        }

        return $this->zvalOperand($boxed, $compilationContext);
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

    /**
     * Writes the result of a zval-producing helper into the expected variable.
     *
     * @throws CompilerException
     */
    protected function zvalResult(
        string $helper,
        array $left,
        array $right,
        array $expression,
        CompilationContext $compilationContext,
        array $dynamicTypes
    ): CompiledExpression {
        $expected     = $this->getExpected($compilationContext, $expression);
        $expectedCode = $compilationContext->backend->getVariableCode($expected);

        $compilationContext->codePrinter->output(
            $helper . '(' . $expectedCode . ', ' . $left['code'] . ', ' . $right['code'] . ');'
        );
        $compilationContext->emitExceptionCheck();

        foreach ([$left['variable'], $right['variable']] as $operandVariable) {
            if (null !== $operandVariable) {
                $this->checkVariableTemporal($operandVariable);
            }
        }

        $expected->setDynamicTypes($dynamicTypes);

        return new CompiledExpression('variable', $expected->getName(), $expression);
    }
}
