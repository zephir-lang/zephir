<?php

/*
 +--------------------------------------------------------------------------+
| Zephir Language                                                          |
+--------------------------------------------------------------------------+
| Copyright (c) 2013-2016 Zephir Team and contributors                     |
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
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * GetDefinedVars
 *
 * Optimizes calls to 'get_defined_vars' using internal function
 */
class GetDefinedVarsOptimizer extends OptimizerAbstract
{
    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        $call->processExpectedReturn($context);
        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
        }

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->headersManager->add('kernel/variables');
        if ($context->backend->isZE3()) {
            $context->codePrinter->output('zephir_get_defined_vars(' . $symbol . ');');
        } else {
            $context->codePrinter->output('zephir_get_defined_vars(' . $symbol . ' TSRMLS_CC);');
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
