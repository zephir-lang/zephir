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
 * StrReplaceOptimizer.
 *
 * Optimizes calls to 'str_replace' using internal function
 */
class StrReplaceOptimizer extends OptimizerAbstract
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

        if (3 != count($expression['parameters'])) {
            if (4 == count($expression['parameters'])) {
                return false;
            }
            throw new CompilerException("'str_replace' only accepts three parameter", $expression);
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $context->headersManager->add('kernel/string');

        $symbolVariable->setDynamicTypes('string');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if ($call->mustInitSymbolVariable()) {
            if ('return_value' == $symbolVariable->getName()) {
                $symbolVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
            } else {
                $symbolVariable->initVariant($context);
            }
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);

        $context->codePrinter->output(
            'zephir_fast_str_replace(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $resolvedParams[2] . ');'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
