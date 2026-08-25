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

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception\CompilerException;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * Factory for creating Let assignment handlers based on assignment type.
 * Eliminates the large switch statement in LetStatement.
 */
final class AssignmentFactory
{
    /**
     * Map of assignment types to their handler classes
     */
    private const ASSIGNMENT_MAP = [
        'variable' => Variable::class,
        'variable-append' => VariableAppend::class,
        'object-property' => ObjectProperty::class,
        'variable-dynamic-object-property' => ObjectDynamicProperty::class,
        'string-dynamic-object-property' => ObjectDynamicStringProperty::class,
        'array-index' => ArrayIndex::class,
        'array-index-append' => ArrayIndexAppend::class,
        'object-property-append' => ObjectPropertyAppend::class,
        'object-property-array-index' => ObjectPropertyArrayIndex::class,
        'object-property-array-index-append' => ObjectPropertyArrayIndexAppend::class,
        'incr' => Incr::class,
        'decr' => Decr::class,
        'object-property-incr' => ObjectPropertyIncr::class,
        'object-property-decr' => ObjectPropertyDecr::class,
        'dynamic-variable' => ExportSymbol::class,
        'dynamic-variable-string' => ExportSymbolString::class,
        'destructure' => Destructure::class,
    ];

    /**
     * Static property assignment types (require special handling)
     */
    private const STATIC_PROPERTY_TYPES = [
        'static-property',
        'static-property-append',
        'static-property-array-index',
        'static-property-array-index-append',
    ];

    /**
     * Process an assignment using the appropriate handler
     *
     * @throws CompilerException
     */
    public static function process(
        array $assignment,
        ?ZephirVariable $symbolVariable,
        ?CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        ReadDetector $readDetector
    ): void {
        $assignType = $assignment['assign-type'];
        // Destructuring carries a `variables` list instead of a single `variable`
        $variable = $assignment['variable'] ?? null;

        // Handle static property assignments (special case with operator logic)
        if (in_array($assignType, self::STATIC_PROPERTY_TYPES, true)) {
            self::processStaticProperty($assignment, $resolvedExpr, $compilationContext);
            return;
        }

        // Handle standard assignments using the map
        if (!isset(self::ASSIGNMENT_MAP[$assignType])) {
            throw new CompilerException('Unknown assignment: ' . $assignType, $assignment);
        }

        $handlerClass = self::ASSIGNMENT_MAP[$assignType];
        $handler = new $handlerClass();

        // Different assignment types have different method signatures
        switch ($assignType) {
            case 'variable':
                $handler->assign(
                    $variable,
                    $symbolVariable,
                    $resolvedExpr,
                    $readDetector,
                    $compilationContext,
                    $assignment
                );
                break;

            case 'incr':
            case 'decr':
                $handler->assign($variable, $symbolVariable, $compilationContext, $assignment);
                break;

            case 'object-property-incr':
            case 'object-property-decr':
                $handler->assign($variable, $assignment['property'], $symbolVariable, $compilationContext, $assignment);
                break;

            case 'dynamic-variable':
            case 'dynamic-variable-string':
                $handler->assign($symbolVariable, $resolvedExpr, $compilationContext, $assignment);
                break;

            case 'destructure':
                $handler->assign($assignment, $resolvedExpr, $compilationContext);
                break;

            default:
                // Standard signature: variable, symbolVariable, resolvedExpr, context, assignment
                $handler->assign($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
                break;
        }
    }

    /**
     * Process static property assignments with operator support
     */
    private static function processStaticProperty(
        array $assignment,
        ?CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext
    ): void {
        $assignType = $assignment['assign-type'];
        $variable = $assignment['variable'];
        $property = $assignment['property'];

        // Determine the handler class based on operator
        if ($assignType === 'static-property' && isset($assignment['operator'])) {
            $handler = match ($assignment['operator']) {
                'add-assign' => new StaticPropertyAdd(),
                'sub-assign' => new StaticPropertySub(),
                default => new StaticProperty(),
            };
        } else {
            $handler = match ($assignType) {
                'static-property' => new StaticProperty(),
                'static-property-append' => new StaticPropertyAppend(),
                'static-property-array-index' => new StaticPropertyArrayIndex(),
                'static-property-array-index-append' => new StaticPropertyArrayIndexAppend(),
            };
        }

        $handler->assignStatic($variable, $property, $resolvedExpr, $compilationContext, $assignment);
    }
}
