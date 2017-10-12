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

use Zephir\FunctionCall;

/**
 * CallFunctionStatement
 *
 * Allows to manually build a function call AST node
 */
class CallFunctionStatement extends AbstractStatement
{
    /**
     * f(x, y, ... , N)
     */
    const TYPE_CALL_DIRECT = FunctionCall::CALL_NORMAL;

    /**
     * {f}(x, y, ... , N)
     */
    const TYPE_CALL_DYNAMIC = FunctionCall::CALL_DYNAMIC;

    /**
     * {"f"}(x, y, ... , N)
     */
    const TYPE_CALL_DYNAMIC_STRING = FunctionCall::CALL_DYNAMIC_STRING;

    private $name;
    private $arguments;
    private $typeCall;

    /**
     * @param string|null $name
     * @param array|null $parameters
     * @param int $typeCall
     */
    public function __construct($name = null, array $parameters = null, $typeCall = self::TYPE_CALL_DIRECT)
    {
        if ($name !== null) {
            $this->setName($name);
        }

        if ($parameters !== null) {
            $this->setArguments($parameters);
        }

        if ($typeCall !== null) {
            $this->setTypeCall($typeCall);
        }
    }

    /**
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * @param string $name
     * @return $this
     */
    public function setName($name)
    {
        $this->name = $name;
        return $this;
    }

    /**
     * @return array
     */
    public function getArguments()
    {
        return $this->arguments;
    }

    /**
     * @param array $arguments
     * @return $this
     */
    public function setArguments(array $arguments = null)
    {
        $this->arguments = $arguments;
        return $this;
    }

    /**
     * @return int
     */
    public function getTypeCall()
    {
        return $this->typeCall;
    }

    /**
     * @param int $typeCall
     * @return $this
     */
    public function setTypeCall($typeCall)
    {
        $this->typeCall = $typeCall;
        return $this;
    }

    /**
     * @param $argument
     * @return $this
     */
    public function addArgument($argument)
    {
        $this->arguments[] = $argument;
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
            'type'       => 'fcall',
            'name'       => $this->getName(),
            'call-type'  => $this->getTypeCall(),
            'parameters' => $arguments,
        );
    }
}
