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
 * Variable
 *
 * This represents a variable in a symbol table
 */
class Variable
{
	/**
	 * Variable's type
	 */
	protected $_type;

	/**
	 * Variable's name
	 */
	protected $_name;

	/**
	 * Branch where the variable was declared
	 */
	protected $_branch;

	/**
	 * Compiled variable's name
	 */
	protected $_lowName;

	/**
	 * Number of times the variable has been read
	 */
	protected $_numberUses = 0;

	/**
	 * Temporal variables are marked as idle
	 */
	protected $_idle = false;

	/**
	 * Number of mutations to the variable
	 */
	protected $_numberMutates = 0;

	protected $_initialized = false;

	protected $_isExternal = false;

	protected $_variantInits = 0;

	protected $_mustInitNull = false;

	protected $_readOnly = false;

	protected $_localOnly = false;

	protected $_doublePointer = false;

	protected $_defaultInitValue;

	/**
	 * @param string $type
	 * @param string $name
	 * @param int $branch
	 */
	public function __construct($type, $name, $branch, $defaultInitValue=null)
	{
		$this->_type = $type;
		$this->_name = $name;
		$this->_branch = $branch;
	}

	/**
	 * @return string
	 */
	public function getType()
	{
		return $this->_type;
	}

	/**
	 * Sets if the variable is local-only scoped
	 *
	 * @param boolean $localOnly
	 */
	public function setLocalOnly($localOnly)
	{
		$this->_localOnly = $localOnly;
	}

	/**
	 * Checks if the variable is local-only scoped
	 *
	 * @return boolean
	 */
	public function isLocalOnly()
	{
		return $this->_localOnly;
	}

	/**
	 * Marks the variable to be defined as a double pointer
	 *
	 * @param boolean $doublePointer
	 */
	public function setIsDoublePointer($doublePointer)
	{
		$this->_doublePointer = $doublePointer;
	}

	/**
	 * Returns the variable
	 */
	public function isDoublePointer()
	{
		return $this->_doublePointer;
	}

	/**
	 * Returns variable's real name
	 *
	 * @return string
	 */
	public function getRealName()
	{
		return $this->_name;
	}

	/**
	 * Returns variable's name
	 *
	 * @return string
	 */
	public function getName()
	{
		if ($this->_lowName) {
			return $this->_lowName;
		}
		return $this->_name;
	}

	/**
	 * Sets the compiled variable's name
	 *
	 * @param string $lowName
	 */
	public function setLowName($lowName)
	{
		$this->_lowName = $lowName;
	}

	/**
	 * Sets if the variable is read only
	 *
	 * @param boolean $readOnly
	 */
	public function setReadOnly($readOnly)
	{
		$this->_readOnly = $readOnly;
	}

	/**
	 * Returns if the variable is read only
	 *
	 * @return boolean
	 */
	public function isReadOnly()
	{
		return $this->_readOnly;
	}

	/**
	 * Once a temporal variable is unused in a specific branch
	 * it is marked as idle
	 *
	 * @param boolean $idle
	 */
	public function setIdle($idle)
	{
		$this->_idle = $idle;
	}

	/**
	 * Checks if the variable is idle
	 *
	 * @return boolean
	 */
	public function isIdle()
	{
		return $this->_idle;
	}

	/**
	 * Get the branch where the variable was declared
	 *
	 * @return int
	 */
	public function getBranch()
	{
		return $this->_branch;
	}

	/**
	 * Increase the number of uses a variable may have
	 */
	public function increaseUses()
	{
		$this->_numberUses++;
	}

	/**
	 * Increase the number of mutations a variable may have
	 */
	public function increaseMutates()
	{
		$this->_numberMutates++;
	}

	/**
	 * Return the number of uses
	 *
	 * @return long
	 */
	public function getNumberUses()
	{
		return $this->_numberUses;
	}

	/**
	 * Returns the number of mutations performed over the variable
	 *
	 * @return long
	 */
	public function getNumberMutations()
	{
		return $this->_numberMutates;
	}

	/**
	 * Sets if the variable is initialized
	 * This allow to throw an exception if the variable is read without initialization
	 *
	 * @param boolean $initialized
	 */
	public function setIsInitialized($initialized)
	{
		$this->_initialized = $initialized;
	}

	/**
	 * Check if the variable is initialized or not
	 *
	 * @return boolean
	 */
	public function isInitialized()
	{
		return $this->_initialized;
	}

