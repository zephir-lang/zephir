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
use Zephir\Utils;

/**
 * EchoStatement
 *
 * Produce output according to the type
 */
class EchoStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        foreach ($this->_statement['expressions'] as $echoExpr) {
            $expr = new Expression($echoExpr);
            $expr->setReadOnly(true);
            $resolvedExpr = $expr->compile($compilationContext);

            switch ($resolvedExpr->getType()) {

                case 'int':
                    $compilationContext->codePrinter->output('php_printf("%d", ' . $resolvedExpr->getCode() . ');');
                    break;

                case 'bool':
                    $compilationContext->codePrinter->output('php_printf("%s", ' . $resolvedExpr->getBooleanCode() . ' ? "1": "");');
                    break;

                case 'double':
                    $compilationContext->codePrinter->output('php_printf("%f", ' . $resolvedExpr->getCode() . ');');
                    break;

                case 'char':
                case 'uchar':
                    $compilationContext->codePrinter->output('php_printf("%c", \'' . $resolvedExpr->getCode() . '\');');
                    break;

                case 'long':
                    $compilationContext->codePrinter->output('php_printf("%ld", ' . $resolvedExpr->getCode() . ');');
                    break;

                case 'string':
                    $compilationContext->codePrinter->output('php_printf("%s", "' . Utils::addSlashes($resolvedExpr->getCode()) . '");');
                    break;

                case 'null':
                    break;

                case 'variable':
                    $variable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $echoExpr);
                    switch ($variable->getType()) {

                        case 'int':
                            $compilationContext->codePrinter->output('php_printf("%d", ' . $variable->getName() . ');');
                            break;

                        case 'long':
                            $compilationContext->codePrinter->output('php_printf("%ld", ' . $variable->getName() . ');');
                            break;

                        case 'double':
                            $compilationContext->codePrinter->output('php_printf("%f", ' . $variable->getName() . ');');
                            break;

                        case 'uchar':
                        case 'char':
                            $compilationContext->codePrinter->output('php_printf("%c", ' . $variable->getName() . ');');
                            break;

                        case 'bool':
                            $compilationContext->codePrinter->output('php_printf("%s", ' . $variable->getName() . ' ? "1": "");');
                            break;

                        case 'string':
                        case 'variable':
                            $compilationContext->codePrinter->output('zend_print_zval(' . $variable->getName() . ', 0);');
                            break;

                        case 'null':
                            break;

                        default:
                            throw new CompilerException("Unknown type: " . $variable->getType(), $echoExpr);
                    }
                    break;

                default:
                    throw new CompilerException("Unknown type: " . $resolvedExpr->getType(), $echoExpr);
            }
        }
    }
}
