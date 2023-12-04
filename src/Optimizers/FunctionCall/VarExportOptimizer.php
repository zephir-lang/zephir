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
use Zephir\Statements\LetStatement;
use Zephir\Traits\VarOptimizerTrait;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

/**
 * VarExportOptimizer.
 *
 * Optimizes calls to 'var_export' using internal function
 */
class VarExportOptimizer extends OptimizerAbstract
{
    use VarOptimizerTrait;

    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return CompiledExpression|mixed
     *
     * @throws CompilerException
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

        $symbolVariable = $call->getSymbolVariable();
        $this->checkNotVariable($symbolVariable, $expression);

        $context->headersManager->add('kernel/variables');
        $resolvedParams = $call->getResolvedParamsAsExpr($expression['parameters'], $context, $expression);
        $resolvedParam  = $resolvedParams[0];

        $variable = $this->processVariable($symbolVariable, $resolvedParam, $context, $expression);

        $variableSymbol = $context->backend->getVariableCode($variable);

        /*
         * let a = var_export(val);
         */
        if ($symbolVariable) {
            if ('return_value' == $symbolVariable->getName()) {
                $symbolVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
            } else {
                $symbolVariable->initVariant($context);
            }
            $symbol = $context->backend->getVariableCode($symbolVariable);
            $context->codePrinter->output('zephir_var_export_ex(' . $symbol . ', ' . $variableSymbol . ');');

            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $context->codePrinter->output('zephir_var_export(' . $variableSymbol . ');');

        return new CompiledExpression('null', 'null', $expression);
    }
}
