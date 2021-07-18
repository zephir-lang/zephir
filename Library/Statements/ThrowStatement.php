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

use Zephir\Classes\Entry;
use Zephir\CodePrinter;
use Zephir\CompilationContext;
use Zephir\Compiler;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;

use function Zephir\add_slashes;
use function Zephir\fqcn;

/**
 * ThrowStatement.
 *
 * Throws exceptions
 */
class ThrowStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     *
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/exception');

        $codePrinter = $compilationContext->codePrinter;
        $statement = $this->statement;
        $expr = $statement['expr'];

        /*
         * This optimizes throw new Exception("hello")
         */
        if (!$compilationContext->insideTryCatch) {
            if (isset($expr['class']) &&
                isset($expr['parameters']) &&
                1 == \count($expr['parameters']) &&
                'string' == $expr['parameters'][0]['parameter']['type']
            ) {
                $className = fqcn(
                    $expr['class'],
                    $compilationContext->classDefinition->getNamespace(),
                    $compilationContext->aliasManager
                );

                if ($compilationContext->compiler->isClass($className)) {
                    $classDefinition = $compilationContext->compiler->getClassDefinition($className);
                    $message = $expr['parameters'][0]['parameter']['value'];
                    try {
                        $class = $classDefinition->getClassEntry($compilationContext);
                        $this->throwStringException($codePrinter, $class, $message, $expr);

                        return;
                    } catch (Exception $e) {
                        throw new CompilerException($e->getMessage(), $expr, $e->getCode(), $e);
                    }
                } else {
                    if ($compilationContext->compiler->isBundledClass($className)) {
                        $classEntry = (new Entry($className, $compilationContext))->get();
                        $message = $expr['parameters'][0]['parameter']['value'];
                        $this->throwStringException($codePrinter, $classEntry, $message, $expr);

                        return;
                    }
                }
            } else {
                if (\in_array($expr['type'], ['string', 'char', 'int', 'double'])) {
                    $class = (new Entry('Exception', $compilationContext))->get();

                    $this->throwStringException($codePrinter, $class, $expr['value'], $expr);

                    return;
                }
            }
        }

        try {
            $throwExpr = new Expression($expr);
            $resolvedExpr = $throwExpr->compile($compilationContext);
        } catch (Exception $e) {
            throw new CompilerException($e->getMessage(), $expr, $e->getCode(), $e);
        }

        if (!\in_array($resolvedExpr->getType(), ['variable', 'string'])) {
            throw new CompilerException(
                "Expression '".$resolvedExpr->getType().'" cannot be used as exception',
                $expr
            );
        }

        $variableVariable = $compilationContext->symbolTable->getVariableForRead(
            $resolvedExpr->getCode(),
            $compilationContext,
            $expr
        );

        if (!\in_array($variableVariable->getType(), ['variable', 'string'])) {
            throw new CompilerException(
                "Variable '".$variableVariable->getType()."' cannot be used as exception",
                $expr
            );
        }

        $variableCode = $compilationContext->backend->getVariableCode($variableVariable);
        $file = Compiler::getShortUserPath($statement['expr']['file']);
        $line = $statement['expr']['line'];

        $codePrinter->output(
            'zephir_throw_exception_debug('.$variableCode.', "'.$file.'", '.$line.');'
        );

        if (!$compilationContext->insideTryCatch) {
            $codePrinter->output('ZEPHIR_MM_RESTORE();');
            $codePrinter->output('return;');
        } else {
            $codePrinter->output('goto try_end_'.$compilationContext->currentTryCatch.';');
            $codePrinter->outputBlankLine();
        }

        if ($variableVariable->isTemporal()) {
            $variableVariable->setIdle(true);
        }
    }

    /**
     * Throws an exception escaping the data.
     *
     * @param CodePrinter $printer
     * @param string      $class
     * @param string      $message
     * @param array       $expression
     */
    private function throwStringException(CodePrinter $printer, string $class, string $message, array $expression): void
    {
        $message = add_slashes($message);
        $path = Compiler::getShortUserPath($expression['file']);
        $printer->output(
            sprintf(
                'ZEPHIR_THROW_EXCEPTION_DEBUG_STR(%s, "%s", "%s", %s);',
                $class,
                $message,
                $path,
                $expression['line']
            )
        );
        $printer->output('return;');
    }
}
