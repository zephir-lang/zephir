<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Expression;

use Zephir\Variable;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\CompilerException;
use Zephir\Expression;

/**
 * PropertyAccess
 *
 * Resolves expressions that read properties
 */
class PropertyAccess
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
     * @return \CompiledExpression
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $codePrinter = $compilationContext->codePrinter;

        $propertyAccess = $expression;

        $expr = new Expression($propertyAccess['left']);
        $expr->setReadOnly(true);
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

        $property = $propertyAccess['right']['value'];

        $propertyDefinition = null;
        $classDefinition = null;
        $currentClassDefinition = $compilationContext->classDefinition;

        /**
         * If the property is accessed on 'this', we check if the method does exist
         */
        if ($variableVariable->getRealName() == 'this') {
            $classDefinition = $currentClassDefinition;
            if (!$classDefinition->hasProperty($property)) {
                throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a property called: '" . $property . "'", $expression);
            }

            $propertyDefinition = $classDefinition->getProperty($property);
        } else {
            /**
             * If we know the class related to a variable we could check if the property
             * is defined on that class
             */
            if ($variableVariable->hasAnyDynamicType('object')) {
                $classType = current($variableVariable->getClassTypes());
                $compiler = $compilationContext->compiler;

                if ($compiler->isClass($classType)) {
                    $classDefinition = $compiler->getClassDefinition($classType);
                    if (!$classDefinition) {
                        throw new CompilerException("Cannot locate class definition for class: " . $classType, $expression);
                    }

                    if (!$classDefinition->hasProperty($property)) {
                        throw new CompilerException("Class '" . $classType . "' does not have a property called: '" . $property . "'", $expression);
                    }

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
                throw new CompilerException("Attempt to access static property '" . $property . "' as non static", $expression);
            }

            if (!$propertyDefinition->isPublic()) {
                /**
                 * Protected variables only can be read in the class context
                 * where they were declared
                 */
                if ($classDefinition == $currentClassDefinition) {
                    if ($propertyDefinition->isPrivate()) {
                        $declarationDefinition = $propertyDefinition->getClassDefinition();
                        if ($declarationDefinition != $currentClassDefinition) {
                            throw new CompilerException("Attempt to access private property '" . $property . "' outside of its declared class context: '" . $declarationDefinition->getCompleteName() . "'", $expression);
                        }
                    }
                } else {
                    if ($propertyDefinition->isProtected()) {
                    } else {
                        if ($propertyDefinition->isPrivate()) {
                            $declarationDefinition = $propertyDefinition->getClassDefinition();
                            if ($declarationDefinition != $currentClassDefinition) {
                                throw new CompilerException("Attempt to access private property '" . $property . "' outside of its declared class context: '" . $declarationDefinition->getCompleteName() . "'", $expression);
                            }
                        }
                    }
                }
            }
        }

        /**
         * Resolves the symbol that expects the value
         */
        $readOnly = false;
        if ($classDefinition == $currentClassDefinition && $this->_readOnly) {
            if ($this->_expecting) {
                if ($this->_expectingVariable) {
                    $symbolVariable = $this->_expectingVariable;

                    /**
                     * If a variable is assigned once in the method, we try to promote it
                     * to a read only variable
                     */
                    if ($symbolVariable->getName() != 'return_value') {
                        $line = $compilationContext->symbolTable->getLastCallLine();
                        if ($line === false || ($line > 0 && $line < $expression['line'])) {
                            $numberMutations = $compilationContext->symbolTable->getExpectedMutations($symbolVariable->getName());
                            if ($numberMutations == 1) {
                                if ($symbolVariable->getNumberMutations() == $numberMutations) {
                                    $symbolVariable->setMemoryTracked(false);
                                    $readOnly = true;
                                }
                            }
                        }
                    }

                    /**
                     * Variable is not read only or it wasn't promoted
                     */
                    if (!$readOnly) {
                        if ($symbolVariable->getName() != 'return_value') {
                            $symbolVariable->observeVariant($compilationContext);
                            $this->_readOnly = false;
                        } else {
                            $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
                        }
                    }

                    $this->_readOnly = false;
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
            }
        } else {
            if ($this->_expecting) {
                if ($this->_expectingVariable) {
                    $symbolVariable = $this->_expectingVariable;

                    /**
                     * If a variable is assigned once in the method, we try to promote it
                     * to a read only variable
                     */
                    if ($symbolVariable->getName() != 'return_value') {
                        $line = $compilationContext->symbolTable->getLastCallLine();
                        if ($line === false || ($line > 0 && $line < $expression['line'])) {
                            $numberMutations = $compilationContext->symbolTable->getExpectedMutations($symbolVariable->getName());
                            if ($numberMutations == 1) {
                                if ($symbolVariable->getNumberMutations() == $numberMutations) {
                                    $symbolVariable->setMemoryTracked(false);
                                    $readOnly = true;
                                }
                            }
                        }
                    }

                    /**
                     * Variable is not read only or it wasn't promoted
                     */
                    if (!$readOnly) {
                        if ($symbolVariable->getName() != 'return_value') {
                            $symbolVariable->observeVariant($compilationContext);
                            $this->_readOnly = false;
                        } else {
                            $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext, $expression);
                        }
                    }
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
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Cannot use variable: " . $symbolVariable->getType() . " to assign property value", $expression);
        }

        /**
         * At this point, we don't know the exact dynamic type fetched from the property
         */
        $symbolVariable->setDynamicTypes('undefined');

        $compilationContext->headersManager->add('kernel/object');
        if ($classDefinition == $currentClassDefinition) {
            if ($this->_readOnly || $readOnly) {
                $codePrinter->output($symbolVariable->getName() . ' = zephir_fetch_nproperty_this(' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');
            } else {
                $codePrinter->output('zephir_read_property_this(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');
            }
        } else {
            $codePrinter->output('zephir_read_property(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');
        }

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
