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

namespace Zephir\Types;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Compiler\CompilerException;

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

            $functionCall = BuilderFactory::getInstance()->statements()
                ->functionCall($this->methodMap[$methodName], $parameters)
                ->setFile($expression['file'])
                ->setLine($expression['line'])
                ->setChar($expression['char']);

            $expression = new Expression($functionCall->build());

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
