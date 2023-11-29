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

/**
 * Incr.
 *
 * Increments a variable
 */
class Incr extends Decr
{
    protected string $operator     = '++';
    protected string $warningText  = 'increment';
    protected string $warningType  = 'non-valid-increment';
    protected string $zephirMethod = 'zephir_increment';
//    /**
//     * Compiles x++.
//     *
//     * @param string             $variable
//     * @param ZephirVariable     $symbolVariable
//     * @param CompilationContext $compilationContext
//     * @param array              $statement
//     *
//     * @throws CompilerException
//     */
//    public function assign(
//        $variable,
//        ZephirVariable $symbolVariable,
//        CompilationContext $compilationContext,
//        $statement
//    ): void {
//        if (!$symbolVariable->isInitialized()) {
//            throw new CompilerException(
//                "Cannot mutate variable '" . $variable . "' because it is not initialized",
//                $statement
//            );
//        }
//
//        if ($symbolVariable->isReadOnly()) {
//            /*
//             * TODO: implement increment of objects members
//             */
//            throw new CompilerException(
//                "Cannot mutate variable '"
//                . $variable
//                . "' because it is read only",
//                $statement
//            );
//        }
//
//        $codePrinter = &$compilationContext->codePrinter;
//
//        switch ($symbolVariable->getType()) {
//            case 'int':
//            case 'uint':
//            case 'long':
//            case 'ulong':
//            case 'double':
//            case 'char':
//            case 'uchar':
//                $codePrinter->output($variable . '++;');
//                break;
//
//            case 'variable':
//                /*
//                 * Update non-numeric dynamic variables could be expensive
//                 */
//                if (!$symbolVariable->hasAnyDynamicType(['undefined', 'long', 'double'])) {
//                    $compilationContext->logger->warning(
//                        'Possible attempt to increment non-numeric dynamic variable',
//                        ['non-valid-increment', $statement]
//                    );
//                }
//
//                $compilationContext->headersManager->add('kernel/operators');
//                if (!$symbolVariable->isLocalOnly()) {
//                    $symbolVariable->separate($compilationContext);
//                }
//                $symbol = $compilationContext->backend->getVariableCode($symbolVariable);
//                $codePrinter->output('zephir_increment(' . $symbol . ');');
//                break;
//
//            default:
//                throw new CompilerException('Cannot increment: ' . $symbolVariable->getType(), $statement);
//        }
//    }
}
