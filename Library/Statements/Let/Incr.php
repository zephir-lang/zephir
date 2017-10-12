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
 * Incr
 *
 * Increments a variable
 */
class Incr
{
    /**
     * Compiles x++
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
            /**
             * @TODO implement increment of objects members
             */
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is read only", $statement);
        }

        $codePrinter = &$compilationContext->codePrinter;

        switch ($symbolVariable->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
            case 'char':
            case 'uchar':
                $codePrinter->output($variable . '++;');
                break;

            case 'variable':
                /**
                 * Update non-numeric dynamic variables could be expensive
                 */
                if (!$symbolVariable->hasAnyDynamicType(array('undefined', 'long', 'double'))) {
                    $compilationContext->logger->warning('Possible attempt to increment non-numeric dynamic variable', 'non-valid-increment', $statement);
                }

                $compilationContext->headersManager->add('kernel/operators');
                if (!$symbolVariable->isLocalOnly()) {
                    $symbolVariable->separate($compilationContext);
                }
                $symbol = $compilationContext->backend->getVariableCode($symbolVariable);
                $codePrinter->output('zephir_increment(' . $symbol . ');');
                break;

            default:
                throw new CompilerException("Cannot increment: " . $symbolVariable->getType(), $statement);
        }
    }
}
