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

use Zephir\CodePrinter;
use Zephir\CompilationContext;
use Zephir\Compiler;
use Zephir\CompilerException;
use Zephir\Expression;
use Zephir\Types;
use Zephir\Utils;

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
            if (isset($expr['class']) &&
                isset($expr['parameters']) &&
                count($expr['parameters']) == 1 &&
                $expr['parameters'][0]['parameter']['type'] == 'string'
            ) {
                $className = Utils::getFullName($expr['class'], $compilationContext->classDefinition->getNamespace(), $compilationContext->aliasManager);
                if ($compilationContext->compiler->isClass($className)) {
                    $classDefinition = $compilationContext->compiler->getClassDefinition($className);
                    $message = $expr['parameters'][0]['parameter']['value'];
                    $class = $classDefinition->getClassEntry();
                    $this->throwStringException($codePrinter, $class, $message, $statement['expr']);
                    return;
                } else {
                    if ($compilationContext->compiler->isBundledClass($className)) {
                        $classEntry = $compilationContext->classDefinition->getClassEntryByClassName($className, $compilationContext, true);
                        if ($classEntry) {
                            $message = $expr['parameters'][0]['parameter']['value'];
                            $this->throwStringException($codePrinter, $classEntry, $message, $statement['expr']);
                            return;
                        }
                    }
                }
            } else {
                if (in_array($expr['type'], array('string', 'char', 'int', 'double'))) {
                    $class = $compilationContext->classDefinition->getClassEntryByClassName('Exception', $compilationContext);
                    $this->throwStringException($codePrinter, $class, $expr['value'], $expr);
                    return;
                }
            }
        }

        $throwExpr = new Expression($expr);
        $resolvedExpr = $throwExpr->compile($compilationContext);

        if (!in_array($resolvedExpr->getType(), array('variable', 'string'))) {
            throw new CompilerException("Expression '" . $resolvedExpr->getType() . '" cannot be used as exception', $expr);
        }

        $variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $expr);
        if (!in_array($variableVariable->getType(), array('variable', 'string'))) {
            throw new CompilerException("Variable '" . $variableVariable->getType() . "' cannot be used as exception", $expr);
        }

        $codePrinter->output('zephir_throw_exception_debug(' . $variableVariable->getName() . ', "' . Compiler::getShortUserPath($statement['expr']['file']) . '", ' . $statement['expr']['line'] . ' TSRMLS_CC);');
        if (!$compilationContext->insideTryCatch) {
            $codePrinter->output('ZEPHIR_MM_RESTORE();');
            $codePrinter->output('return;');
        } else {
            $codePrinter->output('goto try_end_' . $compilationContext->currentTryCatch . ';');
            $codePrinter->outputBlankLine();
        }

        if ($variableVariable->isTemporal()) {
            $variableVariable->setIdle(true);
        }
    }

    /**
     * Throws an exception escaping the data
     *
     * @param CodePrinter $printer
     * @param string $class
     * @param string $message
     * @param array $expression
     */
    private function throwStringException(CodePrinter $printer, $class, $message, $expression)
    {
        $message = Utils::addSlashes($message);
        $path = Compiler::getShortUserPath($expression['file']);
        $printer->output(
            sprintf('ZEPHIR_THROW_EXCEPTION_DEBUG_STR(%s, "%s", "%s", %s);', $class, $message, $path, $expression['line'])
        );
        $printer->output('return;');
    }
}
