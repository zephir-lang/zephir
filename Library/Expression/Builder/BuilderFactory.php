<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder;

use Zephir\Expression\Builder\Factory\OperatorsFactory;
use Zephir\Expression\Builder\Factory\StatementsFactory;

/**
 * Class BuilderFactory
 * @package Zephir\Expression\Builder
 */
class BuilderFactory
{
    /**
     * @var OperatorsFactory
     */
    protected $operatorsFactory;

    /**
     * @var StatementsFactory
     */
    protected $statementsFactory;

    /**
     * @var self
     */
    protected static $_instance;

    /**
     * @return self
     */
    public static function getInstance()
    {
        if (!static::$_instance) {
            static::$_instance = new static();
        }

        return static::$_instance;
    }

    /**
     * @return OperatorsFactory
     */
    public function operators()
    {
        if (!$this->operatorsFactory) {
            $this->operatorsFactory = new OperatorsFactory($this);
        }

        return $this->operatorsFactory;
    }

    /**
     * @return StatementsFactory
     */
    public function statements()
    {
        if (!$this->statementsFactory) {
            $this->statementsFactory = new StatementsFactory($this);
        }

        return $this->statementsFactory;
    }

    /**
     * @param $name
     * @return RawExpression
     */
    public function variable($name)
    {
        return $this->raw(array(
            'type'  => 'variable',
            'value' => $name
        ));
    }

    /**
     * @param $type
     * @param null $value
     * @return RawExpression
     */
    public function literal($type, $value = null)
    {
        return $this->raw(array(
            'type'  => $type,
            'value' => $value
        ));
    }

    /**
     * @param $expression
     * @return RawExpression
     */
    public function raw($expression)
    {
        return new RawExpression($expression);
    }
}
