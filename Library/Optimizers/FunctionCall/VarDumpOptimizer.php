<?php

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
use Zephir\Optimizers\OptimizerAbstract;
use Zephir\Statements\LetStatement;

/**
 * VarDumpOptimizer.
 *
 * Optimizes calls to 'var_dump' using internal function
 */
class VarDumpOptimizer extends OptimizerAbstract
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

        $context->headersManager->add('kernel/variables');
        $resolvedParams = $call->getResolvedParamsAsExpr($expression['parameters'], $context, $expression);

        foreach ($resolvedParams as $resolvedParam) {
            $variable = $context->symbolTable->getVariable($resolvedParam->getCode());
            if (!$variable || !$variable->isVariable()) {
                /*
                 * Complex expressions require a temporary variable
                 */
                switch ($resolvedParam->getType()) {
                    case 'array':
                        $type = 'array';
                        break;
                    default:
                        $type = 'variable';
                        break;
                }

                $variable = $context->symbolTable->addTemp($type, $context);
                $variable->initVariant($context);

                $statement = new LetStatement([
                    'type' => 'let',
                    'assignments' => [
                        [
                            'assign-type' => $type,
                            'variable' => $variable->getName(),
                            'operator' => 'assign',
                            'expr' => [
                                'type' => $resolvedParam->getType(),
                                'value' => $resolvedParam->getCode(),
                                'file' => $expression['file'],
                                'line' => $expression['line'],
                                'char' => $expression['char'],
                            ],
                            'file' => $expression['file'],
                            'line' => $expression['line'],
                            'char' => $expression['char'],
                        ],
                    ],
                ]);
                $statement->compile($context);
            } else {
                /**
                 * This mark the variable as used.
                 */
                $variable = $context->symbolTable->getVariableForRead($resolvedParam->getCode(), $context, $expression);
            }

            $symbol = $context->backend->getVariableCode($variable);
            $context->codePrinter->output('zephir_var_dump('.$symbol.');');
        }

        return new CompiledExpression('null', 'null', $expression);
    }
}
