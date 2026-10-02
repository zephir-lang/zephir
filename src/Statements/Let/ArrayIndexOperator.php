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
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Expression\PropertyAccess;
use Zephir\Expression\StaticPropertyAccess;
use Zephir\Variable\Variable as ZephirVariable;

use function count;
use function in_array;
use function sprintf;

/**
 * Compiles a compound assignment on an array element: `a[k] += v`,
 * `this->p[k][] .= v`, `self::p[k] &= v` and every other combination.
 *
 * PHP compiles all of them to ZEND_ASSIGN_DIM_OP: the containers are fetched
 * read-write and the operator reads the element before the result replaces
 * it. Here that is one call to the kernel's zephir_array_assign_op(), handed
 * the engine's own operator function, whatever the container is. The plain
 * assignment handlers never looked at the operator and overwrote the element.
 *
 * Evaluation order follows PHP's for the parts this handler emits: the
 * offsets, then the container, then the operation.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2747
 */
final class ArrayIndexOperator extends ArrayIndex
{
    /**
     * The engine function each compound operator applies.
     */
    private const OPERATOR_FUNCTIONS = [
        'add-assign'                => 'add_function',
        'sub-assign'                => 'sub_function',
        'mul-assign'                => 'mul_function',
        'div-assign'                => 'div_function',
        'mod-assign'                => 'mod_function',
        'concat-assign'             => 'concat_function',
        'bitwise-and-assign'        => 'bitwise_and_function',
        'bitwise-or-assign'         => 'bitwise_or_function',
        'bitwise-xor-assign'        => 'bitwise_xor_function',
        'bitwise-shiftleft-assign'  => 'shift_left_function',
        'bitwise-shiftright-assign' => 'shift_right_function',
    ];

    private const LOCAL_TYPES = [
        'array-index',
        'array-index-append',
        'variable-append',
    ];

    private const PROPERTY_TYPES = [
        'object-property-array-index',
        'object-property-append',
        'object-property-array-index-append',
    ];

    private const STATIC_PROPERTY_TYPES = [
        'static-property-array-index',
        'static-property-append',
        'static-property-array-index-append',
    ];

    private const APPEND_TYPES = [
        'array-index-append',
        'variable-append',
        'object-property-append',
        'object-property-array-index-append',
        'static-property-append',
        'static-property-array-index-append',
    ];

    private const INDEX_TYPES = ['int', 'uint', 'long', 'ulong', 'string', 'variable'];

    /**
     * Whether the assignment is a compound operator on an array element.
     */
    public static function handles(array $assignment): bool
    {
        $assignType = $assignment['assign-type'];

        return isset(self::OPERATOR_FUNCTIONS[$assignment['operator'] ?? 'assign'])
            && (
                in_array($assignType, self::LOCAL_TYPES, true)
                || in_array($assignType, self::PROPERTY_TYPES, true)
                || in_array($assignType, self::STATIC_PROPERTY_TYPES, true)
            );
    }

    /**
     * @throws Exception
     * @throws ReflectionException
     */
    public function assignOperator(
        ?ZephirVariable $symbolVariable,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement
    ): void {
        $assignType = $statement['assign-type'];
        $isAppend   = in_array($assignType, self::APPEND_TYPES, true);

        if (in_array($assignType, self::LOCAL_TYPES, true)) {
            $this->checkLocalContainer($symbolVariable, $isAppend, $compilationContext, $statement);
        }

        $offsetExprs = $this->compileOffsets($statement, $compilationContext);
        if ($isAppend) {
            $offsetExprs[] = 'a';
        }

        $value = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

        $containerCode = match (true) {
            in_array($assignType, self::LOCAL_TYPES, true)    => $compilationContext->backend->getVariableCode(
                $symbolVariable
            ),
            in_array($assignType, self::PROPERTY_TYPES, true) => $this->propertySlot(
                new PropertyAccess(),
                'property-access',
                $compilationContext,
                $statement
            ),
            default                                            => $this->propertySlot(
                new StaticPropertyAccess(),
                'static-property-access',
                $compilationContext,
                $statement
            ),
        };

        $compilationContext->headersManager->add('kernel/array');
        $compilationContext->backend->assignArrayOperator(
            $containerCode,
            $value,
            self::OPERATOR_FUNCTIONS[$statement['operator']],
            $offsetExprs,
            $compilationContext
        );

        $this->checkVariableTemporal($value);
    }

