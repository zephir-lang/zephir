<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
 | Authors: Rack Lin <racklin@gmail.com>                                    |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompilerException;
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

        /**
         * let a = var_export(val);
         */
        if ($symbolVariable) {
            if ($call->mustInitSymbolVariable()) {
                $symbolVariable->initVariant($context);
            }
            $context->codePrinter->output('zephir_var_export_ex(' . $symbolVariable->getName() . ', &(' . $variable->getName() . ') TSRMLS_CC);');
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $context->codePrinter->output('zephir_var_export(&' . $variable->getName() . ' TSRMLS_CC);');
        return new CompiledExpression('null', 'null', $expression);
    }
}
