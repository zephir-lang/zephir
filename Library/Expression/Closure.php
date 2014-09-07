<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Expression;

use Zephir\Exception;
use Zephir\Variable;
use Zephir\ClassDefinition;
use Zephir\CompilationContext;

/**
 * Closure
 *
 * Creates an anonymous function whitin the extension simulating a closure
 */
class Closure
{
    /**
     * @var bool
     */
    protected $_expecting = true;

    /**
     * @var bool
     */
    protected $_readOnly = false;

    /**
     * @var Variable
     */
    protected $_expectingVariable;

    /**
     * Unique closure ID
     */
    protected static function $_id = 0;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value
     *
     * @param boolean $expecting
     * @param Variable $expectingVariable
     */
    public function setExpectReturn($expecting, Variable $expectingVariable = null)
    {
        $this->_expecting = $expecting;
        $this->_expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only
     *
     * @param boolean $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->_readOnly = $readOnly;
    }

    /**
     * Creates a closure
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws \Zephir\CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $classDefinition = new ClassDefinition($compilationContext->config->get('namespace'), self::$_id . '__closure');

        $compilationContext->compiler->addClassDefinition($classDefinition);

        return new LiteralCompiledExpression('null', null, $expression)
    }
}
