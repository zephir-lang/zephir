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

/**
 * Class RawOperator
 * @package Zephir\Expression\Builder\Operators
 */
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
        if ($expression !== null) {
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
