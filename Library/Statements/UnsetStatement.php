<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\CompilerException;
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

                $expr = new Expression($expression['right']);
                $expr->setReadOnly(true);
                $exprIndex = $expr->compile($compilationContext);
                break;

            case 'property-access':
                $expr = new Expression($expression['left']);
                $expr->setReadOnly(true);
                $exprVar = $expr->compile($compilationContext);

                $compilationContext->headersManager->add('kernel/object');
                if ($exprVar->getCode() == 'this') {
                    $compilationContext->codePrinter->output('zephir_unset_property(this_ptr, "' . $expression['right']['value'] . '" TSRMLS_CC);');
                } else {
                    $compilationContext->codePrinter->output('zephir_unset_property(' . $exprVar->getCode() . ', "' . $expression['right']['value'] . '" TSRMLS_CC);');
                }
                return true;

            case 'property-dynamic-access':
                //@todo fix it

            default:
                throw new CompilerException('Cannot use expression type: ' . $expression['type'] . ' in "unset"', $expression);
        }

        $variable = $compilationContext->symbolTable->getVariableForWrite($exprVar->getCode(), $compilationContext, $this->_statement);
        if (!in_array($variable->getType(), array('variable', 'array'))) {
            throw new CompilerException('Cannot use variable type: ' . $variable->gettype() . ' in "unset"', $expression['left']);
        }

        if ($variable->hasDifferentDynamicType(array('undefined', 'array', 'object', 'null'))) {
            $compilationContext->logger->warning('Possible attempt to use non array/object in unset operator', 'non-valid-unset', $expression['left']);
        }

        switch ($exprIndex->getType()) {

            case 'int':
            case 'uint':
            case 'long':
                $compilationContext->headersManager->add('kernel/array');
                $compilationContext->codePrinter->output('zephir_array_unset_long(&' . $variable->getName() . ', ' . $exprIndex->getCode() . ', ' . $flags . ');');
                break;

            case 'string':
                $compilationContext->codePrinter->output('zephir_array_unset_string(&' . $variable->getName() . ', SS("' . $exprIndex->getCode() . '"), ' . $flags . ');');
                break;

            case 'variable':
                $variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $compilationContext, $exprIndex->getOriginal());
                switch ($variableIndex->getType()) {

                    case 'int':
                    case 'uint':
                    case 'long':
                        $compilationContext->headersManager->add('kernel/array');
                        $compilationContext->codePrinter->output('zephir_array_unset_long(&' . $variable->getName() . ', ' . $variableIndex->getName() . ', ' . $flags . ');');
                        break;

                    case 'string':
                    case 'variable':
                        $compilationContext->headersManager->add('kernel/array');
                        $compilationContext->codePrinter->output('zephir_array_unset(&' . $variable->getName() . ', ' . $variableIndex->getName() . ', ' . $flags . ');');
                        break;

                    default:
                        throw new CompilerException("Variable type: " . $variableIndex->getType() . " cannot be used as array index without cast", $expression['right']);
                }
                break;

            default:
                throw new CompilerException("Cannot use expression: " . $exprIndex->getType() . " as array index without cast", $expression['right']);
        }
    }
}
