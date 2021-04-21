<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder\Operators;

use Zephir\Expression\Builder\AbstractBuilder;

/**
 * Class AssignVariableOperator.
 */
class AssignVariableOperator extends AbstractOperator
{
    // =
    const OPERATOR_ASSIGN = 'assign';

    // +=
    const OPERATOR_ADD = 'add-assign';

    // -=
    const OPERATOR_SUB = 'sub-assign';

    // *=
    const OPERATOR_MUL = 'mul-assign';

    // /=
    const OPERATOR_DIV = 'div-assign';

    // .=
    const OPERATOR_CONCAT = 'concat-assign';

    // %=
    const OPERATOR_MOD = 'mod-assign';

    // &=
    const OPERATOR_BITWISE_AND = 'bitwise-and-assign';

    // |=
    const OPERATOR_BITWISE_OR = 'bitwise-or-assign';

    // ^=
    const OPERATOR_BITWISE_XOR = 'bitwise-xor-assign';

    // <<=
    const OPERATOR_BITWISE_SHIFTLEFT = 'bitwise-shiftleft-assign';

    // >>=
    const OPERATOR_BITWISE_SHIFTRIGHT = 'bitwise-shiftright-assign';



    private $variable;
    private $operator = self::OPERATOR_ASSIGN;
    private $expression;

    /**
     * @param null                 $variable
     * @param AbstractBuilder|null $expression
     */
    public function __construct($variable = null, AbstractBuilder $expression = null)
    {
        if (null !== $variable) {
            $this->setVariable($variable);
        }

        if (null !== $expression) {
            $this->setExpression($expression);
        }
    }

    /**
     * @return string
     */
    public function getVariable()
    {
        return $this->variable;
    }

    /**
     * @param string $variable
     *
     * @return $this
     */
    public function setVariable($variable)
    {
        $this->variable = $variable;

        return $this;
    }

    /**
     * @return string
     */
    public function getOperator()
    {
        return $this->operator;
    }

    /**
     * @param $operator
     *
     * @return $this
     */
    public function setOperator($operator)
    {
        $this->operator = $operator;

        return $this;
    }

    /**
     * @return mixed
     */
    public function getExpression()
    {
        return $this->expression;
    }

    /**
     * @param $expression
     *
     * @return $this
     */
    public function setExpression($expression)
    {
        $this->expression = $expression;

        return $this;
    }

    /**
     * {@inheritdoc}
     */
    protected function preBuild()
    {
        return [
            'assign-type' => 'variable',
            'variable' => $this->getVariable(),
            'operator' => $this->getOperator(),
            'expr' => $this->getExpression(),
        ];
    }
}
