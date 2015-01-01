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
use Zephir\Builder\StatementsBlockBuilder;
use Zephir\Builder\Statements\ReturnStatementBuilder;
use Zephir\Builder\RawExpressionBuilder;

/**
 * ClosureArrow
 *
 * Creates an anonymous function within the extension simulating a closure using the arrow syntax
 */
class ClosureArrow extends Closure
{

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

        /**
         * Builds parameters using the only parameter available
         */
        $parameters = new ClassMethodParameters(array(
            array(
                'type' => 'parameter',
                'name' => $expression['left']['value'],
                'const' => 0,
                'data-type' => 'variable',
                'mandatory' => 0,
                'reference' => 0,
                'file' => $expression['left']['file'],
                'line' => $expression['left']['line'],
                'char' => $expression['left']['char'],
            ),
        ));

        $statementBlock = new StatementsBlockBuilder(array(
            new ReturnStatementBuilder(
                new RawExpressionBuilder($expression['right'])
            )
        ));

        $block = $statementBlock->get();

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
