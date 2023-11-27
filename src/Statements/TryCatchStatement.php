<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Expression\Builder\Operators\BinaryOperator;
use Zephir\StatementsBlock;

use function count;

/**
 * Try/Catch statement the same as in PHP
 */
class TryCatchStatement extends StatementAbstract
{
    public function compile(CompilationContext $compilationContext): void
    {
        $codePrinter = $compilationContext->codePrinter;

        ++$compilationContext->insideTryCatch;
        $currentTryCatch = ++$compilationContext->currentTryCatch;

        $codePrinter->outputBlankLine();
        $codePrinter->output('/* try_start_' . $currentTryCatch . ': */');
        $codePrinter->outputBlankLine();

        if (isset($this->statement['statements'])) {
            $st = new StatementsBlock($this->statement['statements']);
            $st->compile($compilationContext);
        }

        $codePrinter->outputBlankLine();
        $codePrinter->output('try_end_' . $currentTryCatch . ':');

        /**
         * If 'try' is the latest statement add a 'dummy' statement to avoid compilation errors
         */
        $codePrinter->outputBlankLine();

        --$compilationContext->insideTryCatch;

        if (isset($this->statement['catches'])) {
            /**
             * Check if there was an exception
             */
            $codePrinter->output('if (EG(exception)) {');
            $codePrinter->increaseLevel();

            $exc_var = $compilationContext->symbolTable->getTempVariableForWrite(
                'variable',
                $compilationContext,
                $compilationContext
            );
            $compilationContext->backend->copyOnWrite($exc_var, 'EG(exception)', $compilationContext);

            $exprBuilder = BuilderFactory::getInstance();
            $ifs         = [];

            foreach ($this->statement['catches'] as $catch) {
                if (isset($catch['variable'])) {
                    $variable = $compilationContext->symbolTable->getVariableForWrite(
                        $catch['variable']['value'],
                        $compilationContext,
                        $catch['variable']
                    );
                    if ('variable' != $variable->getType()) {
                        throw new CompilerException(
                            'Only dynamic variables can be used to catch exceptions',
                            $catch['exception']
                        );
                    }
                } else {
                    $variable = $compilationContext->symbolTable->getTempVariableForWrite(
                        'variable',
                        $compilationContext,
                        $compilationContext
                    );
                }

                $assignExceptionVarStmt = $exprBuilder->statements()->rawC(
                    'ZEPHIR_CPY_WRT(&' . $variable->getName() . ', &' . $exc_var->getName() . ');'
                );

                /**
                 * TODO:, use a builder here
                 */
                $variable->setIsInitialized(true, $compilationContext);
                $variable->setMustInitNull(true);

                /**
                 * Check if any of the classes in the catch block match the thrown exception
                 */
                foreach ($catch['classes'] as $class) {
                    $ifs[] = $exprBuilder->statements()->ifX()
                                         ->setCondition(
                                             $exprBuilder->operators()->binary(
                                                 BinaryOperator::OPERATOR_INSTANCEOF,
                                                 $exprBuilder->variable($exc_var->getName()),
                                                 $exprBuilder->variable($class['value'])
                                             )
                                         )
                                         ->setStatements(
                                             $exprBuilder->statements()->block(
                                                 array_merge(
                                                     [
                                                         $exprBuilder->statements()->rawC('zend_clear_exception();'),
                                                         $assignExceptionVarStmt,
                                                     ],
                                                     $catch['statements'] ?? []
                                                 )
                                             )
                                         )
                    ;
                }
            }

            $primaryIf = $ifs[0];
            $lastIf    = $ifs[0];
            for ($i = 1; $i < count($ifs); ++$i) {
                $lastIf->setElseStatements($exprBuilder->statements()->block([$ifs[$i]]));
                $lastIf = $ifs[$i];
            }

            $ifStatement = new IfStatement($primaryIf->build());
            $ifStatement->compile($compilationContext);

            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
        } else {
            $codePrinter->output('zend_clear_exception();');
        }
    }
}
