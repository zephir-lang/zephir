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

/**
 * Generates PHP's `%`.
 *
 * PHP's mod_function() converts both operands to `zend_long` and yields an
 * `int`, so two native operands use a `zephir_safe_mod_*` helper typed `int`:
 * typing it `double` loses every result above 2^53. Any zval operand is left
 * to PHP's own mod_function(), which throws the TypeError for an array or a
 * non-numeric string and lets an overloaded object (GMP) return an object.
 * A bool operand is the integer 0 or 1, as in PHP.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2666
 * @see https://github.com/zephir-lang/zephir/issues/2676
 * @see https://github.com/zephir-lang/zephir/issues/2677
 */
class ModOperator extends ArithmeticalBaseOperator
{
    /**
     * mod_function() yields an int, or an object from an overloaded operand.
     */
    private const REMAINDER_TYPES = ['long', 'object'];

    protected string $operator     = '%';
    protected string $zvalOperator = 'mod_function';

    /**
     * Compiles the arithmetical modulus operation.
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
            'long_long'     => $this->intResult('zephir_safe_mod_long_long', $leftOperand, $rightOperand, $expression),
            'long_double'   => $this->intResult('zephir_safe_mod_long_double', $leftOperand, $rightOperand, $expression),
            'double_long'   => $this->intResult('zephir_safe_mod_double_long', $leftOperand, $rightOperand, $expression),
            'double_double' => $this->intResult('zephir_safe_mod_double_double', $leftOperand, $rightOperand, $expression),
            'zval_long'     => $this->zvalResult('zephir_mod_zval_long', $leftOperand, $rightOperand, $expression, $compilationContext, self::REMAINDER_TYPES),
            'long_zval'     => $this->zvalResult('zephir_mod_long_zval', $leftOperand, $rightOperand, $expression, $compilationContext, self::REMAINDER_TYPES),
            'zval_double'   => $this->zvalResult('zephir_mod_zval_double', $leftOperand, $rightOperand, $expression, $compilationContext, self::REMAINDER_TYPES),
            'double_zval'   => $this->zvalResult('zephir_mod_double_zval', $leftOperand, $rightOperand, $expression, $compilationContext, self::REMAINDER_TYPES),
            'zval_zval'     => $this->zvalResult($this->zvalOperator, $leftOperand, $rightOperand, $expression, $compilationContext, self::REMAINDER_TYPES),
        };
    }

    private function intResult(string $helper, array $left, array $right, array $expression): CompiledExpression
    {
        return new CompiledExpression(
            'int',
            $helper . '(' . $left['code'] . ', ' . $right['code'] . ')',
            $expression
        );
    }
}
