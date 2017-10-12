<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Statements;

use Zephir\CompilationContext;

/**
 * Class StatementAbstract
 */
abstract class StatementAbstract
{
    protected $_statement;

    protected $_uniqueId;

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