    /**
     * A local container passes the checks a plain element assignment makes.
     * A declared string can never succeed, so PHP's runtime Error for it is
     * raised at build time.
     *
     * @throws CompilerException
     */
    private function checkLocalContainer(
        ZephirVariable $symbolVariable,
        bool $isAppend,
        CompilationContext $compilationContext,
        array $statement
    ): void {
        $variable = $symbolVariable->getName();

        $this->checkVariableInitialized($variable, $symbolVariable, $statement);
        $this->checkVariableReadOnly($variable, $symbolVariable, $statement);
        $this->checkVariableLocalOnly($variable, $symbolVariable, $statement);

        if ($symbolVariable->isString()) {
            $levels = count($statement['index-expr'] ?? []) + ($isAppend ? 1 : 0);

            throw new CompilerException(
                match (true) {
                    $levels > 1 => 'Cannot use string offset as an array',
                    $isAppend   => '[] operator not supported for strings',
                    default     => 'Cannot use assign-op operators with string offsets',
                },
                $statement
            );
        }

        if ($symbolVariable->isNotVariableAndArray()) {
            throw CompilerException::cannotUseAsArray($symbolVariable->getType(), $statement);
        }

        if ('variable' === $symbolVariable->getType()) {
            if ($symbolVariable->hasAnyDynamicType('unknown')) {
                throw CompilerException::cannotUseNonInitializedVariableAsObject($statement);
            }

            if ($symbolVariable->hasDifferentDynamicType(['undefined', 'array', 'null'])) {
                $compilationContext->logger->warning(
                    'Possible attempt to update index on a non-array dynamic variable',
                    ['non-array-update', $statement]
                );
            }
        }
    }

    /**
     * @return CompiledExpression[]
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function compileOffsets(array $statement, CompilationContext $compilationContext): array
    {
        $offsetExprs = [];
        foreach ($statement['index-expr'] ?? [] as $indexExpr) {
            $expression = new Expression($indexExpr);
            $expression->setReadOnly(true);
            $exprIndex = $expression->compile($compilationContext);

            if (!in_array($exprIndex->getType(), self::INDEX_TYPES, true)) {
                throw new CompilerException(
                    sprintf(
                        'Index: %s cannot be used as array index in assignment without cast',
                        $exprIndex->getType()
                    ),
                    $indexExpr
                );
            }

            $offsetExprs[] = $exprIndex;
        }

        return $offsetExprs;
    }

    /**
     * The read-write slot of `variable->property` or `Class::property`.
     *
     * The property expressions resolve the object or the class, check the
     * property and its visibility, and emit the fetch; this only asks for the
     * read-write form of it.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function propertySlot(
        PropertyAccess|StaticPropertyAccess $access,
        string $type,
        CompilationContext $compilationContext,
        array $statement
    ): string {
        $access->setWriteThrough(true, true);

        $compiled = $access->compile(
            $this->astNode($statement, [
                'type'  => $type,
                'left'  => $this->astNode($statement, ['type' => 'variable', 'value' => $statement['variable']]),
                'right' => $this->astNode($statement, ['type' => 'variable', 'value' => $statement['property']]),
            ]),
            $compilationContext
        );

        $slot = $compilationContext->symbolTable->getVariableForRead(
            $compiled->getCode(),
            $compilationContext,
            $statement
        );

        return $compilationContext->backend->getVariableCode($slot);
    }

    /**
     * Copies the statement's source position onto a synthesized node, so a
     * diagnostic about it points at the original assignment.
     */
    private function astNode(array $statement, array $node): array
    {
        foreach (['file', 'line', 'char'] as $position) {
            if (isset($statement[$position])) {
                $node[$position] = $statement[$position];
            }
        }

        return $node;
    }
}
