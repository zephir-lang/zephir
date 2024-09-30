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

class RawOperator extends AbstractOperator
{
    /**
     * @var array
     */
    private $expression;

    /**
     * @param array|null $expression
     */
    public function __construct(array $expression = null)
    {
        if (null !== $expression) {
            $this->setExpression($expression);
        }
    }

    /**
     * @return array
     */
    public function getExpression()
    {
        return $this->expression;
    }

    /**
     * @param array $expression
     *
     * @return $this
     */
    public function setExpression(array $expression)
    {
        $this->expression = $expression;

        return $this;
    }

    /**
     * @return array
     */
    protected function preBuild()
    {
        return $this->getExpression();
    }
}
