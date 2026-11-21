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
use Zephir\Call;
use Zephir\Class\Entry;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\MethodCall;
use Zephir\Operators\AbstractOperator;
use Zephir\Variable\Variable;

use function count;
use function in_array;
use function is_array;
use function ltrim;
use function sprintf;

/**
 * Creates a new instance of a class
 */
class NewInstanceOperator extends AbstractOperator
{
    /**
     * Argument expressions PHP evaluates without a side effect.
     */
    private const PURE_ARGUMENT_TYPES = [
        'null',
        'int',
        'integer',
        'long',
        'double',
        'bool',
        'string',
        'istring',
        'char',
        'empty-array',
        'variable',
    ];

    protected bool $literalOnly = false;

    /**
     * Creates a new instance.
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
        $targetVariable    = $this->getExpectedNonLiteral($compilationContext, $expression, false);
        if (!$targetVariable->isVariable()) {
            throw new CompilerException('Objects can only be instantiated into dynamic variables', $expression);
        }

        if ($targetVariable->isLocalOnly()) {
            throw new CompilerException('Cannot use non-heap variable to store new instance', $expression);
        }

        if ('return_value' !== $targetVariable->getName()) {
            if ($targetVariable->hasDifferentDynamicType(['unknown', 'undefined', 'object', 'null'])) {
                $compilationContext->logger->warning(
                    'Possible attempt to use non-object in "new" operator',
                    ['non-valid-new', $expression]
                );
            }
        }

        $symbolVariable = $this->getConstructionVariable($targetVariable, $compilationContext, $expression);

        /**
         * Mark variables as dynamic objects
         */
        $symbolVariable->setDynamicTypes('object');

        $dynamic = false;
        $isLateStaticBinding = false;
        if (
            empty($expression['dynamic'])
            && in_array($expression['class'], ['self', 'static', 'parent'], true)
            && null === $compilationContext->classScope($expression['class'], $expression)
        ) {
            return new CompiledExpression('null', null, $expression);
        }

        if ('self' == $expression['class'] || 'static' == $expression['class']) {
            $className = $compilationContext->lexicalClassDefinition()->getCompleteName();
            $isLateStaticBinding = 'static' == $expression['class'];
        } elseif ('parent' === $expression['class'] && !$expression['dynamic']) {
            $className = $compilationContext->parentClassName($expression);
        } else {
            $className = $expression['class'];
            $dynamic   = $expression['dynamic'];
            if (!$dynamic) {
                $className = $compilationContext->getFullName($expression['class']);
            }
        }

        if (!$className) {
            throw new CompilerException('A class name is required to instantiate the object', $expression);
        }

        /**
         * Traits cannot be instantiated (as in PHP); this also covers
         * `new self()` inside a trait body, where `self` is the trait itself.
         */
        if (!$dynamic && $compilationContext->compiler->isTrait($className)) {
            throw new CompilerException(
                sprintf('Cannot instantiate trait "%s"', $className),
                $expression
            );
        }

        /**
         * stdclass doesn't have constructors.
         */
        $lowerClassName = strtolower($className);
        $isStdClass     = 'stdclass' === $lowerClassName || '\stdclass' === $lowerClassName;

