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
use Zephir\Types\Types;
use Zephir\Variable\Variable;

use function count;
use function in_array;

/**
 * VarExportOptimizer.
 *
 * Optimizes calls to 'var_export' using internal function
 */
class VarExportOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return CompiledExpression|bool
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        /**
         * Decided before any argument is compiled: declining afterwards would
         * have the engine call compile them a second time.
         */
        $returnOutput = $this->resolveReturnArgument($expression['parameters'] ?? []);
        if (null === $returnOutput) {
            return false;
        }

        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable();
        $this->checkNotVariable($symbolVariable, $expression);

        $context->headersManager->add('kernel/variables');
        $resolvedParams = $call->getResolvedParamsAsExpr($expression['parameters'], $context, $expression);
        $variableCode   = $context->backend->getVariableCode(
            $this->zvalArgument($resolvedParams[0], $expression, $context)
        );

        /*
         * var_export(val) and var_export(val, false) print and give null.
         */
        if (!$returnOutput) {
            $context->codePrinter->output('zephir_var_export(' . $variableCode . ');');

            return new CompiledExpression('null', 'null', $expression);
        }

        /*
         * var_export(val, true) gives the string and prints nothing. A
         * discarded result still exports, as its warnings and exceptions are
         * PHP's too.
         */
        if (!$symbolVariable || 'return_value' == $symbolVariable->getName()) {
            $symbolVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
        } else {
            $symbolVariable->initVariant($context);
        }

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output('zephir_var_export_ex(' . $symbol . ', ' . $variableCode . ');');

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * Whether the call returns the export instead of printing it, or null
     * when only PHP's own var_export() can tell: a missing or extra argument
     * (ArgumentCountError) or a `return` argument other than a bool literal,
     * which PHP coerces at run time.
     */
    private function resolveReturnArgument(array $parameters): ?bool
    {
        $count = count($parameters);
        if (1 === $count) {
            return false;
        }

        if (2 !== $count || 'bool' !== $parameters[1]['parameter']['type']) {
            return null;
        }

        return 'true' === $parameters[1]['parameter']['value'];
    }

    /**
     * The argument as a zval: a zval variable is used as it is, anything
     * else (a native variable, a cast, a literal, another compiled
     * expression) is boxed into a temporary first.
     *
     * Only a `variable` or `array` result names a symbol: the code of a
     * string literal is its text, which may equal a local's name.
     */
    private function zvalArgument(
        CompiledExpression $resolvedParam,
        array $expression,
        CompilationContext $context
    ): Variable {
        if (in_array($resolvedParam->getType(), [Types::T_VARIABLE, Types::T_ARRAY], true)) {
            $variable = $context->symbolTable->getVariable($resolvedParam->getCode());
            if ($variable && $variable->isComplexZval()) {
                /**
                 * This mark the variable as used.
                 */
                return $context->symbolTable->getVariableForRead($resolvedParam->getCode(), $context, $expression);
            }
        }

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

        return $variable;
    }
}
