<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Expression\Builder\Operators;

use Zephir\Expression\Builder\AbstractBuilder;

class AssignVariableOperator extends AbstractOperator
{
    // =
    public const OPERATOR_ADD = 'add-assign';

    // +=
    public const OPERATOR_ASSIGN = 'assign';

    // -=
    public const OPERATOR_BITWISE_AND = 'bitwise-and-assign';

    // *=
    public const OPERATOR_BITWISE_OR = 'bitwise-or-assign';

    // /=
    public const OPERATOR_BITWISE_SHIFTLEFT = 'bitwise-shiftleft-assign';

    // .=
    public const OPERATOR_BITWISE_SHIFTRIGHT = 'bitwise-shiftright-assign';

    // %=
    public const OPERATOR_BITWISE_XOR = 'bitwise-xor-assign';

    // &=
    public const OPERATOR_CONCAT = 'concat-assign';

    // |=
    public const OPERATOR_DIV = 'div-assign';

    // ^=
    public const OPERATOR_MOD = 'mod-assign';

    // <<=
    public const OPERATOR_MUL = 'mul-assign';

    // >>=
    public const OPERATOR_SUB = 'sub-assign';
    private string $operator = self::OPERATOR_ASSIGN;

    public function __construct(private $variable = null, private ?AbstractBuilder $expression = null)
    {
        if (null !== $variable) {
            $this->setVariable($variable);
        }

        if (null !== $expression) {
            $this->setExpression($expression);
        }
    }

    /**
     * @return mixed
     */
    public function getExpression()
    {
        return $this->expression;
    }

    /**
     * @return string
     */
    public function getOperator()
    {
        return $this->operator;
    }

    /**
     * @return string
     */
    public function getVariable()
    {
        return $this->variable;
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
     * {@inheritdoc}
     */
    protected function preBuild()
    {
        return [
            'assign-type' => 'variable',
            'variable'    => $this->getVariable(),
            'operator'    => $this->getOperator(),
            'expr'        => $this->getExpression(),
        ];
    }
}
