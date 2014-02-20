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

/**
 * StringType
 *
 * Encapsulates built-in methods for the "string" type
 */
class StringType
{

    /**
     * Transforms calls to method "length" to function calls to "strlen"
     *
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     */
    public function length($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        $builder = new FunctionCallBuilder('strlen', array(array('parameter' => $caller)));

        $expression = new Expression($builder->get());

        return $expression->compile($compilationContext);
    }

    /**
     * Transforms calls to method "trim" to function calls to "trim"
     *
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     */
    public function trim($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        $builder = new FunctionCallBuilder('trim', array(array('parameter' => $caller)));

        $expression = new Expression($builder->get());

        return $expression->compile($compilationContext);
    }

    /**
     * Transforms calls to method "index" to function calls to "strpos"
     *
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     */
    public function index($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        $parameters = $expression['parameters'];
        array_unshift($parameters, array('parameter' => $caller));
        $builder = new FunctionCallBuilder('strpos', $parameters);

        $expression = new Expression($builder->get());

        return $expression->compile($compilationContext);
    }

    /**
     * Transforms calls to method "lower" to function calls to "strtolower"
     *
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     */
    public function lower($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        $builder = new FunctionCallBuilder('strtolower', array(array('parameter' => $caller)));

        $expression = new Expression($builder->get());

        return $expression->compile($compilationContext);
    }

    /**
     * Transforms calls to method "upper" to function calls to "upper"
     *
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     */
    public function upper($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        $builder = new FunctionCallBuilder('strotoupper', array(array('parameter' => $caller)));

        $expression = new Expression($builder->get());

        return $expression->compile($compilationContext);
    }

    /**
     * Intercepts calls to built-in methods on the string type
     *
     * @param string $methodName
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     */
    public function invokeMethod($methodName, $caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        if (method_exists($this, $methodName)) {
            return $this->{$methodName}($caller, $compilationContext, $call, $expression);
        }

        throw new CompilerException('Method "' . $methodName . '" is not a built-in method of type "string"', $expression);
    }
}
