<?php

/**
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Optimizers\EvalExpression;

/**
 * Class StatementAbstract
 */
abstract class StatementAbstract
{
    protected $_statement;

    protected $_uniqueId;

    /** @var EvalExpression|null */
    protected $_evalExpression;

    protected static $_uniqueGenerator;

    /**
     * StatementAbstract construct
     *
     * @param array $statement
     */
    public function __construct(array $statement)
    {
        $this->_statement = $statement;
    }

    /**
     * Generates a uniqueId for those instructions that need it
     *
     * @return int
     */
    public function getUniqueId()
    {
        if (!$this->_uniqueId) {
            $this->_uniqueId = self::$_uniqueGenerator++;
        }
        return $this->_uniqueId;
    }

    /**
     * @return null|EvalExpression
     */
    public function getEvalExpression()
    {
        return $this->_evalExpression;
    }

    /**
     * Compiles the statement
     *
     * @param CompilationContext $compilationContext
     */
    abstract public function compile(CompilationContext $compilationContext);
}
