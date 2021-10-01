<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Variable;

use function Zephir\add_slashes;

/**
 * Zephir\Expression\PropertyDynamicAccess.
 *
 * Resolves expressions that read properties with a dynamic variable as property
 */
class PropertyDynamicAccess
{
    protected $expecting = true;

    protected $readOnly = false;

    protected $expectingVariable;

    /** @var bool */
    protected $noisy = true;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     *
     * @param bool     $expecting
     * @param Variable $expectingVariable
     */
    public function setExpectReturn($expecting, Variable $expectingVariable = null)
    {
        $this->expecting = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     *
     * @param bool $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->readOnly = $readOnly;
    }

    /**
     * Sets whether the expression must be resolved in "noisy" mode.
     *
     * @param bool $noisy
     */
    public function setNoisy($noisy)
    {
        $this->noisy = $noisy;
    }

    /**
     * Resolves the access to a property in an object.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $propertyAccess = $expression;

        $expr = new Expression($propertyAccess['left']);
        $exprVariable = $expr->compile($compilationContext);

        switch ($exprVariable->getType()) {
            case 'variable':
                $variableVariable = $compilationContext->symbolTable->getVariableForRead($exprVariable->getCode(), $compilationContext, $expression);
                switch ($variableVariable->getType()) {
                    case 'variable':
                        break;
                    default:
                        throw new CompilerException('Variable type: '.$variableVariable->getType().' cannot be used as object', $propertyAccess['left']);
                }
                break;
            default:
                throw new CompilerException('Cannot use expression: '.$exprVariable->getType().' as an object', $propertyAccess['left']);
        }

        switch ($propertyAccess['right']['type']) {
            case 'variable':
                $propertyVariable = $compilationContext->symbolTable->getVariableForRead($propertyAccess['right']['value'], $compilationContext, $expression);
                break;
            case 'string':
                $propertyVariable = null;
                break;
            default:
                throw new CompilerException('Variable type: '.$propertyAccess['right']['type'].' cannot be used as object', $propertyAccess['left']);
        }

        /*
         * Resolves the symbol that expects the value
         */
        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
                if ('return_value' != $symbolVariable->getName()) {
                    $symbolVariable->observeVariant($compilationContext);
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
            }
        }

        /*
         * Variable that receives a property value must be polymorphic
         */
        if ($symbolVariable && !$symbolVariable->isVariable()) {
            throw new CompilerException('Cannot use variable: '.$symbolVariable->getType().' to assign property value', $expression);
        }

        /*
         * At this point, we don't know the exact dynamic type fetched from the property
         */
        $symbolVariable->setDynamicTypes('undefined');

        $compilationContext->headersManager->add('kernel/object');

        $property = $propertyVariable ?: add_slashes($expression['right']['value']);
        $compilationContext->backend->fetchProperty($symbolVariable, $variableVariable, $property, false, $compilationContext);

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
