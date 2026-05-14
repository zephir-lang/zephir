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

namespace Zephir\Expression\Builder\Factory;

use Zephir\Expression\Builder\AbstractBuilder;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Expression\Builder\Operators\AbstractOperator;
use Zephir\Expression\Builder\Statements\CallFunctionStatement;
use Zephir\Expression\Builder\Statements\IfStatement;
use Zephir\Expression\Builder\Statements\LetStatement;
use Zephir\Expression\Builder\Statements\RawStatement;
use Zephir\Expression\Builder\Statements\StatementsBlock;

class StatementsFactory
{
    public function __construct(protected BuilderFactory $factory)
    {
    }

    public function block(?array $statements = null): StatementsBlock
    {
        return new StatementsBlock($statements);
    }

    public function functionCall(
        string $name,
        ?array $parameters = null,
        int $type = CallFunctionStatement::TYPE_CALL_DIRECT
    ): CallFunctionStatement {
        return new CallFunctionStatement($name, $parameters, $type);
    }

    public function ifX(
        ?AbstractOperator $condition = null,
        ?StatementsBlock $statements = null,
        ?StatementsBlock $elseStatements = null
    ): IfStatement {
        return new IfStatement($condition, $statements, $elseStatements);
    }

    public function let(?array $assignments = null): LetStatement
    {
        return new LetStatement($assignments);
    }

    public function raw(array $expression): RawStatement
    {
        return new RawStatement($expression);
    }

    public function rawC(string $value): RawStatement
    {
        return $this->raw([
            'type'  => 'cblock',
            'value' => $value,
        ]);
    }

    public function returnX(AbstractBuilder $expression): RawStatement
    {
        return $this->raw([
            'type' => 'return',
            'expr' => $expression,
        ]);
    }
}
