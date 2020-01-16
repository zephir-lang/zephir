<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Other;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\BaseOperator;

/**
 * Clone.
 *
 * Clones an object into another one
 */
class CloneOperator extends BaseOperator
{
    /**
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     *
     * @return CompiledExpression
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/object');

        $exprVariable = new Expression($expression['left']);
        $exprVariable->setReadOnly(true);
        $exprVariable->setExpectReturn(true);

        $exprCompiledVariable = $exprVariable->compile($compilationContext);
        if ('variable' != $exprCompiledVariable->getType()) {
            throw new CompilerException('Expression type: '.$exprCompiledVariable->getType().' cannot be used as array', $expression);
        }

        $clonedVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression);
        if ('variable' != $clonedVariable->getType()) {
            throw new CompilerException('Variable type: '.$exprVariable->getType().' cannot be cloned');
        }

        if ($clonedVariable->hasDifferentDynamicType(['undefined', 'object', 'null'])) {
            $compilationContext->logger->warning(
                'Possible attempt to use non array in "clone" operator',
                ['non-valid-clone', $expression]
            );
        }

        $symbolVariable = $this->getExpected($compilationContext, $expression);
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException('Objects can only be cloned into dynamic variables', $expression);
        }

        $symbolVariable->setDynamicTypes('object');

        $symbolVariable->setIsInitialized(true, $compilationContext);

        /* Inherit the dynamic type data from the cloned object */
        $symbolVariable->setDynamicTypes($clonedVariable->getDynamicTypes());
        $symbolVariable->setClassTypes($clonedVariable->getClassTypes());

        $symbol = $compilationContext->backend->getVariableCode($symbolVariable);
        $clonedSymbol = $compilationContext->backend->getVariableCode($clonedVariable);

        $compilationContext->codePrinter->output('if (zephir_clone('.$symbol.', '.$clonedSymbol.') == FAILURE) {');
        $compilationContext->codePrinter->output("\t".'RETURN_MM();');
        $compilationContext->codePrinter->output('}');

        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
    }
}
