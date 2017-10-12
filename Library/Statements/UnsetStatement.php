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

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\Expression;

/**
 * Class UnsetStatement
 */
class UnsetStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/array');
        $expression = $this->_statement['expr'];

        $flags = 'PH_SEPARATE';

        if ($expression['type'] == 'list') {
            $expression = $expression['left'];
        }

        switch ($expression['type']) {
            case 'array-access':
                $expr = new Expression($expression['left']);
                $expr->setReadOnly(true);
                $exprVar = $expr->compile($compilationContext);
                $variable = $compilationContext->symbolTable->getVariableForWrite($exprVar->getCode(), $compilationContext, $this->_statement);

                $expr = new Expression($expression['right']);
                $expr->setReadOnly(true);
                $exprIndex = $expr->compile($compilationContext);
                break;

            case 'property-access':
                $expr = new Expression($expression['left']);
                $expr->setReadOnly(true);
                $exprVar = $expr->compile($compilationContext);
                $variable = $compilationContext->symbolTable->getVariableForWrite($exprVar->getCode(), $compilationContext, $this->_statement);
                $variableCode = $compilationContext->backend->getVariableCode($variable);

                $compilationContext->headersManager->add('kernel/object');
                $compilationContext->codePrinter->output('zephir_unset_property(' . $variableCode . ', "' . $expression['right']['value'] . '" TSRMLS_CC);');
                return true;

            case 'property-dynamic-access':
                //@todo fix it

            default:
                throw new CompilerException('Cannot use expression type: ' . $expression['type'] . ' in "unset"', $expression);
        }

        if (!in_array($variable->getType(), array('variable', 'array'))) {
            throw new CompilerException('Cannot use variable type: ' . $variable->gettype() . ' in "unset"', $expression['left']);
        }

        if ($variable->hasDifferentDynamicType(array('undefined', 'array', 'object', 'null'))) {
            $compilationContext->logger->warning('Possible attempt to use non array/object in unset operator', 'non-valid-unset', $expression['left']);
        }

        $compilationContext->backend->arrayUnset($variable, $exprIndex, $flags, $compilationContext);
    }
}
