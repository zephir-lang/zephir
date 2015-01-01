<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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
                if ($symbolVariable->isLocalOnly()) {
                    $codePrinter->output('zephir_increment(&' . $variable . ');');
                } else {
                    $symbolVariable->separate($compilationContext);
                    $codePrinter->output('zephir_increment(' . $variable . ');');
                }
                break;

            default:
                throw new CompilerException("Cannot increment: " . $symbolVariable->getType(), $statement);
        }
    }
}
