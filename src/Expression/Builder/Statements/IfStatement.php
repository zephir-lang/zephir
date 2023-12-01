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
 * Allows to manually build a 'if' statement AST node
 */
class IfStatement extends AbstractStatement
{
    /**
     * @var AbstractOperator
     */
    private $condition;
    /**
     * @var StatementsBlock
     */
    private $elseStatements;
    /**
     * @var StatementsBlock
     */
    private $statements;

    /**
     * @param AbstractOperator|null $condition
     * @param StatementsBlock|null  $statements
     * @param StatementsBlock|null  $elseStatements
     */
    public function __construct(
        AbstractOperator $condition = null,
        StatementsBlock $statements = null,
        StatementsBlock $elseStatements = null
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

    /**
     * @return AbstractOperator
     */
    public function getCondition()
    {
        return $this->condition;
    }

    /**
     * @return StatementsBlock
     */
    public function getElseStatements()
    {
        return $this->elseStatements;
    }

    /**
     * @return StatementsBlock
     */
    public function getStatements()
    {
        return $this->statements;
    }

    /**
     * @param AbstractOperator $condition
     *
     * @return $this
     */
    public function setCondition(AbstractOperator $condition)
    {
        $this->condition = $condition;

        return $this;
    }

    /**
     * @param StatementsBlock $elseStatements
     *
     * @return $this
     */
    public function setElseStatements(StatementsBlock $elseStatements)
    {
        $this->elseStatements = $elseStatements;

        return $this;
    }

    /**
     * @param StatementsBlock $statements
     *
     * @return $this
     */
    public function setStatements(StatementsBlock $statements)
    {
        $this->statements = $statements;

        return $this;
    }

    /**
     * {@inheritdoc}
     */
    protected function preBuild()
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
