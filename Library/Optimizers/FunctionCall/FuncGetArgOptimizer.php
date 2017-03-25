<?php
/**
 * Created by PhpStorm.
 * User: Wojtek
 * Date: 2017-03-25
 * Time: 11:07
 */

namespace Zephir\Optimizers\FunctionCall;


use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

class FuncGetArgOptimizer extends OptimizerAbstract
{
    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return bool|CompiledExpression|mixed
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
        }

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if ($context->backend->isZE3()) {
            $context->codePrinter->output('zephir_func_get_arg('.$symbol.', '.$resolvedParams[0].' TSRMLS_CC);');
        } else {
            $context->codePrinter->output('zephir_func_get_arg('.$resolvedParams[0].' TSRMLS_CC);');
        }
        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}