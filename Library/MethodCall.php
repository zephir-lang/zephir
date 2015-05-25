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
 * MethodCall
 *
 * Call methods in a non-static context
 */
class MethodCall extends Call
{

    /**
     * Function is called using a normal method name
     */
    const CALL_NORMAL = 1;

    /**
     * Function is called using a dynamic variable as method name
     */
    const CALL_DYNAMIC = 2;

    /**
     * Function is called using a dynamic string as method name
     */
    const CALL_DYNAMIC_STRING = 3;

    /**
     * Examine internal class information and returns the method called
     *
     * @param CompilationContext $compilationContext
     * @param Variable $caller
     * @param string $methodName
     * @return array
     */
    private function getRealCalledMethod(CompilationContext $compilationContext, Variable $caller, $methodName)
    {
        $compiler = $compilationContext->compiler;

        $numberPoly = 0;
        $method = null;

        if ($caller->getRealName() == 'this') {
            $classDefinition = $compilationContext->classDefinition;
            if ($classDefinition->hasMethod($methodName)) {
                $numberPoly++;
                $method = $classDefinition->getMethod($methodName);
            }
        } else {
            $classTypes = $caller->getClassTypes();
            foreach ($classTypes as $classType) {
                if ($compiler->isClass($classType) || $compiler->isInterface($classType) || $compiler->isBundledClass($classType) || $compiler->isBundledInterface($classType)) {
                    if ($compiler->isInterface($classType)) {
                        continue;
                    }

                    if ($compiler->isClass($classType)) {
                        $classDefinition = $compiler->getClassDefinition($classType);
                    } else {
                        $classDefinition = $compiler->getInternalClassDefinition($classType);
                    }

                    if (!$classDefinition) {
                        continue;
                    }

                    if ($classDefinition->hasMethod($methodName) && !$classDefinition->isInterface()) {
                        $numberPoly++;
                        $method = $classDefinition->getMethod($methodName);
                    }
                }
            }
        }

        return array($numberPoly, $method);
    }

    /**
     * Compiles a method call
     *
     * @param Expression $expr
     * @param CompilationContext $compilationContext
     */
    public function compile(Expression $expr, CompilationContext $compilationContext)
    {
        $expression = $expr->getExpression();

        $exprVariable = new Expression($expression['variable']);
        $exprVariable->setReadOnly(true);
        $exprCompiledVariable = $exprVariable->compile($compilationContext);

        $builtInType = false;
        switch ($exprCompiledVariable->getType()) {

            case 'variable':
                $variableVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression);
                switch ($variableVariable->getType()) {

                    case 'variable':
                        $caller = $variableVariable;
                        break;

                    default:

                        /* Check if there is a built-in type optimizer available */
                        $builtInTypeClass = 'Zephir\Types\\' . ucfirst($variableVariable->getType()) . 'Type';
                        if (class_exists($builtInTypeClass)) {
                            /**
                             * @var $builtInType \Zephir\Types\AbstractType
                             */
                            $builtInType = new $builtInTypeClass;
                            $caller = $exprCompiledVariable;
                        } else {
                            throw new CompilerException("Methods cannot be called on variable type: " . $variableVariable->getType(), $expression);
                        }
                }
                break;

            default:

                /* Check if there is a built-in type optimizer available */
                $builtInTypeClass = 'Zephir\Types\\' . ucfirst($exprCompiledVariable->getType()) . 'Type';
                if (class_exists($builtInTypeClass)) {
                    $builtInType = new $builtInTypeClass;
                    $caller = $exprCompiledVariable;
                } else {
                    throw new CompilerException("Cannot use expression: " . $exprCompiledVariable->getType() . " as method caller", $expression['variable']);
                }
        }

        $codePrinter = $compilationContext->codePrinter;

        $type = $expression['call-type'];

