<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use function Zephir\add_slashes;
use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * ImplodeOptimizer.
 *
 * Optimizes calls to 'implode' using internal function
 */
class ImplodeOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @throws CompilerException
     *
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (2 != \count($expression['parameters'])) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException('Returned values by functions can only be assigned to variant variables', $expression);
        }

        if ('string' == $expression['parameters'][0]['parameter']['type']) {
            $str = add_slashes($expression['parameters'][0]['parameter']['value']);
            unset($expression['parameters'][0]);
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $context->headersManager->add('kernel/string');
        $symbolVariable->setDynamicTypes('string');

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        if (isset($str)) {
            $context->codePrinter->output('zephir_fast_join_str('.$symbol.', SL("'.$str.'"), '.$resolvedParams[0].' TSRMLS_CC);');

            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $context->codePrinter->output('zephir_fast_join('.$symbol.', '.$resolvedParams[0].', '.$resolvedParams[1].' TSRMLS_CC);');

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
