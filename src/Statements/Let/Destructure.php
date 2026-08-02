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

namespace Zephir\Statements\Let;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;

use function in_array;

/**
 * Destructuring assignment: `let [a, b, c] = expr;`
 *
 * Every non-skipped slot is compiled as the equivalent indexed read
 * (`let a = source[0];`), which emits one `zephir_array_fetch_long()` per slot
 * and inherits type coercion, memory tracking and read-only promotion from the
 * regular array-access path.
 *
 * The right-hand side is compiled once by LetStatement and only referenced from
 * here, so it is evaluated exactly once no matter how many slots there are.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2496
 */
class Destructure
{
    /**
     * Compiles let [a, b, c] = {expr}.
     *
     * @throws CompilerException
     * @throws Exception
     * @throws ReflectionException
     */
    public function assign(
        array $assignment,
        ?CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext
    ): void {
        $source       = $this->resolveSource($resolvedExpr, $assignment, $compilationContext);
        $readDetector = new ReadDetector();

        foreach ($assignment['variables'] as $index => $slot) {
            /**
             * Skipped slots (`let [a, , c] = arr;`) are null in the AST.
             */
            if (null === $slot) {
                continue;
            }

            $statement = $this->buildAssignment($slot, $source, $index);
            $target    = $compilationContext->symbolTable->getVariableForWrite(
                $slot['value'],
                $compilationContext,
                $statement
            );

            $expression = new Expression($statement['expr']);
            $expression->setReadOnly(true);

            /**
             * A natively typed target (`int i`) cannot receive an array fetch
             * directly, so it takes the value from a temporary zval which the
             * Variable handler then coerces.
             */
            $expression->setExpectReturn(true, $target->isVariable() ? $target : null);

            (new Variable())->assign(
                $slot['value'],
                $target,
                $expression->compile($compilationContext),
                $readDetector,
                $compilationContext,
                $statement
            );
        }
    }

    /**
     * Builds the equivalent `let <slot> = <source>[<index>];` assignment.
     */
    private function buildAssignment(array $slot, string $source, int $index): array
    {
        return [
            'assign-type' => 'variable',
            'operator'    => 'assign',
            'variable'    => $slot['value'],
            'expr'        => [
                'type'  => 'array-access',
                'left'  => [
                    'type'  => 'variable',
                    'value' => $source,
                    'file'  => $slot['file'],
                    'line'  => $slot['line'],
                    'char'  => $slot['char'],
                ],
                'right' => [
                    'type'  => 'int',
                    'value' => (string) $index,
                    'file'  => $slot['file'],
                    'line'  => $slot['line'],
                    'char'  => $slot['char'],
                ],
                'file'  => $slot['file'],
                'line'  => $slot['line'],
                'char'  => $slot['char'],
            ],
            'file'        => $slot['file'],
            'line'        => $slot['line'],
            'char'        => $slot['char'],
        ];
    }

    /**
     * Returns the name of the variable holding the value to destructure.
     *
     * Only arrays and dynamic variables can be destructured. PHP's list()
     * yields nulls for a string, so rather than silently returning characters
     * through Zephir's string-offset path, a string source is rejected.
     *
     * @throws CompilerException
     */
    private function resolveSource(
        ?CompiledExpression $resolvedExpr,
        array $assignment,
        CompilationContext $compilationContext
    ): string {
        if (null === $resolvedExpr || !in_array($resolvedExpr->getType(), ['variable', 'array'], true)) {
            throw new CompilerException(
                'Cannot use expression: ' . ($resolvedExpr?->getType() ?? 'unknown')
                . ' as an array in a destructuring assignment',
                $assignment
            );
        }

        $source = $compilationContext->symbolTable->getVariableForRead(
            $resolvedExpr->getCode(),
            $compilationContext,
            $assignment
        );

        if ($source->isNotVariableAndArray()) {
            throw new CompilerException(
                'Cannot use variable: ' . $source->getType()
                . ' as an array in a destructuring assignment',
                $assignment
            );
        }

        return $resolvedExpr->getCode();
    }
}
