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

namespace Zephir;

use ReflectionException;
use ReflectionMethod;
use Zephir\Class\Method\Method;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception\CompilerException;
use Zephir\Traits\VariablesTrait;
use Zephir\Types\AbstractType;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

use function class_exists;
use function count;
use function implode;
use function is_array;
use function is_object;
use function sprintf;
use function strtolower;
use function ucfirst;

/**
 * Call methods in a non-static context
 */
class MethodCall extends Call
{
    use VariablesTrait;

    /**
     * Function is called using a dynamic variable as method name.
     */
    public const CALL_DYNAMIC = 2;
    /**
     * Function is called using a dynamic string as method name.
     */
    public const CALL_DYNAMIC_STRING = 3;
    /**
     * Function is called using a normal method name.
     */
    public const CALL_NORMAL = 1;

    /**
     * Compiles a method call.
     *
     * @param Expression         $expr
     * @param CompilationContext $compilationContext
     *
     * @return mixed|CompiledExpression
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(Expression $expr, CompilationContext $compilationContext): mixed
    {
        $expression = $expr->getExpression();

        $exprVariable = new Expression($expression['variable']);
        $exprVariable->setReadOnly(true);
        $exprCompiledVariable = $exprVariable->compile($compilationContext);

        $builtInType = false;
        switch ($exprCompiledVariable->getType()) {
            case 'variable':
                $variableVariable = $compilationContext->symbolTable->getVariableForRead(
                    $exprCompiledVariable->getCode(),
                    $compilationContext,
                    $expression
                );
                switch ($variableVariable->getType()) {
                    case 'variable':
                        $caller = $variableVariable;
                        break;
                    default:
                        /* Check if there is a built-in type optimizer available */
                        $builtInTypeClass = 'Zephir\Types\\' . ucfirst($variableVariable->getType()) . 'Type';
                        if (class_exists($builtInTypeClass)) {
                            /**
                             * @var AbstractType $builtInType
                             */
                            $builtInType = new $builtInTypeClass();
                            $caller      = $exprCompiledVariable;
                        } else {
                            throw new CompilerException(
                                'Methods cannot be called on variable type: ' . $variableVariable->getType(),
                                $expression
                            );
                        }
                }
                break;
            default:
                /* Check if there is a built-in type optimizer available */
                $builtInTypeClass = 'Zephir\Types\\' . ucfirst($exprCompiledVariable->getType()) . 'Type';
                if (class_exists($builtInTypeClass)) {
                    $builtInType = new $builtInTypeClass();
                    $caller      = $exprCompiledVariable;
                } else {
                    throw new CompilerException(
                        'Cannot use expression: ' . $exprCompiledVariable->getType() . ' as method caller',
                        $expression['variable']
                    );
                }
        }

        $codePrinter = $compilationContext->codePrinter;

        $type = $expression['call-type'];

        /**
         * In normal method calls and dynamic string method calls we just use the name given by the user
         */
        if (self::CALL_NORMAL == $type || self::CALL_DYNAMIC_STRING == $type) {
            $methodName = strtolower($expression['name']);
        } else {
            $variableMethod = $compilationContext->symbolTable->getVariableForRead(
                $expression['name'],
                $compilationContext,
                $expression
            );

            if (is_object($builtInType)) {
                throw new CompilerException(
                    'Dynamic method invocation for type: ' . $variableMethod->getType() . ' is not supported',
                    $expression
                );
            }

            if ($variableMethod->isNotVariableAndString()) {
                throw CompilerException::cannotUseVariableTypeAs(
                    $variableMethod,
                    'as a dynamic method name',
                    $expression
                );
            }
        }

        $symbolVariable = null;

        /**
         * Create temporary variable if needed.
         */
        $mustInit    = false;
        $isExpecting = $expr->isExpectingReturn();
        if ($isExpecting) {
            $symbolVariable = $expr->getExpectingVariable();
            if (is_object($symbolVariable)) {
                $readDetector = new ReadDetector();
                if ($caller == $symbolVariable || $readDetector->detect($symbolVariable->getName(), $expression)) {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify(
                        'variable',
                        $compilationContext,
                    );
                } else {
                    $mustInit = true;
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify(
                    'variable',
                    $compilationContext,
                );
            }
        }

        /*
         * Method calls only return zvals, so we need to validate the target variable is also a zval
         */
        if (!$builtInType) {
            if ($isExpecting) {
                $this->checkNotVariable($symbolVariable, $expression);

                /*
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

        /*
         * Try to check if the method exist in the callee, only when method call is self::CALL_NORMAL
         */
        if (self::CALL_NORMAL == $type) {
            if ('this' == $variableVariable->getRealName()) {
                $classDefinition = $compilationContext->classDefinition;
                if (!$classDefinition->hasMethod($methodName)) {
                    if ($check) {
                        $found = false;

                        if ($classDefinition->isAbstract()) {
                            $interfaces = $classDefinition->getImplementedInterfaces();
                        } else {
                            $interfaces = null;
                        }

                        if (is_array($interfaces)) {
                            $compiler = $compilationContext->compiler;
                            foreach ($interfaces as $interface) {
                                $classInterfaceDefinition = $compiler->getClassDefinition($interface);

                                if (!$classInterfaceDefinition) {
                                    $classInterfaceDefinition = $compiler->getInternalClassDefinition($interface);
                                    if (!$classInterfaceDefinition) {
                                        throw new CompilerException(
                                            "Couldn't locate internal or external interface: " . $interface,
                                            $expression
                                        );
                                    }
                                }

                                if ($classInterfaceDefinition->hasMethod($methodName)) {
                                    $found       = true;
                                    $classMethod = $classInterfaceDefinition->getMethod($methodName);
                                    break;
                                }
                            }
                        }

                        if (!$found) {
                            $possibleMethod = $classDefinition->getPossibleMethodName($expression['name']);
                            if ($possibleMethod && $expression['name'] != $possibleMethod) {
                                throw new CompilerException(
                                    sprintf(
                                        "Class '%s' does not implement method: '%s'. Did you mean '%s'?",
                                        $classDefinition->getCompleteName(),
                                        $expression['name'],
                                        $possibleMethod
                                    ),
                                    $expression
                                );
                            }

                            throw new CompilerException(
                                sprintf(
                                    "Class '%s' does not implement method: '%s'",
                                    $classDefinition->getCompleteName(),
                                    $expression['name']
                                ),
                                $expression
                            );
                        }
                    }
                } else {
                    if ($check) {
                        $classMethod = $classDefinition->getMethod($methodName);
                    }
                }

                if ($check) {
                    /*
                     * Private methods must be called in their declaration scope
                     */
                    if ($classMethod->isPrivate()) {
                        if ($classMethod->getClassDefinition() !== $classDefinition) {
                            throw new CompilerException(
                                "Cannot call private method '" . $expression['name'] . "' out of its scope",
                                $expression
                            );
                        }
                    }

                    /*
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
                            throw new CompilerException(
                                sprintf(
                                    "Method '%s::%s' called with a wrong number of parameters, " .
                                    'the method has: %d, passed: %d',
                                    $classDefinition->getCompleteName(),
                                    $expression['name'],
                                    $expectedNumberParameters,
                                    $callNumberParameters
                                ),
                                $expression
                            );
                        }
                    }

                    if ($callNumberParameters < $expectedNumberParameters) {
                        throw new CompilerException(
                            sprintf(
                                "Method '%s::%s' called with a wrong number of parameters, " .
                                'the method has: %d, passed: %d',
                                $classDefinition->getCompleteName(),
                                $expression['name'],
                                $expectedNumberParameters,
                                $callNumberParameters
                            ),
                            $expression
                        );
                    }

                    $method = $classMethod;
                }
            } else {
                /*
                 * Variables whose dynamic type is 'object' can be used
                 * to determine method existence in compile time
                 */
                if ($check && $variableVariable->hasAnyDynamicType('object')) {
                    $classTypes = $variableVariable->getClassTypes();

                    if (count($classTypes)) {
                        $numberImplemented = 0;
                        $compiler          = $compilationContext->compiler;
                        foreach ($classTypes as $classType) {
                            if (
                                $compiler->isClass($classType) ||
                                $compiler->isInterface($classType) ||
                                $compiler->isBundledClass($classType) ||
                                $compiler->isBundledInterface($classType)
                            ) {
                                if ($compiler->isClass($classType) || $compiler->isInterface($classType)) {
                                    $classDefinition = $compiler->getClassDefinition($classType);
                                } else {
                                    $classDefinition = $compiler->getInternalClassDefinition($classType);
                                }

                                if (!$classDefinition) {
                                    throw new CompilerException(
                                        'Cannot locate class definition for class ' . $classType,
                                        $expression
                                    );
                                }

                                if (!$classDefinition->hasMethod($methodName)) {
                                    if (!$classDefinition->isInterface()) {
                                        if (1 == count($classTypes)) {
                                            throw new CompilerException(
                                                sprintf(
                                                    "Class '%s' does not implement method: '%s'",
                                                    $classType,
                                                    $expression['name']
                                                ),
                                                $expression
                                            );
                                        }
                                    }
                                    continue;
                                }

                                $method = $classDefinition->getMethod($methodName);

                                /*
                                 * Private methods must be called in their declaration scope
                                 */
                                if ($method->isPrivate()) {
                                    if ($method->getClassDefinition() != $classDefinition) {
                                        throw new CompilerException(
                                            sprintf(
                                                "Cannot call private method '%s' out of its scope",
                                                $expression['name']
                                            ),
                                            $expression
                                        );
                                    }
                                }

                                /*
                                 * Check visibility for protected methods
                                 */
                                if (
                                    $method->isProtected() &&
                                    $method->getClassDefinition() != $classDefinition &&
                                    $method->getClassDefinition() != $classDefinition->getExtendsClass()
                                ) {
                                    throw new CompilerException(
                                        sprintf(
                                            "Cannot call protected method '%s' out of its scope",
                                            $expression['name']
                                        ),
                                        $expression
                                    );
                                }

                                /*
                                 * Try to produce an exception if a method is called with a wrong number of parameters
                                 * We only check extension parameters if methods are extension methods
                                 * Internal methods may have invalid Reflection information
                                 */
                                if ($method instanceof Method && !$method->isBundled()) {
                                    if (isset($expression['parameters'])) {
                                        $callNumberParameters = count($expression['parameters']);
                                    } else {
                                        $callNumberParameters = 0;
                                    }

                                    $classMethod              = $classDefinition->getMethod($methodName);
                                    $expectedNumberParameters = $classMethod->getNumberOfRequiredParameters();

                                    if (!$expectedNumberParameters && $callNumberParameters > 0) {
                                        $numberParameters = $classMethod->getNumberOfParameters();
                                        if ($callNumberParameters > $numberParameters) {
                                            $className = $classDefinition->getCompleteName();
                                            throw new CompilerException(
                                                sprintf(
                                                    "Method '%s::%s' called with a wrong number of parameters, " .
                                                    'the method has: %d, passed: %s',
                                                    $className,
                                                    $expression['name'],
                                                    $expectedNumberParameters,
                                                    $callNumberParameters
                                                ),
                                                $expression
                                            );
                                        }
                                    }

                                    if ($callNumberParameters < $expectedNumberParameters) {
                                        throw new CompilerException(
                                            sprintf(
                                                "Method '%s::%s' called with a wrong number of parameters, " .
                                                'the method has: %d, passed: %d',
                                                $classDefinition->getCompleteName(),
                                                $expression['name'],
                                                $expectedNumberParameters,
                                                $callNumberParameters
                                            ),
                                            $expression
                                        );
                                    }
                                }

                                /*
                                 * The method is checked in the first class that implements the method
                                 * We could probably have collisions here
                                 */
                                ++$numberImplemented;
                                break;
                            } else {
                                ++$numberImplemented;
                                $compilationContext->logger->warning(
                                    'Class "' . $classType . '" does not exist at compile time',
                                    ['nonexistent-class', $expression]
                                );
                            }
                        }

                        if (0 == $numberImplemented) {
                            if (!$classDefinition->isInterface()) {
                                if (count($classTypes) > 1) {
                                    throw new CompilerException(
                                        sprintf(
                                            "None of classes: '%s' implement method: '%s'",
                                            implode(' or ', $classTypes),
                                            $expression['name']
                                        ),
                                        $expression
                                    );
                                } else {
                                    throw new CompilerException(
                                        sprintf(
                                            "Class '%s' does not implement method: '%s'",
                                            $classTypes[0],
                                            $expression['name']
                                        ),
                                        $expression
                                    );
                                }
                            } else {
                                // TODO:, raise an exception here?
                            }
                        }
                    }
                }
            }
        }

        if (isset($method)) {
            $this->reflection = $method;
        }

        /*
         * Transfer the return type-hint to the returned variable
         */
        if ($isExpecting) {
            if (isset($method)) {
                if ($method instanceof Method) {
                    if ($method->isVoid()) {
                        throw new CompilerException(
                            sprintf(
                                "Method '%s::%s' is marked as '%s' and it does not return anything",
                                $classDefinition->getCompleteName(),
                                $expression['name'],
                                Types::T_VOID
                            ),
                            $expression
                        );
                    }

                    $returnClassTypes = $method->getReturnClassTypes();

                    if (null !== $returnClassTypes) {
                        $symbolVariable->setDynamicTypes('object');
                        foreach ($returnClassTypes as &$returnClassType) {
                            $returnClassType = $compilationContext->getFullName($returnClassType);
                        }
                        $symbolVariable->setClassTypes($returnClassTypes);
                    }

                    $returnTypes = $method->getReturnTypes();
                    if (null !== $returnTypes) {
                        foreach ($returnTypes as $dataType => $returnType) {
                            $symbolVariable->setDynamicTypes($dataType);
                        }
                    }
                }
            }
        }

        /*
         * Some parameters in internal methods receive parameters as references
         */
        if (isset($expression['parameters'])) {
            $references = [];
            if (self::CALL_NORMAL == $type || self::CALL_DYNAMIC_STRING == $type) {
                if (isset($method)) {
                    if ($method instanceof ReflectionMethod) {
                        $position = 0;
                        foreach ($method->getParameters() as $parameter) {
                            if ($parameter->isPassedByReference()) {
                                $references[$position] = true;
                            }
                            ++$position;
                        }
                    }
                }
            }
        }

        /*
         * Include fcall header
         */
        $compilationContext->headersManager->add('kernel/fcall');

        /*
         * Call methods must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        /*
         * Mark references
         */
        $params = [];
        if (isset($expression['parameters'])) {
            $params = $this->getResolvedParams(
                $expression['parameters'],
                $compilationContext,
                $expression,
                $method ?? null
            );

            if (count($references)) {
                foreach ($params as $position => $param) {
                    if (isset($references[$position])) {
                        $compilationContext->codePrinter->output('Z_SET_ISREF_P(' . $param . ');');
                    }
                }
            }

            // We check here if a correct parameter type is passed to the called method
            if (self::CALL_NORMAL == $type) {
                if (isset($method) && $method instanceof Method && isset($expression['parameters'])) {
                    $resolvedTypes        = $this->getResolvedTypes();
                    $resolvedDynamicTypes = $this->getResolvedDynamicTypes();

                    foreach ($method->getParameters() as $n => $parameter) {
                        if (isset($parameter['data-type'])) {
                            if (!isset($resolvedTypes[$n])) {
                                continue;
                            }

                            /*
                             * If the passed parameter is different to the expected type we show a warning
                             */
                            if ($resolvedTypes[$n] != $parameter['data-type']) {
                                $template = sprintf(
                                    'Passing possible incorrect type for parameter: %s::%s(%s), ' .
                                    'passing: %s, expecting: %s',
                                    $classDefinition->getCompleteName(),
                                    $method->getName(),
                                    $parameter['name'],
                                    $resolvedTypes[$n],
                                    $parameter['data-type']
                                );

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
                                                $compilationContext->logger->warning(
                                                    $template,
                                                    ['possible-wrong-parameter', $expression]
                                                );
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
                                                /*
                                                 * Array can be a callable type, example: [$this, "method"]
                                                 *
                                                 * TODO: we need to check this array if can...
                                                 */
                                                break;

                                            default:
                                                $compilationContext->logger->warning(
                                                    $template,
                                                    ['possible-wrong-parameter', $expression]
                                                );
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
                                                $compilationContext->logger->warning(
                                                    $template,
                                                    ['possible-wrong-parameter', $expression]
                                                );
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
                                                $compilationContext->logger->warning(
                                                    $template,
                                                    ['possible-wrong-parameter', $expression]
                                                );
                                                break;
                                        }
                                        break;

                                    /**
                                     * Passing polymorphic variables to static typed parameters
                                     * could lead to potential unexpected type coercions
                                     */
                                    case 'variable':
                                        if ($resolvedDynamicTypes[$n] != $parameter['data-type']) {
                                            if ('undefined' == $resolvedDynamicTypes[$n]) {
                                                $compilationContext->logger->warning(
                                                    sprintf(
                                                        'Passing possible incorrect type for parameter: %s::%s(%s), ' .
                                                        'passing: undefined, expecting: %s',
                                                        $classDefinition->getCompleteName(),
                                                        $method->getName(),
                                                        $parameter['name'],
                                                        $parameter['data-type']
                                                    ),
                                                    ['possible-wrong-parameter-undefined', $expression]
                                                );
                                            }
                                        }
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Add the last call status to the current symbol table
        $this->addCallStatusFlag($compilationContext);

        // Initialize non-temporary variables
        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        // Generate the code according to the call type
        if (self::CALL_NORMAL == $type || self::CALL_DYNAMIC_STRING == $type) {
            $realMethod = $this->getRealCalledMethod($compilationContext, $variableVariable, $methodName);

            $isInternal = false;
            if (is_object($realMethod[1])) {
                $realMethod[1] = $realMethod[1]->getOptimizedMethod();
                $method        = $realMethod[1];
                $isInternal    = $realMethod[1]->isInternal();
                if ($isInternal && $realMethod[0] > 1) {
                    throw new CompilerException(
                        "Cannot resolve method: '" . $expression['name'] . "' in polymorphic variable",
                        $expression
                    );
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

                $compilationContext->backend->callMethod(
                    $isExpecting ? $symbolVariable : null,
                    $variableVariable,
                    $methodName,
                    $cachePointer,
                    count($params) ? $params : null,
                    $compilationContext
                );
            } else {
                // TODO: also move to backend
                if ($isExpecting) {
                    $symbolCode = $compilationContext->backend->getVariableCode($symbolVariable);
                }
                $variableCode = $compilationContext->backend->getVariableCode($variableVariable);
                $paramCount   = count($params);
                $paramsStr    = $paramCount ? ', ' . implode(', ', $params) : '';

                if ($isExpecting) {
                    if ('return_value' == $symbolVariable->getName()) {
                        $macro = $compilationContext->backend->getFcallManager()->getMacro(false, 1, $paramCount);
                        $codePrinter->output(
                            $macro . '(' . $variableCode . ', ' . $method->getInternalName() . $paramsStr . ');'
                        );
                    } else {
                        $macro = $compilationContext->backend->getFcallManager()->getMacro(false, 2, $paramCount);
                        $codePrinter->output(
                            $macro . '(' . $symbolCode . ', ' . $variableCode . ', ' . $method->getInternalName(
                            ) . $paramsStr . ');'
                        );
                    }
                } else {
                    $macro = $compilationContext->backend->getFcallManager()->getMacro(false, 0, $paramCount);
                    $codePrinter->output(
                        $macro . '(' . $variableCode . ', ' . $method->getInternalName() . $paramsStr . ');'
                    );
                }
            }
        } else {
            if (self::CALL_DYNAMIC == $type) {
                switch ($variableMethod->getType()) {
                    case 'string':
                    case 'variable':
                        break;
                    default:
                        throw CompilerException::cannotUseVariableTypeAs(
                            $variableMethod,
                            'as a method caller',
                            $expression
                        );
                }

                $cachePointer = 'NULL, 0';

                $compilationContext->backend->callMethod(
                    $isExpecting ? $symbolVariable : null,
                    $variableVariable,
                    $variableMethod,
                    $cachePointer,
                    count($params) ? $params : null,
                    $compilationContext
                );
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

    /**
     * Examine internal class information and returns the method called.
     *
     * @param CompilationContext $compilationContext
     * @param Variable           $caller
     * @param string             $methodName
     *
     * @return array
     *
     * @throws ReflectionException
     */
    private function getRealCalledMethod(
        CompilationContext $compilationContext,
        Variable $caller,
        string $methodName
    ): array {
        $compiler = $compilationContext->compiler;

        $numberPoly = 0;
        $method     = null;

        if ('this' == $caller->getRealName()) {
            $classDefinition = $compilationContext->classDefinition;
            if ($classDefinition->hasMethod($methodName)) {
                ++$numberPoly;
                $method = $classDefinition->getMethod($methodName);
            }
        } else {
            $classTypes = $caller->getClassTypes();
            foreach ($classTypes as $classType) {
                if ($compiler->isInterface($classType)) {
                    continue;
                }

                if (
                    $compiler->isClass($classType) ||
                    $compiler->isBundledClass($classType) ||
                    $compiler->isBundledInterface($classType)
                ) {
                    if ($compiler->isClass($classType)) {
                        $classDefinition = $compiler->getClassDefinition($classType);
                    } else {
                        $classDefinition = $compiler->getInternalClassDefinition($classType);
                    }

                    if (!$classDefinition) {
                        continue;
                    }

                    if ($classDefinition->hasMethod($methodName) && !$classDefinition->isInterface()) {
                        ++$numberPoly;
                        $method = $classDefinition->getMethod($methodName);
                    }
                }
            }
        }

        return [$numberPoly, $method];
    }
}
