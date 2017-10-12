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

namespace Zephir\Operators\Other;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompiledExpression;
use Zephir\MethodCall;
use Zephir\Utils;
use Zephir\Compiler\CompilerException;

/**
 * NewInstance
 *
 * Creates a new instance of a class
 */
class NewInstanceOperator extends BaseOperator
{
    protected $_literalOnly = false;

    /**
     * Creates a new instance
     *
     * @param $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $codePrinter = $compilationContext->codePrinter;

        /**
         * Resolves the symbol that expects the value
         */
        $this->_literalOnly = false;
        $symbolVariable = $this->getExpectedNonLiteral($compilationContext, $expression);
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Objects can only be instantiated into dynamic variables", $expression);
        }

        if ($symbolVariable->isLocalOnly()) {
            throw new CompilerException("Cannot use non-heap variable to store new instance", $expression);
        }

        if ($symbolVariable->getName() != 'return_value') {
            if ($symbolVariable->hasDifferentDynamicType(array('unknown', 'undefined', 'object', 'null'))) {
                $compilationContext->logger->warning('Possible attempt to use non-object in "new" operator', 'non-valid-new', $expression);
            }
        }

        /**
         * Mark variables as dynamic objects
         */
        $symbolVariable->setDynamicTypes('object');

        $dynamic = false;
        if ($expression['class'] == 'self' || $expression['class'] == 'static') {
            $className = $compilationContext->classDefinition->getCompleteName();
        } else {
            $className = $expression['class'];
            $dynamic = $expression['dynamic'];
            if (!$dynamic) {
                $className = $compilationContext->getFullName($expression['class']);
            }
        }

        if (!$className) {
            throw new CompilerException("A class name is required to instantiate the object", $expression);
        }

        /**
         * stdclass doesn't have constructors
         */
        $lowerClassName = strtolower($className);
        $isStdClass = $lowerClassName === 'stdclass' || $lowerClassName === '\stdclass';

        if ($isStdClass) {
            if (isset($expression['parameters']) && count($expression['parameters']) > 0) {
                throw new CompilerException("stdclass does not receive parameters in its constructor", $expression);
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
                        throw new CompilerException("Only dynamic/string variables can be used in new operator. " . $classNameVariable->getName(), $expression);
                    }

                    /**
                     * Use a safe string version of the variable to avoid segfaults
                     */
                    $compilationContext->headersManager->add('kernel/object');
                    $safeSymbolVariable = $compilationContext->symbolTable->getTempVariable('variable', $compilationContext, $expression);
                    $safeSymbolVariable->setMustInitNull(true);
                    $safeSymbolVariable->setIsInitialized(true, $compilationContext, $expression);
                    $safeSymbolVariable->increaseUses();
                    $safeSymbol = $compilationContext->backend->getVariableCode($safeSymbolVariable);
                    $classNameSymbol = $compilationContext->backend->getVariableCode($classNameVariable);

                    $compilationContext->codePrinter->output('zephir_fetch_safe_class(' . $safeSymbol .', ' . $classNameSymbol .');');
                    $classNameToFetch = 'Z_STRVAL_P(' . $safeSymbol . '), Z_STRLEN_P(' . $safeSymbol . ')';
                    $zendClassEntry = $compilationContext->cacheManager->getClassEntryCache()->get($classNameToFetch, true, $compilationContext);
                    $classEntry = $zendClassEntry->getName();
                } else {
                    if (!class_exists($className, false)) {
                        $compilationContext->logger->warning('Class "' . $className . '" does not exist at compile time', "nonexistent-class", $expression);
                        $classNameToFetch = 'SL("' . Utils::escapeClassName($className) . '")';

                        $zendClassEntry = $compilationContext->cacheManager->getClassEntryCache()->get($classNameToFetch, false, $compilationContext);
                        $classEntry = $zendClassEntry->getName();
                    } else {
                        $reflectionClass = new \ReflectionClass($className);
                        if ($reflectionClass->isInterface()) {
                            throw new CompilerException('Interfaces cannot be instantiated', $expression);
                        } else {
                            if (method_exists($reflectionClass, 'isTrait')) {
                                if ($reflectionClass->isTrait()) {
                                    throw new CompilerException('Traits cannot be instantiated', $expression);
                                }
                            }
                        }

                        $classEntry = $compilationContext->classDefinition->getClassEntryByClassName($className, $compilationContext, true);
                        if (!$classEntry) {
                            $classNameToFetch = 'SL("' . Utils::escapeClassName($className) . '")';
                            $zendClassEntry = $compilationContext->cacheManager->getClassEntryCache()->get($classNameToFetch, false, $compilationContext);
                            $classEntry = $zendClassEntry->getName();
                        } else {
                            $symbolVariable->setAssociatedClass($reflectionClass);
                        }
                    }

                    $symbolVariable->setClassTypes($className);
                }
                $compilationContext->backend->initObject($symbolVariable, $classEntry, $compilationContext);
            }
        }

        /**
         * Mark variable initialized
         */
        $symbolVariable->setIsInitialized(true, $compilationContext, $expression);

        /**
         * Don't check the constructor for stdclass instances
         */
        if ($isStdClass) {
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        /**
         * Call the constructor
         * For classes in the same extension we check if the class does implement a constructor
         * For external classes we always assume the class does implement a constructor
         */
        $callConstructor = false;
        if ($compilationContext->compiler->isClass($className)) {
            $classDefinition = $compilationContext->compiler->getClassDefinition($className);

            if ($classDefinition->getType() != 'class') {
                throw new CompilerException("Only classes can be instantiated", $expression);
            }

            $callConstructor = $classDefinition->hasMethod("__construct");
        } else {
            if ($compilationContext->compiler->isBundledClass($className)) {
                $classDefinition = $compilationContext->compiler->getInternalClassDefinition($className);
                $callConstructor = $classDefinition->hasMethod("__construct");
            }
        }

        /* @TODO use the MethodBuilder here */
        if (isset($expression['parameters'])) {
            $callExpr = new Expression(array(
                'variable'   => array('type' => 'variable', 'value' => $symbolVariable->getRealName()),
                'name'       => '__construct',
                'parameters' => $expression['parameters'],
                'call-type'  => MethodCall::CALL_NORMAL,
                'file'       => $expression['file'],
                'line'       => $expression['line'],
                'char'       => $expression['char'],
                'check'      => $callConstructor
            ));
        } else {
            $callExpr = new Expression(array(
                'variable'  => array('type' => 'variable', 'value' => $symbolVariable->getRealName()),
                'name'      => '__construct',
                'call-type' => MethodCall::CALL_NORMAL,
                'file'      => $expression['file'],
                'line'      => $expression['line'],
                'char'      => $expression['char'],
                'check'     => $callConstructor
            ));
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

            /* @todo, generate the code using builders */
            $compilationContext->backend->checkConstructor($symbolVariable, $compilationContext);
            $codePrinter->increaseLevel();

            $methodCall = new MethodCall();
            $callExpr->setExpectReturn(false);
            $methodCall->compile($callExpr, $compilationContext);

            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
