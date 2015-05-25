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

namespace Zephir;

use Zephir\Detectors\ReadDetector;

/**
 * StaticCall
 *
 * Call methods in a static context
 */
class StaticCall extends Call
{
    /**
     * Calls static methods on the 'self/static' context
     *
     * @param string $context SELF / STATIC
     * @param string $methodName
     * @param array $expression
     * @param Variable $symbolVariable
     * @param boolean $mustInit
     * @param boolean $isExpecting
     * @param ClassDefinition $classDefinition
     * @param CompilationContext $compilationContext
     * @param ClassMethod $method
     */
    protected function call($context, $methodName, array $expression, $symbolVariable, $mustInit, $isExpecting, ClassDefinition $classDefinition, CompilationContext $compilationContext, ClassMethod $method)
    {
        if (!in_array($context, array('SELF', 'STATIC'))) {
            $context = 'SELF';
        }

        $codePrinter = $compilationContext->codePrinter;

        /**
         * Call static methods must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        /**
         * Check if the  method call can have an inline cache
         */
        $methodCache = $compilationContext->cacheManager->getStaticMethodCache();
        $cachePointer = $methodCache->get($compilationContext, isset($method) ? $method : null, false);

        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        } else {
            $params = array();
        }

        $isInternal = false;
        if (isset($method)) {
            $isInternal = $method->isInternal();
        }
        