        if ($isStdClass) {
            if (isset($expression['parameters']) && count($expression['parameters']) > 0) {
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
                /**
                 * `new static()` must resolve to the *called* class (late
                 * static binding), not the lexical class. The lexical class
                 * is still recorded for type inference because all LSB
                 * classes are subclasses of it — see
                 * https://github.com/zephir-lang/zephir/issues/2324.
                 */
                $classEntry = $isLateStaticBinding
                    ? $compilationContext->lateStaticClassEntry()
                    : $classDefinition->getClassEntry($compilationContext);

                $compilationContext->backend->initObject(
                    $symbolVariable,
                    $classEntry,
                    $compilationContext
                );
                $symbolVariable->setClassTypes($className);
                $symbolVariable->setAssociatedClass($classDefinition);
            } else {
                /**
                 * Classes outside the extension
                 */
                if ($dynamic) {
                    $classNameVariable = $compilationContext->symbolTable->getVariableForRead(
                        $className,
                        $compilationContext,
                        $expression
                    );
                    if ($classNameVariable->isNotVariableAndString()) {
                        throw new CompilerException(
                            'Only dynamic/string variables can be used in new operator. ' . $classNameVariable->getName(
                            ),
                            $expression
                        );
                    }

                    /**
                     * Use a safe string version of the variable to avoid segfaults
                     */
                    $compilationContext->headersManager->add('kernel/object');
                    $safeSymbolVariable = $compilationContext->symbolTable->getTempVariable(
                        'variable',
                        $compilationContext
                    );
                    $safeSymbolVariable->setMustInitNull(true);
                    $safeSymbolVariable->setIsInitialized(true, $compilationContext);
                    $safeSymbolVariable->increaseUses();
                    $safeSymbol      = $compilationContext->backend->getVariableCode($safeSymbolVariable);
                    $classNameSymbol = $compilationContext->backend->getVariableCode($classNameVariable);

                    $compilationContext->codePrinter->output(
                        'zephir_fetch_safe_class(' . $safeSymbol . ', ' . $classNameSymbol . ');'
                    );
                    $classNameToFetch = 'Z_STRVAL_P(' . $safeSymbol . '), Z_STRLEN_P(' . $safeSymbol . ')';
                    $zendClassEntry   = $compilationContext->cacheManager->getClassEntryCache()->get(
                        $classNameToFetch,
                        true,
                        $compilationContext
                    );
                    $classEntry       = $zendClassEntry->getName();
                    $this->exitIfClassNotFound($classEntry, $compilationContext);
                } else {
                    if (!class_exists($className, false)) {
                        $compilationContext->logger->warning(
                            'Class "' . $className . '" does not exist at compile time',
                            ['nonexistent-class', $expression]
                        );
                        $classNameToFetch = 'SL("' . Entry::escape($className) . '")';

                        $zendClassEntry = $compilationContext->cacheManager->getClassEntryCache()->get(
                            $classNameToFetch,
                            false,
                            $compilationContext
                        );
                        $classEntry     = $zendClassEntry->getName();
                        $this->exitIfClassNotFound($classEntry, $compilationContext);
                    } else {
                        $reflectionClass = new ReflectionClass($className);
                        if ($reflectionClass->isInterface()) {
                            throw new CompilerException('Interfaces cannot be instantiated', $expression);
                        }

                        if (method_exists($reflectionClass, 'isTrait') && $reflectionClass->isTrait()) {
                            throw new CompilerException('Traits cannot be instantiated', $expression);
                        }

                        /**
                         * The resolved name of `parent` is qualified, so Entry
                         * does not resolve it again against the namespace.
                         */
                        $entryName  = 'parent' === $expression['class']
                            ? '\\' . ltrim($className, '\\')
                            : $expression['class'];
                        $classEntry = (new Entry($entryName, $compilationContext))->get();
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
            return $this->assignConstructed($symbolVariable, $targetVariable, $compilationContext, $expression);
        }

        /**
         * `new` honours the constructor's visibility exactly like PHP's `new`
         * operator, with PHP's message and without the `__call()` fallback a
         * method call would take, and stops where object_init_ex() refused an
         * abstract class. Only a concrete class with a constructor known to be
         * public (or absent) skips the check: a dynamic class is resolved at
         * runtime, and a class outside the extension may not exist at compile
         * time. A failure reaches the `catch` of an enclosing `try`.
         *
         * @see https://github.com/zephir-lang/zephir/issues/882
         * @see https://github.com/zephir-lang/zephir/issues/2734
         * @see https://github.com/zephir-lang/zephir/issues/2802
         */
        if ($dynamic || $this->needsConstructorCheck($className, $isLateStaticBinding, $compilationContext)) {
            $statusCall = new Call();
            $statusCall->addCallStatusFlag($compilationContext);

            $symbolCode = $compilationContext->backend->getVariableCode($symbolVariable);
            $codePrinter->output('ZEPHIR_LAST_CALL_STATUS = zephir_check_constructor_access(' . $symbolCode . ');');
            $statusCall->addCallStatusOrJump($compilationContext);
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
                'variable'   => ['type' => 'variable', 'value' => $symbolVariable->getRealName()],
                'name'       => '__construct',
                'parameters' => $expression['parameters'],
                'call-type'  => MethodCall::CALL_NORMAL,
                'file'       => $expression['file'],
                'line'       => $expression['line'],
                'char'       => $expression['char'],
                'check'      => $callConstructor,
            ]);
        } else {
            $callExpr = new Expression([
                'variable'  => ['type' => 'variable', 'value' => $symbolVariable->getRealName()],
                'name'      => '__construct',
                'call-type' => MethodCall::CALL_NORMAL,
                'file'      => $expression['file'],
                'line'      => $expression['line'],
                'char'      => $expression['char'],
                'check'     => $callConstructor,
            ]);
        }

        /**
         * If we are certain that there is a constructor we call it, otherwise we checked it at runtime.
         */
        if ($callConstructor) {
            $this->callConstructor($callExpr, $symbolVariable, $compilationContext);
        } else {
            $symbolCode = $compilationContext->backend->getVariableCode($symbolVariable);

            $compilationContext->headersManager->add('kernel/fcall');
            $compilationContext->codePrinter->output('if (zephir_has_constructor(' . $symbolCode . ')) {');
            $codePrinter->increaseLevel();
            $this->callConstructor($callExpr, $symbolVariable, $compilationContext);
            $codePrinter->decreaseLevel();

            /**
             * Without a constructor PHP still evaluates the arguments, in a
             * call to a function that does nothing. Only an argument whose
             * evaluation can be observed is compiled again here.
             */
            $arguments = $this->argumentsWithEffects($expression['parameters'] ?? []);
            if ([] !== $arguments) {
                $codePrinter->output('} else {');
                $codePrinter->increaseLevel();
                $codePrinter->output('zephir_constructor_start(' . $symbolCode . ');');
                foreach ($arguments as $argument) {
                    $argumentExpression = new Expression($argument);
                    $argumentExpression->setReadOnly(true);
                    $argumentExpression->compile($compilationContext);
                }
                $codePrinter->output('zephir_constructor_finish(' . $symbolCode . ');');
                $codePrinter->decreaseLevel();
            }

            $codePrinter->output('}');
            $codePrinter->outputBlankLine();
        }

        return $this->assignConstructed($symbolVariable, $targetVariable, $compilationContext, $expression);
    }

