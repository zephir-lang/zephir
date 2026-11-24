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
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Class\Visibility;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception\CompilerException;
use Zephir\Traits\VariablesTrait;
use Zephir\Types\AbstractType;
use Zephir\Types\ArrayType;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

use function array_unshift;
use function class_exists;
use function count;
use function implode;
use function in_array;
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
     * Built-in array-method names that are dispatched via ArrayType when the
     * caller is a generic `variable` (i.e. `var`) that may hold an array.
     * A receiver that holds an object calls its method, as PHP does: a class
     * can declare `join()` or `toJson()` and call it on `this` (#2770).
     *
     * Methods that overlap with common object APIs (count, push, pop, shift,
     * sort, current, next, end, key, reset, each) are deliberately excluded
     * to avoid changing semantics for objects implementing Iterator/Countable.
     *
     * @see https://github.com/zephir-lang/zephir/issues/733
     * @see https://github.com/zephir-lang/zephir/issues/2770
     */
    private const ARRAY_BUILTIN_METHODS_ON_VAR = [
        'join',
        'reversed',
        'rev',
        'tojson',
        'haskey',
        'mergerecursive',
        'replacerecursive',
        'sortbykey',
        'reversesort',
        'reversesortbykey',
    ];

    /**
     * Compiles a method call.
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

                        /**
                         * Built-in array methods called on a `var` variable: an
                         * array calls PHP's matching function (#733), anything
                         * else calls the method, as PHP does (#2770).
                         */
                        $receiverType = $this->getArrayBuiltinReceiverType($expression, $variableVariable);
                        if ('array' === $receiverType) {
                            $builtInType = new ArrayType();
                            $caller      = $exprCompiledVariable;
                        } elseif ('either' === $receiverType) {
                            return $this->compileArrayOrObjectCall($expr, $exprCompiledVariable, $compilationContext);
                        }
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

            if ($variableMethod->isNotVariableAndMixedAndString()) {
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

        /**
         * Method calls only return zvals, so we need to validate the target variable is also a zval
         */
        if (!$builtInType) {
            if ($isExpecting) {
                $this->checkNotVariable($symbolVariable, $expression);

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
        if (self::CALL_NORMAL == $type) {
            if ('this' == $variableVariable->getRealName()) {
                $classDefinition = $compilationContext->classDefinition;

                /**
                 * If this is a closure class with an enclosing class, resolve
                 * method calls on `this` against the enclosing class definition.
                 * @see https://github.com/zephir-lang/zephir/issues/2497
                 */
                $enclosingClassDefinition = $classDefinition->getEnclosingClassDefinition();
                if ($enclosingClassDefinition !== null) {
                    $classDefinition = $enclosingClassDefinition;
                }

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
                    $this->checkVisibility($classMethod, $classDefinition, $expression, $compilationContext);

                    /*
                     * Try to produce an exception if method is called with a wrong number of parameters
                     */
                    if (isset($expression['parameters'])) {
                        $callNumberParameters = count($expression['parameters']);
                    } else {
                        $callNumberParameters = 0;
                    }

                    $expectedNumberParameters = $classMethod->getNumberOfRequiredParameters();
                    if (!$expectedNumberParameters && $callNumberParameters > 0 && !$classMethod->isVariadic()) {
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

                                $this->checkVisibility($method, $classDefinition, $expression, $compilationContext);

                                /**
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

                                    if (!$expectedNumberParameters && $callNumberParameters > 0 && !$classMethod->isVariadic()) {
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

                                /**
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

        /**
         * Transfer the return type-hint to the returned variable
         */
        if ($isExpecting && isset($method)) {
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
                    /*
                     * `self`, `static`, and `parent` resolve relative to the
                     * RECEIVER of the call, not the lexical class of the
                     * call site: `$classDefinition` already carries the
                     * receiver's class, resolved from the variable's class
                     * types (#2505). Every other name resolves in the file
                     * the method is written in (#2824).
                     */
                    $receiverDefinition = $classDefinition ?? $compilationContext->classDefinition;
                    $symbolVariable->setClassTypes(
                        $method->resolveReturnClassTypes($receiverDefinition, $compilationContext)
                    );
                }

                $returnTypes = $method->getReturnTypes();
                if (null !== $returnTypes) {
                    foreach ($returnTypes as $dataType => $returnType) {
                        $symbolVariable->setDynamicTypes($dataType);
                    }
                }
            }
        }

        /**
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
        $params = [];
        $named  = null;
        if (isset($expression['parameters'])) {
            $params = $this->getResolvedParams(
                $expression['parameters'],
                $compilationContext,
                $expression,
            );

            [$params, $named] = $this->splitNamedArguments($params, $compilationContext);

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

                            /**
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

                                /*
                                 * `mixed` (PHP 8.0+) is defined as "any type", so anything that's
                                 * compatible with `variable` is also compatible with `mixed`. The
                                 * arms below list it next to `variable` instead of relying on a
                                 * blanket fall-through, to keep the existing per-source-type shape
                                 * obvious. See https://github.com/zephir-lang/zephir/issues/2512.
                                 */
                                switch ($resolvedTypes[$n]) {
                                    case 'bool':
                                    case 'boolean':
                                        switch ($parameter['data-type']) {
                                            /* compatible types */
                                            case 'bool':
                                            case 'boolean':
                                            case 'variable':
                                            case 'mixed':
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
                                            case 'mixed':
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
                                            case 'mixed':
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
                                            case 'mixed':
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
            [$numberPoly, $resolvedMethod, $receiverDefinition] = $this->getRealCalledMethod(
                $compilationContext,
                $variableVariable,
                $methodName
            );

            $isInternal = false;
            $twin       = null;
            if (is_object($resolvedMethod)) {
                $method = $resolvedMethod;

                /**
                 * A method declared `internal` in Zephir source is deliberately
                 * kept out of the class' method_entry, so there is no PHP-level
                 * method to dispatch to and a direct call is the only option.
                 *
                 * Named arguments are bound by the engine, which a direct C
                 * call skips (#2824).
                 */
                if (null !== $named && $method->isInternal()) {
                    throw new CompilerException(
                        "Named arguments cannot be passed to the internal method '" . $expression['name'] . "'",
                        $expression
                    );
                }

                $isInternal = $method->isInternal();
                if ($isInternal && $numberPoly > 1) {
                    throw new CompilerException(
                        "Cannot resolve method: '" . $expression['name'] . "' in polymorphic variable",
                        $expression
                    );
                }

                /**
                 * The C copy that `internal-call-transformation` makes of a
                 * method is a different matter: it runs only where it gives
                 * what PHP's call would give (#2791).
                 */
                if (!$isInternal && null === $named && self::CALL_NORMAL == $type && 1 === $numberPoly) {
                    $twin = $method->getOptimizedMethod();
                    if ($twin === $method || $method->isStatic()) {
                        $twin = null;
                    }
                }
            }

            if (!$isInternal) {
                /**
                 * The site's own inline cache: the kernel keys it by the
                 * receiver's class, so it serves any object the site sees.
                 *
                 * @see https://github.com/zephir-lang/zephir/issues/2790
                 */
                $cachePointer = $compilationContext->cacheManager->getMethodCache()->get($compilationContext);

                /**
                 * The call carries the name as it was written, not the lowered
                 * name the lookups above used: PHP hands `__call()` the
                 * spelling from the call site.
                 *
                 * @see https://github.com/zephir-lang/zephir/issues/2715
                 */
                $call = fn () => $compilationContext->backend->callMethod(
                    $isExpecting ? $symbolVariable : null,
                    $variableVariable,
                    $expression['name'],
                    $cachePointer,
                    count($params) ? $params : null,
                    $compilationContext,
                    $named
                );

                $conditions = null;
                if (null !== $twin) {
                    $conditions = $this->twinConditions(
                        $method,
                        $variableVariable,
                        $receiverDefinition,
                        $cachePointer,
                        $params,
                        $compilationContext
                    );
                }

                if (null === $conditions) {
                    $call();
                } else {
                    $this->emitGuardedDirectCall(
                        $conditions,
                        fn () => $this->emitInternalMethodCall(
                            $twin,
                            false,
                            $compilationContext->backend->getVariableCode($variableVariable)
                                . ', ' . $method->getClassDefinition()->getClassEntry($compilationContext),
                            $params,
                            $symbolVariable,
                            $isExpecting,
                            $compilationContext
                        ),
                        $call,
                        $compilationContext
                    );
                }
            } else {
                $this->emitInternalMethodCall(
                    $method,
                    false,
                    $compilationContext->backend->getVariableCode($variableVariable)
                        . ', ' . $method->getClassDefinition()->getClassEntry($compilationContext),
                    $params,
                    $symbolVariable,
                    $isExpecting,
                    $compilationContext
                );
            }
        } else {
            if (self::CALL_DYNAMIC == $type) {
                switch ($variableMethod->getType()) {
                    case 'string':
                    case 'variable':
                    case 'mixed':
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
                    $compilationContext,
                    $named
                );
            }
        }

        $this->releaseNamedArguments($compilationContext);

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
     * What a `var` receiver of an array built-in method name holds: 'array'
     * calls the array built-in, 'object' calls the method, 'either' chooses at
     * run time. Any other name, or a name held in a variable, is a method.
     *
     * Dynamic types are the union of every assignment seen so far, and class
     * types come from a hint, a cast, `new` or a typed return, so they mean an
     * object only while no array was assigned too.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2770
     */
    private function getArrayBuiltinReceiverType(array $expression, Variable $receiver): string
    {
        if (isset($expression['receiver-type'])) {
            return $expression['receiver-type'];
        }

        if (self::CALL_DYNAMIC == $expression['call-type']) {
            return 'object';
        }

        if (!in_array(strtolower($expression['name']), self::ARRAY_BUILTIN_METHODS_ON_VAR, true)) {
            return 'object';
        }

        $unknown    = $receiver->hasAnyDynamicType(['unknown', 'undefined']);
        $classTyped = count($receiver->getClassTypes()) > 0;

        $mayBeArray = $receiver->hasAnyDynamicType('array') || ($unknown && !$classTyped);
        if (!$mayBeArray) {
            return 'object';
        }

        $mayBeObject = $receiver->hasAnyDynamicType('object') || $unknown || $classTyped;

        return $mayBeObject ? 'either' : 'array';
    }

    /**
     * Compiles `receiver->name(...)` on a receiver that may hold an array or
     * an object as `typeof receiver == "array" ? <array built-in> : <method>`.
     * Both calls read the receiver already compiled, so it is evaluated once.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2770
     */
    private function compileArrayOrObjectCall(
        Expression $expr,
        CompiledExpression $receiver,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $expression = $expr->getExpression();
        $position   = [
            'file' => $expression['file'],
            'line' => $expression['line'],
            'char' => $expression['char'],
        ];
        $variable   = ['type' => 'variable', 'value' => $receiver->getCode()] + $position;

        $ternary = new Expression([
            'type'  => 'ternary',
            'left'  => [
                'type'  => 'equals',
                'left'  => ['type' => 'typeof', 'left' => $variable] + $position,
                'right' => ['type' => 'string', 'value' => 'array'] + $position,
            ] + $position,
            'right' => ['variable' => $variable, 'receiver-type' => 'array'] + $expression,
            /* The class types may come from an assignment before the array one */
            'extra' => ['variable' => $variable, 'receiver-type' => 'object', 'check' => false] + $expression,
        ] + $position);
        $ternary->setExpectReturn(true, $expr->isExpectingReturn() ? $expr->getExpectingVariable() : null);

        return $ternary->compile($compilationContext);
    }

    /**
     * Examine internal class information and returns the method called.
     *
     * @return array{0: int, 1: Method|null, 2: Definition|null} Number of
     *         candidate classes, the resolved method, and the class the method
     *         was resolved *through* (which is not necessarily the class that
     *         declares it, since resolution walks the `extends` chain).
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
        $receiver   = null;

        if ('this' == $caller->getRealName()) {
            $classDefinition = $compilationContext->classDefinition;
            if ($classDefinition->hasMethod($methodName)) {
                ++$numberPoly;
                $method   = $classDefinition->getMethod($methodName);
                $receiver = $classDefinition;
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
                        $method   = $classDefinition->getMethod($methodName);
                        $receiver = $classDefinition;
                    }
                }
            }
        }

        return [$numberPoly, $method, $receiver];
    }

    /**
     * The run-time conditions under which this call runs the C copy of
     * $method, or null when it cannot take the call.
     *
     * Where PHP itself does not consult the object (isStaticallyDispatched())
     * only the arguments are checked. Everywhere else the call site's inline
     * cache must hold this very method for the object's class and the calling
     * class: an override, in Zephir or in PHP, a `__call()` fallback, an
     * object of another class or no object at all makes PHP's call.
     *
     * @param list<string> $params
     *
     * @return list<string>|null
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    private function twinConditions(
        Method $method,
        Variable $caller,
        ?Definition $receiver,
        string $cachePointer,
        array $params,
        CompilationContext $compilationContext
    ): ?array {
        $checks = $this->twinArgumentChecks($method, $params, $compilationContext);
        if (null === $checks) {
            return null;
        }

        if ('this' === $caller->getRealName() && $this->isStaticallyDispatched($method, $receiver, $compilationContext)) {
            $checks[] = 'zephir_has_stack_for_direct_call()';

            return $checks;
        }

        $declaring = $method->getClassDefinition();
        array_unshift(
            $checks,
            sprintf(
                'zephir_cached_method_is(%s, %s, ZEND_MN(%s_%s_%s), %s)',
                $cachePointer,
                $compilationContext->backend->getVariableCode($caller),
                $declaring->getCNamespace(),
                $declaring->getName(),
                $method->getName(),
                $declaring->getClassEntry($compilationContext)
            )
        );

        return $checks;
    }

    /**
     * Whether PHP itself resolves a call on `this` without consulting the
     * object's class, so that the call can go straight to the method's own C
     * copy, which is what the `internal-call-transformation` optimization
     * does.
     *
     * `this->m()` is a virtual call: any subclass may override `m` — including
     * a userland PHP class extending a Zephir one, which no amount of
     * whole-program analysis of the extension can see. Only three premises make
     * the target static, and the engine enforces each of them itself
     * (ZEND_ACC_FINAL, ZEND_ACC_PRIVATE, ZEND_ACC_FINAL_CLASS):
     *
     *  - the method is final, so nothing can override it;
     *  - the method is private to the class being compiled, so a same-named
     *    method in a subclass is a separate method rather than an override;
     *  - its class is final, so the runtime class is exactly the class being
     *    compiled.
     *
     * Only `this` is known to hold an object of the class being compiled. A
     * class type declared on a variable is no proof of anything: `let v =
     * <Vector> x` and declared return types assert a class without verifying
     * it, and a nullable class parameter may hold null (#2791).
     *
     * @see https://github.com/zephir-lang/zephir/issues/2021
     */
    private function isStaticallyDispatched(
        Method $method,
        ?Definition $receiver,
        CompilationContext $compilationContext
    ): bool {
        /**
         * The engine settles the target only once it knows the calling class:
         * an inaccessible method goes to `__call()`, and a private method of
         * the calling class wins over a same-named method of a subclass
         * (ZEND_ACC_CHANGED). Either way it is not the method resolved here.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2734
         */
        if (
            !Visibility::canCallMethod($method, $compilationContext)
            || $this->isShadowedByScope($method, $compilationContext)
        ) {
            return false;
        }

        if ($method->isFinal()) {
            return true;
        }

        $scope = $compilationContext->classDefinition;

        if ($method->isPrivate() && $method->getClassDefinition() === $scope) {
            return true;
        }

        /**
         * The class synthesized for a closure is marked final (see
         * Expression\Closure), yet `this_ptr` inside its `__invoke` is the
         * *enclosing* object, whose class is not final at all. Finality of a
         * closure class says nothing about the receiver, so refuse it.
         */
        return $receiver === $scope
            && $receiver->isFinal()
            && null === $scope->getEnclosingClassDefinition();
    }

    /**
     * Rejects a call PHP would refuse from the class being compiled.
     *
     * A receiver whose class has `__call()` takes every call it cannot make
     * itself, so that decision is left to the run time.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2734
     */
    private function checkVisibility(
        Method $method,
        Definition $receiver,
        array $expression,
        CompilationContext $compilationContext
    ): void {
        if (Visibility::canCallMethod($method, $compilationContext) || $receiver->hasMethod('__call')) {
            return;
        }

        throw new CompilerException(
            sprintf(
                "Cannot call %s method '%s' out of its scope",
                $method->isPrivate() ? 'private' : 'protected',
                $expression['name']
            ),
            $expression
        );
    }

    /**
     * Whether the calling class has a private method of the same name that is
     * not $method: PHP calls that one on any object of a subclass.
     */
    private function isShadowedByScope(Method $method, CompilationContext $compilationContext): bool
    {
        $own = Visibility::scopeOf($compilationContext)?->getMethod($method->getName(), false);

        return null !== $own && $own !== $method && $own->isPrivate();
    }
}
