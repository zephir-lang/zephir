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
     * Built-in array-method names that we dispatch via ArrayType even when the
     * caller is statically a generic `variable` (i.e. `var`). These are names
     * that effectively don't exist as real object methods in PHP land, so
     * redispatching them as PHP function calls does the right thing without
     * silently breaking any object dispatch a user might have relied on.
     *
     * Methods that overlap with common object APIs (count, push, pop, shift,
     * sort, current, next, end, key, reset, each) are deliberately excluded
     * to avoid changing semantics for objects implementing Iterator/Countable.
     *
     * @see https://github.com/zephir-lang/zephir/issues/733
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
                         * Built-in array methods called on a `var` variable.
                         *
                         * When the caller is statically `variable` (e.g. a `var` parameter
                         * or an array stored in a `var` slot) we previously emitted
                         * ZEPHIR_CALL_METHOD(b, "join", ...) which fails at runtime with
                         * "Trying to call method on non-object" the moment `b` actually
                         * holds an array. Several Zephir-only built-in array method names
                         * cannot meaningfully exist as real object methods, so for those
                         * we dispatch via ArrayType (which lowers to PHP's matching
                         * function call). If the runtime value isn't an array, PHP's
                         * function will surface a proper TypeError instead of the
                         * misleading "non-object" message.
                         *
                         * @see https://github.com/zephir-lang/zephir/issues/733
                         */
                        $methodName = strtolower($expression['name'] ?? '');
                        if (in_array($methodName, self::ARRAY_BUILTIN_METHODS_ON_VAR, true)) {
                            $builtInType = new \Zephir\Types\ArrayType();
                            $caller      = $exprCompiledVariable;
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
                     * call site. For `this->returnsStatic()` the two are the
                     * same (handled at the top of this method where
                     * `$classDefinition` was assigned from
                     * `$compilationContext->classDefinition`). For
                     * `other->returnsStatic()->chain()` they differ:
                     * `$classDefinition` already carries the receiver's
                     * class (resolved earlier from
                     * `$variableVariable->getClassTypes()`), and that is
                     * what `self`/`static`/`parent` must rewrite to so the
                     * chained method lookup happens on the right class.
                     * Using `$compilationContext->classDefinition` here
                     * makes the chained call look up the method on the
                     * enclosing class instead, producing a spurious
                     * "Class '<enclosing>' does not implement method ..."
                     * error.
                     * See https://github.com/zephir-lang/zephir/issues/2505.
                     */
                    $receiverDefinition = $classDefinition ?? $compilationContext->classDefinition;
                    foreach ($returnClassTypes as &$returnClassType) {
                        $lower = strtolower($returnClassType);
                        if ($lower === 'self' || $lower === 'static') {
                            $returnClassType = $receiverDefinition->getCompleteName();
                        } elseif ($lower === 'parent') {
                            $parent = $receiverDefinition->getExtendsClass();
                            if ($parent !== null && $parent !== '') {
                                $returnClassType = $parent;
                            }
                        } else {
                            $returnClassType = $compilationContext->getFullName($returnClassType);
                        }
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
        if (isset($expression['parameters'])) {
            $params = $this->getResolvedParams(
                $expression['parameters'],
                $compilationContext,
                $expression,
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
            if (is_object($resolvedMethod)) {
                $method = $resolvedMethod;

                /**
                 * A method declared `internal` in Zephir source is deliberately
                 * kept out of the class' method_entry, so there is no PHP-level
                 * method to dispatch to and a direct call is the only option.
                 *
                 * The twin that `internal-call-transformation` generates is a
                 * different matter: substituting it is only correct where PHP
                 * would not have dispatched virtually in the first place.
                 */
                if (
                    !$method->isInternal()
                    && $this->isStaticallyDispatched(
                        $method,
                        $receiverDefinition,
                        $numberPoly,
                        $compilationContext
                    )
                ) {
                    $method = $method->getOptimizedMethod();
                }

                $isInternal = $method->isInternal();
                if ($isInternal && $numberPoly > 1) {
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
     * Whether PHP itself resolves this call without consulting the receiver's
     * class. That is the precondition for replacing the call with a direct call
     * to the method's own C function, which is what the
     * `internal-call-transformation` optimization does.
     *
     * `obj->m()` is a virtual call: any subclass may override `m` — including a
     * userland PHP class extending a Zephir one, which no amount of
     * whole-program analysis of the extension can see. Only three premises make
     * the target static, and the engine enforces each of them itself
     * (ZEND_ACC_FINAL, ZEND_ACC_PRIVATE, ZEND_ACC_FINAL_CLASS):
     *
     *  - the method is final, so nothing can override it;
     *  - the method is private to the class being compiled, so a same-named
     *    method in a subclass is a separate method rather than an override;
     *  - the receiver is `this` and its class is final, so the runtime class is
     *    exactly the class being compiled.
     *
     * A class type declared on a variable is deliberately *not* accepted as
     * proof of the receiver's class: `let v = <Vector> x` and declared return
     * types assert a class without verifying it.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2021
     */
    private function isStaticallyDispatched(
        Method $method,
        ?Definition $receiver,
        int $numberPoly,
        CompilationContext $compilationContext
    ): bool {
        /**
         * Resolution collected more than one candidate class (a variable
         * declared with several class types) and kept only the last match, so
         * the receiver's class — and with it the target — is unknown.
         */
        if (1 !== $numberPoly) {
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
}
