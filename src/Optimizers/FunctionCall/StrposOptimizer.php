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
 * StrposOptimizer.
 *
 * Optimizes calls to 'strpos' using internal function
 */
class StrposOptimizer extends OptimizerAbstract
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

        if (count($expression['parameters']) < 2) {
            throw new CompilerException("'strpos' require two or three parameters");
        }

        /**
         * Process offset.
         */
        $offset = '0 ';
        if (count($expression['parameters']) >= 3 && 'int' == $expression['parameters'][2]['parameter']['type']) {
            $offset = $expression['parameters'][2]['parameter']['value'] . ' ';
            unset($expression['parameters'][2]);
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        if (count($resolvedParams) >= 3) {
            $context->headersManager->add('kernel/operators');
            $offset = 'zephir_get_intval(' . $resolvedParams[2] . ') ';
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $context->headersManager->add('kernel/string');
        $symbolVariable->setDynamicTypes('int');

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }
        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output(
            'zephir_fast_strpos(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $offset . ');'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
