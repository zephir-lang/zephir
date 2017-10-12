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
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Expression\Builder\Operators\BinaryOperator;
use Zephir\StatementsBlock;

/**
 * TryCatchStatement
 *
 * Try/Catch statement the same as in PHP
 */
class TryCatchStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     */
    public function compile(CompilationContext $compilationContext)
    {
        $codePrinter = $compilationContext->codePrinter;

        $compilationContext->insideTryCatch++;
        $currentTryCatch = ++$compilationContext->currentTryCatch;

        $codePrinter->outputBlankLine();
        $codePrinter->output('/* try_start_' . $currentTryCatch . ': */');
        $codePrinter->outputBlankLine();

        if (isset($this->_statement['statements'])) {
            $st = new StatementsBlock($this->_statement['statements']);
            $st->compile($compilationContext);
        }

        $codePrinter->outputBlankLine();
        $codePrinter->output('try_end_' . $currentTryCatch . ':');

        /**
         * If 'try' is the latest statement add a 'dummy' statement to avoid compilation errors
         */
        $codePrinter->outputBlankLine();

        $compilationContext->insideTryCatch--;

        if (isset($this->_statement['catches'])) {
            /**
             * Check if there was an exception
             */
            $codePrinter->output('if (EG(exception)) {');
            $codePrinter->increaseLevel();

            $exc_var = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $compilationContext);
            $compilationContext->backend->copyOnWrite($exc_var, 'EG(exception)', $compilationContext);

            $exprBuilder = BuilderFactory::getInstance();
            $ifs         = array();

            foreach ($this->_statement['catches'] as $catch) {
                if (isset($catch['variable'])) {
                    $variable = $compilationContext->symbolTable->getVariableForWrite($catch['variable']['value'], $compilationContext, $catch['variable']);
                    if ($variable->getType() != 'variable') {
                        throw new CompilerException('Only dynamic variables can be used to catch exceptions', $catch['exception']);
                    }
                } else {
                    $variable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $compilationContext);
                }

                if ($compilationContext->backend->isZE3()) {
                    $assignExceptionVarStmt = $exprBuilder->statements()->rawC('ZEPHIR_CPY_WRT(&' . $variable->getName() . ', &' . $exc_var->getName() . ');');
                } else {
                    $assignExceptionVarStmt = $exprBuilder->statements()->rawC('ZEPHIR_CPY_WRT(' . $variable->getName() . ', ' . $exc_var->getName() . ');');
                }

                /**
                 * @TODO, use a builder here
                 */
                $variable->setIsInitialized(true, $compilationContext, $catch);
                $variable->setMustInitNull(true);

                /**
                 * Check if any of the classes in the catch block match the thrown exception
                 */
                foreach ($catch['classes'] as $class) {
                    $assignExceptVar = $exprBuilder->statements()->let(array(
                        $exprBuilder->operators()->assignVariable($variable->getName(), $exprBuilder->variable($variable->getName()))
                    ));

                    $assignExceptVarStmt = new \Zephir\Expression\Builder\Statements\LetStatement($assignExceptVar->build());

                    $ifs[] = $exprBuilder->statements()->ifX()
                        ->setCondition(
                            $exprBuilder->operators()->binary(
                                BinaryOperator::OPERATOR_INSTANCEOF,
                                $exprBuilder->variable($exc_var->getName()),
                                $exprBuilder->variable($class['value'])
                            )
                        )
                        ->setStatements($exprBuilder->statements()->block(array_merge(
                            array(
                                $exprBuilder->statements()->rawC('zend_clear_exception(TSRMLS_C);'),
                                 $assignExceptionVarStmt
                            ),
                            isset($catch['statements']) ? $catch['statements'] : array()
                        )));
                }
            }

            $primaryIf = $ifs[0];
            $lastIf    = $ifs[0];
            for ($i=1; $i<count($ifs); ++$i) {
                $lastIf->setElseStatements($exprBuilder->statements()->block(array($ifs[$i])));
                $lastIf = $ifs[$i];
            }

            $ifStatement = new IfStatement($primaryIf->build());
            $ifStatement->compile($compilationContext);

            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
        } else {
            $codePrinter->output('zend_clear_exception(TSRMLS_C);');
        }
    }
}
