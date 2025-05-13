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
use Zephir\Variable\Variable as ZephirVariable;

/**
 * Adds/Updates an array index
 */
class ArrayIndexAppend extends ArrayIndex
{
    /**
     * Compiles foo[y][] = {expr}.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function assign(
        $variable,
        ZephirVariable $symbolVariable,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        $statement
    ): void {
        /**
         * Arrays must be stored in the HEAP
         */
        $this->checkVariableInitialized($variable, $symbolVariable, $statement);
        $this->checkVariableReadOnly($variable, $symbolVariable, $statement);
        $this->checkVariableLocalOnly($variable, $symbolVariable, $statement);

        /**
         * Only dynamic variables and arrays can be used as arrays
         */
        if ($symbolVariable->isNotVariableAndArray()) {
            throw CompilerException::cannotUseAsArray(
                $symbolVariable->getType(),
                $statement
            );
        }

        if ($symbolVariable->hasAnyDynamicType('unknown')) {
            throw CompilerException::cannotUseNonInitializedVariableAsObject($statement);
        }

        /**
         * Trying to use a non-object dynamic variable as object
         */
        if ($symbolVariable->hasDifferentDynamicType(['undefined', 'array', 'null'])) {
            $compilationContext->logger->warning(
                'Possible attempt to update index on a non-array dynamic variable',
                ['non-array-update', $statement]
            );
        }

        $this->assignArrayIndexMultiple($variable, $resolvedExpr, $compilationContext, $statement);
    }

    /**
     * Compiles foo[y][x][] = {expr} (multiple offset).
     *
     * @throws Exception
     * @throws ReflectionException
     */
    protected function assignArrayIndexMultiple(
        $variable,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        $statement
    ): void {
        $offsetExprs = [];
        $types = ['int', 'uint', 'long', 'ulong', 'string', 'variable'];
        foreach ($statement['index-expr'] as $indexExpr) {
            $expression = new Expression($indexExpr);
            $expression->setReadOnly(true);
            $exprIndex = $expression->compile($compilationContext);

            if (!in_array($exprIndex->getType(), $types)) {
                throw new CompilerException(
                    'Index: '
                    . $exprIndex->getType()
                    . ' cannot be used as array index in assignment without cast',
                    $indexExpr
                );
            }

            $offsetExprs[] = $exprIndex;
        }

        $compilationContext->headersManager->add('kernel/array');

        /**
         * Create a temporal zval (if needed).
         */
        $symbolVariable = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);
        $targetVariable = $compilationContext->symbolTable->getVariableForWrite(
            $variable,
            $compilationContext,
            $statement
        );

        $offsetExprs[] = 'a';
        $compilationContext->backend->assignArrayMulti(
            $targetVariable,
            $symbolVariable,
            $offsetExprs,
            $compilationContext
        );
    }
}
