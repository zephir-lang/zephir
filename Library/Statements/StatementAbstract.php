<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Optimizers\EvalExpression;

abstract class StatementAbstract
{
    protected ?EvalExpression $evalExpression;

    public function __construct(protected array $statement)
    {
    }

    public function getEvalExpression(): ?EvalExpression
    {
        return $this->evalExpression;
    }

    /**
     * Compiles the statement.
     *
     * @param CompilationContext $compilationContext
     */
    abstract public function compile(CompilationContext $compilationContext);
}
