<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 | Authors: Rack Lin <racklin@gmail.com>                                    |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;
use Zephir\Statements\LetStatement;

/**
 * VarExportOptimizer
 *
 * Optimizes calls to 'var_export' using internal function
 */
class VarExportOptimizer extends OptimizerAbstract
{
    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return mixed|CompiledExpression
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        /**
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable();
        if ($symbolVariable) {
            if (!$symbolVariable->isVariable()) {
                throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
            }
        }

        $context->headersManager->add('kernel/variables');
        $resolvedParams = $call->getResolvedParamsAsExpr($expression['parameters'], $context, $expression);
        $resolvedParam = $resolvedParams[0];

        if (!$symbolVariable || !$symbolVariable->isVariable()) {
            /**
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

            $statement = new LetStatement(array(
                'type' => 'let',
                'assignments' => array(
                    array(
                        'assign-type' => $type,
                        'variable' => $variable->getName(),
                        'operator' => 'assign',
                        'expr' => array(
                            'type'  => $resolvedParam->getType(),
                            'value' => $resolvedParam->getCode(),
                            'file'  => $expression['file'],
                            'line'  => $expression['line'],
                            'char'  => $expression['char'],
                        ),
                        'file'  => $expression['file'],
                        'line'  => $expression['line'],
                        'char'  => $expression['char'],
                    )
                )
            ));
            $statement->compile($context);
        } else {
            /**
             * This mark the variable as used
             */
            $variable = $context->symbolTable->getVariableForRead($resolvedParam->getCode(), $context, $expression);
        }
        $variableSymbol = $context->backend->getVariableCodePointer($variable);

        /**
         * let a = var_export(val);
         */
        if ($symbolVariable) {
            if ($symbolVariable->getName() == 'return_value') {
                $symbolVariable = $context->symbolTable->getTempVariableForWrite('variable', $context);
            } else {
                $symbolVariable->initVariant($context);
            }
            $symbol = $context->backend->getVariableCode($symbolVariable);
            $context->codePrinter->output('zephir_var_export_ex(' . $symbol . ', ' . $variableSymbol . ' TSRMLS_CC);');
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $context->codePrinter->output('zephir_var_export(' . $variableSymbol . ' TSRMLS_CC);');
        return new CompiledExpression('null', 'null', $expression);
    }
}
