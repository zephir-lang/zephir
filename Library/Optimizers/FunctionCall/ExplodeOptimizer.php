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

use function Zephir\add_slashes;
use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * ExplodeOptimizer.
 *
 * Optimizes calls to 'explode' using internal function
 */
class ExplodeOptimizer extends OptimizerAbstract
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
        if (\count($expression['parameters']) < 2) {
            throw new CompilerException("'explode' require two parameter");
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException('Returned values by functions can only be assigned to variant variables', $expression);
        }

        /**
         * Process limit.
         */
        $limit = 'LONG_MAX';
        $limitOffset = 2;
        if (3 == \count($expression['parameters']) && 'int' == $expression['parameters'][2]['parameter']['type']) {
            $limit = $expression['parameters'][2]['parameter']['value'].' ';
            unset($expression['parameters'][2]);
        }

        if ('string' == $expression['parameters'][0]['parameter']['type']) {
            $str = add_slashes($expression['parameters'][0]['parameter']['value']);
            unset($expression['parameters'][0]);
            if (2 == \count($expression['parameters'])) {
                $limitOffset = 1;
            }
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if (isset($resolvedParams[$limitOffset])) {
            $context->headersManager->add('kernel/operators');
            $limit = 'zephir_get_intval('.$resolvedParams[$limitOffset].') ';
        }

        $context->headersManager->add('kernel/string');
        $symbolVariable->setDynamicTypes('array');
        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        if (isset($str)) {
            $context->codePrinter->output('zephir_fast_explode_str('.$symbol.', SL("'.$str.'"), '.$resolvedParams[0].', '.$limit.');');
            $symbolVariable->addMemoryEntry($context);
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $context->codePrinter->output('zephir_fast_explode('.$symbol.', '.$resolvedParams[0].', '.$resolvedParams[1].', '.$limit.');');
        $symbolVariable->addMemoryEntry($context);

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
