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
use Zephir\Types\Types;

use function in_array;

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
            /**
             * Only a `variable` or `array` result names a symbol: the code of
             * a string literal is its text, which may equal a variable's name.
             */
            $variable = in_array($resolvedParam->getType(), [Types::T_VARIABLE, Types::T_ARRAY], true)
                ? $context->symbolTable->getVariable($resolvedParam->getCode())
                : null;
            if (!$variable || !$variable->isVariable()) {
                /*
                 * Complex expressions require a temporary variable
                 */
                $type = match ($resolvedParam->getType()) {
                    Types::T_ARRAY => 'array',
                    default        => 'variable',
                };

                $variable = $context->symbolTable->addTemp($type, $context);
                $variable->initVariant($context);

                $statement = new LetStatement([
                    'type'        => 'let',
                    'assignments' => [
                        [
                            'assign-type' => $type,
                            'variable'    => $variable->getName(),
                            'operator'    => 'assign',
                            'expr'        => [
                                'type'  => $resolvedParam->getType(),
                                'value' => $resolvedParam->getCode(),
                                'file'  => $expression['file'],
                                'line'  => $expression['line'],
                                'char'  => $expression['char'],
                            ],
                            'file'        => $expression['file'],
                            'line'        => $expression['line'],
                            'char'        => $expression['char'],
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
            $context->codePrinter->output('zephir_var_dump(' . $symbol . ');');
        }

        return new CompiledExpression('null', 'null', $expression);
    }
}
