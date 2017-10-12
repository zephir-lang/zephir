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

use Zephir\ClassProperty;
use Zephir\Variable;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Compiler\CompilerException;

/**
 * StaticPropertyAccess
 *
 * Resolves class static properties
 */
class StaticPropertyAccess
{
    protected $_expecting = true;

    protected $_readOnly = false;

    protected $_expectingVariable;

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
     * Access a static property
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $className = $expression['left']['value'];
        $compiler = $compilationContext->compiler;
        $property = $expression['right']['value'];

        /**
         * Fetch the class definition according to the class where the constant
         * is supposed to be declared
         */
        if (!in_array($className, array('self', 'static', 'parent'))) {
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
            if (in_array($className, array('self', 'static'))) {
                $classDefinition = $compilationContext->classDefinition;
            } else {
                if ($className == 'parent') {
                    $classDefinition = $compilationContext->classDefinition;
                    $extendsClass = $classDefinition->getExtendsClass();
                    if (!$extendsClass) {
                        throw new CompilerException('Cannot access static property "' . $property . '" on parent because class ' . $classDefinition->getCompleteName() . ' does not extend any class', $expression);
                    } else {
                        $classDefinition = $classDefinition->getExtendsClassDefinition();
                    }
                }
            }
        }

        if (!$classDefinition->hasProperty($property)) {
            throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a property called: '" . $property . "'", $expression);
        }

        /** @var $propertyDefinition ClassProperty */
        $propertyDefinition = $classDefinition->getProperty($property);
        if (!$propertyDefinition->isStatic()) {
            throw new CompilerException("Cannot access non-static property '" . $classDefinition->getCompleteName() . '::' . $property . "'", $expression);
        }

        if ($propertyDefinition->isPrivate()) {
            if ($classDefinition != $compilationContext->classDefinition) {
                throw new CompilerException("Cannot access private static property '" . $classDefinition->getCompleteName() . '::' . $property . "' out of its declaring context", $expression);
            }
        }

        if ($propertyDefinition->isProtected()) {
            if ($classDefinition != $compilationContext->classDefinition && $classDefinition != $compilationContext->classDefinition->getExtendsClassDefinition()) {
                throw new CompilerException("Cannot access protected static property '" . $classDefinition->getCompleteName() . '::' . $property . "' out of its declaring context", $expression);
            }
        }

        /**
         * Resolves the symbol that expects the value
         */
        if ($this->_expecting) {
            if ($this->_expectingVariable) {
                $symbolVariable = $this->_expectingVariable;
                if ($symbolVariable->getName() == 'return_value') {
                    $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
        }

        /**
         * Variable that receives property accesses must be polymorphic
         */
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Cannot use variable: " . $symbolVariable->getType() . " to assign class constants", $expression);
        }

        $symbolVariable->setDynamicTypes('undefined');

        $compilationContext->headersManager->add('kernel/object');

        $readOnly = $this->_readOnly;
        if (!$readOnly) {
            if ($symbolVariable->getName() != 'return_value') {
                $symbolVariable->observeVariant($compilationContext);
            }
        }
        $compilationContext->backend->fetchStaticProperty($symbolVariable, $classDefinition, $property, $this->_readOnly, $compilationContext);

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
