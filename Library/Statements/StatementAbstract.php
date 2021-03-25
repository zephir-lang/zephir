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

/**
 * Zephir\Statements\StatementAbstract.
 */
abstract class StatementAbstract
{
    protected $statement;

    protected $uniqueId;

    /** @var EvalExpression|null */
    protected $evalExpression;

    protected static $uniqueGenerator;

    /**
     * StatementAbstract construct.
     *
     * @param array $statement
     */
    public function __construct(array $statement)
    {
        $this->statement = $statement;
    }

    /**
     * Generates a uniqueId for those instructions that need it.
     *
     * @return int
     */
    public function getUniqueId()
    {
        if (!$this->uniqueId) {
            $this->uniqueId = self::$uniqueGenerator++;
        }

        return $this->uniqueId;
    }

    /**
     * @return EvalExpression|null
     */
    public function getEvalExpression()
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
