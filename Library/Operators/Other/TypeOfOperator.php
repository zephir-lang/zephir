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
*/

namespace Zephir\Operators\Other;

use Zephir\Expression;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Operators\BaseOperator;
use Zephir\Compiler\CompilerException;
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
