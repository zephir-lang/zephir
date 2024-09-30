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

namespace Zephir\Expression;

use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Class\Method\Parameters;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\CompilerFileAnonymous;
use Zephir\Exception\CompilerException;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\StatementsBlock;

/**
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
     * @return CompiledExpression
     *
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $classDefinition = new Definition(
            $compilationContext->config->get('namespace'),
            self::$id . '__closure'
        );

        $classDefinition->setIsFinal(true);

        $compilerFile = new CompilerFileAnonymous($classDefinition, $compilationContext->config);
        $compilerFile->setLogger($compilationContext->logger);

        $compilationContext->compiler->addClassDefinition($compilerFile, $classDefinition);

        /**
         * Builds parameters using the only parameter available.
         */
        $parameters = new Parameters([
            [
                'type'      => 'parameter',
                'name'      => $expression['left']['value'],
                'const'     => 0,
                'data-type' => 'variable',
                'mandatory' => 0,
                'reference' => 0,
                'file'      => $expression['left']['file'],
                'line'      => $expression['left']['line'],
                'char'      => $expression['left']['char'],
            ],
        ]);

        $exprBuilder           = BuilderFactory::getInstance();
        $statementBlockBuilder = $exprBuilder->statements()->block([
            $exprBuilder->statements()
                        ->returnX($exprBuilder->raw($expression['right'])),
        ]);

        $block = $statementBlockBuilder->build();

        $classMethod = new Method(
            $classDefinition,
            ['public', 'final'],
            '__invoke',
            $parameters,
            new StatementsBlock($block),
            null,
            null,
            $expression
        );

        $symbolVariable = $this->generateClosure(
            $classDefinition,
            $classMethod,
            $block,
            $compilationContext,
            $expression
        );

        ++self::$id;

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
