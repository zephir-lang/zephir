<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression;

use Zephir\ClassDefinition;
use Zephir\ClassMethod;
use Zephir\ClassMethodParameters;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\CompilerFileAnonymous;
use Zephir\Exception\CompilerException;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\StatementsBlock;

/**
 * ClosureArrow.
 *
 * Creates an anonymous function within the extension simulating a closure using the arrow syntax
 */
class ClosureArrow extends Closure
{
    /**
     * Creates a closure.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     *
     * @return CompiledExpression
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $classDefinition = new ClassDefinition(
            $compilationContext->config->get('namespace'),
            self::$id.'__closure'
        );

        $classDefinition->setIsFinal(true);

        $compilerFile = new CompilerFileAnonymous($classDefinition, $compilationContext->config);
        $compilerFile->setLogger($compilationContext->logger);

        $compilationContext->compiler->addClassDefinition($compilerFile, $classDefinition);

        /**
         * Builds parameters using the only parameter available.
         */
        $parameters = new ClassMethodParameters([
            [
                'type' => 'parameter',
                'name' => $expression['left']['value'],
                'const' => 0,
                'data-type' => 'variable',
                'mandatory' => 0,
                'reference' => 0,
                'file' => $expression['left']['file'],
                'line' => $expression['left']['line'],
                'char' => $expression['left']['char'],
            ],
        ]);

        $exprBuilder = BuilderFactory::getInstance();
        $statementBlockBuilder = $exprBuilder->statements()->block([
            $exprBuilder->statements()
                ->returnX($exprBuilder->raw($expression['right'])),
        ]);

        $block = $statementBlockBuilder->build();

        $classMethod = new ClassMethod(
            $classDefinition,
            ['public', 'final'],
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

        ++self::$id;

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
