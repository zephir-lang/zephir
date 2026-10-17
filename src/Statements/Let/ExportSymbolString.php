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
use Zephir\Exception\CompilerException;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * Exports a symbol to the current PHP symbol table using a string literal as name
 */
class ExportSymbolString extends ExportSymbol
{
    /**
     * Compiles {"var"} = {expr}.
     *
     * A literal `this` is rejected at compile time, as `${'this'} = $value` is
     * in PHP.
     */
    public function assign(
        ?ZephirVariable $symbolVariable = null,
        ?CompiledExpression $resolvedExpr = null,
        ?CompilationContext $compilationContext = null,
        array $statement = [],
    ): void {
        if ('this' === $statement['variable']) {
            throw new CompilerException('Cannot re-assign $this', $statement);
        }

        parent::assign($symbolVariable, $resolvedExpr, $compilationContext, $statement);
    }

    protected function setSymbolCall(
        CompilationContext $compilationContext,
        ?ZephirVariable $symbolVariable,
        array $statement,
        string $suffix = '',
    ): string {
        return 'zephir_set_symbol_str' . $suffix . '(SL("' . $statement['variable'] . '")';
    }
}
