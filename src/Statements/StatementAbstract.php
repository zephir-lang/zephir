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

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Optimizers\EvalExpression;
use Zephir\Traits\VariablesTrait;

abstract class StatementAbstract
{
    use VariablesTrait;

    protected ?EvalExpression $evalExpression;

    public function __construct(protected array $statement)
    {
    }

    /**
     * Compiles the statement.
     */
    abstract public function compile(CompilationContext $compilationContext);

    public function getEvalExpression(): ?EvalExpression
    {
        return $this->evalExpression;
    }
}
