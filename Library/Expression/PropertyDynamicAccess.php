<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/


namespace Zephir\Expression;

use Zephir\Variable;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Compiler\CompilerException;
use Zephir\Expression;
use Zephir\Utils;

/**
 * PropertyDynamicAccess
 *
 * Resolves expressions that read properties with a dynamic variable as property
 */
class PropertyDynamicAccess
{
    protected $_expecting = true;

    protected $_readOnly = false;

    protected $_expectingVariable;

    /**
     * @var boolean
     */
    protected $_noisy = true;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value
     *
     * @param boolean $expecting
     * @param Variable $expectingVariable
     */
    public function setExpectReturn($expecting, Variable $expectingVariable = null)
    {
        $this->_expecting = $expecting;
        $this->_expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only
     *
     * @param boolean $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->_readOnly = $readOnly;
    }

    /**
     * Sets whether the expression must be resolved in "noisy" mode
     *
     * @param boolean $noisy
     */
    public function setNoisy($noisy)
    {
        $this->_noisy = $noisy;
    }

    /**
     * Resolves the access to a property in an object
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $codePrinter = $compilationContext->codePrinter;

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
                        throw new CompilerException("Variable type: " . $variableVariable->getType() . " cannot be used as object", $propertyAccess['left']);
                }
                break;
            default:
                throw new CompilerException("Cannot use expression: " . $exprVariable->getType() . " as an object", $propertyAccess['left']);
        }

        switch ($propertyAccess['right']['type']) {
            case 'variable':
                $propertyVariable = $compilationContext->symbolTable->getVariableForRead($propertyAccess['right']['value'], $compilationContext, $expression);
                break;
            case 'string':
                $propertyVariable = null;
                break;
            default:
                throw new CompilerException("Variable type: " . $propertyAccess['right']['type'] . " cannot be used as object", $propertyAccess['left']);
        }

        /**
         * Resolves the symbol that expects the value
         */
        if ($this->_expecting) {
            if ($this->_expectingVariable) {
                $symbolVariable = $this->_expectingVariable;
                if ($symbolVariable->getName() != 'return_value') {
                    $symbolVariable->observeVariant($compilationContext);
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
            }
        }

        /**
         * Variable that receives a property value must be polymorphic
         */
        if ($symbolVariable && !$symbolVariable->isVariable()) {
            throw new CompilerException("Cannot use variable: " . $symbolVariable->getType() . " to assign property value", $expression);
        }

        /**
         * At this point, we don't know the exact dynamic type fetched from the property
         */
        $symbolVariable->setDynamicTypes('undefined');

        $compilationContext->headersManager->add('kernel/object');

        $property = $propertyVariable ? $propertyVariable : Utils::addSlashes($expression['right']['value']);
        $compilationContext->backend->fetchProperty($symbolVariable, $variableVariable, $property, false, $compilationContext, false);


        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