        /**
         * In normal method calls and dynamic string method calls we just use the name given by the user
         */
        if ($type == self::CALL_NORMAL || $type == self::CALL_DYNAMIC_STRING) {
            $methodName = strtolower($expression['name']);
        } else {
            $variableMethod = $compilationContext->symbolTable->getVariableForRead($expression['name'], $compilationContext, $expression);

            if (is_object($builtInType)) {
                throw new CompilerException("Dynamic method invocation for type: " . $variableMethod->getType() . " is not supported", $expression);
            }

            if ($variableMethod->isNotVariableAndString()) {
                throw new CompilerException("Cannot use variable type: " . $variableMethod->getType() . " as dynamic method name", $expression);
            }
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
                if ($caller == $symbolVariable || $readDetector->detect($symbolVariable->getName(), $expression)) {
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
        if (!$builtInType) {
            if ($isExpecting) {
                if (!$symbolVariable->isVariable()) {
                    throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
                }

                /**
                 * At this point, we don't know the exact dynamic type returned by the method call
                 */
                $symbolVariable->setDynamicTypes('undefined');
            }
        } else {
            return $builtInType->invokeMethod($methodName, $caller, $compilationContext, $this, $expression);
        }

        $check = true;
        if (isset($expression['check'])) {
            $check = $expression['check'];
        }

        /**
         * Try to check if the method exist in the callee, only when method call is self::CALL_NORMAL
         */
        if ($type == self::CALL_NORMAL) {
            if ($variableVariable->getRealName() == 'this') {
                $classDefinition = $compilationContext->classDefinition;
                if (!$classDefinition->hasMethod($methodName)) {
                    if ($check) {
                        $found = false;
                        $interfaces = $classDefinition->isAbstract() ? $classDefinition->getImplementedInterfaces() : null;
                        if (is_array($interfaces)) {
                            $compiler = $compilationContext->compiler;
                            foreach ($interfaces as $interface) {
                                $classInterfaceDefinition = $compiler->getClassDefinition($interface);
                                if ($classInterfaceDefinition->hasMethod($methodName)) {
                                    $found = true;
                                    $classMethod = $classInterfaceDefinition->getMethod($methodName);
                                    break;
                                }
                            }
                        }
                        if (!$found) {
                            $possibleMethod = $classDefinition->getPossibleMethodName($expression['name']);
                            if ($possibleMethod && $expression['name'] != $possibleMethod) {
                                throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not implement method: '" . $expression['name'] . "'. Did you mean '" . $possibleMethod . "'?", $expression);
                            } else {
                                throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not implement method: '" . $expression['name'] . "'", $expression);
                            }
                        }
                    }
                } else {
                    if ($check) {
                        $classMethod = $classDefinition->getMethod($methodName);
                    }
                }

                if ($check) {
                    /**
                     * Private methods must be called in their declaration scope
                     */
                    if ($classMethod->isPrivate()) {
                        if ($classMethod->getClassDefinition() != $classDefinition) {
                            throw new CompilerException("Cannot call private method '" . $expression['name'] . "' out of its scope", $expression);
                        }
                    }

                    /**
                     * Try to produce an exception if method is called with a wrong number of parameters
                     */
                    if (isset($expression['parameters'])) {
                        $callNumberParameters = count($expression['parameters']);
                    } else {
                        $callNumberParameters = 0;
                    }

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

                    $method = $classMethod;
                }
            } else {
                /**
                 * Variables whose dynamic type is 'object' can be used
                 * to determine method existence in compile time
                 */
                if ($check && $variableVariable->hasAnyDynamicType('object')) {
                    $classTypes = $variableVariable->getClassTypes();

                    if (count($classTypes)) {
                        $numberImplemented = 0;
                        $compiler = $compilationContext->compiler;
                        foreach ($classTypes as $classType) {
                            if ($compiler->isClass($classType) || $compiler->isInterface($classType) ||
                                $compiler->isBundledClass($classType) || $compiler->isBundledInterface($classType)) {
                                if ($compiler->isClass($classType) || $compiler->isInterface($classType)) {
                                    $classDefinition = $compiler->getClassDefinition($classType);
                                } else {
                                    $classDefinition = $compiler->getInternalClassDefinition($classType);
                                }

                                if (!$classDefinition) {
                                    throw new CompilerException("Cannot locate class definition for class " . $classType, $expression);
                                }

                                if (!$classDefinition->hasMethod($methodName)) {
                                    if (!$classDefinition->isInterface()) {
                                        if (count($classTypes) == 1) {
                                            throw new CompilerException("Class '" . $classType . "' does not implement method: '" . $expression['name'] . "'", $expression);
                                        }
                                    }
                                    continue;
                                }

                                $method = $classDefinition->getMethod($methodName);

                                /**
                                 * Private methods must be called in their declaration scope
                                 */
                                if ($method->isPrivate()) {
                                    if ($method->getClassDefinition() != $classDefinition) {
                                        throw new CompilerException("Cannot call private method '" . $expression['name'] . "' out of its scope", $expression);
                                    }
                                }

                                /**
                                 * Check visibility for protected methods
                                 */
                                if ($method->isProtected() && $method->getClassDefinition() != $classDefinition && $method->getClassDefinition() != $classDefinition->getExtendsClass()) {
                                    throw new CompilerException("Cannot call protected method '" . $expression['name'] . "' out of its scope", $expression);
                                }

                                /**
                                 * Try to produce an exception if a method is called with a wrong number of parameters
                                 * We only check extension parameters if methods are extension methods
                                 * Internal methods may have invalid Reflection information
                                 */
                                if ($method instanceof ClassMethod && !$method->isBundled()) {
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
                                            $className = $classDefinition->getCompleteName();
                                            throw new CompilerException("Method '" . $className . "::" . $expression['name'] . "' called with a wrong number of parameters, the method has: " . $expectedNumberParameters . ", passed: " . $callNumberParameters, $expression);
                                        }
                                    }

                                    if ($callNumberParameters < $expectedNumberParameters) {
                                        throw new CompilerException("Method '" . $classDefinition->getCompleteName() . "::" . $expression['name'] . "' called with a wrong number of parameters, the method has: " . $expectedNumberParameters . ", passed: " . $callNumberParameters, $expression);
                                    }
                                }

                                /**
                                 * The method is checked in the first class that implements the method
                                 * We could probably have collisions here
                                 */
                                $numberImplemented++;
                                break;
                            } else {
                                $numberImplemented++;
                                $compilationContext->logger->warning("Class \"" . $classType . "\" does not exist at compile time", "nonexistent-class", $expression);
                            }
                        }

                        if ($numberImplemented == 0) {
                            if (!$classDefinition->isInterface()) {
                                if (count($classTypes) > 1) {
                                    throw new CompilerException("None of classes: '" . join(' or ', $classTypes) . "' implement method: '" . $expression['name'] . "'", $expression);
                                } else {
                                    throw new CompilerException("Class '" . $classTypes[0] . "' does not implement method: '" . $expression['name'] . "'", $expression);
                                }
                            } else {
                                // @TODO, raise an exception here?
                            }
                        }
                    }
                }
            }
        }

