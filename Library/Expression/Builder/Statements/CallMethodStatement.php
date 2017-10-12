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
 * CallMethodStatement
 *
 * Allows to manually build a method call AST node
 */
class CallMethodStatement extends CallFunctionStatement
{

    private $variable;


    /**
     * @param null $variable
     * @param null $name
     * @param array|null $parameters
     * @param int $type
     */
    public function __construct($variable = null, $name = null, array $parameters = null, $type = self::TYPE_CALL_DIRECT)
    {
        parent::__construct($name, $parameters, $type);

        if ($name !== null) {
            $this->setVariable($variable);
        }
    }

    /**
     * @return mixed
     */
    public function getVariable()
    {
        return $this->variable;
    }

    /**
     * @param $variable
     * @return $this
     */
    public function setVariable($variable)
    {
        $this->variable = $variable;
        return $this;
    }

    /**
     * @return array
     */
    protected function preBuild()
    {
        $expression = parent::preBuild();
        $expression['type'] = 'mcall';
        $expression['variable'] = $this->getVariable();

        return $expression;
    }
}
