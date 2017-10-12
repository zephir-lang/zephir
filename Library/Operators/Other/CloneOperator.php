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

namespace Zephir\Operators\Other;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\Compiler\CompilerException;
use Zephir\CompiledExpression;

/**
 * Clone
 *
 * Clones an object into another one
 */
class CloneOperator extends BaseOperator
{
    /**
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/object');

        $exprVariable = new Expression($expression['left']);
        $exprVariable->setReadOnly(true);
        $exprVariable->setExpectReturn(true);

        $exprCompiledVariable = $exprVariable->compile($compilationContext);
        if ($exprCompiledVariable->getType() != 'variable') {
            throw new CompilerException("Expression type: " . $exprCompiledVariable->getType() . " cannot be used as array", $expression);
        }

        $clonedVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression);
        if ($clonedVariable->getType() != 'variable') {
            throw new CompilerException("Variable type: " . $exprVariable->getType() . " cannot be cloned");
        }

        if ($clonedVariable->hasDifferentDynamicType(array('undefined', 'object', 'null'))) {
            $compilationContext->logger->warning('Possible attempt to use non array in "clone" operator', 'non-valid-clone', $expression);
        }

        $symbolVariable = $this->getExpected($compilationContext, $expression);
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Objects can only be cloned into dynamic variables", $expression);
        }

        $symbolVariable->setDynamicTypes('object');

        $symbolVariable->setIsInitialized(true, $compilationContext, $expression);

        /* Inherit the dynamic type data from the cloned object */
        $symbolVariable->setDynamicTypes($clonedVariable->getDynamicTypes());
        $symbolVariable->setClassTypes($clonedVariable->getClassTypes());

        $symbol = $compilationContext->backend->getVariableCode($symbolVariable);
        $clonedSymbol = $compilationContext->backend->getVariableCode($clonedVariable);

        $compilationContext->codePrinter->output('if (zephir_clone(' . $symbol . ', ' . $clonedSymbol . ' TSRMLS_CC) == FAILURE) {');
        $compilationContext->codePrinter->output("\t" . 'RETURN_MM();');
        $compilationContext->codePrinter->output('}');

        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
    }
}
