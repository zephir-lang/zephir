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

namespace Zephir\Expression;

use Zephir\ClassMethod;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\ClassMethodParameters;
use Zephir\CompiledExpression;
use Zephir\StatementsBlock;
use Zephir\ClassDefinition;
use Zephir\CompilationContext;
use Zephir\CompilerFileAnonymous;

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
     * @throws \Zephir\Compiler\CompilerException
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

        $exprBuilder = BuilderFactory::getInstance();
        $statementBlockBuilder = $exprBuilder->statements()->block(array(
            $exprBuilder->statements()
                ->returnX($exprBuilder->raw($expression['right']))
        ));

        $block = $statementBlockBuilder->build();

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
        $compilationContext->backend->createClosure($symbolVariable, $classDefinition, $compilationContext);

        self::$id++;

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
