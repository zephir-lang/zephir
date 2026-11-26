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
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Class\Visibility;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception\CompilerException;
use Zephir\Traits\VariablesTrait;
use Zephir\Variable\Variable;

use function array_merge;
use function count;
use function implode;
use function in_array;
use function is_object;
use function is_string;
use function sprintf;
use function str_replace;
use function strtolower;
use function strtoupper;

/**
 * Call methods in a static context
 */
class StaticCall extends Call
{
    use VariablesTrait;

    /**
     * `dynamic` of a call whose method name is held in a variable,
     * `X::{name}()`. A `X::{"name"}()` string is 2 and an identifier 0.
     */
    private const METHOD_NAME_VARIABLE = 1;

    /**
     * Compiles a static method call.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(Expression $expr, CompilationContext $compilationContext): CompiledExpression
    {
        $expression = $expr->getExpression();

        /**
         * The lowered name is the key every compile time lookup uses, since a
         * class definition holds its methods under that key. The emitted call
         * gets `$expression['name']` instead: PHP hands `__callStatic()` the
         * name as it was written at the call site.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2715
         */
        $methodName = strtolower($expression['name']);

        /**
         * Only a variable makes the name dynamic: PHP compiles `X::{"name"}()`
         * exactly as `X::name()`, so a string goes down the literal paths,
         * compile time checks included.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2723
         */
        $dynamicMethod = self::METHOD_NAME_VARIABLE === (int) ($expression['dynamic'] ?? 0);

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
                if ($readDetector->detect($symbolVariable->getName(), $expression)) {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify(
                        'variable',
                        $compilationContext
                    );
                } else {
                    $mustInit = true;
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify(
                    'variable',
                    $compilationContext
                );
            }
        }

        /**
         * Method calls only return zvals, so we need to validate the target variable is also a zval
         */
        if ($isExpecting) {
            /**
             * At this point, we don't know the exact dynamic type returned by the static method call
             */
            $symbolVariable->setDynamicTypes('undefined');
            $this->checkNotVariable($symbolVariable, $expression);
        }

        /**
         * Include fcall header
         */
        $compilationContext->headersManager->add('kernel/fcall');

        $compiler = $compilationContext->compiler;

        $dynamicClass = $expression['dynamic-class'];
        if (!$dynamicClass) {
            $className       = $expression['class'];
            $classDefinition = false;

            if (!in_array($className, ['self', 'static', 'parent'])) {
                if (is_string($className)) {
                    $className = $compilationContext->getFullName($className);
                    /**
                     * PHP compiles a call through an interface and throws at
                     * run time, as its methods are abstract.
                     *
                     * @see https://github.com/zephir-lang/zephir/issues/2789
                     */
                    if ($compiler->isClass($className) || $compiler->isInterface($className)) {
                        $classDefinition = $compiler->getClassDefinition($className);
                    } else {
                        if ($compiler->isBundledClass($className) || $compiler->isBundledInterface($className)) {
                            $classDefinition = $compiler->getInternalClassDefinition($className);
                        } else {
                            throw new CompilerException('Class name: ' . $className . ' does not exist', $expression);
                        }
                    }
                } else {
                    foreach ($className as $singleClass) {
                        $className = $compilationContext->getFullName($singleClass);
                        if ($compiler->isClass($singleClass)) {
                            $classDefinition = $compiler->getClassDefinition($singleClass);
                        } else {
                            throw new CompilerException('Class name: ' . $className . ' does not exist', $expression);
                        }
                    }
                }
            } else {
                $classDefinition = $compilationContext->classScope($className, $expression);
                if (null === $classDefinition) {
                    return new CompiledExpression('null', null, $expression);
                }

                if ('parent' === $className) {
                    $currentClassDefinition = $classDefinition;
                    $classDefinition        = $compilationContext->parentClassDefinition(
                        'call method "' . $expression['name'] . '"',
                        $expression
                    );
                }
            }
        }

        /**
         * Check if the class implements the method
         */
        if (!$dynamicMethod && !$dynamicClass) {
            /**
             * PHP compiles `X::__construct()` without looking the method up:
             * the class's constructor is taken when the call runs, with PHP's
             * own checks (zephir_call_constructor() in kernel/fcall.c). So
             * neither a missing constructor nor a private one stops the
             * compilation.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2852
             * @see https://github.com/zephir-lang/zephir/issues/2853
             */
            $isConstructor = '__construct' === $methodName;

            if (!$classDefinition->hasMethod($methodName)) {
                if (!$isConstructor) {
                    $possibleMethod = $classDefinition->getPossibleMethodName($methodName);
                    if ($possibleMethod) {
                        throw new CompilerException(
                            "Class '"
                            . $classDefinition->getCompleteName()
                            . "' does not implement static method: '"
                            . $expression['name']
                            . "'. Did you mean '"
                            . $possibleMethod
                            . "'?",
                            $expression
                        );
                    }

                    throw new CompilerException(
                        "Class '"
                        . $classDefinition->getCompleteName()
                        . "' does not implement static method: '"
                        . $expression['name']
                        . "'",
                        $expression
                    );
                }
            } else {
                $method           = $classDefinition->getMethod($methodName);
                $this->reflection = $method;

                /**
                 * PHP checks the method against the class the call is written
                 * in, and an inaccessible one goes to `__callStatic()` or, with
                 * an object of the class at hand, to `__call()`.
                 *
                 * @see https://github.com/zephir-lang/zephir/issues/2734
                 */
                if (
                    !$isConstructor
                    && !Visibility::canCallMethod($method, $compilationContext)
                    && !$classDefinition->hasMethod('__callStatic')
                    && !$classDefinition->hasMethod('__call')
                ) {
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
                 * PHP runs a non-static method named through a class on $this
                 * when $this is an instance of that class, and throws when the
                 * call runs otherwise; the kernel does the same. An internal
                 * method has no PHP method such a call could reach, so only
                 * `self::`, `static::`, `parent::` or an object reach it.
                 *
                 * @see https://github.com/zephir-lang/zephir/issues/2854
                 */
                if (
                    !in_array($className, ['self', 'static', 'parent'])
                    && $method->isInternal()
                    && !$method->isStatic()
                ) {
                    throw new CompilerException(
                        "Cannot call non-static method '" . $expression['name'] . "' in a static way",
                        $expression
                    );
                }

                if (!$classDefinition->hasMethod('__callStatic')) {
                    if ($method instanceof Method && !$method->isBundled()) {
                        /**
                         * Try to produce an exception if method is called with a wrong number of parameters
                         */
                        $callNumberParameters = isset($expression['parameters']) ? count(
                            $expression['parameters']
                        ) : 0;

                        $classMethod              = $classDefinition->getMethod($methodName);
                        $expectedNumberParameters = $classMethod->getNumberOfRequiredParameters();

                        if (!$expectedNumberParameters && $callNumberParameters > 0 && !$classMethod->isVariadic()) {
                            $numberParameters = $classMethod->getNumberOfParameters();
                            if ($callNumberParameters > $numberParameters) {
                                throw new CompilerException(
                                    "Method '" . $classDefinition->getCompleteName() . '::' . $expression['name']
                                    . "' called with a wrong number of parameters, the method has: "
                                    . $expectedNumberParameters
                                    . ', passed: '
                                    . $callNumberParameters,
                                    $expression
                                );
                            }
                        }

                        if ($callNumberParameters < $expectedNumberParameters) {
                            throw new CompilerException(
                                "Method '"
                                . $classDefinition->getCompleteName()
                                . '::'
                                . $expression['name']
                                . "' called with a wrong number of parameters, the method has: "
                                . $expectedNumberParameters
                                . ', passed: '
                                . $callNumberParameters,
                                $expression
                            );
                        }
                    }
                } else {
                    if (!isset($method)) {
                        $method = $classDefinition->getMethod('__callStatic');
                        if (
                            $method->isPrivate() &&
                            $method->getClassDefinition() !== $compilationContext->lexicalClassDefinition()
                        ) {
                            throw new CompilerException(
                                "Cannot call private magic method '__call' out of its scope",
                                $expression
                            );
                        }
                    }
                }
            }
        }

        /**
         * `self::` and `static::` forward the called class to a static method.
         * A class name does not, the name of the class the call is written in
         * included: PHP gives `Own::sm()` the called class `Own`.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2854
         */
        if (!$dynamicMethod) {
            if ($dynamicClass) {
                $this->callFromDynamicClass(
                    $expression['name'],
                    $expression,
                    $symbolVariable,
                    $mustInit,
                    $isExpecting,
                    $compilationContext
                );
            } else {
                if (in_array($className, ['self', 'static'])) {
                    $this->call(
                        strtoupper($className),
                        $expression['name'],
                        $expression,
                        $mustInit,
                        $isExpecting,
                        $compilationContext,
                        $symbolVariable,
                        $method ?? null
                    );
                } else {
                    if ('parent' == $className) {
                        $this->callParent(
                            $expression['name'],
                            $expression,
                            $symbolVariable,
                            $mustInit,
                            $isExpecting,
                            $currentClassDefinition,
                            $compilationContext,
                            $method ?? null
                        );
                    } else {
                        $this->callFromClass(
                            $expression['name'],
                            $expression,
                            $symbolVariable,
                            $mustInit,
                            $isExpecting,
                            $classDefinition,
                            $compilationContext,
                            $method ?? null
                        );
                    }
                }
            }
        } else {
            if ($dynamicClass) {
                $this->callFromDynamicClassDynamicMethod(
                    $expression,
                    $symbolVariable,
                    $mustInit,
                    $isExpecting,
                    $compilationContext
                );
            } else {
                $this->callFromClassDynamicMethod(
                    $className,
                    $classDefinition,
                    $currentClassDefinition ?? null,
                    $expression,
                    $symbolVariable,
                    $mustInit,
                    $isExpecting,
                    $compilationContext
                );
            }
        }

        /**
         * Add the last call status to the current symbol table
         */
        $this->addCallStatusFlag($compilationContext);

        /**
         * Transfer the return type-hint to the returned variable
         */
        if ($isExpecting && isset($method) && $method instanceof Method) {
            /*
             * `parent::` forwards the called class, so `static` and `self`
             * stand for the class the call is written in there.
             */
            $symbolVariable->setDynamicTypes('object');
            $symbolVariable->setClassTypes(
                $method->resolveReturnClassTypes($currentClassDefinition ?? $classDefinition, $compilationContext)
            );

            foreach ($method->getReturnTypes() as $dataType => $returnType) {
                $symbolVariable->setDynamicTypes($dataType);
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

    /**
     * Calls static methods on the 'self/static' context.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    protected function call(
        string $context,
        string $callName,
        array $expression,
        bool $mustInit,
        bool $isExpecting,
        CompilationContext $compilationContext,
        ?Variable $symbolVariable = null,
        ?Method $method = null
    ): void {
        /**
         * Do not optimize static:: calls, to allow late static binding. A
         * closure calls the method itself: the internal call would hand it
         * the closure's own $this. Neither is `self::__construct()` optimized:
         * PHP refuses a private constructor there when $this is an object of
         * a subclass, which the kernel checks (#2853). Without a constructor
         * there is no method at all (#2852).
         */
        $insideClosure = true === $compilationContext->classDefinition?->isClosure();
        $named         = FunctionCall::hasNamedArgument($expression);
        $twin          = null;
        if (
            'SELF' === $context
            && null !== $method
            && !$insideClosure
            && !$named
            && !$method->isInternal()
            && '__construct' !== strtolower($callName)
        ) {
            $twin = $method->getOptimizedMethod();
            if ($twin === $method) {
                $twin = null;
            }
        }

        $codePrinter = $compilationContext->codePrinter;

        /**
         * Call static methods must grow the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        $params = [];
        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        }

        [$params, $named] = $this->splitNamedArguments($params, $compilationContext);

        $paramCount = count($params);
        $paramsStr  = $paramCount ? ', ' . implode(', ', $params) : '';

        $isInternal = isset($method) && $method->isInternal();
        $this->assertNamedArgumentsReachEngine($named, $isInternal, $expression);
        if (null !== $named) {
            $macro   = $context . ($insideClosure ? '_THIS' : '') . '_CACHED';
            $leading = $insideClosure ? [$compilationContext->enclosingObjectCode()] : [];
            $compilationContext->backend->emitCall(
                $isExpecting ? $symbolVariable : null,
                'CALL_CLASS_METHOD_NAMED',
                $this->namedCallScope($macro, $leading) . ', "' . $callName . '", ' . $named . $paramsStr,
                $compilationContext
            );
            $this->releaseNamedArguments($compilationContext);
        } elseif (!$isInternal) {
            /**
             * The site's own inline cache, keyed by the class the call resolves
             * against, which for `static::` is the called class (#2790).
             */
            $cachePointer = $compilationContext->cacheManager->getMethodCache()->get($compilationContext);

            /**
             * A closure passes on the object it was created with, which its
             * own $this is not when that is its capture carrier (#2825).
             */
            $macro  = $context . ($insideClosure ? '_THIS' : '') . '_CACHED';
            $object = $insideClosure ? $compilationContext->enclosingObjectCode() . ', ' : '';

            $call = function () use ($isExpecting, $symbolVariable, $macro, $object, $callName, $cachePointer, $paramsStr, $codePrinter, $compilationContext): void {
                if (!$isExpecting) {
                    $codePrinter->output(
                        'ZEPHIR_CALL_' . $macro . '(NULL, ' . $object . '"' . $callName . '", ' . $cachePointer . $paramsStr . ');'
                    );
                } elseif ('return_value' == $symbolVariable->getName()) {
                    $codePrinter->output(
                        'ZEPHIR_RETURN_CALL_' . $macro . '(' . $object . '"' . $callName . '", ' . $cachePointer . $paramsStr . ');'
                    );
                } else {
                    $codePrinter->output(
                        'ZEPHIR_CALL_' . $macro . '(' . $compilationContext->backend->getVariableCode($symbolVariable)
                        . ', ' . $object . '"' . $callName . '", ' . $cachePointer . $paramsStr . ');'
                    );
                }
            };

            $conditions = null;
            if (null !== $twin && ($method->isStatic() || !$this->inStaticContext($compilationContext))) {
                $conditions = $this->twinArgumentChecks($method, $params, $compilationContext);
            }

            if (null === $conditions) {
                $call();
            } else {
                $conditions[] = 'zephir_has_stack_for_direct_call()';
                $this->emitGuardedDirectCall(
                    $conditions,
                    fn () => $this->emitSelfInternalCall($twin, $params, $symbolVariable, $isExpecting, $compilationContext),
                    $call,
                    $compilationContext
                );
            }
        } else {
            $this->emitSelfInternalCall($method, $params, $symbolVariable, $isExpecting, $compilationContext);
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
     * Calls a method through a class name, `Other::m()`, the name of the class
     * the call is written in included. The class named is the called class of
     * a static method, which runs in the scope of the class declaring it.
     *
     * A constructor, with or without a method behind it, or a non-static
     * method runs on the current $this when that is an instance of the class
     * named, so $this goes along and the kernel checks it; PHP throws without
     * one. A static method gets none, which keeps its call as it was.
     *
     * @param string             $callName
     * @param array              $expression
     * @param Variable|null      $symbolVariable
     * @param bool               $mustInit
     * @param bool               $isExpecting
     * @param Definition         $classDefinition
     * @param CompilationContext $compilationContext
     * @param Method|null        $method
     *
     * @throws Exception
     *
     * @see https://github.com/zephir-lang/zephir/issues/2854
     */
    protected function callFromClass(
        $callName,
        array $expression,
        $symbolVariable,
        $mustInit,
        $isExpecting,
        Definition $classDefinition,
        CompilationContext $compilationContext,
        ?Method $method
    ): void {
        $codePrinter = $compilationContext->codePrinter;
        $classEntry  = $this->classEntryOf($classDefinition, $compilationContext);

        /**
         * Call static methods must grow the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        $object = null;
        if ((null === $method || !$method->isStatic()) && !$this->inStaticContext($compilationContext)) {
            $object = $compilationContext->enclosingObjectCode();
        }

        /**
         * A direct call to the transformed twin skips the engine, so it is
         * only taken where the engine would run this very method.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2734
         */
        $twin = null;
        if (
            null !== $method
            && Visibility::canCallMethod($method, $compilationContext)
            && !FunctionCall::hasNamedArgument($expression)
            && !$method->isInternal()
            && $method->isStatic()
        ) {
            $twin = $method->getOptimizedMethod();
            if ($twin === $method) {
                $twin = null;
            }
        }

        $params = [];
        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        }

        [$params, $named] = $this->splitNamedArguments($params, $compilationContext);

        $paramCount = count($params);
        $paramsStr  = $paramCount ? ', ' . implode(', ', $params) : '';

        $isInternal = null !== $method && $method->isInternal();
        $this->assertNamedArgumentsReachEngine($named, $isInternal, $expression);

        /**
         * `Other::m()` names the class: it is the called class, and the method
         * runs in the scope of the class that declares it.
         */
        $target = null === $method ? '' : $method->getClassDefinition()->getClassEntry($compilationContext)
            . ', ' . $classDefinition->getClassEntry($compilationContext);

        if (null === $named && $isInternal) {
            $this->emitInternalMethodCall(
                $method,
                true,
                $target,
                $params,
                $symbolVariable,
                $isExpecting,
                $compilationContext
            );
        } else {
            $call = fn () => $this->emitClassCall(
                $symbolVariable,
                $isExpecting,
                $classEntry,
                $object,
                $callName,
                $paramsStr,
                $compilationContext,
                $named
            );

            $conditions = null;
            if (null !== $twin && null === $named) {
                $conditions = $this->twinArgumentChecks($method, $params, $compilationContext);
            }

            if (null === $conditions) {
                $call();
            } else {
                $conditions[] = 'zephir_has_stack_for_direct_call()';
                $this->emitGuardedDirectCall(
                    $conditions,
                    fn () => $this->emitInternalMethodCall(
                        $twin,
                        true,
                        $target,
                        $params,
                        $symbolVariable,
                        $isExpecting,
                        $compilationContext
                    ),
                    $call,
                    $compilationContext
                );
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
     * Calls static methods on using a dynamic variable as class.
     *
     * @param string             $callName
     * @param array              $expression
     * @param Variable           $symbolVariable
     * @param bool               $mustInit
     * @param bool               $isExpecting
     * @param CompilationContext $compilationContext
     */
    protected function callFromDynamicClass(
        string $callName,
        array $expression,
        $symbolVariable,
        $mustInit,
        $isExpecting,
        CompilationContext $compilationContext
    ): void {
        [$params, $classEntry] = $this->fetchClassParams($expression, $compilationContext, $symbolVariable, $mustInit);
        [$params, $named]      = $this->splitNamedArguments($params, $compilationContext);

        $paramsStr = count($params) ? ', ' . implode(', ', $params) : '';

        /**
         * Which method that is, static or not, is only known at run time, so
         * the current $this goes along for the kernel to check (#2854).
         */
        $object = $this->inStaticContext($compilationContext) ? null : $compilationContext->enclosingObjectCode();

        $this->emitClassCall($symbolVariable, $isExpecting, $classEntry, $object, $callName, $paramsStr, $compilationContext, $named);

        /**
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $compilationContext->codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }

        $this->addCallStatusOrJump($compilationContext);
    }

    /**
     * Calls static methods on using a dynamic variable as class and a dynamic method.
     *
     * @param array              $expression
     * @param Variable|null      $symbolVariable
     * @param bool               $mustInit
     * @param bool               $isExpecting
     * @param CompilationContext $compilationContext
     */
    protected function callFromDynamicClassDynamicMethod(
        array $expression,
        ?Variable $symbolVariable,
        bool $mustInit,
        bool $isExpecting,
        CompilationContext $compilationContext
    ): void {
        [$params, $classEntry] = $this->fetchClassParams($expression, $compilationContext, $symbolVariable, $mustInit);
        [$params, $named]      = $this->splitNamedArguments($params, $compilationContext);

        /**
         * A closure passes on the object it was created with, which its own
         * $this is not when that is its capture carrier (#2825, #2854).
         */
        $insideClosure = true === $compilationContext->classDefinition?->isClosure();

        $this->emitDynamicMethodCall(
            $insideClosure ? 'CE_STATIC_THIS_ZVAL' : 'CE_STATIC_ZVAL',
            $insideClosure ? [$classEntry, $compilationContext->enclosingObjectCode()] : [$classEntry],
            $this->methodNameVariable($expression, $compilationContext),
            $params,
            $symbolVariable,
            $isExpecting,
            $compilationContext,
            $named
        );
    }

    /**
     * Calls a method named by a variable on a class known while compiling:
     * `self::{name}()`, `parent::{name}()`, `static::{name}()` and
     * `Cls::{name}()`. Which method that is can only be known at run time, so
     * the kernel looks it up the way PHP does.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2723
     */
    private function callFromClassDynamicMethod(
        string $className,
        Definition $classDefinition,
        ?Definition $currentClassDefinition,
        array $expression,
        ?Variable $symbolVariable,
        bool $mustInit,
        bool $isExpecting,
        CompilationContext $compilationContext
    ): void {
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        $methodNameVariable = $this->methodNameVariable($expression, $compilationContext);

        /**
         * A closure passes on the object it was created with, which its own
         * $this is not when that is its capture carrier (#2825).
         */
        $insideClosure = true === $compilationContext->classDefinition?->isClosure();
        $object        = $compilationContext->enclosingObjectCode();

        switch ($className) {
            case 'self':
                $macro            = $insideClosure ? 'SELF_THIS_ZVAL' : 'SELF_ZVAL';
                $leadingArguments = $insideClosure
                    ? [$classDefinition->getClassEntry($compilationContext), $object]
                    : [$classDefinition->getClassEntry($compilationContext)];
                break;

            case 'parent':
                $macro            = 'PARENT_ZVAL';
                $leadingArguments = [$currentClassDefinition->getClassEntry($compilationContext), $object];
                break;

            case 'static':
                $macro            = $insideClosure ? 'STATIC_THIS_ZVAL' : 'STATIC_ZVAL';
                $leadingArguments = $insideClosure ? [$object] : [];
                break;

            default:
                $macro            = $insideClosure ? 'CE_STATIC_THIS_ZVAL' : 'CE_STATIC_ZVAL';
                $leadingArguments = $insideClosure
                    ? [$this->classEntryOf($classDefinition, $compilationContext), $object]
                    : [$this->classEntryOf($classDefinition, $compilationContext)];
                break;
        }

        $params = [];
        if (!empty($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        }

        [$params, $named] = $this->splitNamedArguments($params, $compilationContext);

        $this->emitDynamicMethodCall(
            $macro,
            $leadingArguments,
            $methodNameVariable,
            $params,
            $symbolVariable,
            $isExpecting,
            $compilationContext,
            $named
        );
    }

    /**
     * Emits one of the `_ZVAL` call macros of kernel/fcall.h. The method name
     * goes in as the `zval *` of whatever variable holds it: a local, a
     * parameter or the companion zval of a native string.
     *
     * @param list<string> $leadingArguments
     * @param list<string> $params
     */
    private function emitDynamicMethodCall(
        string $macro,
        array $leadingArguments,
        Variable $methodNameVariable,
        array $params,
        ?Variable $symbolVariable,
        bool $isExpecting,
        CompilationContext $compilationContext,
        ?string $named = null
    ): void {
        $backend = $compilationContext->backend;

        if (null !== $named) {
            $backend->emitCall(
                $isExpecting ? $symbolVariable : null,
                'CALL_CLASS_METHOD_ZVAL_NAMED',
                implode(', ', array_merge(
                    [$this->namedCallScope($macro, $leadingArguments), $backend->getVariableCode($methodNameVariable), $named],
                    $params
                )),
                $compilationContext
            );
            $this->releaseNamedArguments($compilationContext);

            foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
                $compilationContext->codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
            }

            $this->addCallStatusOrJump($compilationContext);

            return;
        }

        $arguments = array_merge(
            $leadingArguments,
            [$backend->getVariableCode($methodNameVariable), 'NULL', '0'],
            $params
        );

        if ($isExpecting && 'return_value' === $symbolVariable->getName()) {
            $call = 'ZEPHIR_RETURN_CALL_' . $macro . '(' . implode(', ', $arguments) . ');';
        } else {
            $result = $isExpecting ? $backend->getVariableCode($symbolVariable) : 'NULL';
            $call   = 'ZEPHIR_CALL_' . $macro . '(' . implode(', ', array_merge([$result], $arguments)) . ');';
        }

        $compilationContext->codePrinter->output($call);

        /**
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $compilationContext->codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }

        $this->addCallStatusOrJump($compilationContext);
    }

    /**
     * The variable holding the method name of `X::{name}()`.
     */
    private function methodNameVariable(array $expression, CompilationContext $compilationContext): Variable
    {
        $variable = $compilationContext->symbolTable->getVariableForRead(
            $expression['name'],
            $compilationContext,
            $expression
        );

        if ($variable->isNotVariableAndMixedAndString()) {
            throw new CompilerException('Only dynamic/string variables can be used in dynamic methods', $expression);
        }

        return $variable;
    }

    /**
     * The C class entry of a class known while compiling. An internal PHP class
     * has no C symbol of its own, so it is fetched into a temporary by name.
     */
    private function classEntryOf(Definition $classDefinition, CompilationContext $compilationContext): string
    {
        if (!$classDefinition->isBundled()) {
            return $classDefinition->getClassEntry($compilationContext);
        }

        $classEntryVariable = $compilationContext->symbolTable->addTemp('zend_class_entry', $compilationContext);
        $compilationContext->backend->fetchClass(
            $classEntryVariable,
            'SL("' . str_replace('\\', '\\\\', $classDefinition->getName()) . '")',
            false,
            $compilationContext
        );

        return $classEntryVariable->getName();
    }

    /**
     * Calls static methods on the 'parent' context.
     *
     * @param string             $callName
     * @param array              $expression
     * @param Variable           $symbolVariable
     * @param bool               $mustInit
     * @param bool               $isExpecting
     * @param Definition         $classDefinition
     * @param CompilationContext $compilationContext
     * @param Method|null        $method             null for a constructor the parent does not have (#2852)
     *
     * @throws Exception
     */
    protected function callParent(
        string $callName,
        array $expression,
        $symbolVariable,
        $mustInit,
        $isExpecting,
        Definition $classDefinition,
        CompilationContext $compilationContext,
        ?Method $method
    ): void {
        $codePrinter = $compilationContext->codePrinter;
        $classCe     = $classDefinition->getClassEntry($compilationContext);

        /**
         * Call static methods must grow the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        $params = [];
        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        }

        [$params, $named] = $this->splitNamedArguments($params, $compilationContext);

        $paramsStr = count($params) ? ', ' . implode(', ', $params) : '';
        if (null !== $named) {
            $compilationContext->backend->emitCall(
                $isExpecting ? $symbolVariable : null,
                'CALL_CLASS_METHOD_NAMED',
                $this->namedCallScope('PARENT_CACHED', [$classCe, $compilationContext->enclosingObjectCode()])
                . ', "' . $callName . '", ' . $named . $paramsStr,
                $compilationContext
            );
            $this->releaseNamedArguments($compilationContext);

            foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
                $codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
            }

            $this->addCallStatusOrJump($compilationContext);

            return;
        }

        $cachePointer = $compilationContext->cacheManager->getMethodCache()->get($compilationContext);
        $target       = $classCe . ', ' . $compilationContext->enclosingObjectCode() . ', "' . $callName . '", '
            . $cachePointer . $paramsStr;

        if (!$isExpecting) {
            $codePrinter->output('ZEPHIR_CALL_PARENT_CACHED(NULL, ' . $target . ');');
        } elseif ('return_value' == $symbolVariable->getName()) {
            $codePrinter->output('ZEPHIR_RETURN_CALL_PARENT_CACHED(' . $target . ');');
        } else {
            $codePrinter->output(
                'ZEPHIR_CALL_PARENT_CACHED('
                . $compilationContext->backend->getVariableCode($symbolVariable) . ', ' . $target . ');'
            );
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
     * Emits `Class::method()` for a class entry known in C, through the call
     * site's own inline cache (#2790). With `$object`, the C code of the
     * current $this, a non-static method runs on it when it is an instance of
     * the class (#2854).
     */
    private function emitClassCall(
        ?Variable $symbolVariable,
        bool $isExpecting,
        string $classEntry,
        ?string $object,
        string $callName,
        string $paramsStr,
        CompilationContext $compilationContext,
        ?string $named = null
    ): void {
        $macro   = null === $object ? 'CE_STATIC_CACHED' : 'CE_STATIC_THIS_CACHED';
        $leading = null === $object ? [$classEntry] : [$classEntry, $object];

        if (null !== $named) {
            $compilationContext->backend->emitCall(
                $isExpecting ? $symbolVariable : null,
                'CALL_CLASS_METHOD_NAMED',
                $this->namedCallScope($macro, $leading) . ', "' . $callName . '", ' . $named . $paramsStr,
                $compilationContext
            );
            $this->releaseNamedArguments($compilationContext);

            return;
        }

        $cachePointer = $compilationContext->cacheManager->getMethodCache()->get($compilationContext);
        $target       = implode(', ', $leading) . ', "' . $callName . '", ' . $cachePointer . $paramsStr;

        if (!$isExpecting) {
            $compilationContext->codePrinter->output('ZEPHIR_CALL_' . $macro . '(NULL, ' . $target . ');');
        } elseif ('return_value' == $symbolVariable->getName()) {
            $compilationContext->codePrinter->output('ZEPHIR_RETURN_CALL_' . $macro . '(' . $target . ');');
        } else {
            $compilationContext->codePrinter->output(
                'ZEPHIR_CALL_' . $macro . '('
                . $compilationContext->backend->getVariableCode($symbolVariable) . ', ' . $target . ');'
            );
        }
    }

    /**
     * Fetches the class of `{className}::...()` into a temporary and resolves
     * the arguments, in PHP's order: the class comes first, and a class that
     * cannot be fetched stops the call before any argument is evaluated.
     *
     * @return array{0: list<string>, 1: string}
     *
     * @see https://github.com/zephir-lang/zephir/issues/2723
     */
    /**
     * The `type, class entry, object` a call with named arguments passes for
     * each static call macro, from the leading arguments that macro takes: the
     * same class and object the macro would call with (#2824).
     *
     * @param list<string> $leading
     */
    private function namedCallScope(string $macro, array $leading): string
    {
        $calling = 'zephir_get_calling_scope()';
        $called  = 'zephir_get_called_scope(EG(current_execute_data))';

        return implode(', ', match ($macro) {
            'SELF_CACHED'           => ['zephir_fcall_self', $calling, 'getThis()'],
            'SELF_THIS_CACHED'      => ['zephir_fcall_self', $calling, $leading[0]],
            'STATIC_CACHED'         => ['zephir_fcall_static', $called, 'getThis()'],
            'STATIC_THIS_CACHED'    => ['zephir_fcall_static', $called, $leading[0]],
            'PARENT_CACHED'         => ['zephir_fcall_parent', $leading[0], $leading[1]],
            'CE_STATIC_CACHED'      => ['zephir_fcall_ce', $leading[0], 'NULL'],
            'CE_STATIC_THIS_CACHED' => ['zephir_fcall_ce', $leading[0], $leading[1]],
            'SELF_ZVAL'             => ['zephir_fcall_self', $leading[0], 'getThis()'],
            'SELF_THIS_ZVAL'        => ['zephir_fcall_self', $leading[0], $leading[1]],
            'PARENT_ZVAL'           => ['zephir_fcall_parent', $leading[0], $leading[1]],
            'STATIC_ZVAL'           => ['zephir_fcall_static', $called, 'getThis()'],
            'STATIC_THIS_ZVAL'      => ['zephir_fcall_static', $called, $leading[0]],
            'CE_STATIC_ZVAL'        => ['zephir_fcall_ce', $leading[0], 'getThis()'],
            'CE_STATIC_THIS_ZVAL'   => ['zephir_fcall_ce', $leading[0], $leading[1]],
        });
    }

    /**
     * Named arguments are bound by the engine, which the direct C call to an
     * internal method skips (#2824).
     */
    private function assertNamedArgumentsReachEngine(?string $named, bool $isInternal, array $expression): void
    {
        if (null !== $named && $isInternal) {
            throw new CompilerException(
                "Named arguments cannot be passed to the internal method '" . $expression['name'] . "'",
                $expression
            );
        }
    }

    private function fetchClassParams(
        array $expression,
        CompilationContext $compilationContext,
        ?Variable $symbolVariable,
        bool $mustInit
    ): array {
        /**
         * Call static methods must grow the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        /**
         * Obtain the class entry from the variable.
         */
        $classNameVariable = $compilationContext->symbolTable->getVariableForRead(
            $expression['class'],
            $compilationContext,
            $expression
        );
        if ($classNameVariable->isNotVariableAndMixedAndString()) {
            throw new CompilerException('Only dynamic/string variables can be used in dynamic classes', $expression);
        }

        $compilationContext->headersManager->add('kernel/object');
        $classEntryVariable = $compilationContext->symbolTable->addTemp('zend_class_entry', $compilationContext);

        $compilationContext->codePrinter->output(
            sprintf(
                '%s = zephir_fetch_class(%s);',
                $classEntryVariable->getName(),
                $compilationContext->backend->getVariableCode($classNameVariable)
            )
        );
        $compilationContext->emitExceptionCheck();

        $params = [];
        if (!empty($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        }

        return [$params, $classEntryVariable->getName()];
    }

    /**
     * Calls an internal method, or a method's C copy, reached through
     * `self::`. PHP runs a non-static method with the current $this, and
     * `self::` passes on the called class to a static one: it is a forwarding
     * call.
     *
     * @param list<string> $params
     *
     * @see https://github.com/zephir-lang/zephir/issues/2791
     */
    private function emitSelfInternalCall(
        Method $method,
        array $params,
        ?Variable $symbolVariable,
        bool $isExpecting,
        CompilationContext $compilationContext
    ): void {
        $classEntry = $method->getClassDefinition()->getClassEntry($compilationContext);

        if ($method->isStatic()) {
            $this->emitInternalMethodCall(
                $method,
                true,
                $classEntry . ', zephir_get_called_scope(execute_data)',
                $params,
                $symbolVariable,
                $isExpecting,
                $compilationContext
            );

            return;
        }

        /**
         * There is no $this to pass on from a static method. PHP throws for
         * that call, which the C copy is never used for; an internal method
         * has no other way to be called and keeps running without one.
         */
        if ($this->inStaticContext($compilationContext)) {
            $this->emitInternalMethodCall(
                $method,
                true,
                $classEntry . ', ' . $classEntry,
                $params,
                $symbolVariable,
                $isExpecting,
                $compilationContext
            );

            return;
        }

        $thisVariable = $compilationContext->symbolTable->getVariableForRead('this', $compilationContext);

        $this->emitInternalMethodCall(
            $method,
            false,
            $compilationContext->backend->getVariableCode($thisVariable) . ', ' . $classEntry,
            $params,
            $symbolVariable,
            $isExpecting,
            $compilationContext
        );
    }

    /**
     * Whether the code being compiled runs without $this.
     */
    private function inStaticContext(CompilationContext $compilationContext): bool
    {
        return true === $compilationContext->currentMethod?->isStatic();
    }
}
