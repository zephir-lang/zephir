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
use Zephir\Exception\CompilerException;
use Zephir\Types\Types;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * Decr.
 *
 * Decrements a variable
 */
class Decr
{
    protected string $operator = '--';
    protected string $verb = 'decrement';
    protected string $warningName = 'non-valid-decrement';
    protected string $zephirMethod = 'zephir_decrement';

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
    public function assign(
        $variable,
        ZephirVariable $symbolVariable,
        CompilationContext $compilationContext,
        array $statement
    ): void {
        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException(
                "Cannot mutate variable '"
                . $variable
                . "' because it is not initialized",
                $statement
            );
        }

        if ($symbolVariable->isReadOnly()) {
            /*
             * TODO: implement increment of objects members
             */
            throw new CompilerException(
                "Cannot mutate variable '"
                . $variable
                . "' because it is read only",
                $statement
            );
        }

        $codePrinter = $compilationContext->codePrinter;

        switch ($symbolVariable->getType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
            case Types::T_DOUBLE:
            case Types::T_CHAR:
            case Types::T_UCHAR:
                $codePrinter->output($variable . $this->operator . ';');
                break;

            case Types::T_VARIABLE:
                /**
                 * Variable is probably not initialized here
                 */
                if ($symbolVariable->hasAnyDynamicType('unknown')) {
                    throw new CompilerException(
                        'Attempt to decrement uninitialized variable',
                        $statement
                    );
                }

                /**
                 * Increment/Decrement non-numeric variables could be expensive
                 */
                if (!$symbolVariable->hasAnyDynamicType(['undefined', 'int', 'long', 'double', 'uint'])) {
                    $compilationContext->logger->warning(
                        'Possible attempt to ' . $this->verb . ' non-numeric dynamic variable',
                        [$this->warningName, $statement]
                    );
                }

                $compilationContext->headersManager->add('kernel/operators');
                if (!$symbolVariable->isLocalOnly()) {
                    $symbolVariable->separate($compilationContext);
                }
                $codePrinter->output(
                    $this->zephirMethod
                    . '(' . $compilationContext->backend->getVariableCode($symbolVariable) . ');'
                );

                break;

            default:
                throw new CompilerException(
                    'Cannot ' . $this->verb . ' variable: '
                    . $symbolVariable->getType(),
                    $statement
                );
        }
    }
}
