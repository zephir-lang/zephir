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
use Zephir\Optimizers\OptimizerAbstract;
use Zephir\Statements\LetStatement;
use Zephir\Traits\VarOptimizerTrait;
use Zephir\Types\Types;

/**
 * VarDumpOptimizer.
 *
 * Optimizes calls to 'var_dump' using internal function
 */
class VarDumpOptimizer extends OptimizerAbstract
{
    use VarOptimizerTrait;

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

        $context->headersManager->add('kernel/variables');
        $resolvedParams = $call->getResolvedParamsAsExpr($expression['parameters'], $context, $expression);

        foreach ($resolvedParams as $resolvedParam) {
            $variable = $context->symbolTable->getVariable($resolvedParam->getCode());
            $variable = $this->processVariable($variable, $resolvedParam, $context, $expression);

            $symbol = $context->backend->getVariableCode($variable);
            $context->codePrinter->output('zephir_var_dump(' . $symbol . ');');
        }

        return new CompiledExpression('null', 'null', $expression);
    }
}
