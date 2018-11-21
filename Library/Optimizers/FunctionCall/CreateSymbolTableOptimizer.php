<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * CreateSymbolTableOptimizer
 *
 * Built-in function that creates a virtual symbol table
 */
class CreateSymbolTableOptimizer extends OptimizerAbstract
{
    /**
     * @param  array                    $expression
     * @param  Call                     $call
     * @param  CompilationContext       $context
     * @return CompiledExpression|mixed
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (isset($expression['parameters'])) {
            if (count($expression['parameters']) != 0) {
                throw new CompilerException("This function doesn't require parameters", $expression);
            }
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable) {
            if (!$symbolVariable->isVariable()) {
                throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
            }
        }

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $context->headersManager->add('kernel/memory');

        $context->codePrinter->output('zephir_create_symbol_table(TSRMLS_C);');
        $context->codePrinter->output('');

        return new CompiledExpression('null', null, $expression);
    }
}
