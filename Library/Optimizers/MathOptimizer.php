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
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return bool|CompiledExpression|mixed
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (count($expression['parameters']) > 1) {
            return false;
        }

        $resolvedParameter = $expression['parameters'][0]['parameter'];
        if ($resolvedParameter instanceof \Zephir\CompiledExpression) {
            /**
             * @var $resolvedParameter \Zephir\CompiledExpression
             */
//            var_dump($resolvedParameter->getType());
//            var_dump($resolvedParameter->getCode());
        } else {
            $expr = new Expression($expression['parameters'][0]['parameter']);
            $expr->setEvalMode(true);
            $expr->setReadOnly(true);

            $compiledExpression = $expr->compile($context);

            switch ($compiledExpression->getType()) {
                case 'int':
                case 'float':
                case 'long':
                case 'ulong':
                case 'double':
                    return new CompiledExpression('double', $this->getFunctionName() . '(' . $compiledExpression->getCode() . ')',
                        $expression);
                    break;
            }
        }

        return false;
    }
}
