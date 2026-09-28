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
use Zephir\Exception\CompilerException;
use Zephir\Variable\Variable;

use function array_diff;
use function array_keys;

/**
 * Generates PHP's `/`.
 *
 * The result type follows div_function_base() in Zend/zend_operators.c: a
 * float operand always yields a float, two integers yield an int when the
 * quotient is exact and a float otherwise, and any other operand is left to
 * PHP's own div_function(). A bool operand is an integer 0 or 1, as in PHP.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2675
 * @see https://github.com/zephir-lang/zephir/issues/2676
 * @see https://github.com/zephir-lang/zephir/issues/2677
 */
class DivOperator extends ArithmeticalBaseOperator
{
    private const KIND_LONG   = 'long';
    private const KIND_DOUBLE = 'double';
    private const KIND_ZVAL   = 'zval';

    protected string $operator     = '/';
    protected string $zvalOperator = 'div_function';

    /**
     * Compiles the arithmetical division operation.
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile($expression, CompilationContext $compilationContext): CompiledExpression|bool
    {
        [$left, $right] = $this->preCompileChecks($expression, $compilationContext);

        $leftOperand  = $this->operand($left, $expression, $compilationContext);
        $rightOperand = $this->operand($right, $expression, $compilationContext);
        /**
         * Against a zval a bool stays a bool, so a TypeError names it as PHP does.
         */
        if (self::KIND_ZVAL === $leftOperand['kind'] && isset($rightOperand['bool'])) {
            $rightOperand = $this->boxedBool($rightOperand['bool'], $compilationContext);
        }
        if (self::KIND_ZVAL === $rightOperand['kind'] && isset($leftOperand['bool'])) {
            $leftOperand = $this->boxedBool($leftOperand['bool'], $compilationContext);
        }

        $shape = $leftOperand['kind'] . '_' . $rightOperand['kind'];

        return match ($shape) {
            'long_long'     => $this->consumerIsDouble($compilationContext)
                ? $this->doubleResult('zephir_safe_div_long_long', $leftOperand, $rightOperand, $expression)
                : $this->zvalResult('zephir_div_long_long', $leftOperand, $rightOperand, $expression, $compilationContext),
            'long_double'   => $this->doubleResult('zephir_safe_div_long_double', $leftOperand, $rightOperand, $expression),
            'double_long'   => $this->doubleResult('zephir_safe_div_double_long', $leftOperand, $rightOperand, $expression),
            'double_double' => $this->doubleResult('zephir_safe_div_double_double', $leftOperand, $rightOperand, $expression),
            'zval_long'     => $this->zvalResult('zephir_div_zval_long', $leftOperand, $rightOperand, $expression, $compilationContext),
            'long_zval'     => $this->zvalResult('zephir_div_long_zval', $leftOperand, $rightOperand, $expression, $compilationContext),
            'zval_double'   => $this->zvalResult('zephir_div_zval_double', $leftOperand, $rightOperand, $expression, $compilationContext),
            'double_zval'   => $this->zvalResult('zephir_div_double_zval', $leftOperand, $rightOperand, $expression, $compilationContext),
            'zval_zval'     => $this->zvalResult($this->zvalOperator, $leftOperand, $rightOperand, $expression, $compilationContext),
        };
    }

    /**
     * Classifies one compiled operand as a C integer, a C double or a zval,
     * and returns the C code that reads it as that kind.
     *
     * @return array{kind: string, code: string, variable: ?Variable}
     *
     * @throws CompilerException
     */
    private function operand(
        CompiledExpression $operand,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        switch ($operand->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return ['kind' => self::KIND_LONG, 'code' => $operand->getCode(), 'variable' => null];

            case 'bool':
                return $this->boolOperand($operand->getBooleanCode());

            case 'double':
                return ['kind' => self::KIND_DOUBLE, 'code' => $operand->getCode(), 'variable' => null];

            case 'variable':
                $variable = $compilationContext->symbolTable->getVariableForRead(
                    $operand->getCode(),
                    $compilationContext,
                    $expression
                );

                return $this->variableOperand($variable, $expression, $compilationContext);

            default:
                throw new CompilerException(
                    'Cannot operate ' . $operand->getType() . ' with the division operator',
                    $expression
                );
        }
    }

    /**
     * @return array{kind: string, code: string, variable: ?Variable}
     *
     * @throws CompilerException
     */
    private function variableOperand(
        Variable $variable,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        switch ($variable->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return ['kind' => self::KIND_LONG, 'code' => $variable->getName(), 'variable' => null];

            case 'bool':
                return $this->boolOperand($variable->getName());

            case 'double':
                return ['kind' => self::KIND_DOUBLE, 'code' => $variable->getName(), 'variable' => null];

            case 'variable':
            case 'mixed':
                return [
                    'kind'     => self::KIND_ZVAL,
                    'code'     => $compilationContext->backend->getVariableCode($variable),
                    'variable' => $variable,
                ];

            default:
                throw new CompilerException(
                    'Cannot operate ' . $variable->getType() . ' variables with the division operator',
                    $expression
                );
        }
    }

    /**
     * A bool divides as the integer 0 or 1; `bool` keeps its C code for boxing.
     *
     * @return array{kind: string, code: string, variable: ?Variable, bool: string}
     */
    private function boolOperand(string $code): array
    {
        return ['kind' => self::KIND_LONG, 'code' => '(zend_long) ' . $code, 'variable' => null, 'bool' => $code];
    }

    /**
     * @return array{kind: string, code: string, variable: ?Variable}
     */
    private function boxedBool(string $code, CompilationContext $compilationContext): array
    {
        $boxed = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext);
        $compilationContext->backend->assignBool($boxed, $code, $compilationContext);

        return [
            'kind'     => self::KIND_ZVAL,
            'code'     => $compilationContext->backend->getVariableCode($boxed),
            'variable' => $boxed,
        ];
    }

    /**
     * True when the quotient lands in a C double, where PHP coerces an int
     * quotient to float anyway: a `double` local, or the return value of a
     * method declared `-> double` (optionally nullable).
     */
    private function consumerIsDouble(CompilationContext $compilationContext): bool
    {
        $target = $this->expectingVariable;
        if (!$this->expecting || null === $target) {
            return false;
        }

        if ('double' === $target->getType()) {
            return true;
        }

        $method = $compilationContext->currentMethod;
        if ('return_value' !== $target->getName() || null === $method || $method->isMixed()) {
            return false;
        }

        $returnTypes = array_keys($method->getReturnTypes());

        return $method->areReturnTypesDoubleCompatible() && [] === array_diff($returnTypes, ['double', 'null']);
    }

    private function doubleResult(string $helper, array $left, array $right, array $expression): CompiledExpression
    {
        return new CompiledExpression(
            'double',
            $helper . '(' . $left['code'] . ', ' . $right['code'] . ')',
            $expression
        );
    }

    /**
     * Writes the quotient into a zval, which holds an int or a float.
     *
     * @throws CompilerException
     */
    private function zvalResult(
        string $helper,
        array $left,
        array $right,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $expected     = $this->getExpected($compilationContext, $expression);
        $expectedCode = $compilationContext->backend->getVariableCode($expected);

        $compilationContext->codePrinter->output(
            $helper . '(' . $expectedCode . ', ' . $left['code'] . ', ' . $right['code'] . ');'
        );

        foreach ([$left['variable'], $right['variable']] as $operandVariable) {
            if (null !== $operandVariable) {
                $this->checkVariableTemporal($operandVariable);
            }
        }

        $expected->setDynamicTypes(['long', 'double']);

        return new CompiledExpression('variable', $expected->getName(), $expression);
    }
}
