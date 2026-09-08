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
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Traits\VariablesTrait;
use Zephir\Traits\WriteContextSlotTrait;
use Zephir\Variable\Variable;

use function current;

/**
 * Resolves expressions that read properties
 */
class PropertyAccess
{
    use VariablesTrait;
    use WriteContextSlotTrait;

    protected bool      $expecting = true;
    protected ?Variable $expectingVariable;
    protected bool      $noisy        = true;
    protected bool      $readOnly     = false;
    protected bool      $writeThrough = false;

    /**
     * Resolves the access to a property in an object.
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile($expression, CompilationContext $compilationContext): CompiledExpression
    {
        $propertyAccess = $expression;

        $expr = new Expression($propertyAccess['left']);
        $expr->setReadOnly(true);
        $exprVariable = $expr->compile($compilationContext);

        switch ($exprVariable->getType()) {
            case 'variable':
                $variableVariable = $compilationContext->symbolTable->getVariableForRead(
                    $exprVariable->getCode(),
                    $compilationContext,
                    $expression
                );
                switch ($variableVariable->getType()) {
                    case 'variable':
                        break;

                    default:
                        throw new CompilerException(
                            'Variable type: ' . $variableVariable->getType() . ' cannot be used as object',
                            $propertyAccess['left']
                        );
                }
                break;

            default:
                throw new CompilerException(
                    'Cannot use expression: ' . $exprVariable->getType() . ' as an object',
                    $propertyAccess['left']
                );
        }

        $property = $propertyAccess['right']['value'];

        $propertyDefinition     = null;
        $classDefinition        = null;
        $currentClassDefinition = $compilationContext->classDefinition;

        /**
         * If the property is accessed on 'this', we check if the method does exist
         */
        if ('this' == $variableVariable->getRealName()) {
            $classDefinition = $currentClassDefinition;

            /**
             * If this is a closure class with an enclosing class, resolve
             * property access on `this` against the enclosing class definition.
             * @see https://github.com/zephir-lang/zephir/issues/2497
             */
            $enclosingClassDefinition = $classDefinition->getEnclosingClassDefinition();
            if ($enclosingClassDefinition !== null) {
                $classDefinition = $enclosingClassDefinition;
            }

            $this->checkClassHasProperty(
                $classDefinition,
                $property,
                $expression
            );

            $propertyDefinition = $classDefinition->getProperty($property);
        } else {
            /**
             * If we know the class related to a variable we could check if the property
             * is defined on that class
             */
            if ($variableVariable->hasAnyDynamicType('object')) {
                $classType = current($variableVariable->getClassTypes());
                $compiler  = $compilationContext->compiler;

                if ($classType !== false && $compiler->isClass($classType)) {
                    $classDefinition = $compiler->getClassDefinition($classType);
                    if (!$classDefinition) {
                        throw new CompilerException(
                            'Cannot locate class definition for class: ' . $classType,
                            $expression
                        );
                    }

                    $this->checkClassHasProperty(
                        $classDefinition,
                        $property,
                        $expression,
                        $classType
                    );

                    $propertyDefinition = $classDefinition->getProperty($property);
                }
            }
        }

        /**
         * Having a proper propertyDefinition we can check if the property is readable
         * according to its modifiers
         */
        if ($propertyDefinition) {
            if ($propertyDefinition->isStatic()) {
                throw new CompilerException(
                    "Attempt to access static property '" . $property . "' as non static",
                    $expression
                );
            }

            if (!$propertyDefinition->isPublic()) {
                /**
                 * Protected variables only can be read in the class context
                 * where they were declared
                 */
                if ($classDefinition == $currentClassDefinition) {
                    if ($propertyDefinition->isPrivate()) {
                        $declarationDefinition = $propertyDefinition->getClassDefinition();
                        if ($declarationDefinition !== $currentClassDefinition) {
                            throw new CompilerException(
                                "Attempt to access private property '"
                                . $property
                                . "' outside of its declared class context: '"
                                . $declarationDefinition->getCompleteName()
                                . "'",
                                $expression
                            );
                        }
                    }
                } else {
                    if (!$propertyDefinition->isProtected() && $propertyDefinition->isPrivate()) {
                        $declarationDefinition = $propertyDefinition->getClassDefinition();
                        if ($declarationDefinition !== $currentClassDefinition) {
                            throw new CompilerException(
                                "Attempt to access private property '"
                                . $property
                                . "' outside of its declared class context: '"
                                . $declarationDefinition->getCompleteName()
                                . "'",
                                $expression
                            );
                        }
                    }
                }
            }
        }

