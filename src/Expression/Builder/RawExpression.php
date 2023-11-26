<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder;

/**
 * Allows to use a raw expression in a builder
 */
class RawExpression extends AbstractBuilder
{
    private $expression;

    /**
     * @param null $expression
     */
    public function __construct($expression = null)
    {
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
     * @param $expr
     *
     * @return $this
     */
    public function setExpression($expr)
    {
        $this->expression = $expr;

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
