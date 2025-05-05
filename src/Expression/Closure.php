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
use Zephir\Class\Property;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\CompilerFileAnonymous;
use Zephir\Exception;
use Zephir\StatementsBlock;
use Zephir\Variable\Variable;

use function in_array;
use function is_array;

/**
 * Creates an anonymous function within the extension simulating a closure
 */
class Closure
{
    /**
     * @var bool
     */
    protected bool $expecting = true;
    /**
     * @var Variable|null
     */
    protected ?Variable $expectingVariable = null;
    /**
     * Unique closure ID.
     */
    protected static $id = 0;
    /**
     * @var bool
     */
    protected bool $readOnly = false;

    /**
     * Creates a closure.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $classDefinition = new Definition(
            $compilationContext->config->get('namespace'),
            self::$id . '__closure'
        );

        $classDefinition->setIsFinal(true);

        $compilerFile = new CompilerFileAnonymous($classDefinition, $compilationContext->config, $compilationContext);
        $compilerFile->setLogger($compilationContext->logger);

        $compilationContext->compiler->addClassDefinition($compilerFile, $classDefinition);

        $parameters = null;
        if (isset($expression['left'])) {
            $parameters = new Parameters($expression['left']);
        }

        $block = $expression['right'] ?? [];

        $useVariables = [];

        if (isset($expression['use']) && is_array($expression['use'])) {
            foreach ($expression['use'] as $parameter) {
                $useVariables[$parameter['name']] = $compilationContext->symbolTable->getVariable(
                    $parameter['name']
                );
            }
        }

        foreach ($useVariables as $var) {
            $classDefinition->addProperty(
                new Property(
                    $classDefinition,
                    ['public'],
                    $var->getName(),
                    null,
                    null,
                    null
                )
            );

            array_unshift(
                $block,
                [
                    'type'      => 'declare',
                    'data-type' => 'variable',
                    'variables' => [
                        [
                            'variable' => $var->getName()
                        ]
                    ]
                ],
                [
                    'type'      => 'let',
                    'assignments' => [
                        [
                            'assign-type' => 'variable',
                            'operator'    => 'assign',
                            'variable'    => $var->getName(),
                            'expr'        => [
                                'type'      => 'property-access',
                                'left'      => [
                                    'type'      => 'variable',
                                    'value'     => 'this',
                                ],
                                'right' => [
                                    'type'      => 'variable',
                                    'value'     => $var->getName(),
                                ]
                            ]
                        ]
                    ]
                ]
            );
        }

        $classMethod = new Method(
            $classDefinition,
            ['public', 'final'],
            '__invoke',
            $parameters,
            new StatementsBlock($block),
            null,
            null,
            $expression,
        );

        $symbolVariable = $this->generateClosure(
            $classDefinition,
            $classMethod,
            $block,
            $compilationContext,
            $expression
        );
        $compilationContext->headersManager->add('kernel/object');

        //Only Compile Them If They Exist
        if (count($useVariables)) {
            $classDefinition->addMethod($this->buildConstructor($classDefinition, $expression['use']));
            $this->compileVariables($compilationContext, $symbolVariable, $useVariables);
        }

        ++self::$id;

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    private function buildConstructor(Definition $classDefinition, array $useProperties): Method
    {
        $useStatements = [];

        foreach ($useProperties as $property) {
            $useStatements[] = [
                'type'      => 'let',
                'assignments' => [
                    [
                        'assign-type' => 'variable',
                        'operator'    => 'assign',
                        'variable'    => $property['name'],
                        'expr'        => [
                            'type'      => 'property-access',
                            'left'      => [
                                'type'      => 'variable',
                                'value'     => 'this',
                            ],
                            'right' => [
                                'type'      => 'variable',
                                'value'     => $property['name'],
                            ]
                        ]
                    ]
                ]
            ];
        }

        return new Method(
            $classDefinition,
            ['public', 'final'],
            '__construct',
            new Parameters($useProperties),
            new StatementsBlock($useStatements),
            null,
            null,
            [],
        );
    }

    private function compileVariables(CompilationContext $compilationContext, Variable $symbolVariable, array $useVariables): void
    {
        foreach ($useVariables as $var) {
            if (in_array($var->getType(), ['variable', 'array'])) {
                $compilationContext->backend->updateRawProperty(
                    $symbolVariable->getName(),
                    $var->getName(),
                    $var,
                    $compilationContext
                );

                continue;
            }

            $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                'variable',
                $compilationContext,
                true
            );

            switch ($var->getType()) {
                case 'int':
                case 'uint':
                case 'long':
                case 'ulong':
                case 'char':
                case 'uchar':
                    $compilationContext->backend->assignLong($tempVariable, $var, $compilationContext);
                    break;
                case 'double':
                    $compilationContext->backend->assignDouble($tempVariable, $var, $compilationContext);
                    break;
                case 'bool':
                    $compilationContext->backend->assignBool($tempVariable, $var, $compilationContext);
                    break;
                case 'string':
                    $compilationContext->backend->assignString($tempVariable, $var, $compilationContext);
                    break;
                default:
                    break;
            }

            $compilationContext->backend->updateRawProperty(
                $symbolVariable->getName(),
                $var->getName(),
                $tempVariable,
                $compilationContext
            );
        }
    }

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     */
    public function setExpectReturn(bool $expecting, ?Variable $expectingVariable = null): void
    {
        $this->expecting         = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     *
     * @param bool $readOnly
     */
    public function setReadOnly(bool $readOnly): void
    {
        $this->readOnly = $readOnly;
    }

    /**
     * @param Definition         $classDefinition
     * @param Method             $classMethod
     * @param mixed              $block
     * @param CompilationContext $compilationContext
     * @param array              $expression
     *
     * @return Variable|null
     */
    protected function generateClosure(
        Definition $classDefinition,
        Method $classMethod,
        mixed $block,
        CompilationContext $compilationContext,
        array $expression
    ): ?Variable {
        $classDefinition->addMethod($classMethod, $block);

        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext,
                    $expression
                );
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'variable',
                $compilationContext,
                $expression
            );
        }

        $symbolVariable->initVariant($compilationContext);
        $compilationContext->backend->createClosure(
            $symbolVariable,
            $classDefinition,
            $compilationContext,
            $expression['use'] ?? []
        );

        return $symbolVariable;
    }
}
