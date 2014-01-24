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
		$builder = new FunctionCallBuilder('strlen', array($caller));

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
		$builder = new FunctionCallBuilder('trim', array($caller));

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
		$builder = new FunctionCallBuilder('strpos', array($caller));

		$expression = new Expression($builder->get());

		return $expression->compile($compilationContext);
	}

	public function invokeMethod($methodName, $caller, CompilationContext $compilationContext, Call $call, array $expression)
	{
		if (method_exists($this, $methodName)) {
			return $this->{$methodName}($caller, $compilationContext, $call, $expression);
		}

		throw new CompilerException('Method "' . $methodName . '" is not a built-in method of type "string"', $expression);
	}

}