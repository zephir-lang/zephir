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

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\Variable as ZephirVariable;

/**
 * Decr
 *
 * Decrements a variable
 */
class Decr
{
    /**
     * Compiles x--
     *
     * @param string $variable
     * @param ZephirVariable $symbolVariable
     * @param CompilationContext $compilationContext
     * @param array $statement
     *
     * @throws CompilerException
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompilationContext $compilationContext, $statement)
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is not initialized", $statement);
        }

        if ($symbolVariable->isReadOnly()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is read only", $statement);
        }

        $codePrinter = $compilationContext->codePrinter;

        switch ($symbolVariable->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
            case 'char':
            case 'uchar':
                $codePrinter->output($variable . '--;');
                break;

            case 'variable':
                /**
                 * Variable is probably not initialized here
                 */
                if ($symbolVariable->hasAnyDynamicType('unknown')) {
                    throw new CompilerException("Attempt to decrement uninitialized variable", $statement);
                }

                /**
                 * Decrement non-numeric variables could be expensive
                 */
                if (!$symbolVariable->hasAnyDynamicType(array('undefined', 'int', 'long', 'double', 'uint'))) {
                    $compilationContext->logger->warning('Possible attempt to decrement non-numeric dynamic variable', 'non-valid-decrement', $statement);
                }

                $compilationContext->headersManager->add('kernel/operators');
                if (!$symbolVariable->isLocalOnly()) {
                    $symbolVariable->separate($compilationContext);
                }
                $codePrinter->output('zephir_decrement(' . $compilationContext->backend->getVariableCode($symbolVariable) . ');');
                break;

            default:
                throw new CompilerException("Cannot decrement variable: " . $symbolVariable->getType(), $statement);
        }
    }
}
