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
use Zephir\Class\Property;
use Zephir\Class\Visibility;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;

use function in_array;
use function sprintf;

/**
 * Updates object properties dynamically
 */
class StaticPropertyAppend extends ArrayIndex
{
    /**
     * Compiles ClassName::foo[index] = {expr}.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function assignStatic(
        $className,
        $property,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement
    ): void {
        $compiler = $compilationContext->compiler;
        if (!in_array($className, ['self', 'static', 'parent'])) {
            $className = $compilationContext->getFullName($className);
            if ($compiler->isClass($className)) {
                $classDefinition = $compiler->getClassDefinition($className);
            } else {
                if ($compiler->isBundledClass($className)) {
                    $classDefinition = $compiler->getInternalClassDefinition($className);
                } else {
                    throw new CompilerException("Cannot locate class '" . $className . "'", $statement);
                }
            }
        } else {
            if (in_array($className, ['self', 'static'])) {
                $classDefinition = $compilationContext->classDefinition;
            } else {
                if ('parent' == $className) {
                    $classDefinition = $compilationContext->parentClassDefinition(
                        'assign static property "' . $property . '"',
                        $statement
                    );
                }
            }
        }

        $this->checkClassHasProperty(
            $classDefinition,
            $property,
            $statement
        );

        /** @var Property $propertyDefinition */
        $propertyDefinition = $classDefinition->getProperty($property);
        $this->checkAccessNonStaticProperty(
            $propertyDefinition,
            $classDefinition,
            $property,
            $statement
        );

        /**
         * @see https://github.com/zephir-lang/zephir/issues/2734
         */
        if (!Visibility::canAccessProperty($propertyDefinition, $compilationContext)) {
            throw new CompilerException(
                sprintf(
                    "Cannot access %s static property '%s::%s' out of its declaring context",
                    $propertyDefinition->isPrivate() ? 'private' : 'protected',
                    $classDefinition->getCompleteName(),
                    $property
                ),
                $statement
            );
        }

        $compilationContext->headersManager->add('kernel/object');
        $classEntry = $classDefinition->getClassEntry($compilationContext);
        $this->assignStaticPropertyArrayMultipleIndex(
            $classEntry,
            $resolvedExpr,
            $compilationContext,
            $statement,
        );
    }

    /**
     * Compiles x::y[a][b][] = {expr} (multiple offset assignment).
     */
    protected function assignStaticPropertyArrayMultipleIndex(
        $classEntry,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement,
    ): void {
        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /**
         * Create a temporal zval (if needed).
         */
        $variableExpr      = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);
        $offsetExpressions = $this->getOffsetExpressions($statement, $compilationContext);

        $compilationContext->backend->assignStaticPropertyArrayMulti(
            $classEntry,
            $variableExpr,
            $property,
            $offsetExpressions,
            $compilationContext
        );

        $this->checkVariableTemporal($variableExpr);
    }

    protected function getOffsetExpressions(array $statement, CompilationContext $compilationContext): array
    {
        $offsetExpressions[] = 'a';

        return $offsetExpressions;
    }
}
