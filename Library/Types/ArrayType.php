<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

namespace Zephir\Types;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\Builder\FunctionCallBuilder;
use Zephir\FunctionCall;

class ArrayType extends AbstractType
{
    /**
     * {@inheritdoc}
     */
    public function getTypeName()
    {
        return 'array';
    }

    /**
     * Transforms calls to method "join" to function calls to "join"
     *
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     */
    public function join($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {

        $builder = new FunctionCallBuilder(
            'join',
            array_merge($expression['parameters'], array(array('parameter' => $caller))),
            FunctionCall::CALL_NORMAL,
            $expression['file'],
            $expression['line'],
            $expression['char']
        );

        $expression = new Expression($builder->get());

        return $expression->compile($compilationContext);
    }

    /**
     * Transforms calls to method "reverse" to function calls to "array_reverse"
     *
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     */
    public function reverse($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        $builder = new FunctionCallBuilder('array_reverse', array(array('parameter' => $caller)));

        $expression = new Expression($builder->get());

        return $expression->compile($compilationContext);
    }
}
