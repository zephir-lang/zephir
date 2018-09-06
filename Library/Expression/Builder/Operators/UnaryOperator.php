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
namespace Zephir\Expression\Builder\Operators;

use Zephir\Expression\Builder\AbstractBuilder;

/**
 * UnaryOperator
 *
 * Allows to manually build a unary operator AST node
 */
class UnaryOperator extends AbstractOperator
{
    // y = &a
    const OPERATOR_REFERENCE = 'reference';

    // y = !a
    const OPERATOR_NOT = 'not';

    // y = ~a
    const OPERATOR_BITWISE_NOT = 'bitwise_not';

    // y = -a
    const OPERATOR_MINUS = 'minus';

    // y = +a
    const OPERATOR_PLUS = 'plus';

    // y = isset a
    const OPERATOR_ISSET = 'isset';

    // y = require a
    const OPERATOR_REQUIRE = 'require';

    // y = clone a
    const OPERATOR_CLONE = 'clone';

    // y = empty a
    const OPERATOR_EMPTY = 'empty';

    // y = likely a
    const OPERATOR_LIKELY = 'likely';

    // y = unlikely a
    const OPERATOR_UNLIKELY = 'unlikely';

    // y = list a
    const OPERATOR_LIST = 'list';

    // y = typeof a
    const OPERATOR_TYPEOF = 'typeof';

    private $operator;
    private $expression;

    /**
     * @param null $operator
     * @param AbstractBuilder|null $expression
     */
    public function __construct($operator = null, AbstractBuilder $expression = null)
    {
        if ($operator !== null) {
            $this->setOperator($operator);
        }

        if ($expression !== null) {
            $this->setExpression($expression);
        }
    }

    /**
     * @return mixed
     */
    public function getOperator()
    {
        return $this->operator;
    }

    /**
     * @param $operator
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
     * @param AbstractBuilder $expression
     * @return $this
     */
    public function setExpression(AbstractBuilder $expression)
    {
        $this->expression = $expression;
        return $this;
    }

    /**
     * {@inheritdoc}
     */
    protected function preBuild()
    {
        return array(
            'type' => $this->getOperator(),
            'left' => $this->getExpression(),
        );
    }
}
