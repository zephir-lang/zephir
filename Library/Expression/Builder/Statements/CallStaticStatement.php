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
namespace Zephir\Expression\Builder\Statements;

/**
 * CallStaticStatement
 *
 * Allows to manually build a static function call AST node
 */
class CallStaticStatement extends AbstractStatement
{
    private $class;
    private $method;
    private $arguments;
    private $dynamicClass     = false;
    private $dynamicMethod    = false;

    /**
     * @param null $class
     * @param null $method
     * @param array|null $parameters
     */
    public function __construct($class = null, $method = null, array $parameters = null)
    {
        if ($class !== null) {
            $this->setClass($class);
        }

        if ($method !== null) {
            $this->setMethod($method);
        }

        if ($parameters !== null) {
            $this->setArguments($parameters);
        }
    }

    /**
     * @return mixed
     */
    public function getClass()
    {
        return $this->class;
    }

    /**
     * @param mixed $class
     * @return CallStaticStatement
     */
    public function setClass($class)
    {
        $this->class = $class;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getMethod()
    {
        return $this->method;
    }

    /**
     * @param mixed $method
     * @return CallStaticStatement
     */
    public function setMethod($method)
    {
        $this->method = $method;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getArguments()
    {
        return $this->arguments;
    }

    /**
     * @param mixed $arguments
     * @return CallStaticStatement
     */
    public function setArguments($arguments)
    {
        $this->arguments = $arguments;
        return $this;
    }

    /**
     * @return boolean
     */
    public function isDynamicClass()
    {
        return $this->dynamicClass;
    }

    /**
     * @param boolean $dynamicClass
     * @return CallStaticStatement
     */
    public function setDynamicClass($dynamicClass)
    {
        $this->dynamicClass = $dynamicClass;
        return $this;
    }

    /**
     * @return boolean
     */
    public function isDynamicMethod()
    {
        return $this->dynamicMethod;
    }

    /**
     * @param boolean $dynamicMethod
     * @return CallStaticStatement
     */
    public function setDynamicMethod($dynamicMethod)
    {
        $this->dynamicMethod = $dynamicMethod;
        return $this;
    }

    /**
     * @return array
     */
    protected function preBuild()
    {
        if ($arguments = $this->getArguments()) {
            foreach ($arguments as &$argument) {
                if (!is_array($argument) || !isset($argument['parameter'])) {
                    $argument = array('parameter' => $argument);
                }
            }
        }

        return array(
            'type'           => 'scall',
            'class'          => $this->getClass(),
            'name'           => $this->getMethod(),
            'parameters'     => $arguments,
            'dynamic-class'  => $this->isDynamicClass(),
            'dynamic'        => $this->isDynamicMethod(),

        );
    }
}
