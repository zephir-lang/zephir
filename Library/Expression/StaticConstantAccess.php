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

use Zephir\ClassConstant;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Variable;

/**
 * StaticConstantAccess.
 *
 * Resolves class constants
 */
class StaticConstantAccess
{
    /** @var bool */
    protected $expecting = true;

    /** @var bool */
    protected $readOnly = false;

    /** @var Variable|null */
    protected $expectingVariable;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     *
     * @param bool          $expecting
     * @param Variable|null $expectingVariable
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
        $this->readOnly = (bool) $readOnly;
    }

    /**
     * Access a static constant class.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException|Exception
     *
     * @return CompiledExpression
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $compiler = &$compilationContext->compiler;
        $className = $expression['left']['value'];
        $constant = $expression['right']['value'];

        /*
         * Fetch the class definition according to the class where the constant
         * is supposed to be declared
         */
        if (!\in_array($className, ['this', 'self', 'static', 'parent'])) {
            $className = $compilationContext->getFullName($className);
            if ($compiler->isClass($className) || $compiler->isInterface($className)) {
                $classDefinition = $compiler->getClassDefinition($className);
            } else {
                if ($compiler->isBundledClass($className) || $compiler->isBundledInterface($className)) {
                    $classDefinition = $compiler->getInternalClassDefinition($className);
                } else {
                    throw new CompilerException("Cannot locate class '".$className."'", $expression['left']);
                }
            }
        } else {
            if (\in_array($className, ['self', 'static', 'this'])) {
                $classDefinition = $compilationContext->classDefinition;
            } else {
                if ('parent' == $className) {
                    $classDefinition = $compilationContext->classDefinition;
                    $extendsClass = $classDefinition->getExtendsClass();
                    if (!$extendsClass) {
                        throw new CompilerException(
                            sprintf(
                                'Cannot find constant called "%s" on parent because class %s does not extend any class',
                                $constant,
                                $classDefinition->getCompleteName()
                            ),
                            $expression
                        );
                    } else {
                        $classDefinition = $classDefinition->getExtendsClassDefinition();
                    }
                }
            }
        }

        /*
         * Constants are resolved to their values at compile time
         * so we need to check that they effectively do exist
         */
        if (!$classDefinition->hasConstant($constant)) {
            throw new CompilerException(
                sprintf(
                    "Class '%s' does not have a constant called: '%s'",
                    $classDefinition->getCompleteName(),
                    $constant
                ),
                $expression
            );
        }

        /*
         * We can optimize the reading of constants by avoiding query their value every time
         */
        if (!$compilationContext->config->get('static-constant-class-folding', 'optimizations')) {
            /*
             * Resolves the symbol that expects the value
             */
            if ($this->expecting) {
                if ($this->expectingVariable) {
                    $symbolVariable = $this->expectingVariable;
                    $symbolVariable->initVariant($compilationContext);
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

            /*
             * Variable that receives property accesses must be polymorphic
             */
            if (!$symbolVariable->isVariable()) {
                throw new CompilerException(
                    'Cannot use variable: '.$symbolVariable->getType().' to assign class constants',
                    $expression
                );
            }

            $symbolVariable->setDynamicTypes('undefined');

            $compilationContext->headersManager->add('kernel/object');

            $compilationContext->codePrinter->output(
                sprintf(
                    'zephir_get_class_constant(%s, %s, SS("%s"));',
                    $symbolVariable->getName(),
                    $classDefinition->getClassEntry($compilationContext),
                    $constant
                )
            );

            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $constantDefinition = $classDefinition->getConstant($constant);

        if ($constantDefinition instanceof ClassConstant) {
            $constantDefinition->processValue($compilationContext);
            $value = $constantDefinition->getValueValue();
            $type = $constantDefinition->getValueType();
        } else {
            $value = $constantDefinition;
            $type = \gettype($value);
            if ('integer' == $type) {
                $type = 'int';
            }
        }

        switch ($type) {
            case 'string':
            case 'int':
            case 'double':
            case 'float':
            case 'bool':
            case 'null':
                break;
            default:
                $compilationContext->logger->warning(
                    "Constant '".$constantDefinition->getName()."' does not exist at compile time",
                    ['nonexistent-constant', $expression]
                );

                return new CompiledExpression('null', null, $expression);
        }

        /*
         * Return the value as a literal expression
         */
        return new CompiledExpression($type, $value, $expression);
    }
}
