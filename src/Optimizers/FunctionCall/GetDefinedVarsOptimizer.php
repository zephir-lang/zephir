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

/**
 * GetDefinedVars.
 *
 * Optimizes calls to 'get_defined_vars' using internal function
 */
class GetDefinedVarsOptimizer extends OptimizerAbstract
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
        $call->processExpectedReturn($context);
        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->headersManager->add('kernel/variables');
        $context->codePrinter->output('zephir_get_defined_vars(' . $symbol . ');');

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