        if (!$isInternal) {
            if (!count($params)) {
                if ($isExpecting) {
                    if ($symbolVariable->getName() == 'return_value') {
                        $codePrinter->output('ZEPHIR_RETURN_CALL_' . $context . '("' . $methodName . '", ' . $cachePointer . ');');
                    } else {
                        $codePrinter->output('ZEPHIR_CALL_' . $context . '(&' . $symbolVariable->getName() . ', "' . $methodName . '", ' . $cachePointer . ');');
                    }
                } else {
                    $codePrinter->output('ZEPHIR_CALL_' . $context . '(NULL, "' . $methodName . '", ' . $cachePointer . ');');
                }
            } else {
                if ($isExpecting) {
                    if ($symbolVariable->getName() == 'return_value') {
                        $codePrinter->output('ZEPHIR_RETURN_CALL_' . $context . '("' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                    } else {
                        $codePrinter->output('ZEPHIR_CALL_' . $context . '(&' . $symbolVariable->getName() . ', "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                    }
                } else {
                    $codePrinter->output('ZEPHIR_CALL_' . $context . '(NULL, "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                }
            }
        } else {
            if (!count($params)) {
                if ($isExpecting) {
                    if ($symbolVariable->getName() == 'return_value') {
                        $codePrinter->output('ZEPHIR_RETURN_CALL_INTERNAL_METHOD_P0(EG(This), ' . $method->getInternalName() . ');');
                    } else {
                        $codePrinter->output('ZEPHIR_CALL_INTERNAL_METHOD_P0(&' . $symbolVariable->getName() . ', EG(This), ' . $method->getInternalName() . ');');
                    }
                } else {
                    $codePrinter->output('ZEPHIR_CALL_INTERNAL_METHOD_NORETURN_P0(EG(This), ' . $method->getInternalName() . ');');
                }
            } else {
                if ($isExpecting) {
                    if ($symbolVariable->getName() == 'return_value') {
                        $codePrinter->output('ZEPHIR_RETURN_CALL_INTERNAL_METHOD_P' . count($params) . '(EG(This), ' . $method->getInternalName() . ', ' . join(', ', $params) . ');');
                    } else {
                        $codePrinter->output('ZEPHIR_CALL_INTERNAL_METHOD_P' . count($params) . '(&' . $symbolVariable->getName() . ', EG(This), ' . $method->getInternalName() . ', ' . join(', ', $params) . ');');
                    }
                } else {
                    $codePrinter->output('ZEPHIR_CALL_INTERNAL_METHOD_NORETURN_P' . count($params) . '(EG(This), ' . $method->getInternalName() . ', ' . join(', ', $params) . ');');
                }
            }
        }

        /**
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }

        $this->addCallStatusOrJump($compilationContext);
    }

    /**
     * Calls static methods on the 'parent' context
     *
     * @param string $methodName
     * @param array $expression
     * @param Variable $symbolVariable
     * @param boolean $mustInit
     * @param boolean $isExpecting
     * @param ClassDefinition $classDefinition
     * @param CompilationContext $compilationContext
     * @param ClassMethod $method
     */
    protected function callParent($methodName, array $expression, $symbolVariable, $mustInit, $isExpecting, ClassDefinition $classDefinition, CompilationContext $compilationContext, ClassMethod $method)
    {
        $codePrinter = $compilationContext->codePrinter;
        $classCe = $classDefinition->getClassEntry($compilationContext);
        //$className = str_replace('\\', '\\\\', $classDefinition->getCompleteName());

        /**
         * Call static methods must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        /**
         * Check if the  method call can have an inline cache
         */
        $methodCache = $compilationContext->cacheManager->getStaticMethodCache();
        $cachePointer = $methodCache->get($compilationContext, isset($method) ? $method : null);

        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        } else {
            $params = array();
        }

        if (!count($params)) {
            if ($isExpecting) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_PARENT(' . $classCe . ', this_ptr, "' . $methodName . '", ' . $cachePointer . ');');
                } else {
                    $codePrinter->output('ZEPHIR_CALL_PARENT(&' . $symbolVariable->getName() . ', ' . $classCe . ', this_ptr, "' . $methodName . '", ' . $cachePointer . ');');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_PARENT(NULL, ' . $classCe . ', this_ptr, "' . $methodName . '", ' . $cachePointer . ');');
            }
        } else {
            if ($isExpecting) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_PARENT(' . $classCe . ', this_ptr, "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                } else {
                    $codePrinter->output('ZEPHIR_CALL_PARENT(&' . $symbolVariable->getName() . ', ' . $classCe . ', this_ptr, "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_PARENT(NULL, ' . $classCe . ', this_ptr, "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
            }
        }

        /**
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }

        $this->addCallStatusOrJump($compilationContext);
    }

    /**
     * Calls static methods on the some class context
     *
     * @param string $methodName
     * @param array $expression
     * @param Variable $symbolVariable
     * @param boolean $mustInit
     * @param boolean $isExpecting
     * @param ClassDefinition $classDefinition
     * @param CompilationContext $compilationContext
     * @param ClassMethod $method
     */
    protected function callFromClass($methodName, array $expression, $symbolVariable, $mustInit, $isExpecting, ClassDefinition $classDefinition, CompilationContext $compilationContext, ClassMethod $method)
    {
        $codePrinter = $compilationContext->codePrinter;

        if ($classDefinition->isBundled()) {
            //if (!$compilationContext->symbolTable->hasVariable($variableName)) {
                $classEntryVariable = $compilationContext->symbolTable->addTemp('zend_class_entry', $compilationContext);
                $codePrinter->output($classEntryVariable->getName().' = zend_fetch_class(SL("' . str_replace('\\', '\\\\', $classDefinition->getName()) . '"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);');
            //}

            //$classEntryVariable = $compilationContext->symbolTable->getVariableForWrite($variableName, $compilationContext, $expression);
            $classEntry = $classEntryVariable->getName();
        } else {
            $classEntry = $classDefinition->getClassEntry($compilationContext);
        }

        /**
         * Call static methods must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        /**
         * Check if the  method call can have an inline cache
         */
        $methodCache = $compilationContext->cacheManager->getStaticMethodCache();
        $cachePointer = $methodCache->get($compilationContext, isset($method) ? $method : null);

        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        } else {
            $params = array();
        }

        if (!count($params)) {
            if ($isExpecting) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_CE_STATIC(' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ');');
                } else {
                    $codePrinter->output('ZEPHIR_CALL_CE_STATIC(&' . $symbolVariable->getName() . ', ' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ');');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_CE_STATIC(NULL, ' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ');');
            }
        } else {
            if ($isExpecting) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_CE_STATIC(' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                } else {
                    $codePrinter->output('ZEPHIR_CALL_CE_STATIC(&' . $symbolVariable->getName() . ', ' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_CE_STATIC(NULL, ' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
            }
        }

        /**
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }

        $this->addCallStatusOrJump($compilationContext);
    }

    /**
     * Calls static methods on using a dynamic variable as class
     *
     * @param string $methodName
     * @param array $expression
     * @param Variable $symbolVariable
     * @param boolean $mustInit
     * @param boolean $isExpecting
     * @param CompilationContext $compilationContext
     */
    protected function callFromDynamicClass($methodName, array $expression, $symbolVariable, $mustInit, $isExpecting, CompilationContext $compilationContext)
    {
        $codePrinter = $compilationContext->codePrinter;

        /**
         * Call static methods must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        $cachePointer = 'NULL, 0';

        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        } else {
            $params = array();
        }

        /**
         * Obtain the class entry from the variable
         */
        $classNameVariable = $compilationContext->symbolTable->getVariableForRead($expression['class'], $compilationContext, $expression);
        if ($classNameVariable->isNotVariableAndString()) {
            throw new CompilerException("Only dynamic/string variables can be used in dynamic classes", $expression);
        }

        $compilationContext->headersManager->add('kernel/object');

        $classEntryVariable = $compilationContext->symbolTable->addTemp('zend_class_entry', $compilationContext);
        $codePrinter->output($classEntryVariable->getName() . ' = zephir_fetch_class(' . $classNameVariable->getName() . ' TSRMLS_CC);');
        $classEntry = $classEntryVariable->getName();

        if (!count($params)) {
            if ($isExpecting) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_CE_STATIC(' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ');');
                } else {
                    $codePrinter->output('ZEPHIR_CALL_CE_STATIC(&' . $symbolVariable->getName() . ', ' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ');');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_CE_STATIC(NULL, ' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ');');
            }
        } else {
            if ($isExpecting) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_CE_STATIC(' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                } else {
                    $codePrinter->output('ZEPHIR_CALL_CE_STATIC(&' . $symbolVariable->getName() . ', ' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_CE_STATIC(NULL, ' . $classEntry . ', "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
            }
        }

        /**
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }

        $this->addCallStatusOrJump($compilationContext);
    }

    /**
     * Calls static methods on using a dynamic variable as class and a dynamic method
     *
     * @param array $expression
     * @param Variable $symbolVariable
     * @param boolean $mustInit
     * @param boolean $isExpecting
     * @param CompilationContext $compilationContext
     */
    protected function callFromDynamicClassDynamicMethod(array $expression, $symbolVariable, $mustInit, $isExpecting, CompilationContext $compilationContext)
    {
        $codePrinter = $compilationContext->codePrinter;

        /**
         * Call static methods must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        $cachePointer = 'NULL, 0';

        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        } else {
            $params = array();
        }

        /**
         * Obtain the class entry from the variable
         */
        $classNameVariable = $compilationContext->symbolTable->getVariableForRead($expression['class'], $compilationContext, $expression);
        if ($classNameVariable->isNotVariableAndString()) {
            throw new CompilerException("Only dynamic/string variables can be used in dynamic classes", $expression);
        }

        $compilationContext->headersManager->add('kernel/object');

        $classEntryVariable = $compilationContext->symbolTable->addTemp('zend_class_entry', $compilationContext);
        $codePrinter->output($classEntryVariable->getName() . ' = zephir_fetch_class(' . $classNameVariable->getName() . ' TSRMLS_CC);');
        $classEntry = $classEntryVariable->getName();

        /**
         * Obtain the method name from the variable
         */
        $methodNameVariable = $compilationContext->symbolTable->getVariableForRead($expression['name'], $compilationContext, $expression);
        if ($methodNameVariable->isNotVariableAndString()) {
            throw new CompilerException("Only dynamic/string variables can be used in dynamic methods", $expression);
        }

        if (!count($params)) {
            if ($isExpecting) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(' . $classEntry . ', ' . $methodNameVariable->getName() . ', ' . $cachePointer . ');');
                } else {
                    $codePrinter->output('ZEPHIR_CALL_CE_STATIC_ZVAL(&' . $symbolVariable->getName() . ', ' . $classEntry . ', ' . $methodNameVariable->getName() . ', ' . $cachePointer . ');');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_CE_STATIC_ZVAL(NULL, ' . $classEntry . ', ' . $methodNameVariable->getName() . ', ' . $cachePointer . ');');
            }
        } else {
            if ($isExpecting) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(' . $classEntry . ', ' . $methodNameVariable->getName() . ', ' . $cachePointer . ', ' . join(', ', $params) . ');');
                } else {
                    $codePrinter->output('ZEPHIR_CALL_CE_STATIC_ZVAL(&' . $symbolVariable->getName() . ', ' . $classEntry . ', ' . $methodNameVariable->getName() . ', ' . $cachePointer . ', ' . join(', ', $params) . ');');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_CE_STATIC_ZVAL(NULL, ' . $classEntry . ', ' . $methodNameVariable->getName() . ', ' . $cachePointer . ', ' . join(', ', $params) . ');');
            }
        }

        /**
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }

        $this->addCallStatusOrJump($compilationContext);
    }

    /**
     * Compiles a static method call
     *
     * @param Expression $expr
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(Expression $expr, CompilationContext $compilationContext)
    {
        $expression = $expr->getExpression();

        $methodName = strtolower($expression['name']);

        if (isset($expression['dynamic'])) {
            $dynamicMethod = $expression['dynamic'];
        } else {
            $dynamicMethod = false;
        }

        $symbolVariable = null;

        /**
         * Create temporary variable if needed
         */
        $mustInit = false;
        $isExpecting = $expr->isExpectingReturn();
        if ($isExpecting) {
            $symbolVariable = $expr->getExpectingVariable();
            if (is_object($symbolVariable)) {
                $readDetector = new ReadDetector($expression);
                if ($readDetector->detect($symbolVariable->getName(), $expression)) {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify('variable', $compilationContext, $expression);
                } else {
                    $mustInit = true;
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify('variable', $compilationContext, $expression);
            }
        }

        /**
         * Method calls only return zvals so we need to validate the target variable is also a zval
         */
        if ($isExpecting) {
            /**
             * At this point, we don't know the exact dynamic type returned by the static method call
             */
            $symbolVariable->setDynamicTypes('undefined');

            if (!$symbolVariable->isVariable()) {
                throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
            }
        }

        /**
         * Include fcall header
         */
        $compilationContext->headersManager->add('kernel/fcall');

        $compiler = $compilationContext->compiler;

        $dynamicClass = $expression['dynamic-class'];
        if (!$dynamicClass) {
            $className = $expression['class'];
            $classDefinition = false;

            if (!in_array($className, array('self', 'static', 'parent'))) {
                if (is_string($className)) {
                    $className = $compilationContext->getFullName($className);
                    if ($compiler->isClass($className)) {
                        $classDefinition = $compiler->getClassDefinition($className);
                    } else {
                        if ($compiler->isBundledClass($className)) {
                            $classDefinition = $compiler->getInternalClassDefinition($className);
                        } else {
                            throw new CompilerException("Class name: " . $className . " does not exist", $expression);
                        }
                    }
                } else {
                    foreach ($className as $singleClass) {
                        $className = $compilationContext->getFullName($singleClass);
                        if ($compiler->isClass($singleClass)) {
                            $classDefinition = $compiler->getClassDefinition($singleClass);
                        } else {
                            throw new CompilerException("Class name: " . $className . " does not exist", $expression);
                        }
                    }
                }
            } else {
                if ($className == 'parent') {
                    $classDefinition = $compilationContext->classDefinition;
                    $extendsClass = $classDefinition->getExtendsClass();
                    if (!$extendsClass) {
                        throw new CompilerException('Cannot call method "' . $methodName . '" on parent because class ' . $classDefinition->getCompleteName() . ' does not extend any class', $expression);
                    }
                    $currentClassDefinition = $classDefinition;
                    $classDefinition = $classDefinition->getExtendsClassDefinition();
                } else {
                    $classDefinition = $compilationContext->classDefinition;
                }
            }
        }

        /**
         * Check if the class implements the method
         */
        if (!$dynamicMethod && !$dynamicClass) {
            if (!$classDefinition->hasMethod($methodName)) {
                $possibleMethod = $classDefinition->getPossibleMethodName($methodName);
                if ($possibleMethod) {
                    throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not implement static method: '" . $expression['name'] . "'. Did you mean '" . $possibleMethod . "'?", $expression);
                } else {
                    throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not implement static method: '" . $expression['name'] . "'", $expression);
                }
            } else {
                $method = $classDefinition->getMethod($methodName);

                if ($method->isPrivate() && $method->getClassDefinition() != $compilationContext->classDefinition) {
                    throw new CompilerException("Cannot call private method '" . $methodName . "' out of its scope", $expression);
                }

                if (!in_array($className, array('self', 'static', 'parent'))) {
                    if (!$method->isStatic()) {
                        throw new CompilerException("Cannot call non-static method '" . $methodName . "' in a static way", $expression);
                    }
                }

                if (!$classDefinition->hasMethod("__callStatic")) {
                    if ($method instanceof ClassMethod && !$method->isBundled()) {
                        /**
                         * Try to produce an exception if method is called with a wrong number of parameters
                         */
                        if (isset($expression['parameters'])) {
                            $callNumberParameters = count($expression['parameters']);
                        } else {
                            $callNumberParameters = 0;
                        }

                        $classMethod = $classDefinition->getMethod($methodName);
                        $expectedNumberParameters = $classMethod->getNumberOfRequiredParameters();

                        if (!$expectedNumberParameters && $callNumberParameters > 0) {
                            $numberParameters = $classMethod->getNumberOfParameters();
                            if ($callNumberParameters > $numberParameters) {
                                throw new CompilerException("Method '" . $classDefinition->getCompleteName() . "::" . $expression['name'] . "' called with a wrong number of parameters, the method has: " . $expectedNumberParameters . ", passed: " . $callNumberParameters, $expression);
                            }
                        }

                        if ($callNumberParameters < $expectedNumberParameters) {
                            throw new CompilerException("Method '" . $classDefinition->getCompleteName() . "::" . $expression['name'] . "' called with a wrong number of parameters, the method has: " . $expectedNumberParameters . ", passed: " . $callNumberParameters, $expression);
                        }
                    }
                } else {
                    if (!isset($method)) {
                        $method = $classDefinition->getMethod("__callStatic");
                        if ($method->isPrivate() && $method->getClassDefinition() != $compilationContext->classDefinition) {
                            throw new CompilerException("Cannot call private magic method '__call' out of its scope", $expression);
                        }
                    }
                }
            }
        }

        /**
         * Call static methods in the same class, use the special context 'self' or special context 'static'
         * Call static methods in the 'self' context
         */
        if (!$dynamicMethod) {
            if ($dynamicClass) {
                $this->callFromDynamicClass($methodName, $expression, $symbolVariable, $mustInit, $isExpecting, $compilationContext);
            } else {
                if (in_array($className, array('self', 'static')) || $classDefinition == $compilationContext->classDefinition) {
                    $this->call(strtoupper($className), $methodName, $expression, $symbolVariable, $mustInit, $isExpecting, $classDefinition, $compilationContext, isset($method) ? $method : null);
                } else {
                    if ($className == 'parent') {
                        $this->callParent($methodName, $expression, $symbolVariable, $mustInit, $isExpecting, $currentClassDefinition, $compilationContext, isset($method) ? $method : null);
                    } else {
                        $this->callFromClass($methodName, $expression, $symbolVariable, $mustInit, $isExpecting, $classDefinition, $compilationContext, isset($method) ? $method : null);
                    }
                }
            }
        } else {
            if ($dynamicClass) {
                $this->callFromDynamicClassDynamicMethod($expression, $symbolVariable, $mustInit, $isExpecting, $compilationContext);
            }
        }

        /**
         * Add the last call status to the current symbol table
         */
        $this->addCallStatusFlag($compilationContext);

        /**
         * Transfer the return type-hint to the returned variable
         */
        if ($isExpecting) {
            if (isset($method)) {
                if ($method instanceof ClassMethod) {
                    $returnClassTypes = $method->getReturnClassTypes();
                    if ($returnClassTypes !== null) {
                        $symbolVariable->setDynamicTypes('object');
                        foreach ($returnClassTypes as $classType) {
                            $symbolVariable->setClassTypes($compilationContext->getFullName($classType));
                        }
                    }

                    $returnTypes = $method->getReturnTypes();
                    if ($returnTypes !== null) {
                        foreach ($returnTypes as $dataType => $returnType) {
                            $symbolVariable->setDynamicTypes($dataType);
                        }
                    }
                }
            }
        }

        /**
         * We can mark temporary variables generated as idle here
         */
        foreach ($this->getTemporalVariables() as $tempVariable) {
            $tempVariable->setIdle(true);
        }

        if ($isExpecting) {
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }
        return new CompiledExpression('null', null, $expression);
    }
}