        if (isset($method)) {
            $this->_reflection = $method;
        }

        /**
         * Transfer the return type-hint to the returned variable
         */
        if ($isExpecting) {
            if (isset($method)) {
                if ($method instanceof ClassMethod) {
                    if ($method->isVoid()) {
                        throw new CompilerException("Method '" . $classDefinition->getCompleteName() . "::" . $expression['name'] . "' is marked as 'void' and it does not return anything", $expression);
                    }

                    $returnClassTypes = $method->getReturnClassTypes();

                    if ($returnClassTypes !== null) {
                        $symbolVariable->setDynamicTypes('object');
                        foreach ($returnClassTypes as &$returnClassType) {
                            $returnClassType = $compilationContext->getFullName($returnClassType);
                        }
                        $symbolVariable->setClassTypes($returnClassTypes);
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
         * Some parameters in internal methods receive parameters as references
         */
        if (isset($expression['parameters'])) {
            $references = array();
            if ($type == self::CALL_NORMAL || $type == self::CALL_DYNAMIC_STRING) {
                if (isset($method)) {
                    if ($method instanceof \ReflectionMethod) {
                        $position = 0;
                        foreach ($method->getParameters() as $parameter) {
                            if ($parameter->isPassedByReference()) {
                                $references[$position] = true;
                            }
                            $position++;
                        }
                    }
                }
            }
        }

        /**
         * Include fcall header
         */
        $compilationContext->headersManager->add('kernel/fcall');

        /**
         * Call methods must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        /**
         * Mark references
         */
        if (isset($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression, isset($method) ? $method : null);
            if (count($references)) {
                foreach ($params as $position => $param) {
                    if (isset($references[$position])) {
                        $compilationContext->codePrinter->output('Z_SET_ISREF_P(' . $param . ');');
                    }
                }
            }

            // We check here if a correct parameter type is passed to the called method
            if ($type == self::CALL_NORMAL) {
                if (isset($method) && $method instanceof ClassMethod && isset($expression['parameters'])) {
                    $resolvedTypes = $this->getResolvedTypes();
                    $resolvedDynamicTypes = $this->getResolvedDynamicTypes();
                    //$typeInference = $method->getStaticTypeInferencePass();

                    foreach ($method->getParameters() as $n => $parameter) {
                        if (isset($parameter['data-type'])) {
                            if (!isset($resolvedTypes[$n])) {
                                continue;
                            }

                            /**
                             * If the passed parameter is different to the expected type we show a warning
                             */
                            if ($resolvedTypes[$n] != $parameter['data-type']) {
                                switch ($resolvedTypes[$n]) {

                                    case 'bool':
                                    case 'boolean':
                                        switch ($parameter['data-type']) {

                                            /* compatible types */
                                            case 'bool':
                                            case 'boolean':
                                            case 'variable':
                                                break;

                                            default:
                                                $compilationContext->logger->warning("Passing possible incorrect type for parameter: " . $classDefinition->getCompleteName() . '::' . $method->getName() . '(' . $parameter['name'] . '), passing: ' . $resolvedDynamicTypes[$n] . ', ' . "expecting: " . $parameter['data-type'], "possible-wrong-parameter", $expression);
                                                break;
                                        }
                                        break;

                                    case 'array':
                                        switch ($parameter['data-type']) {

                                            /* compatible types */
                                            case 'array':
                                            case 'variable':
                                                break;

                                            case 'callable':
                                                /**
                                                 * Array can be a callable type, example: [$this, "method"]
                                                 *
                                                 * @todo we need to check this array if can...
                                                 */
                                                break;

                                            default:
                                                $compilationContext->logger->warning("Passing possible incorrect type for parameter: " . $classDefinition->getCompleteName() . '::' . $method->getName() . '(' . $parameter['name'] . '), passing: ' . $resolvedDynamicTypes[$n] . ', ' . "expecting: " . $parameter['data-type'], "possible-wrong-parameter", $expression);
                                                break;
                                        }
                                        break;

                                    case 'callable':
                                        switch ($parameter['data-type']) {

                                            /* compatible types */
                                            case 'callable':
                                            case 'variable':
                                                break;

                                            default:
                                                $compilationContext->logger->warning("Passing possible incorrect type for parameter: " . $classDefinition->getCompleteName() . '::' . $method->getName() . '(' . $parameter['name'] . '), passing: ' . $resolvedDynamicTypes[$n] . ', ' . "expecting: " . $parameter['data-type'], "possible-wrong-parameter", $expression);
                                                break;
                                        }
                                        break;

                                    case 'string':
                                        switch ($parameter['data-type']) {

                                            /* compatible types */
                                            case 'string':
                                            case 'variable':
                                                break;

                                            default:
                                                $compilationContext->logger->warning("Passing possible incorrect type for parameter: " . $classDefinition->getCompleteName() . '::' . $method->getName() . '(' . $parameter['name'] . '), passing: ' . $resolvedDynamicTypes[$n] . ', ' . "expecting: " . $parameter['data-type'], "possible-wrong-parameter", $expression);
                                                break;
                                        }
                                        break;

                                    /**
                                     * Passing polymorphic variables to static typed parameters
                                     * could lead to potential unexpected type coercions
                                     */
                                    case 'variable':
                                        if ($resolvedDynamicTypes[$n] != $parameter['data-type']) {
                                            if ($resolvedDynamicTypes[$n] == 'undefined') {
                                                $compilationContext->logger->warning("Passing possible incorrect type to parameter: " . $classDefinition->getCompleteName() . '::' . $parameter[$n]['name'] . ', passing: ' . $resolvedDynamicTypes[$n] . ', ' . "expecting: " . $parameter[$n]['data-type'], "possible-wrong-parameter-undefined", $expression);
                                            }
                                            //echo '1: ', $resolvedTypes[$n], ' ', $resolvedDynamicTypes[$n], ' ', $parameter[0]['data-type'], ' ', PHP_EOL;
                                        }
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            $params = array();
        }

        // Add the last call status to the current symbol table
        $this->addCallStatusFlag($compilationContext);

        // Initialize non-temporary variables
        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        // Generate the code according to the call type
        if ($type == self::CALL_NORMAL || $type == self::CALL_DYNAMIC_STRING) {
            $realMethod = $this->getRealCalledMethod($compilationContext, $variableVariable, $methodName);

            $isInternal = false;
            if (is_object($realMethod[1])) {
                $isInternal = $realMethod[1]->isInternal();
                if ($isInternal && $realMethod[0] > 1) {
                    throw new CompilerException("Cannot resolve method: '" . $expression['name'] . "' in polymorphic variable", $expression);
                }
            }

            if (!$isInternal) {
                // Check if the method call can have an inline cache
                $methodCache = $compilationContext->cacheManager->getMethodCache();

                $cachePointer = $methodCache->get(
                    $compilationContext,
                    $methodName,
                    $variableVariable
                );

                if (!count($params)) {
                    if ($isExpecting) {
                        if ($symbolVariable->getName() == 'return_value') {
                            $codePrinter->output('ZEPHIR_RETURN_CALL_METHOD(' . $variableVariable->getName() . ', "' . $methodName . '", ' . $cachePointer . ');');
                        } else {
                            $codePrinter->output('ZEPHIR_CALL_METHOD(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', "' . $methodName . '", ' . $cachePointer . ');');
                        }
                    } else {
                        $codePrinter->output('ZEPHIR_CALL_METHOD(NULL, ' . $variableVariable->getName() . ', "' . $methodName . '", ' . $cachePointer . ');');
                    }
                } else {
                    if ($isExpecting) {
                        if ($symbolVariable->getName() == 'return_value') {
                            $codePrinter->output('ZEPHIR_RETURN_CALL_METHOD(' . $variableVariable->getName() . ', "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                        } else {
                            $codePrinter->output('ZEPHIR_CALL_METHOD(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                        }
                    } else {
                        $codePrinter->output('ZEPHIR_CALL_METHOD(NULL, ' . $variableVariable->getName() . ', "' . $methodName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                    }
                }
            } else {
                if (!count($params)) {
                    if ($isExpecting) {
                        if ($symbolVariable->getName() == 'return_value') {
                            $codePrinter->output('ZEPHIR_RETURN_CALL_INTERNAL_METHOD_P0(' . $variableVariable->getName() . ', ' . $method->getInternalName() . ');');
                        } else {
                            $codePrinter->output('ZEPHIR_CALL_INTERNAL_METHOD_P0(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $method->getInternalName() . ');');
                        }
                    } else {
                        $codePrinter->output('ZEPHIR_CALL_INTERNAL_METHOD_NORETURN_P0(' . $variableVariable->getName() . ', ' . $method->getInternalName() . ');');
                    }
                } else {
                    if ($isExpecting) {
                        if ($symbolVariable->getName() == 'return_value') {
                            $codePrinter->output('ZEPHIR_RETURN_CALL_INTERNAL_METHOD_P' . count($params) . '(' . $variableVariable->getName() . ', ' . $method->getInternalName() . ', ' . join(', ', $params) . ');');
                        } else {
                            $codePrinter->output('ZEPHIR_CALL_INTERNAL_METHOD_P' . count($params) . '(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $method->getInternalName() . ', ' . join(', ', $params) . ');');
                        }
                    } else {
                        $codePrinter->output('ZEPHIR_CALL_INTERNAL_METHOD_NORETURN_P' . count($params) . '(' . $variableVariable->getName() . ', ' . $method->getInternalName() . ', ' . join(', ', $params) . ');');
                    }
                }
            }
        } else {
            if ($type == self::CALL_DYNAMIC) {
                switch ($variableMethod->getType()) {
                    case 'string':
                    case 'variable':
                        break;
                    default:
                        throw new Exception('Cannot use variable type: ' . $variableMethod->getType() . ' as method caller');
                }

                $cachePointer = 'NULL, 0';

                if (!count($params)) {
                    if ($isExpecting) {
                        if ($symbolVariable->getName() == 'return_value') {
                            $codePrinter->output('ZEPHIR_RETURN_CALL_METHOD_ZVAL(' . $variableVariable->getName() . ', ' . $variableMethod->getName() . ', ' . $cachePointer . ');');
                        } else {
                            $codePrinter->output('ZEPHIR_CALL_METHOD_ZVAL(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $variableMethod->getName() . ',  ' . $cachePointer . ');');
                        }
                    } else {
                        $codePrinter->output('ZEPHIR_CALL_METHOD_ZVAL(NULL, ' . $variableVariable->getName() . ', ' . $variableMethod->getName() . ', ' . $cachePointer . ');');
                    }
                } else {
                    if ($isExpecting) {
                        if ($symbolVariable->getName() == 'return_value') {
                            $codePrinter->output('ZEPHIR_RETURN_CALL_METHOD_ZVAL(' . $variableVariable->getName() . ', ' . $variableMethod->getName() . ', ' . $cachePointer . ', ' . join(', ', $params) . ');');
                        } else {
                            $codePrinter->output('ZEPHIR_CALL_METHOD_ZVAL(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $variableMethod->getName() . ', ' . $cachePointer . ', ' . join(', ', $params) . ');');
                        }
                    } else {
                        $codePrinter->output('ZEPHIR_CALL_METHOD_ZVAL(NULL, ' . $variableVariable->getName() . ', ' . $variableMethod->getName() . ', ' . $cachePointer . ', ' . join(', ', $params) . ');');
                    }
                }
            }
        }

        // Temporary variables must be copied if they have more than one reference
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }

        // We can mark temporary variables generated as idle
        foreach ($this->getTemporalVariables() as $tempVariable) {
            $tempVariable->setIdle(true);
        }

        // Release parameters marked as references
        if (isset($expression['parameters'])) {
            if (count($references)) {
                foreach ($params as $position => $param) {
                    if (isset($references[$position])) {
                        $compilationContext->codePrinter->output('Z_UNSET_ISREF_P(' . $param . ');');
                    }
                }
            }
        }

        $this->addCallStatusOrJump($compilationContext);

        if ($isExpecting) {
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        return new CompiledExpression('null', null, $expression);
    }
}