    /**
     * A class that does not exist at runtime throws PHP's Error, which leaves
     * the method or reaches the `catch` of an enclosing `try`.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2802
     */
    private function exitIfClassNotFound(string $classEntry, CompilationContext $compilationContext): void
    {
        $compilationContext->codePrinter->output('if (!' . $classEntry . ') {');
        $compilationContext->codePrinter->increaseLevel();
        $compilationContext->emitExceptionExit();
        $compilationContext->codePrinter->decreaseLevel();
        $compilationContext->codePrinter->output('}');
    }

    /**
     * The variable the object is built in.
     *
     * PHP assigns the result of `new` only once the constructor has returned:
     * until then a variable keeps its old value, so a refused or throwing
     * constructor leaves it as it was, and its old object is released after
     * the new one is constructed. The object is built in place only where
     * nothing can see it before that: a temporary, or the return value
     * outside a `try`, which a failure discards.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2802
     */
    private function getConstructionVariable(
        Variable $targetVariable,
        CompilationContext $compilationContext,
        array $expression
    ): Variable {
        if ($targetVariable->isTemporal()) {
            return $targetVariable;
        }

        if ('return_value' === $targetVariable->getName() && !$compilationContext->insideTryCatch) {
            $targetVariable->initVariant($compilationContext);

            return $targetVariable;
        }

        return $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
    }

