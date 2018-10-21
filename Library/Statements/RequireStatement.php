<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\Expression;

/**
 * RequireStatement
 *
 * Require statement is used to execute PHP scripts in a given path
 */
class RequireStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        $expression = array(
            'type' => 'require',
            'left' => $this->_statement['expr'],
            'file' => $this->_statement['file'],
            'line' => $this->_statement['line'],
            'char' => $this->_statement['char']
        );

        $expr = new Expression($expression);
        $expr->setExpectReturn(false, null);
        $expr->compile($compilationContext);
    }
}
