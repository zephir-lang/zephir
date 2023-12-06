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

namespace Zephir\Statements\Let;

use Zephir\Code\Printer;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Statements\LetStatement;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * Exports a symbol to the current PHP symbol table using a variable as parameter
 */
class ExportSymbol
{
    /**
     * Compiles {var} = {expr}.
     *
     * @param string                  $variable
     * @param ZephirVariable|null     $symbolVariable
     * @param CompiledExpression|null $resolvedExpr
     * @param CompilationContext|null $compilationContext ,
     * @param array|null              $statement
     */
    public function assign(
        $variable,
        ZephirVariable $symbolVariable = null,
        CompiledExpression $resolvedExpr = null,
        CompilationContext $compilationContext = null,
        $statement = null
    ): void {
        $codePrinter = $compilationContext->codePrinter;

        $variable = $compilationContext->symbolTable->getTempVariable(
            'variable',
            $compilationContext
        );
        $variable->setMustInitNull(true);

        $letStatement = new LetStatement([
            'type'        => 'let',
            'assignments' => [
                [
                    'assign-type' => 'variable',
                    'variable'    => $variable->getName(),
                    'operator'    => 'assign',
                    'expr'        => [
                        'type'  => $resolvedExpr->getType(),
                        'value' => $resolvedExpr->getCode(),
                        'file'  => $statement['file'],
                        'line'  => $statement['line'],
                        'char'  => $statement['char'],
                    ],
                    'file'        => $statement['file'],
                    'line'        => $statement['line'],
                    'char'        => $statement['char'],
                ],
            ],
        ]);
        $letStatement->compile($compilationContext);

        $this->getOutput(
            $compilationContext,
            $symbolVariable,
            $variable,
            $codePrinter,
            $statement
        );
    }

    /**
     * @param CompilationContext|null   $compilationContext
     * @param ZephirVariable|null       $symbolVariable
     * @param ZephirVariable            $variable
     * @param Printer|null              $codePrinter
     * @param array|null                $statement
     *
     * @return void
     */
    protected function getOutput(
        ?CompilationContext $compilationContext,
        ?ZephirVariable $symbolVariable,
        ZephirVariable $variable,
        ?Printer $codePrinter,
        ?array $statement
    ): void {
        $symbol   = $compilationContext->backend->getVariableCode($symbolVariable);
        $variable = $compilationContext->backend->getVariableCode($variable);

        $codePrinter->output('if (zephir_set_symbol(' . $symbol . ', ' . $variable . ') == FAILURE) {');
        $codePrinter->output("\t" . 'return;');
        $codePrinter->output('}');
    }
}