    /**
     * Moves the constructed object into the variable it is assigned to, the
     * way a `let` copies a variable, and drops the temporary's reference at
     * once, so the object is released when the variable lets it go. A `return`
     * copies the temporary itself.
     */
    private function assignConstructed(
        Variable $symbolVariable,
        Variable $targetVariable,
        CompilationContext $compilationContext,
        array $expression
    ): CompiledExpression {
        if ($symbolVariable === $targetVariable || 'return_value' === $targetVariable->getName()) {
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $targetVariable->setMustInitNull(true);
        $compilationContext->symbolTable->mustGrownStack(true);
        $targetVariable->setDynamicTypes('object');
        $targetVariable->setClassTypes($symbolVariable->getClassTypes());
        $associatedClass = $symbolVariable->getAssociatedClass();
        if (null !== $associatedClass) {
            $targetVariable->setAssociatedClass($associatedClass);
        }
        $targetVariable->increaseVariantIfNull();
        $targetVariable->setIsInitialized(true, $compilationContext);

        $compilationContext->backend->copyOnWrite($targetVariable, $symbolVariable, $compilationContext);
        $compilationContext->codePrinter->output(
            'ZEPHIR_INIT_NVAR(' . $compilationContext->backend->getVariableCode($symbolVariable) . ');'
        );
        $symbolVariable->setIdle(true);

        return new CompiledExpression('variable', $targetVariable->getRealName(), $expression);
    }

    /**
     * Calls the constructor with the object marked as destructed until it
     * returns: an exception from the constructor or from one of its arguments
     * leaves the mark, so the object is freed without its destructor, as
     * PHP's cleanup of an unfinished `new` does.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2802
     */
    private function callConstructor(
        Expression $callExpr,
        Variable $symbolVariable,
        CompilationContext $compilationContext
    ): void {
        $symbolCode = $compilationContext->backend->getVariableCode($symbolVariable);

        $compilationContext->headersManager->add('kernel/fcall');
        $compilationContext->codePrinter->output('zephir_constructor_start(' . $symbolCode . ');');

        $methodCall = new MethodCall();
        $callExpr->setExpectReturn(false);
        $methodCall->compile($callExpr, $compilationContext);

        $compilationContext->codePrinter->output('zephir_constructor_finish(' . $symbolCode . ');');
    }

    /**
     * The arguments whose evaluation can be observed: anything but a literal
     * or a variable, which PHP reads without a side effect.
     *
     * @return list<array>
     */
    private function argumentsWithEffects(array $parameters): array
    {
        $arguments = [];
        foreach ($parameters as $parameter) {
            $argument = $parameter['parameter'] ?? null;
            if (!is_array($argument) || in_array($argument['type'], self::PURE_ARGUMENT_TYPES, true)) {
                continue;
            }

            $arguments[] = $argument;
        }

        return $arguments;
    }

    /**
     * Whether `new` must check at runtime that the object can be
     * constructed: PHP refuses a constructor the calling code cannot see, and
     * object_init_ex() refuses an abstract class. A concrete class with a
     * constructor known to be public, or none, skips it.
     *
     * `new static()` always checks: the called class may be abstract, or
     * narrow the constructor it inherits, and PHP checks the class actually
     * instantiated.
     */
    private function needsConstructorCheck(
        string $className,
        bool $isLateStaticBinding,
        CompilationContext $compilationContext
    ): bool {
        if ($isLateStaticBinding) {
            return true;
        }

        $compiler = $compilationContext->compiler;
        if ($compiler->isClass($className)) {
            $classDefinition = $compiler->getClassDefinition($className);
            $constructor     = $classDefinition->getMethod('__construct');

            return $classDefinition->isAbstract()
                || (null !== $constructor && ($constructor->isPrivate() || $constructor->isProtected()));
        }

        if (!class_exists($className, false)) {
            return true;
        }

        $reflectionClass = new ReflectionClass($className);
        $constructor     = $reflectionClass->getConstructor();

        return $reflectionClass->isAbstract() || (null !== $constructor && !$constructor->isPublic());
    }
}
