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

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

use function count;

/**
 * CreateSymbolTableOptimizer.
 *
 * Built-in function that creates a virtual symbol table
 */
class CreateSymbolTableOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return CompiledExpression|mixed
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (isset($expression['parameters'])) {
            if (0 != count($expression['parameters'])) {
                throw new CompilerException("This function doesn't require parameters", $expression);
            }
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariable($symbolVariable, $expression);

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);


        // TODO: Still needed?
        $context->symbolTable->mustGrownStack(true);
        $context->codePrinter->output('ZEPHIR_CREATE_SYMBOL_TABLE();');
        $context->codePrinter->output('');

        return new CompiledExpression('null', null, $expression);
    }
}
