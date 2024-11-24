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
use Zephir\Variable\Variable as ZephirVariable;

/**
 * Exports a symbol to the current PHP symbol table using a variable as parameter
 */
class ExportSymbolString extends ExportSymbol
{
    /**
     * Compiles {"var"} = {expr}.
     */
    protected function getOutput(
        ?CompilationContext $compilationContext,
        ?ZephirVariable $symbolVariable,
        ZephirVariable $variable,
        ?Printer $codePrinter,
        ?array $statement
    ): void {
        $symbol = $compilationContext->backend->getVariableCode($variable);
        $codePrinter->output(
            'if (zephir_set_symbol_str(SS("' . $statement['variable'] . '"), ' . $symbol . ') == FAILURE) {'
        );
        $codePrinter->output('  return;');
        $codePrinter->output('}');
    }
}
