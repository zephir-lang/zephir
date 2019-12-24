<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder\Factory;

use Zephir\Expression\Builder\AbstractBuilder;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Expression\Builder\Operators\AbstractOperator;
use Zephir\Expression\Builder\Statements\CallFunctionStatement;
use Zephir\Expression\Builder\Statements\CallMethodStatement;
use Zephir\Expression\Builder\Statements\CallStaticStatement;
use Zephir\Expression\Builder\Statements\IfStatement;
use Zephir\Expression\Builder\Statements\LetStatement;
use Zephir\Expression\Builder\Statements\RawStatement;
use Zephir\Expression\Builder\Statements\StatementsBlock;

/**
 * Class StatementsFactory.
 */
class StatementsFactory
{
    protected $factory;

    /**
     * @param BuilderFactory $factory
     */
    public function __construct(BuilderFactory $factory)
    {
        $this->factory = $factory;
    }

    /**
     * @param string     $name
     * @param array|null $parameters
     * @param int        $type
     *
     * @return CallFunctionStatement
     */
    public function functionCall($name, $parameters = null, $type = CallFunctionStatement::TYPE_CALL_DIRECT)
    {
        return new CallFunctionStatement($name, $parameters, $type);
    }

    /**
     * @param $variable
     * @param $name
     * @param null $parameters
     * @param int  $type
     *
     * @return CallMethodStatement
     */
    public function methodCall($variable, $name, $parameters = null, $type = CallMethodStatement::TYPE_CALL_DIRECT)
    {
        return new CallMethodStatement($variable, $name, $parameters, $type);
    }

    /**
     * @param $class
     * @param $method
     * @param null $parameters
     *
     * @return CallStaticStatement
     */
    public function staticCall($class, $method, $parameters = null)
    {
        return new CallStaticStatement($class, $method, $parameters);
    }

    /**
     * @param array|null $statements
     *
     * @return StatementsBlock
     */
    public function block(array $statements = null)
    {
        return new StatementsBlock($statements);
    }

    /**
     * @param array|null $assignments
     *
     * @return LetStatement
     */
    public function let(array $assignments = null)
    {
        return new LetStatement($assignments);
    }

    /**
     * @param AbstractOperator|null $condition
     * @param StatementsBlock|null  $statements
     * @param StatementsBlock|null  $elseStatements
     *
     * @return IfStatement
     */
    public function ifX(AbstractOperator $condition = null, StatementsBlock $statements = null, StatementsBlock $elseStatements = null)
    {
        return new IfStatement($condition, $statements, $elseStatements);
    }

    /**
     * @param AbstractBuilder $expression
     *
     * @return RawStatement
     */
    public function returnX(AbstractBuilder $expression)
    {
        return $this->raw([
            'type' => 'return',
            'expr' => $expression,
        ]);
    }

    /**
     * @param AbstractBuilder $expression
     *
     * @return RawStatement
     */
    public function throwX(AbstractBuilder $expression)
    {
        return $this->raw([
            'type' => 'throw',
            'expr' => $expression,
        ]);
    }

    /**
     * @param string $value
     *
     * @return RawStatement
     */
    public function rawC($value)
    {
        return $this->raw([
            'type' => 'cblock',
            'value' => $value,
        ]);
    }

    /**
     * @param array $expression
     *
     * @return RawStatement
     */
    public function raw(array $expression)
    {
        return new RawStatement($expression);
    }
}