	/**
	 * Set if the symbol is a parameter of the method or not
	 *
	 * @param boolean $isExternal
	 */
	public function setIsExternal($isExternal)
	{
		$this->_isExternal = $isExternal;
		$this->_variantInits = 1;
	}

	/**
	 * Check if the variable is a parameter
	 *
	 * @return boolean
	 */
	public function isExternal()
	{
		return $this->_isExternal;
	}

	/**
	 * Set if the variable must be initialized to null
	 *
	 * @return boolean
	 */
	public function mustInitNull()
	{
		return $this->_mustInitNull;
	}

	/**
	 * Set if the variable must be initialized to null
	 *
	 * @param boolean $mustInitNull
	 * @return boolean
	 */
	public function setMustInitNull($mustInitNull)
	{
		$this->_mustInitNull = $mustInitNull;
	}

	/**
	 * Sets the default init value
	 *
	 * @param mixed $value
	 */
	public function setDefaultInitValue($value)
	{
		$this->_defaultInitValue = $value;
	}

	/**
	 * Returns the default init value
	 *
	 * @return mixed
	 */
	public function getDefaultInitValue()
	{
		return $this->_defaultInitValue;
	}

	/**
	 * Initializes a variant variable
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function initVariant(CompilationContext $compilationContext)
	{
		if ($this->getName() != 'this_ptr' && $this->getName() != 'return_value') {
			$compilationContext->headersManager->add('kernel/memory');
			if (!$this->isLocalOnly()) {
				$compilationContext->symbolTable->mustGrownStack(true);
				if ($this->_variantInits > 0 || $compilationContext->insideCycle) {
					$this->_mustInitNull = true;
					$compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(' . $this->getName() . ');');
				} else {
					$compilationContext->codePrinter->output('ZEPHIR_INIT_VAR(' . $this->getName() . ');');
				}
			} else {
				if ($this->_variantInits > 0 || $compilationContext->insideCycle) {
					$this->_mustInitNull = true;
					$compilationContext->codePrinter->output('ZEPHIR_SINIT_NVAR(' . $this->getName() . ');');
				} else {
					$compilationContext->codePrinter->output('ZEPHIR_SINIT_VAR(' . $this->getName() . ');');
				}
			}
			$this->_variantInits++;
		}
	}

	/**
	 * Initializes a variant variable that is intended to have the special
	 * behavior of only freed its body value instead of the full variable
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function initComplexLiteralVariant(CompilationContext $compilationContext)
	{
		if ($this->getName() != 'this_ptr' && $this->getName() != 'return_value') {
			$compilationContext->headersManager->add('kernel/memory');
			if (!$this->isLocalOnly()) {
				$compilationContext->symbolTable->mustGrownStack(true);
				if ($this->_variantInits > 0 || $compilationContext->insideCycle) {
					$this->_mustInitNull = true;
					$compilationContext->codePrinter->output('ZEPHIR_INIT_LNVAR(' . $this->getName() . ');');
				} else {
					$compilationContext->codePrinter->output('ZEPHIR_INIT_VAR(' . $this->getName() . ');');
				}
			} else {
				if ($this->_variantInits > 0 || $compilationContext->insideCycle) {
					$this->_mustInitNull = true;
					$compilationContext->codePrinter->output('ZEPHIR_SINIT_LNVAR(' . $this->getName() . ');');
				} else {
					$compilationContext->codePrinter->output('ZEPHIR_SINIT_VAR(' . $this->getName() . ');');
				}
			}
			$this->_variantInits++;
		}
	}

	/**
	 * Observes a variable in the memory frame without initialization
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function observeVariant(CompilationContext $compilationContext)
	{
		if ($this->getName() != 'this_ptr' && $this->getName() != 'return_value') {
			$compilationContext->headersManager->add('kernel/memory');
			$compilationContext->symbolTable->mustGrownStack(true);
			if ($this->_variantInits > 0 || $compilationContext->insideCycle) {
				$this->_mustInitNull = true;
				$compilationContext->codePrinter->output('ZEPHIR_OBS_NVAR(' . $this->getName() . ');');
			} else {
				$compilationContext->codePrinter->output('ZEPHIR_OBS_VAR(' . $this->getName() . ');');
			}
			$this->_variantInits++;
		}
	}

}