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

/**
 * Allows to manually build a unary operator AST node
 */
class UnaryOperator extends AbstractOperator
{
    // y = &a
    public const OPERATOR_BITWISE_NOT = 'bitwise_not';

    // y = !a
    public const OPERATOR_CLONE = 'clone';

    // y = ~a
    public const OPERATOR_EMPTY = 'empty';

    // y = -a
    public const OPERATOR_ISSET = 'isset';

    // y = +a
    public const OPERATOR_LIKELY = 'likely';

    // y = isset a
    public const OPERATOR_LIST = 'list';

    // y = require a
    public const OPERATOR_MINUS = 'minus';

    // y = require_once a
    public const OPERATOR_NOT = 'not';

    // y = clone a
    public const OPERATOR_PLUS = 'plus';

    // y = empty a
    public const OPERATOR_REFERENCE = 'reference';

    // y = likely a
    public const OPERATOR_REQUIRE = 'require';

    // y = unlikely a
    public const OPERATOR_REQUIRE_ONCE = 'require_once';

    // y = list a
    public const OPERATOR_TYPEOF = 'typeof';

    // y = typeof a
    public const OPERATOR_UNLIKELY = 'unlikely';
    private $expression;
    private $operator;

    /**
     * @param null                 $operator
     * @param AbstractBuilder|null $expression
     */
    public function __construct($operator = null, AbstractBuilder $expression = null)
    {
        if (null !== $operator) {
            $this->setOperator($operator);
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
     * @return mixed
     */
    public function getOperator()
    {
        return $this->operator;
    }

    /**
     * @param AbstractBuilder $expression
     *
     * @return $this
     */
    public function setExpression(AbstractBuilder $expression)
    {
        $this->expression = $expression;

        return $this;
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
     * {@inheritdoc}
     */
    protected function preBuild()
    {
        return [
            'type' => $this->getOperator(),
            'left' => $this->getExpression(),
        ];
    }
}
