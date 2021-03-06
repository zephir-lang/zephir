<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder\Statements;

/**
 * CallStaticStatement.
 *
 * Allows to manually build a static function call AST node
 */
class CallStaticStatement extends AbstractStatement
{
    private $class;
    private $method;
    private $arguments;
    private $dynamicClass = false;
    private $dynamicMethod = false;

    /**
     * @param null       $class
     * @param null       $method
     * @param array|null $parameters
     */
    public function __construct($class = null, $method = null, array $parameters = null)
    {
        if (null !== $class) {
            $this->setClass($class);
        }

        if (null !== $method) {
            $this->setMethod($method);
        }

        if (null !== $parameters) {
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
     *
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
     *
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
     *
     * @return CallStaticStatement
     */
    public function setArguments($arguments)
    {
        $this->arguments = $arguments;

        return $this;
    }

    /**
     * @return bool
     */
    public function isDynamicClass()
    {
        return $this->dynamicClass;
    }

    /**
     * @param bool $dynamicClass
     *
     * @return CallStaticStatement
     */
    public function setDynamicClass($dynamicClass)
    {
        $this->dynamicClass = $dynamicClass;

        return $this;
    }

    /**
     * @return bool
     */
    public function isDynamicMethod()
    {
        return $this->dynamicMethod;
    }

    /**
     * @param bool $dynamicMethod
     *
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
                if (!\is_array($argument) || !isset($argument['parameter'])) {
                    $argument = ['parameter' => $argument];
                }
            }
        }

        return [
            'type' => 'scall',
            'class' => $this->getClass(),
            'name' => $this->getMethod(),
            'parameters' => $arguments,
            'dynamic-class' => $this->isDynamicClass(),
            'dynamic' => $this->isDynamicMethod(),
        ];
    }
}
