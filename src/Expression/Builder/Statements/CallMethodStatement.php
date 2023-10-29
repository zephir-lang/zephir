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
 * Allows to manually build a method call AST node
 */
class CallMethodStatement extends CallFunctionStatement
{
    private $variable;

    /**
     * @param null       $variable
     * @param null       $name
     * @param array|null $parameters
     * @param int        $type
     */
    public function __construct($variable = null, $name = null, array $parameters = null, $type = self::TYPE_CALL_DIRECT)
    {
        parent::__construct($name, $parameters, $type);

        if (null !== $name) {
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
     *
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
