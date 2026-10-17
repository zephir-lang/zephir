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

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Statements\LetStatement;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * Exports a symbol to the current PHP symbol table using a variable as parameter
 *
 * `let {name} = value` and `let {name} op= value` behave as PHP's
 * `${$name} = $value` and `${$name} op= $value`: the kernel converts the name,
 * reads the old value for a compound operator and writes through the caller's
 * own variables. A failure leaves an exception pending, which stops the method.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2710
 */
class ExportSymbol
{
    /**
     * Compiles {var} = {expr}.
     */
    public function assign(
        ?ZephirVariable $symbolVariable = null,
        ?CompiledExpression $resolvedExpr = null,
        ?CompilationContext $compilationContext = null,
        array $statement = [],
    ): void {
        $variable = $compilationContext->symbolTable->getTempVariable(
            'variable',
            $compilationContext
        );
        $variable->setMustInitNull(true);

        /**
         * A compiled array literal is already a variable holding the array;
         * re-read as an `array` node it would be compiled again from its C name.
         */
        $exprType = 'array' === $resolvedExpr->getType() ? 'variable' : $resolvedExpr->getType();

        $letStatement = new LetStatement([
            'type'        => 'let',
            'assignments' => [
                [
                    'assign-type' => 'variable',
                    'variable'    => $variable->getName(),
                    'operator'    => 'assign',
                    'expr'        => [
                        'type'  => $exprType,
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

        $value    = $compilationContext->backend->getVariableCode($variable);
        $operator = $statement['operator'] ?? 'assign';
        $function = ArrayIndexOperator::OPERATOR_FUNCTIONS[$operator] ?? null;

        $compilationContext->codePrinter->output(
            null === $function
                ? $this->setSymbolCall($compilationContext, $symbolVariable, $statement) . ', ' . $value . ');'
                : $this->setSymbolCall($compilationContext, $symbolVariable, $statement, '_op')
                    . ', ' . $value . ', ' . $function . ');'
        );
        $compilationContext->emitExceptionCheck();
    }

    /**
     * The kernel call up to its value argument, `zephir_set_symbol<suffix>(name`.
     */
    protected function setSymbolCall(
        CompilationContext $compilationContext,
        ?ZephirVariable $symbolVariable,
        array $statement,
        string $suffix = '',
    ): string {
        return 'zephir_set_symbol' . $suffix . '(' . $compilationContext->backend->getVariableCode($symbolVariable);
    }
}
