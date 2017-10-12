<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\Variable as ZephirVariable;
use Zephir\Expression;
use Zephir\CompiledExpression;

/**
 * ObjectPropertyArrayIndexAppend
 *
 * Updates object properties dynamically
 */
class ObjectPropertyArrayIndexAppend extends ArrayIndex
{
    /**
     * Compiles x->y[a][b][] = {expr} (multiple offset assignment)
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array $statement
     * @throws CompilerException
     */
    protected function _assignPropertyArrayMultipleIndex($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, array $statement)
    {
        $codePrinter = $compilationContext->codePrinter;

        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /**
         * Create a temporal zval (if needed)
         */
        $variableExpr = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

        if (count($statement['index-expr']) > 16) {
            throw new CompilerException("Too many array indexes", $statement);
        }

        /**
         * Only string/variable/int
         */
        $offsetExprs = array();
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
                    throw new CompilerException("Expression: " . $resolvedIndex->getType() . " cannot be used as index without cast", $statement);
            }

            $offsetExprs[] = $resolvedIndex;
        }

        /**
         * Check if the property to update is defined
         */
        if ($symbolVariable->getRealName() == 'this') {
            $classDefinition = $compilationContext->classDefinition;
            if (!$classDefinition->hasProperty($property)) {
                throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a property called: '" . $property . "'", $statement);
            }

            $propertyDefinition = $classDefinition->getProperty($property);
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
                        throw new CompilerException("Cannot locate class definition for class: " . $classType, $statement);
                    }

                    if (!$classDefinition->hasProperty($property)) {
                        throw new CompilerException("Class '" . $classType . "' does not have a property called: '" . $property . "'", $statement);
                    }
                }
            }
        }

        $offsetExprs[] = 'a';
        $compilationContext->backend->assignPropertyArrayMulti($symbolVariable, $variableExpr, $property, $offsetExprs, $compilationContext);
    }

    /**
     * Compiles x->y[z][] = foo
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext,
     * @param array $statement
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is not initialized", $statement);
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Attempt to use variable type: " . $symbolVariable->getType() . " as object", $statement);
        }

        $this->_assignPropertyArrayMultipleIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
    }
}
