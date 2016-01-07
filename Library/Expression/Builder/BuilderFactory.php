<?php
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
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
