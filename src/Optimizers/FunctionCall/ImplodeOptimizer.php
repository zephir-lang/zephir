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
use Zephir\Name;
use Zephir\Optimizers\OptimizerAbstract;

use function count;

/**
 * Optimizes calls to 'implode' using internal function
 */
class ImplodeOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return bool|CompiledExpression|mixed
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (2 != count($expression['parameters'])) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        if ('string' == $expression['parameters'][0]['parameter']['type']) {
            $str = Name::addSlashes($expression['parameters'][0]['parameter']['value']);
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
            $context->codePrinter->output(
                'zephir_fast_join_str(' . $symbol . ', SL("' . $str . '"), ' . $resolvedParams[0] . ');'
            );

            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $context->codePrinter->output(
            'zephir_fast_join(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
