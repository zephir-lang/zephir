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
namespace Zephir\Expression\Builder;

/**
 * RawExpression
 *
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
        if ($expression !== null) {
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
