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
use Zephir\Class\Property;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Traits\VariablesTrait;
use Zephir\Traits\WriteContextSlotTrait;
use Zephir\Variable\Variable;

use function in_array;

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

    /**
     * Access a static property.
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $className = $expression['left']['value'];
        $compiler  = $compilationContext->compiler;
        $property  = $expression['right']['value'];

        /**
         * Fetch the class definition according to the class where the constant
         * is supposed to be declared
         */
        if (!in_array($className, ['self', 'static', 'parent'])) {
            $className = $compilationContext->getFullName($className);
            if ($compiler->isClass($className)) {
                $classDefinition = $compiler->getClassDefinition($className);
            } else {
                if ($compiler->isBundledClass($className)) {
                    $classDefinition = $compiler->getInternalClassDefinition($className);
                } else {
                    throw new CompilerException("Cannot locate class '" . $className . "'", $expression['left']);
                }
            }
        } else {
            if (in_array($className, ['self', 'static'])) {
                $classDefinition = $compilationContext->classDefinition;
            } else {
                if ('parent' === $className) {
                    $classDefinition = $compilationContext->classDefinition;
                    if (!$classDefinition->getExtendsClass()) {
                        throw new CompilerException(
                            'Cannot access static property "'
                            . $property
                            . '" on parent because class '
                            . $classDefinition->getCompleteName()
                            . ' does not extend any class',
                            $expression
                        );
                    } else {
                        $classDefinition = $classDefinition->getExtendsClassDefinition();
                    }
                }
            }
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

        if ($propertyDefinition->isPrivate()) {
            if ($classDefinition != $compilationContext->classDefinition) {
                throw new CompilerException(
                    "Cannot access private static property '"
                    . $classDefinition->getCompleteName()
                    . '::'
                    . $property
                    . "' out of its declaring context",
                    $expression
                );
            }
        }

        if (
            $propertyDefinition->isProtected() &&
            $classDefinition != $compilationContext->classDefinition &&
            $classDefinition != $compilationContext->classDefinition->getExtendsClassDefinition()
        ) {
            throw new CompilerException(
                "Cannot access protected static property '"
                . $classDefinition->getCompleteName()
                . '::'
                . $property
                . "' out of its declaring context",
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
                $compilationContext
            );

            return new CompiledExpression('variable', $slot->getRealName(), $expression);
        }

        /**
         * Resolves the symbol that expects the value
         */
        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
                if ('return_value' === $symbolVariable->getName()) {
                    $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                        'variable',
                        $compilationContext
                    );
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                    'variable',
                    $compilationContext
                );
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                'variable',
                $compilationContext
            );
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

        if (!$this->readOnly && 'return_value' != $symbolVariable->getName()) {
            $symbolVariable->observeVariant($compilationContext);
        }

        $compilationContext->backend->fetchStaticProperty(
            $symbolVariable,
            $classDefinition,
            $property,
            $this->readOnly,
            $compilationContext
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
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
     * @see https://github.com/zephir-lang/zephir/issues/2691
     */
    public function setWriteThrough(bool $writeThrough): void
    {
        $this->writeThrough = $writeThrough;
    }
}
