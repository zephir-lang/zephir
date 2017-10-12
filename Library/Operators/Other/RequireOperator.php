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
use Zephir\CompiledExpression;
use Zephir\Compiler\CompilerException;

/**
 * Require
 *
 * Includes a plain PHP file
 */
class RequireOperator extends BaseOperator
{
    /**
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $expr = new Expression($expression['left']);
        $expr->setReadOnly(true);
        $expr->setExpectReturn(true);

        $exprPath = $expr->compile($compilationContext);
        $exprVar = $exprPath->getCode();
        if ($exprPath->getType() == 'variable') {
            $exprVariable = $compilationContext->symbolTable->getVariableForRead($exprPath->getCode(), $compilationContext, $expression);
            $exprVar = $compilationContext->backend->getVariableCode($exprVariable);
            if ($exprVariable->getType() == 'variable') {
                if ($exprVariable->hasDifferentDynamicType(array('undefined', 'string'))) {
                    $compilationContext->logger->warning('Possible attempt to use invalid type as path in "require" operator', 'non-valid-require', $expression);
                }
            }
        } else {
            $exprVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
            $compilationContext->backend->assignString($exprVar, $exprPath->getCode(), $compilationContext);
            $exprVar = $compilationContext->backend->getVariableCode($exprVar);
        }

        $symbolVariable = false;
        if ($this->isExpecting()) {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify('variable', $compilationContext, $expression);
        }

        $compilationContext->headersManager->add('kernel/memory');
        $compilationContext->headersManager->add('kernel/require');

        $codePrinter = $compilationContext->codePrinter;

        if ($symbolVariable) {
            $codePrinter->output('ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(&' . $symbolVariable->getName() . ');');
            $symbol = $compilationContext->backend->getVariableCodePointer($symbolVariable);
            $codePrinter->output('if (zephir_require_zval_ret(' . $symbol . ', ' . $exprVar . ' TSRMLS_CC) == FAILURE) {');
        } else {
            $codePrinter->output('if (zephir_require_zval(' . $exprVar . ' TSRMLS_CC) == FAILURE) {');
        }
        $codePrinter->output("\t" . 'RETURN_MM_NULL();');
        $codePrinter->output('}');

        if ($symbolVariable) {
            return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
        }

        return new CompiledExpression('null', null, $expression);
    }
}
