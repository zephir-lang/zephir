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

namespace Zephir\Operators\Other;

use ReflectionClass;
use ReflectionException;
use Zephir\Classes\Entry;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\MethodCall;
use Zephir\Operators\AbstractOperator;

use function Zephir\escape_class;

/**
 * Creates a new instance of a class
 */
class NewInstanceOperator extends AbstractOperator
{
    protected bool $literalOnly = false;

    /**
     * Creates a new instance.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $codePrinter = $compilationContext->codePrinter;

        /**
         * Resolves the symbol that expects the value
         */
        $this->literalOnly = false;
        $symbolVariable = $this->getExpectedNonLiteral($compilationContext, $expression);
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException('Objects can only be instantiated into dynamic variables', $expression);
        }

        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException('Cannot use non-heap variable to store new instance', $expression);
        }

        if ('return_value' !== $symbolVariable->getName()) {
            if ($symbolVariable->hasDifferentDynamicType(['unknown', 'undefined', 'object', 'null'])) {
                $compilationContext->logger->warning(
                    'Possible attempt to use non-object in "new" operator',
                    ['non-valid-new', $expression]
                );
            }
        }

        /**
         * Mark variables as dynamic objects
         */
        $symbolVariable->setDynamicTypes('object');

        $dynamic = false;
        if ('self' == $expression['class'] || 'static' == $expression['class']) {
            $className = $compilationContext->classDefinition->getCompleteName();
        } else {
            $className = $expression['class'];
            $dynamic = $expression['dynamic'];
            if (!$dynamic) {
                $className = $compilationContext->getFullName($expression['class']);
            }
        }

        if (!$className) {
            throw new CompilerException('A class name is required to instantiate the object', $expression);
        }

        /**
         * stdclass doesn't have constructors.
         */
        $lowerClassName = strtolower($className);
        $isStdClass = 'stdclass' === $lowerClassName || '\stdclass' === $lowerClassName;

        if ($isStdClass) {
            if (isset($expression['parameters']) && \count($expression['parameters']) > 0) {
                throw new CompilerException('stdclass does not receive parameters in its constructor', $expression);
            }

            $compilationContext->backend->initObject($symbolVariable, null, $compilationContext);
            $symbolVariable->setClassTypes('stdclass');
        } else {
            $classDefinition = false;
            if ($compilationContext->compiler->isClass($className)) {
                $classDefinition = $compilationContext->compiler->getClassDefinition($className);
            }

            /**
             * Classes inside the same extension
             */
            if ($classDefinition) {
                $compilationContext->backend->initObject($symbolVariable, $classDefinition->getClassEntry($compilationContext), $compilationContext);
                $symbolVariable->setClassTypes($className);
                $symbolVariable->setAssociatedClass($classDefinition);
            } else {
                /**
                 * Classes outside the extension
                 */
                if ($dynamic) {
                    $classNameVariable = $compilationContext->symbolTable->getVariableForRead($className, $compilationContext, $expression);
                    if ($classNameVariable->isNotVariableAndString()) {
                        throw new CompilerException('Only dynamic/string variables can be used in new operator. '.$classNameVariable->getName(), $expression);
                    }

                    /**
                     * Use a safe string version of the variable to avoid segfaults
                     */
                    $compilationContext->headersManager->add('kernel/object');
                    $safeSymbolVariable = $compilationContext->symbolTable->getTempVariable('variable', $compilationContext);
                    $safeSymbolVariable->setMustInitNull(true);
                    $safeSymbolVariable->setIsInitialized(true, $compilationContext);
                    $safeSymbolVariable->increaseUses();
                    $safeSymbol = $compilationContext->backend->getVariableCode($safeSymbolVariable);
                    $classNameSymbol = $compilationContext->backend->getVariableCode($classNameVariable);

                    $compilationContext->codePrinter->output('zephir_fetch_safe_class('.$safeSymbol.', '.$classNameSymbol.');');
                    $classNameToFetch = 'Z_STRVAL_P('.$safeSymbol.'), Z_STRLEN_P('.$safeSymbol.')';
                    $zendClassEntry = $compilationContext->cacheManager->getClassEntryCache()->get($classNameToFetch, true, $compilationContext);
                    $classEntry = $zendClassEntry->getName();

                    $compilationContext->codePrinter->output('if(!'.$classEntry.') {');
                    $compilationContext->codePrinter->increaseLevel();
                    $compilationContext->codePrinter->output('RETURN_MM_NULL();');
                    $compilationContext->codePrinter->decreaseLevel();
                    $compilationContext->codePrinter->output('}');
                } else {
                    if (!class_exists($className, false)) {
                        $compilationContext->logger->warning(
                            'Class "'.$className.'" does not exist at compile time',
                            ['nonexistent-class', $expression]
                        );
                        $classNameToFetch = 'SL("'.escape_class($className).'")';

                        $zendClassEntry = $compilationContext->cacheManager->getClassEntryCache()->get($classNameToFetch, false, $compilationContext);
                        $classEntry = $zendClassEntry->getName();
                    } else {
                        $reflectionClass = new ReflectionClass($className);
                        if ($reflectionClass->isInterface()) {
                            throw new CompilerException('Interfaces cannot be instantiated', $expression);
                        }

                        if (method_exists($reflectionClass, 'isTrait') && $reflectionClass->isTrait()) {
                            throw new CompilerException('Traits cannot be instantiated', $expression);
                        }

                        $classEntry = (new Entry($expression['class'], $compilationContext))->get();
                        $symbolVariable->setAssociatedClass($reflectionClass);
                    }

                    $symbolVariable->setClassTypes($className);
                }

                $compilationContext->backend->initObject($symbolVariable, $classEntry, $compilationContext);
            }
        }

        /**
         * Mark variable initialized
         */
        $symbolVariable->setIsInitialized(true, $compilationContext);

        /**
         * Don't check the constructor for stdclass instances
         */
        if ($isStdClass) {
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        /**
         * Call the constructor
         * For classes in the same extension we check if the class does implement a constructor
         * For external classes we always assume the class does implement a constructor.
         */
        $callConstructor = false;
        if ($compilationContext->compiler->isClass($className)) {
            $classDefinition = $compilationContext->compiler->getClassDefinition($className);

            if ('class' != $classDefinition->getType()) {
                throw new CompilerException('Only classes can be instantiated', $expression);
            }

            $callConstructor = $classDefinition->hasMethod('__construct');
        } else {
            if ($compilationContext->compiler->isBundledClass($className)) {
                $classDefinition = $compilationContext->compiler->getInternalClassDefinition($className);
                $callConstructor = $classDefinition->hasMethod('__construct');
            }
        }

        /* TODO: use the MethodBuilder here */
        if (isset($expression['parameters'])) {
            $callExpr = new Expression([
                'variable' => ['type' => 'variable', 'value' => $symbolVariable->getRealName()],
                'name' => '__construct',
                'parameters' => $expression['parameters'],
                'call-type' => MethodCall::CALL_NORMAL,
                'file' => $expression['file'],
                'line' => $expression['line'],
                'char' => $expression['char'],
                'check' => $callConstructor,
            ]);
        } else {
            $callExpr = new Expression([
                'variable' => ['type' => 'variable', 'value' => $symbolVariable->getRealName()],
                'name' => '__construct',
                'call-type' => MethodCall::CALL_NORMAL,
                'file' => $expression['file'],
                'line' => $expression['line'],
                'char' => $expression['char'],
                'check' => $callConstructor,
            ]);
        }

        /**
         * If we are certain that there is a constructor we call it, otherwise we checked it at runtime.
         */
        if ($callConstructor) {
            $methodCall = new MethodCall();
            $callExpr->setExpectReturn(false);
            $methodCall->compile($callExpr, $compilationContext);
        } else {
            $compilationContext->headersManager->add('kernel/fcall');

            /* TODO:, generate the code using builders */
            $compilationContext->backend->checkConstructor($symbolVariable, $compilationContext);
            $codePrinter->increaseLevel();

            $methodCall = new MethodCall();
            $callExpr->setExpectReturn(false);
            $methodCall->compile($callExpr, $compilationContext);

            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
            $codePrinter->outputBlankLine();
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
