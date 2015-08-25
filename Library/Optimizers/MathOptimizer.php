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
*/

namespace Zephir\Optimizers;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\CompilerException;
use Zephir\Expression;

/**
 * Class OptimizerAbstract
 */
abstract class MathOptimizer extends OptimizerAbstract
{
    abstract public function getFunctionName();

    /**
     * Get array of needed parameters for function optimizer
     *
     * @return array
     */
    public function getParameters()
    {
        return array(
            array(
                'name' => 'value',
                'type' => 'mixed'
            )
        );
    }

    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return bool|CompiledExpression|mixed
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        $parameters = $this->getParameters();

        if (!isset($expression['parameters'])) {
            return false;
        }

        if (count($expression['parameters']) != count($parameters)) {
            return false;
        }

        /**
         * Resolve parameters as vars
         */
        $call->getResolvedParams($expression['parameters'], $context, $expression);
        /**
         * Get CompiledExpression(s) for resolved var(s)
         */
        $resolvedParams = $call->getResolvedParamsAsExpr($expression['parameters'], $context, $expression);
        $compiledExpression = $resolvedParams[0];

        switch ($compiledExpression->getType()) {
            case 'int':
            case 'float':
            case 'long':
            case 'ulong':
            case 'double':
                $context->headersManager->add('math');
                return $this->passNativeFCall($compiledExpression, $expression);
                break;
            case 'variable':
                $variable = $context->symbolTable->getVariable($compiledExpression->getCode());
                switch ($variable->getType()) {
                    case 'int':
                    case 'float':
                    case 'long':
                    case 'ulong':
                    case 'double':
                        $context->headersManager->add('math');
                        return $this->passNativeFCall($compiledExpression, $expression);
                        break;
                    case 'variable':
                        $context->headersManager->add('kernel/math');
                        return new CompiledExpression(
                            'double',
                            'zephir_' . $this->getFunctionName() . '(' . $context->backend->getVariableCode($variable) . ' TSRMLS_CC)',
                            $expression
                        );
                        break;
                }
                break;
        }

        return false;
    }

    /**
     * @param CompiledExpression $compiledExpression
     * @param array $expression
     * @return CompiledExpression
     */
    protected function passNativeFCall($compiledExpression, $expression)
    {
        return new CompiledExpression(
            'double',
            $this->getFunctionName() . '(' . $compiledExpression->getCode() . ')',
            $expression
        );
    }
}
