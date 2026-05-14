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

namespace Zephir\Expression\Builder\Statements;

use Zephir\Expression\Builder\Operators\AbstractOperator;

/**
 * Allows to manually build an 'if' statement AST node.
 */
class IfStatement extends AbstractStatement
{
    public function __construct(
        private ?AbstractOperator $condition = null,
        private ?StatementsBlock $statements = null,
        private ?StatementsBlock $elseStatements = null
    ) {
        if (null !== $condition) {
            $this->setCondition($condition);
        }

        if (null !== $statements) {
            $this->setStatements($statements);
        }

        if (null !== $elseStatements) {
            $this->setElseStatements($elseStatements);
        }
    }

    public function getCondition(): ?AbstractOperator
    {
        return $this->condition;
    }

    public function getElseStatements(): ?StatementsBlock
    {
        return $this->elseStatements;
    }

    public function getStatements(): ?StatementsBlock
    {
        return $this->statements;
    }

    public function setCondition(AbstractOperator $condition): self
    {
        $this->condition = $condition;

        return $this;
    }

    public function setElseStatements(StatementsBlock $elseStatements): self
    {
        $this->elseStatements = $elseStatements;

        return $this;
    }

    public function setStatements(StatementsBlock $statements): self
    {
        $this->statements = $statements;

        return $this;
    }

    protected function preBuild(): array
    {
        $expression = [
            'type'       => 'if',
            'expr'       => $this->getCondition(),
            'statements' => $this->getStatements(),
        ];

        if ($statements = $this->getElseStatements()) {
            $expression['else_statements'] = $statements;
        }

        return $expression;
    }
}
