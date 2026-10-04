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
    /**
     * An integer quotient is an int when exact and a float otherwise.
     */
    private const QUOTIENT_TYPES = ['long', 'double'];

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

        [$leftOperand, $rightOperand] = $this->classifiedOperands($left, $right, $expression, $compilationContext);

        $shape = $leftOperand['kind'] . '_' . $rightOperand['kind'];

        return match ($shape) {
            'long_long'     => $this->consumerIsDouble($compilationContext)
                ? $this->nativeResult('double', 'zephir_safe_div_long_long', $leftOperand, $rightOperand, $expression, $compilationContext)
                : $this->zvalResult('zephir_div_long_long', $leftOperand, $rightOperand, $expression, $compilationContext, self::QUOTIENT_TYPES),
            'long_double'   => $this->nativeResult('double', 'zephir_safe_div_long_double', $leftOperand, $rightOperand, $expression, $compilationContext),
            'double_long'   => $this->nativeResult('double', 'zephir_safe_div_double_long', $leftOperand, $rightOperand, $expression, $compilationContext),
            'double_double' => $this->nativeResult('double', 'zephir_safe_div_double_double', $leftOperand, $rightOperand, $expression, $compilationContext),
            'zval_long'     => $this->zvalResult('zephir_div_zval_long', $leftOperand, $rightOperand, $expression, $compilationContext, self::QUOTIENT_TYPES),
            'long_zval'     => $this->zvalResult('zephir_div_long_zval', $leftOperand, $rightOperand, $expression, $compilationContext, self::QUOTIENT_TYPES),
            'zval_double'   => $this->zvalResult('zephir_div_zval_double', $leftOperand, $rightOperand, $expression, $compilationContext, self::QUOTIENT_TYPES),
            'double_zval'   => $this->zvalResult('zephir_div_double_zval', $leftOperand, $rightOperand, $expression, $compilationContext, self::QUOTIENT_TYPES),
            'zval_zval'     => $this->zvalResult($this->zvalOperator, $leftOperand, $rightOperand, $expression, $compilationContext, self::QUOTIENT_TYPES),
        };
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
}
