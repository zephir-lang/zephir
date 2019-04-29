<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Expression;

use Zephir\ClassDefinition;
use Zephir\ClassMethod;
use Zephir\ClassMethodParameters;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\CompilerFileAnonymous;
use Zephir\Exception\CompilerException;
use Zephir\StatementsBlock;
use Zephir\Variable;

/**
 * Closure.
 *
 * Creates an anonymous function within the extension simulating a closure
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
     * Unique closure ID.
     */
    protected static $id = 0;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     *
     * @param bool     $expecting
     * @param Variable $expectingVariable
     */
    public function setExpectReturn($expecting, Variable $expectingVariable = null)
    {
        $this->expecting = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     *
     * @param bool $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->readOnly = $readOnly;
    }

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

        $compilerFile = new CompilerFileAnonymous($classDefinition, $compilationContext->config, $compilationContext);
        $compilerFile->setLogger($compilationContext->logger);

        $compilationContext->compiler->addClassDefinition($compilerFile, $classDefinition);

        if (isset($expression['left'])) {
            $parameters = new ClassMethodParameters($expression['left']);
        } else {
            $parameters = null;
        }

        if (isset($expression['right'])) {
            $block = $expression['right'];
        } else {
            $block = [];
        }

        $staticVariables = [];
        if (isset($expression['use']) && \is_array($expression['use'])) {
            foreach ($expression['use'] as $parameter) {
                $staticVariables[$parameter['name']] = $compilationContext->symbolTable->getVariable($parameter['name']);
            }
        }

        foreach ($staticVariables as $var) {
            $classDefinition->addProperty(new \Zephir\ClassProperty(
                $classDefinition,
                ['public', 'static'],
                $var->getName(),
                null,
                null,
                null
            ));
        }

        $classMethod = new ClassMethod(
            $classDefinition,
            ['public', 'final'],
            '__invoke',
            $parameters,
            new StatementsBlock($block),
            null,
            null,
            $expression,
            $staticVariables
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
        $compilationContext->headersManager->add('kernel/object');
        foreach ($staticVariables as $var) {
            if ('variable' == $var->getType() || 'array' == $var->getType()) {
                $compilationContext->backend->updateStaticProperty($classDefinition->getClassEntry(), $var->getName(), $var, $compilationContext);
            } else {
                $tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, true);
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
                $compilationContext->backend->updateStaticProperty($classDefinition->getClassEntry(), $var->getName(), $tempVariable, $compilationContext);
            }
        }
        ++self::$id;

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
