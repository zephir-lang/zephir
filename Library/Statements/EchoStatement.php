<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;
use Zephir\Expression;

use function Zephir\add_slashes;

/**
 * EchoStatement.
 *
 * Produce output according to the type
 */
class EchoStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        foreach ($this->statement['expressions'] as $echoExpr) {
            $expr = new Expression($echoExpr);
            $expr->setReadOnly(true);
            $resolvedExpr = $expr->compile($compilationContext);

            switch ($resolvedExpr->getType()) {
                case 'int':
                    $compilationContext->codePrinter->output('php_printf("%d", '.$resolvedExpr->getCode().');');
                    break;

                case 'bool':
                    $compilationContext->codePrinter->output('php_printf("%s", '.$resolvedExpr->getBooleanCode().' ? "1": "");');
                    break;

                case 'double':
                    $compilationContext->codePrinter->output('php_printf("%f", '.$resolvedExpr->getCode().');');
                    break;

                case 'char':
                case 'uchar':
                    $compilationContext->codePrinter->output('php_printf("%c", \''.$resolvedExpr->getCode().'\');');
                    break;

                case 'long':
                    $compilationContext->codePrinter->output('php_printf("%ld", '.$resolvedExpr->getCode().');');
                    break;

                case 'string':
                    $compilationContext->codePrinter->output(
                        'php_printf("%s", "'.add_slashes($resolvedExpr->getCode()).'");'
                    );
                    break;

                case 'null':
                    break;

                case 'variable':
                    $variable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $echoExpr);
                    switch ($variable->getType()) {
                        case 'int':
                            $compilationContext->codePrinter->output('php_printf("%d", '.$variable->getName().');');
                            break;

                        case 'long':
                            $compilationContext->codePrinter->output('php_printf("%ld", '.$variable->getName().');');
                            break;

                        case 'double':
                            $compilationContext->codePrinter->output('php_printf("%f", '.$variable->getName().');');
                            break;

                        case 'uchar':
                        case 'char':
                            $compilationContext->codePrinter->output('php_printf("%c", '.$variable->getName().');');
                            break;

                        case 'bool':
                            $compilationContext->codePrinter->output('php_printf("%s", '.$variable->getName().' ? "1": "");');
                            break;

                        case 'string':
                        case 'variable':
                        case 'mixed':
                            $compilationContext->codePrinter->output('zend_print_zval('.$compilationContext->backend->getVariableCode($variable).', 0);');
                            break;

                        case 'null':
                            break;

                        default:
                            throw new CompilerException('Unknown type: '.$variable->getType(), $echoExpr);
                    }
                    break;

                default:
                    throw new CompilerException('Unknown type: '.$resolvedExpr->getType(), $echoExpr);
            }
        }
    }
}
