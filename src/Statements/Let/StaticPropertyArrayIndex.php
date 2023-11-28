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

use Zephir\Class\Property;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;

/**
 * Zephir\Statements\Let\StaticPropertyArrayIndex.
 *
 * Updates object properties dynamically.
 */
class StaticPropertyArrayIndex extends ArrayIndex
{
    /**
     * Compiles ClassName::foo[index] = {expr}.
     *
     * @param string             $className
     * @param string             $property
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws Exception
     * @throws CompilerException
     */
    public function assignStatic(
        $className,
        $property,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement
    ): void {
        $compiler = $compilationContext->compiler;
        if (!\in_array($className, ['self', 'static', 'parent'])) {
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
            if (\in_array($className, ['self', 'static'])) {
                $classDefinition = $compilationContext->classDefinition;
            } else {
                if ('parent' == $className) {
                    $classDefinition = $compilationContext->classDefinition;
                    $extendsClass = $classDefinition->getExtendsClass();
                    if (!$extendsClass) {
                        throw new CompilerException('Cannot assign static property "' . $property . '" on parent because class ' . $classDefinition->getCompleteName() . ' does not extend any class', $statement);
                    } else {
                        $classDefinition = $classDefinition->getExtendsClassDefinition();
                    }
                }
            }
        }

        if (!$classDefinition->hasProperty($property)) {
            throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a property called: '" . $property . "'", $statement);
        }

        /** @var Property $propertyDefinition */
        $propertyDefinition = $classDefinition->getProperty($property);
        if (!$propertyDefinition->isStatic()) {
            throw new CompilerException("Cannot access non-static property '" . $classDefinition->getCompleteName() . '::' . $property . "'", $statement);
        }

        if ($propertyDefinition->isPrivate()) {
            if ($classDefinition != $compilationContext->classDefinition) {
                throw new CompilerException("Cannot access private static property '" . $classDefinition->getCompleteName() . '::' . $property . "' out of its declaring context", $statement);
            }
        }

        $compilationContext->headersManager->add('kernel/object');
        $classEntry = $classDefinition->getClassEntry($compilationContext);
        $this->_assignStaticPropertyArrayMultipleIndex($classEntry, $property, $resolvedExpr, $compilationContext, $statement);
    }

    /**
     * Compiles x::y[a][b] = {expr} (multiple offset assignment).
     *
     * @param string             $classEntry
     * @param string             $property
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext,
     * @param array              $statement
     *
     * @throws Exception
     * @throws CompilerException
     */
    protected function _assignStaticPropertyArrayMultipleIndex(
        $classEntry,
        $property,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        $statement
    ): void {
        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /**
         * Create a temporal zval (if needed).
         */
        $variableExpr = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

        /**
         * Only string/variable/int.
         */
        $offsetExpressions = [];
        foreach ($statement['index-expr'] as $indexExpr) {
            $indexExpression = new Expression($indexExpr);

            $resolvedIndex = $indexExpression->compile($compilationContext);
            switch ($resolvedIndex->getType()) {
                case 'string':
                case 'int':
                case 'uint':
                case 'ulong':
                case 'long':
                case 'variable':
                    break;
                default:
                    throw new CompilerException(
                        sprintf('Expression: %s cannot be used as index without cast', $resolvedIndex->getType()),
                        $statement['index-expr']
                    );
            }

            $offsetExpressions[] = $resolvedIndex;
        }

        $compilationContext->backend->assignStaticPropertyArrayMulti(
            $classEntry,
            $variableExpr,
            $property,
            $offsetExpressions,
            $compilationContext
        );

        if ($variableExpr->isTemporal()) {
            $variableExpr->setIdle(true);
        }
    }
}
