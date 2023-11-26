<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * Updates object properties dynamically
 */
class ObjectPropertyArrayIndexAppend extends ArrayIndex
{
    /**
     * Compiles x->y[z][] = foo.
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext,
     * @param array              $statement
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement): void
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is not initialized", $statement);
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException('Attempt to use variable type: '.$symbolVariable->getType().' as object', $statement);
        }

        $this->_assignPropertyArrayMultipleIndex($symbolVariable, $resolvedExpr, $compilationContext, $statement);
    }

    /**
     * Compiles x->y[a][b][] = {expr} (multiple offset assignment).
     *
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws \ReflectionException
     * @throws Exception
     */
    protected function _assignPropertyArrayMultipleIndex(ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, array $statement): void
    {
        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /**
         * Create a temporal zval (if needed).
         */
        $variableExpr = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

        if (\count($statement['index-expr']) > 16) {
            throw new CompilerException('Too many array indexes', $statement);
        }

        /**
         * Only string/variable/int.
         */
        $offsetExprs = [];
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
                    throw new CompilerException('Expression: '.$resolvedIndex->getType().' cannot be used as index without cast', $statement);
            }

            $offsetExprs[] = $resolvedIndex;
        }

        /**
         * Check if the property to update is defined
         */
        if ('this' == $symbolVariable->getRealName()) {
            $classDefinition = $compilationContext->classDefinition;
            if (!$classDefinition->hasProperty($property)) {
                throw new CompilerException("Class '".$classDefinition->getCompleteName()."' does not have a property called: '".$property."'", $statement);
            }
        } else {
            /**
             * If we know the class related to a variable we could check if the property
             * is defined on that class
             */
            if ($symbolVariable->hasAnyDynamicType('object')) {
                $classType = current($symbolVariable->getClassTypes());
                $compiler = $compilationContext->compiler;

                if ($compiler->isClass($classType)) {
                    $classDefinition = $compiler->getClassDefinition($classType);
                    if (!$classDefinition) {
                        throw new CompilerException('Cannot locate class definition for class: '.$classType, $statement);
                    }

                    if (!$classDefinition->hasProperty($property)) {
                        throw new CompilerException("Class '".$classType."' does not have a property called: '".$property."'", $statement);
                    }
                }
            }
        }

        $offsetExprs[] = 'a';
        $compilationContext->backend->assignPropertyArrayMulti($symbolVariable, $variableExpr, $property, $offsetExprs, $compilationContext);
    }
}
