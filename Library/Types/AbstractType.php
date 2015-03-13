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

namespace Zephir\Types;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\Builder\FunctionCallBuilder;
use Zephir\FunctionCall;

abstract class AbstractType
{
    /**
     * The array of methods in zephir mapped to PHP internal methods
     *
     * @var array
     */
    protected $methodMap = array();

    /**
     * Intercepts calls to built-in methods
     *
     * @param string $methodName
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     * @throws CompilerException
     * @return bool|\Zephir\CompiledExpression
     */
    public function invokeMethod($methodName, $caller, CompilationContext $compilationContext, Call $call, array $expression)
    {

        /**
         * Checks first whether the method exist in the array type definition
         */
        if (method_exists($this, $methodName)) {
            return $this->{$methodName}($caller, $compilationContext, $call, $expression);
        }

        /**
         * Check the method map
         */
        if (isset($this->methodMap[$methodName])) {
            $paramNumber = $this->getNumberParam($methodName);
            if ($paramNumber == 0) {
                if (isset($expression['parameters'])) {
                    $parameters = $expression['parameters'];
                    array_unshift($parameters, array('parameter' => $caller));
                } else {
                    $parameters = array(array('parameter' => $caller));
                }
            } else {
                if (isset($expression['parameters'])) {
                    $parameters = array();
                    foreach ($expression['parameters'] as $number => $parameter) {
                        if ($number == $paramNumber) {
                            $parameters[] = null;
                        }
                        $parameters[] = $parameter;
                    }
                    $parameters[$paramNumber] = array('parameter' => $caller);
                } else {
                    $parameters = array(array('parameter' => $caller));
                }
            }

            $builder = new FunctionCallBuilder(
                $this->methodMap[$methodName],
                $parameters,
                FunctionCall::CALL_NORMAL,
                $expression['file'],
                $expression['line'],
                $expression['char']
            );
            $expression = new Expression($builder->get());

            return $expression->compile($compilationContext);
        }

        throw new CompilerException(sprintf('Method "%s" is not a built-in method of type "%s"', $methodName, $this->getTypeName()), $expression);
    }

    /**
     * Get the name of the type
     *
     * @return string
     */
    abstract public function getTypeName();

    /**
     * Returns the number of the parameter where the object must be bound
     *
     * @param $methodName
     * @return int
     */
    protected function getNumberParam($methodName)
    {
        return 0;
    }

    /**
     * @return array
     */
    public function getMethodMap()
    {
        return $this->methodMap;
    }
}
