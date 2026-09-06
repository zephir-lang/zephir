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

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Name;
use Zephir\Traits\WriteContextSlotTrait;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

/**
 * Resolves expressions that read properties with a dynamic variable as property
 */
class PropertyDynamicAccess
{
    use WriteContextSlotTrait;

    protected bool      $expecting         = true;
    protected ?Variable $expectingVariable = null;
    protected bool      $noisy             = true;
    protected bool      $readOnly          = false;
    protected bool      $writeThrough      = false;

    /**
     * Resolves the access to a property in an object.
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $propertyAccess = $expression;

        $expr         = new Expression($propertyAccess['left']);
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

        $propertyVariable = match ($propertyAccess['right']['type']) {
            Types::T_VARIABLE => $compilationContext->symbolTable->getVariableForRead(
                $propertyAccess['right']['value'],
                $compilationContext,
                $expression
            ),
            Types::T_STRING   => null,
            default           => throw new CompilerException(
                'Variable type: ' . $propertyAccess['right']['type'] . ' cannot be used as object',
                $propertyAccess['left']
            ),
        };

        $property = $propertyVariable ?: Name::addSlashes($expression['right']['value']);

        /**
         * A write context takes the slot the property lives in, and the engine
         * reaches it through the same handler whether the name came from a
         * literal or from a variable.
         *
         * @see \Zephir\Expression\PropertyAccess::setWriteThrough()
         */
        if ($this->writeThrough) {
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

        /**
         * Resolves the symbol that expects the value
         */
        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
                if ('return_value' != $symbolVariable->getName()) {
                    $symbolVariable->observeVariant($compilationContext);
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve(
                        'variable',
                        $compilationContext
                    );
                }
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
        if ($symbolVariable && !$symbolVariable->isVariable()) {
            throw new CompilerException(
                'Cannot use variable: '
                . $symbolVariable->getType()
                . ' to assign property value',
                $expression
            );
        }

        /*
         * At this point, we don't know the exact dynamic type fetched from the property
         */
        $symbolVariable->setDynamicTypes('undefined');

        $compilationContext->headersManager->add('kernel/object');

        $compilationContext->backend->fetchProperty(
            $symbolVariable,
            $variableVariable,
            $property,
            false,
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
     * @see https://github.com/zephir-lang/zephir/issues/2691
     */
    public function setWriteThrough(bool $writeThrough): void
    {
        $this->writeThrough = $writeThrough;
    }
}
