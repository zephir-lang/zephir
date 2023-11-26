<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * SubstrOptimizer.
 *
 * Optimizes calls to 'strpos' using internal function
 */
class SubstrOptimizer extends OptimizerAbstract
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

        if (\count($expression['parameters']) < 2 || \count($expression['parameters']) > 3) {
            throw new CompilerException("'substr' require two or three parameters");
        }

        /**
         * Process parameters.
         */
        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
        $params = [];
        $flags = '0';

        for ($param = 1; $param <= 2; ++$param) {
            if (!isset($expression['parameters'][$param])) {
                $params[] = '0';
                if (2 == $param) {
                    $flags = 'ZEPHIR_SUBSTR_NO_LENGTH';
                }
                continue;
            }
            if ('int' == $expression['parameters'][$param]['parameter']['type']) {
                $params[] = $expression['parameters'][$param]['parameter']['value'].' ';
            } else {
                $context->headersManager->add('kernel/operators');
                $params[] = 'zephir_get_intval('.$resolvedParams[$param].')';
            }
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if ($symbolVariable->isNotVariableAndString()) {
            throw new CompilerException('Returned values by functions can only be assigned to variant variables', $expression);
        }

        $context->headersManager->add('kernel/string');

        $symbolVariable->setDynamicTypes('string');

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }
        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output('zephir_substr('.$symbol.', '.$resolvedParams[0].', '.$params[0].', '.$params[1].', '.$flags.');');

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
