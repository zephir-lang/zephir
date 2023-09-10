<?php

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
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * ArrayIndexAppend.
 *
 * Adds/Updates an array index
 */
class ArrayIndexAppend extends ArrayIndex
{
    /**
     * Compiles foo[y][] = {expr}.
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        /*
         * Arrays must be stored in the HEAP
         */
        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is local only", $statement);
        }

        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is not initialized", $statement);
        }

        if ($symbolVariable->isReadOnly()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is read only", $statement);
        }

        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is local only", $statement);
        }

        /*
         * Only dynamic variables and arrays can be used as arrays
         */
        if ($symbolVariable->isNotVariableAndArray()) {
            throw new CompilerException("Cannot use variable type: '".$symbolVariable->getType()."' as array", $statement);
        }

        if ($symbolVariable->hasAnyDynamicType('unknown')) {
            throw new CompilerException('Cannot use non-initialized variable as an object', $statement);
        }

        /*
         * Trying to use a non-object dynamic variable as object
         */
        if ($symbolVariable->hasDifferentDynamicType(['undefined', 'array', 'null'])) {
            $compilationContext->logger->warning(
                'Possible attempt to update index on a non-array dynamic variable',
                ['non-array-update', $statement]
            );
        }

        $this->_assignArrayIndexMultiple($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
    }

    /**
     * Compiles foo[y][x][] = {expr} (multiple offset).
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     */
    protected function _assignArrayIndexMultiple($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {
        $offsetExprs = [];
        foreach ($statement['index-expr'] as $indexExpr) {
            $expression = new Expression($indexExpr);
            $expression->setReadOnly(true);
            $exprIndex = $expression->compile($compilationContext);

            switch ($exprIndex->getType()) {
                case 'int':
                case 'uint':
                case 'long':
                case 'ulong':
                case 'string':
                case 'variable':
                    break;
                default:
                    throw new CompilerException('Index: '.$exprIndex->getType().' cannot be used as array index in assignment without cast', $indexExpr);
            }

            $offsetExprs[] = $exprIndex;
        }

        $compilationContext->headersManager->add('kernel/array');

        /**
         * Create a temporal zval (if needed).
         */
        $symbolVariable = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);
        $targetVariable = $compilationContext->symbolTable->getVariableForWrite($variable, $compilationContext, $statement);

        $offsetExprs[] = 'a';
        $compilationContext->backend->assignArrayMulti($targetVariable, $symbolVariable, $offsetExprs, $compilationContext);
    }
}
