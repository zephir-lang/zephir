<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

use Zephir\Utils;
use Zephir\Compiler;
use Zephir\Expression;
use Zephir\CompilationContext;
use Zephir\CompilerException;

/**
 * ThrowStatement
 *
 * Throws exceptions
 */
class ThrowStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/exception');

        $codePrinter = $compilationContext->codePrinter;
        $statement = $this->_statement;
        $expr = $statement['expr'];

        /**
         * This optimizes throw new Exception("hello")
         */
        if (!$compilationContext->insideTryCatch) {
            if (isset($expr['class'])) {
                if (isset($expr['parameters']) && count($expr['parameters']) == 1) {
                    if ($expr['parameters'][0]['parameter']['type'] == 'string') {
                        $className = Utils::getFullName($expr['class'], $compilationContext->classDefinition->getNamespace(), $compilationContext->aliasManager);
                        if ($compilationContext->compiler->isClass($className)) {
                            $classDefinition = $compilationContext->compiler->getClassDefinition($className);
                            $message = $expr['parameters'][0]['parameter']['value'];
                            $codePrinter->output('ZEPHIR_THROW_EXCEPTION_DEBUG_STR(' . $classDefinition->getClassEntry() . ', "' . Utils::addSlashes($message) . '", "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                            $codePrinter->output('return;');
                            return;
                        } else {
                            if ($compilationContext->compiler->isInternalClass($className)) {
                                $classEntry = $compilationContext->classDefinition->getClassEntryByClassName($className, true);
                                if ($classEntry) {
                                    $message = $expr['parameters'][0]['parameter']['value'];
                                    $codePrinter->output('ZEPHIR_THROW_EXCEPTION_DEBUG_STR(' . $classEntry . ', "' . Utils::addSlashes($message) . '", "' . Compiler::getShortUserPath($statement['expr']['file']) . '", ' . $statement['expr']['line'] . ');');
                                    $codePrinter->output('return;');
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }

        $throwExpr = new Expression($expr);
        $resolvedExpr = $throwExpr->compile($compilationContext);

        if ($resolvedExpr->getType() != 'variable') {
            throw new CompilerException("Expression '" . $resolvedExpr->getType() . '" cannot be used as exception', $expr);
        }

        $variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $expr);
        if ($variableVariable->getType() != 'variable') {
            throw new CompilerException("Variable '" . $variableVariable->getType() . "' cannot be used as exception", $expr);
        }

        $codePrinter->output('zephir_throw_exception_debug(' . $variableVariable->getName() . ', "' . Compiler::getShortUserPath($statement['expr']['file']) . '", ' . $statement['expr']['line'] . ' TSRMLS_CC);');
        if (!$compilationContext->insideTryCatch) {
            $codePrinter->output('ZEPHIR_MM_RESTORE();');
            $codePrinter->output('return;');
        } else {
            $codePrinter->output('goto try_end_' . $compilationContext->insideTryCatch . ';');
            $codePrinter->outputBlankLine();
        }

        if ($variableVariable->isTemporal()) {
            $variableVariable->setIdle(true);
        }
    }
}
