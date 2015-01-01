<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

use Zephir\ClassMethod;
use Zephir\Exception;
use Zephir\Variable;
use Zephir\ClassMethodParameters;
use Zephir\CompiledExpression;
use Zephir\StatementsBlock;
use Zephir\ClassDefinition;
use Zephir\CompilationContext;
use Zephir\CompilerFileAnonymous;
use Zephir\LiteralCompiledExpression;

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
    protected $expecting = true;

    /**
     * @var bool
     */
    protected $readOnly = false;

    /**
     * @var Variable
     */
    protected $expectingVariable;

    /**
     * Unique closure ID
     */
    protected static $id = 0;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value
     *
     * @param boolean $expecting
     * @param Variable $expectingVariable
     */
    public function setExpectReturn($expecting, Variable $expectingVariable = null)
    {
        $this->expecting = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only
     *
     * @param boolean $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->readOnly = $readOnly;
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
        $classDefinition = new ClassDefinition(
            $compilationContext->config->get('namespace'),
            self::$id . '__closure'
        );

        $classDefinition->setIsFinal(true);

        $compilerFile = new CompilerFileAnonymous(
            $classDefinition,
            $compilationContext->config,
            $compilationContext->logger
        );

        $compilationContext->compiler->addClassDefinition($compilerFile, $classDefinition);

        if (isset($expression['left'])) {
            $parameters = new ClassMethodParameters($expression['left']);
        } else {
            $parameters = null;
        }

        if (isset($expression['right'])) {
            $block = $expression['right'];
        } else {
            $block = array();
        }

        $classMethod = new ClassMethod(
            $classDefinition,
            array('public', 'final'),
            '__invoke',
            $parameters,
            new StatementsBlock($block),
            null,
            null,
            $expression
        );
        $classDefinition->addMethod($classMethod, $block);

        $compilationContext->headersManager->add('kernel/object');

        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
        }

        $symbolVariable->initVariant($compilationContext);
        $compilationContext->codePrinter->output('zephir_create_closure_ex(' . $symbolVariable->getName() . ', NULL, ' . $classDefinition->getClassEntry() . ', SS("__invoke") TSRMLS_CC);');

        self::$id++;

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
