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
 * JsonEncodeOptimizer.
 *
 * Optimizes calls to 'json_encode' using internal function
 */
class JsonEncodeOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return bool|CompiledExpression|mixed
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariable($symbolVariable, $expression);

        $context->headersManager->add('kernel/string');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        /*
         * Process encode options
         */
        if (count($resolvedParams) >= 2) {
            $context->headersManager->add('kernel/operators');
            $options = 'zephir_get_intval(' . $resolvedParams[1] . ') ';
        } else {
            $options = '0 ';
        }

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output(
            'zephir_json_encode(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $options . ');'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
