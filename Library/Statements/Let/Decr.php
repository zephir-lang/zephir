<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;
use Zephir\Variable as ZephirVariable;

/**
 * Decr.
 *
 * Decrements a variable
 */
class Decr
{
    /**
     * Compiles x--.
     *
     * @param string             $variable
     * @param ZephirVariable     $symbolVariable
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @throws CompilerException
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompilationContext $compilationContext, $statement)
    {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is not initialized", $statement);
        }

        if ($symbolVariable->isReadOnly()) {
            throw new CompilerException("Cannot mutate variable '".$variable."' because it is read only", $statement);
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
                $codePrinter->output($variable.'--;');
                break;

            case 'variable':
                /*
                 * Variable is probably not initialized here
                 */
                if ($symbolVariable->hasAnyDynamicType('unknown')) {
                    throw new CompilerException('Attempt to decrement uninitialized variable', $statement);
                }

                /*
                 * Decrement non-numeric variables could be expensive
                 */
                if (!$symbolVariable->hasAnyDynamicType(['undefined', 'int', 'long', 'double', 'uint'])) {
                    $compilationContext->logger->warning(
                        'Possible attempt to decrement non-numeric dynamic variable',
                        ['non-valid-decrement', $statement]
                    );
                }

                $compilationContext->headersManager->add('kernel/operators');
                if (!$symbolVariable->isLocalOnly()) {
                    $symbolVariable->separate($compilationContext);
                }
                $codePrinter->output('zephir_decrement('.$compilationContext->backend->getVariableCode($symbolVariable).');');
                break;

            default:
                throw new CompilerException('Cannot decrement variable: '.$symbolVariable->getType(), $statement);
        }
    }
}
