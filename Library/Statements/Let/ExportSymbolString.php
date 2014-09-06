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

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\Variable as ZephirVariable;
use Zephir\Detectors\ReadDetector;
use Zephir\Expression;
use Zephir\CompiledExpression;
use Zephir\Compiler;
use Zephir\Utils;
use Zephir\GlobalConstant;

/**
 * ExportSymbolString
 *
 * Exports a symbol to the current PHP symbol table using a variable as parameter
 */
class ExportSymbolString
{

    /**
     * Compiles {"var"} = {expr}
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext,
     * @param array $statement
     */
    public function assign($variable, ZephirVariable $symbolVariable = null, CompiledExpression $resolvedExpr = null, CompilationContext $compilationContext = null, $statement = null)
    {
        $codePrinter = $compilationContext->codePrinter;

        $codePrinter->output('if (zephir_set_symbol_str(SS("' . $statement["variable"] . '"), ' . $resolvedExpr->getCode() . ' TSRMLS_CC) == FAILURE){');
        $codePrinter->output('  return;');
        $codePrinter->output('}');
    }
}
