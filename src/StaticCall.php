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
                    if ($compiler->isClass($className)) {
                        $classDefinition = $compiler->getClassDefinition($className);
                    } else {
                        if ($compiler->isBundledClass($className)) {
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
                $classDefinition = $compilationContext->classDefinition;

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
            if (!$classDefinition->hasMethod($methodName)) {
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
                } else {
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
                $method = $classDefinition->getMethod($methodName);

                if ($method->isPrivate() && $method->getClassDefinition() !== $compilationContext->classDefinition) {
                    throw new CompilerException(
                        "Cannot call private method '" . $expression['name'] . "' out of its scope",
                        $expression
                    );
                }

                if (!in_array($className, ['self', 'static', 'parent']) && !$method->isStatic()) {
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
                            $method->getClassDefinition() !== $compilationContext->classDefinition
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
         * Call static methods in the same class, use the special context 'self' or special context 'static'
         * Call static methods in the 'self' context
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
                if (
                    in_array($className, ['self', 'static']) ||
                    $classDefinition === $compilationContext->classDefinition
                ) {
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
            $symbolVariable->setDynamicTypes('object');
            foreach ($method->getReturnClassTypes() as $classType) {
                $symbolVariable->setClassTypes($compilationContext->getFullName($classType));
            }

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
        if (!in_array($context, ['SELF', 'STATIC'])) {
            $context = 'SELF';
        }

        /**
         * Do not optimize static:: calls, to allow late static binding
         */
        if ('SELF' === $context) {
            $method = $method->getOptimizedMethod();
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

        /**
         * Check if the  method call can have an inline cache.
         */
        $methodCache  = $compilationContext->cacheManager->getStaticMethodCache();
        $cachePointer = $methodCache->get($compilationContext, $method ?? null);

        $params = [];
        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        }

        if ($symbolVariable) {
            $symbol = $compilationContext->backend->getVariableCode($symbolVariable);
        }

        $paramCount = count($params);
        $paramsStr  = $paramCount ? ', ' . implode(', ', $params) : '';

        $isInternal = isset($method) && $method->isInternal();
        if (!$isInternal) {
            if ($isExpecting) {
                if ('return_value' == $symbolVariable->getName()) {
                    $codePrinter->output(
                        'ZEPHIR_RETURN_CALL_' . $context . '("' . $callName . '", ' . $cachePointer . $paramsStr . ');'
                    );
                } else {
                    $codePrinter->output(
                        'ZEPHIR_CALL_' . $context . '(' . $symbol . ', "' . $callName . '", ' . $cachePointer . $paramsStr . ');'
                    );
                }
            } else {
                $codePrinter->output(
                    'ZEPHIR_CALL_' . $context . '(NULL, "' . $callName . '", ' . $cachePointer . $paramsStr . ');'
                );
            }
        } else {
            $ce = $method->getClassDefinition()->getClassEntry($compilationContext);
            if ($isExpecting) {
                if ('return_value' == $symbolVariable->getName()) {
                    $macro = $compilationContext->backend->getFcallManager()->getMacro(true, 1, $paramCount);
                    $codePrinter->output($macro . '(' . $ce . ', ' . $method->getInternalName() . $paramsStr . ');');
                } else {
                    $macro = $compilationContext->backend->getFcallManager()->getMacro(true, 2, $paramCount);
                    $codePrinter->output(
                        $macro . '(' . $symbol . ', ' . $ce . ', ' . $method->getInternalName() . $paramsStr . ');'
                    );
                }
            } else {
                $macro = $compilationContext->backend->getFcallManager()->getMacro(true, 0, $paramCount);
                $codePrinter->output($macro . '(' . $ce . ', ' . $method->getInternalName() . $paramsStr . ');');
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
     * Calls static methods on some class context.
     *
     * @param string             $callName
     * @param array              $expression
     * @param Variable           $symbolVariable
     * @param bool               $mustInit
     * @param bool               $isExpecting
     * @param Definition         $classDefinition
     * @param CompilationContext $compilationContext
     * @param Method             $method
     *
     * @throws Exception
     */
    protected function callFromClass(
        $callName,
        array $expression,
        $symbolVariable,
        $mustInit,
        $isExpecting,
        Definition $classDefinition,
        CompilationContext $compilationContext,
        Method $method
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

        $method = $method->getOptimizedMethod();

        /**
         * Check if the  method call can have an inline cache.
         */
        $methodCache  = $compilationContext->cacheManager->getStaticMethodCache();
        $cachePointer = $methodCache->get($compilationContext, $method);

        $params = [];
        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        }

        if ($symbolVariable) {
            $symbol = $compilationContext->backend->getVariableCode($symbolVariable);
        }

        $paramCount = count($params);
        $paramsStr  = $paramCount ? ', ' . implode(', ', $params) : '';

        if ($method->isInternal()) {
            $ce = $classDefinition->getClassEntry($compilationContext);
            if ($isExpecting) {
                if ('return_value' == $symbolVariable->getName()) {
                    $macro = $compilationContext->backend->getFcallManager()->getMacro(true, 1, $paramCount);
                    $codePrinter->output($macro . '(' . $ce . ', ' . $method->getInternalName() . $paramsStr . ');');
                } else {
                    $macro = $compilationContext->backend->getFcallManager()->getMacro(true, 2, $paramCount);
                    $codePrinter->output(
                        $macro . '(' . $symbol . ', ' . $ce . ', ' . $method->getInternalName() . $paramsStr . ');'
                    );
                }
            } else {
                $macro = $compilationContext->backend->getFcallManager()->getMacro(true, 0, $paramCount);
                $codePrinter->output($macro . '(' . $ce . ', ' . $method->getInternalName() . $paramsStr . ');');
            }
        } else {
            if ($isExpecting) {
                if ('return_value' == $symbolVariable->getName()) {
                    $codePrinter->output(
                        'ZEPHIR_RETURN_CALL_CE_STATIC(' . $classEntry . ', "' . $callName . '", ' . $cachePointer . $paramsStr . ');'
                    );
                } else {
                    $codePrinter->output(
                        'ZEPHIR_CALL_CE_STATIC(' . $symbol . ', ' . $classEntry . ', "' . $callName . '", ' . $cachePointer . $paramsStr . ');'
                    );
                }
            } else {
                $codePrinter->output(
                    'ZEPHIR_CALL_CE_STATIC(NULL, ' . $classEntry . ', "' . $callName . '", ' . $cachePointer . $paramsStr . ');'
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

        if ($symbolVariable) {
            $symbol = $compilationContext->backend->getVariableCode($symbolVariable);
        }

        $cachePointer = 'NULL, 0';

        if (!count($params)) {
            if ($isExpecting) {
                if ('return_value' == $symbolVariable->getName()) {
                    $compilationContext->codePrinter->output(
                        'ZEPHIR_RETURN_CALL_CE_STATIC(' . $classEntry . ', "' . $callName . '", ' . $cachePointer . ');'
                    );
                } else {
                    $compilationContext->codePrinter->output(
                        'ZEPHIR_CALL_CE_STATIC(' . $symbol . ', ' . $classEntry . ', "' . $callName . '", ' . $cachePointer . ');'
                    );
                }
            } else {
                $compilationContext->codePrinter->output(
                    'ZEPHIR_CALL_CE_STATIC(NULL, ' . $classEntry . ', "' . $callName . '", ' . $cachePointer . ');'
                );
            }
        } else {
            if ($isExpecting) {
                if ('return_value' == $symbolVariable->getName()) {
                    $compilationContext->codePrinter->output(
                        'ZEPHIR_RETURN_CALL_CE_STATIC(' . $classEntry . ', "' . $callName . '", ' . $cachePointer . ', ' . implode(
                            ', ',
                            $params
                        ) . ');'
                    );
                } else {
                    $compilationContext->codePrinter->output(
                        'ZEPHIR_CALL_CE_STATIC(' . $symbol . ', ' . $classEntry . ', "' . $callName . '", ' . $cachePointer . ', ' . implode(
                            ', ',
                            $params
                        ) . ');'
                    );
                }
            } else {
                $compilationContext->codePrinter->output(
                    'ZEPHIR_CALL_CE_STATIC(NULL, ' . $classEntry . ', "' . $callName . '", ' . $cachePointer . ', ' . implode(
                        ', ',
                        $params
                    ) . ');'
                );
            }
        }

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

        $this->emitDynamicMethodCall(
            'CE_STATIC_ZVAL',
            [$classEntry],
            $this->methodNameVariable($expression, $compilationContext),
            $params,
            $symbolVariable,
            $isExpecting,
            $compilationContext
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
        if (in_array($className, ['self', 'static'], true) && $compilationContext->classDefinition->isClosure()) {
            throw new CompilerException(
                sprintf('Cannot call %s::{%s}() inside a closure', $className, $expression['name']),
                $expression
            );
        }

        $compilationContext->symbolTable->mustGrownStack(true);

        if ($mustInit) {
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->trackVariant($compilationContext);
        }

        $methodNameVariable = $this->methodNameVariable($expression, $compilationContext);

        switch ($className) {
            case 'self':
                $macro            = 'SELF_ZVAL';
                $leadingArguments = [$classDefinition->getClassEntry($compilationContext)];
                break;

            case 'parent':
                $macro            = 'PARENT_ZVAL';
                $leadingArguments = [$currentClassDefinition->getClassEntry($compilationContext), 'getThis()'];
                break;

            case 'static':
                $macro            = 'STATIC_ZVAL';
                $leadingArguments = [];
                break;

            default:
                $macro            = 'CE_STATIC_ZVAL';
                $leadingArguments = [$this->classEntryOf($classDefinition, $compilationContext)];
                break;
        }

        $params = [];
        if (!empty($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        }

        $this->emitDynamicMethodCall(
            $macro,
            $leadingArguments,
            $methodNameVariable,
            $params,
            $symbolVariable,
            $isExpecting,
            $compilationContext
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
        CompilationContext $compilationContext
    ): void {
        $backend   = $compilationContext->backend;
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
     * @param Method             $method
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
        Method $method
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

        /**
         * Check if the  method call can have an inline cache.
         */
        $methodCache  = $compilationContext->cacheManager->getStaticMethodCache();
        $cachePointer = $methodCache->get($compilationContext, $method ?? null);

        $params = [];
        if (isset($expression['parameters']) && count($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        }

        if (!count($params)) {
            if ($isExpecting) {
                if ('return_value' == $symbolVariable->getName()) {
                    $codePrinter->output(
                        'ZEPHIR_RETURN_CALL_PARENT(' . $classCe . ', getThis(), "' . $callName . '", ' . $cachePointer . ');'
                    );
                } else {
                    $codePrinter->output(
                        'ZEPHIR_CALL_PARENT(' . $compilationContext->backend->getVariableCode($symbolVariable)
                        . ', ' . $classCe . ', getThis(), "' . $callName . '", ' . $cachePointer . ');'
                    );
                }
            } else {
                $codePrinter->output(
                    'ZEPHIR_CALL_PARENT(NULL, ' . $classCe . ', getThis(), "' . $callName . '", ' . $cachePointer . ');'
                );
            }
        } else {
            if ($isExpecting) {
                if ('return_value' == $symbolVariable->getName()) {
                    $codePrinter->output(
                        'ZEPHIR_RETURN_CALL_PARENT(' . $classCe . ', getThis(), "' . $callName . '", ' . $cachePointer . ', ' . implode(
                            ', ',
                            $params
                        ) . ');'
                    );
                } else {
                    $codePrinter->output(
                        'ZEPHIR_CALL_PARENT(' . $compilationContext->backend->getVariableCode($symbolVariable)
                        . ', ' . $classCe . ', getThis(), "' . $callName . '", ' . $cachePointer . ', ' . implode(
                            ', ',
                            $params
                        ) . ');'
                    );
                }
            } else {
                $codePrinter->output(
                    'ZEPHIR_CALL_PARENT(NULL, ' . $classCe . ', getThis(), "' . $callName . '", ' . $cachePointer . ', ' . implode(
                        ', ',
                        $params
                    ) . ');'
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
     * Fetches the class of `{className}::...()` into a temporary and resolves
     * the arguments, in PHP's order: the class comes first, and a class that
     * cannot be fetched stops the call before any argument is evaluated.
     *
     * @return array{0: list<string>, 1: string}
     *
     * @see https://github.com/zephir-lang/zephir/issues/2723
     */
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
}
