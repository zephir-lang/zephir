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
use Zephir\Traits\VariablesTrait;
use Zephir\Types\Types;
use Zephir\Variable\Variable as ZephirVariable;

/**
 * Decrements a variable
 */
class Decr
{
    use VariablesTrait;

    protected string $operator     = '--';
    protected string $warningText  = 'decrement';
    protected string $warningType  = 'non-valid-decrement';
    protected string $zephirMethod = 'zephir_decrement';

    /**
     * Compiles x++/x--.
     *
     * @throws CompilerException
     */
    public function assign(
        string $variable,
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

        /**
         * TODO: implement increment of objects members
         */
        $this->checkVariableReadOnly($variable, $symbolVariable, $statement);

        switch ($symbolVariable->getType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
            case Types::T_DOUBLE:
            case Types::T_CHAR:
            case Types::T_UCHAR:
                $compilationContext->codePrinter->output($variable . $this->operator . ';');
                break;

            case Types::T_VARIABLE:
                /**
                 * Variable is probably not initialized here
                 */
                if ($symbolVariable->hasAnyDynamicType('unknown')) {
                    throw new CompilerException(
                        'Attempt to '
                        . $this->warningText
                        . ' uninitialized variable',
                        $statement
                    );
                }

                /**
                 * Decrement non-numeric variables could be expensive
                 */
                if (!$symbolVariable->hasAnyDynamicType(['undefined', 'int', 'long', 'double', 'uint'])) {
                    $compilationContext->logger->warning(
                        'Possible attempt to '
                        . $this->warningText
                        . ' non-numeric dynamic variable',
                        [$this->warningType, $statement]
                    );
                }

                $compilationContext->headersManager->add('kernel/operators');
                if (!$symbolVariable->isLocalOnly()) {
                    $symbolVariable->separate($compilationContext);
                }

                $symbol = $compilationContext->backend->getVariableCode($symbolVariable);
                $compilationContext->codePrinter->output($this->zephirMethod . '(' . $symbol . ');');
                break;

            default:
                throw new CompilerException(
                    'Cannot '
                    . $this->warningText
                    . ' variable: '
                    . $symbolVariable->getType(),
                    $statement
                );
        }
    }
}
