<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

/**
 * StaticConstantAccess
 *
 * Resolves class constants
 */
class StaticConstantAccess
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
	public function setExpectReturn($expecting, Variable $expectingVariable=null)
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
	 * Access a static constant class
	 *
	 * @param array $expression
	 * @param CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{
		$compiler = $compilationContext->compiler;

		$className = $expression['left']['value'];

		/**
		 * Fetch the class definition according to the class where the constant
		 * is supposed to be declared
		 */
		if ($className != 'self' && $className != 'parent') {
			if (!$compiler->isClass($className)) {
				throw new CompilerException("Cannot locate class '" . $className . "'", $expression['left']);
			}
			$classDefinition = $compiler->getClassDefinition($className);
		} else {
			if ($className == 'self') {
				$classDefinition = $compilationContext->classDefinition;
			} else {
				if ($className == 'parent') {
					$classDefinition = $compilationContext->classDefinition;
					$extendsClass = $classDefinition->getExtendsClass();
					if (!$extendsClass) {
						throw new CompilerException('Cannot call method "' . $methodName . '" on parent because class ' .
							$classDefinition->getCompleteName() . ' does not extend any class', $expression);
					} else {
						$classDefinition = $classDefinition->getExtendsClassDefinition();
					}
				}
			}
		}

		/**
		 * Constants are resolved to its values in compile time
		 * so we need to check that they effectively do exist
		 */
		$constant = $expression['right']['value'];
		if (!$classDefinition->hasConstant($constant)) {
			throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a constant called: '" . $constant . "'", $expression);
		}

		/**
		 * Resolves the symbol that expects the value
		 */
		if ($this->_expecting) {
			if ($this->_expectingVariable) {
				$symbolVariable = $this->_expectingVariable;
				$symbolVariable->initVariant($compilationContext);
			} else {
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
			}
		} else {
			$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
		}

		/**
		 * Variable that receives property accesses must be polimorphic
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompiledException("Cannot use variable: " . $symbolVariable->getType() . " to assign class constants", $expression);
		}

		/**
		 * We can optimize the reading of constants by avoiding query their value everytime
		 */
		//if (!$config->get('static-constant-class-folding')) {
			$compilationContext->headersManager->add('kernel/object');
			$compilationContext->codePrinter->output('zephir_get_class_constant(' . $symbolVariable->getName() . ', ' . $classDefinition->getClassEntry() . ', SS("' . $constant . '") TSRMLS_CC);');
			return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
		//}

		//$constantDefinition = $classDefinition->getConstant($constant);
	}

}