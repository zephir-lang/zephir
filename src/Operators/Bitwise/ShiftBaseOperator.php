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

use function is_numeric;

/**
 * `<<` and `>>` on C integers follow PHP rather than C: a count of the integer
 * width or more yields 0 (or -1 for a negative `>>`), and a negative count
 * throws ArithmeticError. A C shift is undefined for both, and for a negative
 * left operand of `<<`.
 */
abstract class ShiftBaseOperator extends BitwiseBaseOperator
{
    /**
     * Kernel helper applying the shift with PHP's rules.
     */
    protected string $safeHelper;

    protected function nativeOperation(
        int|string $left,
        int|string $right,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $compilationContext->headersManager->add('kernel/operators');
        $left  = (string) $left;
        $right = (string) $right;

        $count = is_numeric($right) ? (int) $right : null;
        if (null !== $count && $count >= 0 && $count < 64) {
            return new CompiledExpression('int', $this->inlineShift($left, $right), $expression);
        }

        $call = $this->safeHelper . '(' . $left . ', ' . $right . ')';

        /**
         * Only a negative count throws. A known non-negative count cannot, and
         * a returned result leaves the method right away anyway.
         */
        $isReturned = $this->expecting && 'return_value' === $this->expectingVariable?->getName();
        if ((null !== $count && $count >= 0) || $isReturned) {
            return new CompiledExpression('int', $call, $expression);
        }

        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('long', $compilationContext);
        $compilationContext->codePrinter->output($tempVariable->getName() . ' = ' . $call . ';');
        $compilationContext->emitExceptionCheck();

        return new CompiledExpression('int', $tempVariable->getName(), $expression);
    }

    /**
     * The shift for a literal count in range, which C defines.
     */
    abstract protected function inlineShift(string $left, string $right): string;
}
