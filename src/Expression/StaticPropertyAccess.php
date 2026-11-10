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

namespace Zephir\Expression;

use ReflectionException;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Property;
use Zephir\Class\Visibility;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Traits\VariablesTrait;
use Zephir\Traits\WriteContextSlotTrait;
use Zephir\Variable\Variable;

use function in_array;
use function sprintf;

/**
 * Resolves class static properties
 */
class StaticPropertyAccess
{
    use VariablesTrait;
    use WriteContextSlotTrait;

    protected bool      $expecting         = true;
    protected ?Variable $expectingVariable = null;
    protected bool      $readOnly          = false;
    protected bool      $writeThrough      = false;
    protected bool      $readWrite         = false;
    protected bool      $noisy             = true;

    /**
     * Access a static property.
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $property        = $expression['right']['value'];
        $classDefinition = $this->classDefinitionOf($expression, $compilationContext);
        if (null === $classDefinition) {
            return new CompiledExpression('null', null, $expression);
        }

        $this->checkClassHasProperty(
            $classDefinition,
            $property,
            $expression
        );

        /** @var Property $propertyDefinition */
        $propertyDefinition = $classDefinition->getProperty($property);
        $this->checkAccessNonStaticProperty(
            $propertyDefinition,
            $classDefinition,
            $property,
            $expression
        );

        /**
         * Checked against the class the code is written in, at any depth of
         * inheritance, as PHP checks it.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2734
         */
        if (!Visibility::canAccessProperty($propertyDefinition, $compilationContext)) {
            throw new CompilerException(
                sprintf(
                    "Cannot access %s static property '%s::%s' out of its declaring context",
                    $propertyDefinition->isPrivate() ? 'private' : 'protected',
                    $classDefinition->getCompleteName(),
                    $property
                ),
                $expression
            );
        }

        /**
         * A write context takes the slot the static property lives in rather
         * than a copy of it, for the same reasons an instance property does.
         *
         * @see \Zephir\Expression\PropertyAccess::setWriteThrough()
         */
        if ($this->writeThrough) {
            [$slot, $fallback] = $this->writeContextSlot($compilationContext);

            $compilationContext->backend->fetchStaticPropertyWrite(
                $slot,
                $classDefinition,
                $property,
                $fallback,
                $compilationContext,
                $this->readWrite
            );

            return new CompiledExpression('variable', $slot->getRealName(), $expression);
        }

        /**
         * Resolves the symbol that expects the value.
         *
         * A borrowed read (PH_READONLY) only goes to a temp nothing releases,
         * and an owned one to a temp the memory frame tracks. Taking the
         * owned temp from the untracked pool and observing it there handed it
         * back to that pool tracked, and the next borrowed read into it was
         * released at the end of the method: a use after free of the property.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2805
         */
        $readOnly = $this->readOnly;
        if ($this->expecting && $this->expectingVariable && 'return_value' !== $this->expectingVariable->getName()) {
            $symbolVariable = $this->expectingVariable;
            $readOnly       = false;
        } elseif ($readOnly) {
            $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext);
        }

        /**
         * Variable that receives property accesses must be polymorphic
         */
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException(
                'Cannot use variable: ' . $symbolVariable->getType() . ' to assign class constants',
                $expression
            );
        }

        $symbolVariable->setDynamicTypes('undefined');
        $compilationContext->headersManager->add('kernel/object');

        if ($symbolVariable === $this->expectingVariable) {
            $symbolVariable->observeVariant($compilationContext);
        }

        $compilationContext->backend->fetchStaticProperty(
            $symbolVariable,
            $classDefinition,
            $property,
            $readOnly,
            $compilationContext,
            $this->noisy
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * The class an `X::y` access names, or null when `self` or `parent` has no
     * class here and the code already throws PHP's Error where it is used.
     *
     * @throws CompilerException
     */
    public function classDefinitionOf(array $expression, CompilationContext $compilationContext): ?Definition
    {
        $className = $expression['left']['value'];
        $compiler  = $compilationContext->compiler;

        if (!in_array($className, ['self', 'static', 'parent'])) {
            $className = $compilationContext->getFullName($className);
            if ($compiler->isClass($className)) {
                return $compiler->getClassDefinition($className);
            }

            if ($compiler->isBundledClass($className)) {
                return $compiler->getInternalClassDefinition($className);
            }

            throw new CompilerException("Cannot locate class '" . $className . "'", $expression['left']);
        }

        if (null === $compilationContext->classScope($className, $expression)) {
            return null;
        }

        if ('parent' === $className) {
            return $compilationContext->parentClassDefinition(
                'access static property "' . $expression['right']['value'] . '"',
                $expression
            );
        }

        return $compilationContext->lexicalClassDefinition();
    }

    /**
     * Sets whether a read that fails reports it, as a read does, or stays
     * silent, as `isset` and `empty` ask.
     */
    public function setNoisy(bool $noisy): void
    {
        $this->noisy = $noisy;
    }

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     */
    public function setExpectReturn(bool $expecting, ?Variable $expectingVariable = null): void
    {
        $this->expecting         = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     */
    public function setReadOnly(bool $readOnly): void
    {
        $this->readOnly = $readOnly;
    }

    /**
     * Sets whether the caller will write through the property instead of only
     * reading it, which is what a by-reference call argument does.
     *
     * `readWrite` asks for the slot a compound assignment reads and then
     * writes, which PHP fetches read-write.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2691
     * @see https://github.com/zephir-lang/zephir/issues/2747
     */
    public function setWriteThrough(bool $writeThrough, bool $readWrite = false): void
    {
        $this->writeThrough = $writeThrough;
        $this->readWrite    = $readWrite;
    }
}
