<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2016 Zephir Team and contributors                     |
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

namespace Zephir\Operators\Other;

use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Operators\BaseOperator;
use Zephir\Builder\FunctionCallBuilder;
use Zephir\Expression\Builder\BuilderFactory;

/**
 * TypeOf
 *
 * Obtains the type of a dynamic variable
 */
class TypeOfOperator extends BaseOperator
{
    /**
     * @param $expression
     * @param CompilationContext $compilationContext
     * @return bool|CompiledExpression
     * @throws CompilerException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new CompilerException("Invalid 'left' operand for 'typeof' expression", $expression['left']);
        }

        $functionCall = BuilderFactory::getInstance()->statements()->functionCall('gettype', array($expression['left']));
        $expression = new Expression($functionCall->build());
        return $expression->compile($compilationContext);
    }
}
