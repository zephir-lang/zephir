<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder;

use Zephir\Expression\Builder\Factory\OperatorsFactory;
use Zephir\Expression\Builder\Factory\StatementsFactory;

class BuilderFactory
{
    /** @var OperatorsFactory */
    protected $operatorsFactory;

    /** @var StatementsFactory */
    protected $statementsFactory;

    /** @var self */
    protected static $instance;

    /**
     * @return self
     */
    public static function getInstance()
    {
        if (!static::$instance) {
            static::$instance = new static();
        }

        return static::$instance;
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
     *
     * @return RawExpression
     */
    public function variable($name)
    {
        return $this->raw([
            'type' => 'variable',
            'value' => $name,
        ]);
    }

    /**
     * @param      $type
     * @param null $value
     *
     * @return RawExpression
     */
    public function literal($type, $value = null)
    {
        return $this->raw([
            'type' => $type,
            'value' => $value,
        ]);
    }

    /**
     * @param $expression
     *
     * @return RawExpression
     */
    public function raw($expression)
    {
        return new RawExpression($expression);
    }
}
