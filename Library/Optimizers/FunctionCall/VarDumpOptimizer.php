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
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;
use Zephir\Statements\LetStatement;

/**
 * VarDumpOptimizer
 *
 * Optimizes calls to 'var_dump' using internal function
 */
class VarDumpOptimizer extends OptimizerAbstract
{
    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
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

            $symbol = $context->backend->getVariableCodePointer($variable);
            $context->codePrinter->output('zephir_var_dump(' . $symbol . ' TSRMLS_CC);');
        }

        return new CompiledExpression('null', 'null', $expression);
    }
}