        /**
         * A write context does not read the property at all, it takes the slot
         * the property lives in. Everything a by-reference argument needs
         * follows from that, and none of the symbol resolution below applies:
         * the target is a pointer, not a value the caller could have promoted
         * to read-only or asked to be put in a variable of its own.
         */
        if ($this->writeThrough) {
            return $this->fetchWritableSlot($variableVariable, $property, $expression, $compilationContext);
        }

        /**
         * Resolves the symbol that expects the value.
         */
        $readOnly           = false;
        $makeSymbolVariable = false;
        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;

                /**
                 * If a variable is assigned once in the method, we try to promote it
                 * to a read only variable
                 */
                if ('return_value' != $symbolVariable->getName()) {
                    $line = $compilationContext->symbolTable->getLastCallLine();
                    if (false === $line || ($line > 0 && $line < $expression['line'])) {
                        $numberMutations = $compilationContext->symbolTable->getExpectedMutations(
                            $symbolVariable->getName()
                        );
                        if (1 == $numberMutations) {
                            if ($symbolVariable->getNumberMutations() == $numberMutations) {
                                $symbolVariable->setMemoryTracked(false);
                                $readOnly = true;
                            }
                        }
                    }
                }

                /**
                 * Variable is not read only, or it wasn't promoted
                 */
                if (!$readOnly) {
                    if ('return_value' != $symbolVariable->getName()) {
                        $symbolVariable->observeVariant($compilationContext);
                    } else {
                        $makeSymbolVariable = true;
                    }
                }

                $this->readOnly = false;
            } else {
                $makeSymbolVariable = true;
            }
        } else {
            $makeSymbolVariable = true;
        }

        $readOnly = $this->readOnly || $readOnly;

        if ($makeSymbolVariable) {
            if ($readOnly) {
                $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable(
                    'variable',
                    $compilationContext
                );
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve(
                    'variable',
                    $compilationContext
                );
            }
        }

        /**
         * Variable that receives a property value must be polymorphic
         */
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException(
                'Cannot use variable: '
                . $symbolVariable->getType()
                . ' to assign property value',
                $expression
            );
        }

        /**
         * At this point, we don't know the exact dynamic type fetched from the property
         */
        $symbolVariable->setDynamicTypes('undefined');
        $compilationContext->headersManager->add('kernel/object');
        $compilationContext->backend->fetchProperty(
            $symbolVariable,
            $variableVariable,
            $property,
            $readOnly,
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
     * Sets whether the expression must be resolved in "noisy" mode.
     */
    public function setNoisy(bool $noisy): void
    {
        $this->noisy = $noisy;
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
     * @see \Zephir\Expression\NativeArrayAccess::setWriteThrough()
     * @see https://github.com/zephir-lang/zephir/issues/2691
     */
    public function setWriteThrough(bool $writeThrough): void
    {
        $this->writeThrough = $writeThrough;
    }

    /**
     * The property's storage slot, in a temp the memory frame does not own.
     *
     * @see \Zephir\Traits\WriteContextSlotTrait
     * @see https://github.com/zephir-lang/zephir/issues/2691
     */
    private function fetchWritableSlot(
        Variable $variableVariable,
        string $property,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        [$slot, $fallback] = $this->writeContextSlot($compilationContext);

        $compilationContext->backend->fetchPropertyWrite(
            $slot,
            $variableVariable,
            $property,
            $fallback,
            $compilationContext
        );

        return new CompiledExpression('variable', $slot->getRealName(), $expression);
    }
}
