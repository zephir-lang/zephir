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

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * CreateArrayOptimizer
 *
 * Built-in function that creates new instances of objects from its class name
 */
class CreateArrayOptimizer extends OptimizerAbstract
{
    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return CompiledExpression|mixed
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (isset($expression['parameters'])) {
            if (count($expression['parameters']) > 1) {
                throw new CompilerException("This function only requires one parameter", $expression);
            }
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
        }

        /**
         * Add the last call status to the current symbol table
         */
        $call->addCallStatusFlag($context);

        $context->headersManager->add('kernel/array');

        $symbolVariable->setDynamicTypes('array');

        if (isset($expression['parameters'])) {
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        } else {
            $resolvedParams = null;
        }

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }
        
        if ($resolvedParams) {
            $context->codePrinter->output('zephir_create_array(' . $symbolVariable->getName() . ', zephir_get_intval(' . $resolvedParams[0] . '), 1 TSRMLS_CC);');
        } else {
            $context->codePrinter->output('zephir_create_array(' . $symbolVariable->getName() . ', 0, 1 TSRMLS_CC);');
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
