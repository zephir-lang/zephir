<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
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
	 * Current dynamic type of the variable
	 */
	protected $_dynamicTypes = array('unknown' => true);

	/**
	 * Variable's name
	 */
	protected $_name;

	/**
	 * Branch where the variable was declared
	 */
	protected $_branch;

	/**
	 * Branch where the variable was initialized for the first time
	 */
	protected $_initBranch = false;

	/**
	 * Compiled variable's name
	 */
	protected $_lowName;

	/**
	 * Number of times the variable has been read
	 */
	protected $_numberUses = 0;

	/**
	 * Whether the variable is temporal or not
	 */
	protected $_temporal = false;

	/**
	 * Temporal variables are marked as idle
	 */
	protected $_idle = false;

	/**
	 * Reusable temporary variables?
	 */
	protected $_reusable = true;

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

	protected $_memoryTracked = true;

	protected $_doublePointer = false;

	protected $_defaultInitValue;

	/**
	 * Class type
	 */
	protected $_classTypes = array();

	/**
	 * Initialization skips
	 */
	protected $_numberSkips = 0;

	/**
	 * AST node where the variable was originally declared or created
	 */
	protected $_node;

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
	 * Returns the type of variable
	 *
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
	 * Sets whether the variable is temporal or not
	 *
	 * @param boolean $temporal
	 */
	public function setTemporal($temporal)
	{
		$this->_temporal = $temporal;
	}

	/**
	 * Returns whether the variable is temporal or not
	 *
	 * @return boolean
	 */
	public function isTemporal()
	{
		return $this->_temporal;
	}

	/**
	 * Once a temporal variable is unused in a specific branch it is marked as idle
	 *
	 * @param boolean $idle
	 */
	public function setIdle($idle)
	{
		if ($this->_reusable) {
			$this->_classTypes = array();
			$this->_dynamicTypes = array('unknown' => true);
			$this->_idle = $idle;
		} else {
			$this->_idle = false;
		}
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
	 * Some temporary variables can't be reused
	 *
	 * @param boolean $reusable
	 */
	public function setReusable($reusable)
	{
		$this->_reusable = $reusable;
	}

	/**
	 * Checks if the temporary variable is reusable
	 *
	 * @return boolean
	 */
	public function isReusable()
	{
		return $this->_reusable;
	}

	/**
	 * Sets if the variable is not tracked by the memory manager
	 *
	 * @param boolean $memoryTracked
	 */
	public function setMemoryTracked($memoryTracked)
	{
		$this->_memoryTracked = $memoryTracked;
	}

	/**
	 * Checks if the variable is tracked by the memory manager
	 *
	 * @return boolean
	 */
	public function isMemoryTracked()
	{
		return $this->_memoryTracked;
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
	 * Set the original AST node where the variable was declared
	 *
	 * @param array $node
	 */
	public function setOriginal($node)
	{
		$this->_node = $node;
	}

	/**
	 * Returns the original AST node where the variable was declared
	 *
	 * @return array
	 */
	public function getOriginal()
	{
		$node = $this->_node;
		if ($node) {
			return $node;
		}
		return array('file' => 'unknown', 'line' => 0, 'char' => 0);
	}

	/**
	 * Sets the PHP class related to variable
	 *
	 * @param string|array $classTypes
	 */
	public function setClassTypes($classTypes)
	{
		if ($classTypes) {
			if (is_string($classTypes)) {
				if (!in_array($classTypes, $this->_classTypes)) {
					$this->_classTypes[] = $classTypes;
				}
			} else {
				foreach ($classTypes as $classType) {
					if (!in_array($classType, $this->_classTypes)) {
						$this->_classTypes[] = $classType;
					}
				}
			}
		}
	}

	/**
	 * Returns the PHP classes associated to the variable
	 *
	 * @return string
	 */
	public function getClassTypes()
	{
		return $this->_classTypes;
	}

	/**
	 * Sets the current dynamic type in a polimorphic variable
	 *
	 * @param string|array $types
	 */
	public function setDynamicTypes($types)
	{
		if ($types) {
			unset($this->_dynamicTypes['unknown']);
			if (is_string($types)) {
				if (!isset($this->_dynamicType[$types])) {
					$this->_dynamicTypes[$types] = true;
				}
			} else {
				foreach ($types as $type => $one) {
					if (!isset($this->_dynamicTypes[$type])) {
						$this->_dynamicTypes[$type] = true;
					}
				}
			}
		}
	}

	/**
	 * Returns the current dynamic type in a polimorphic variable
	 *
	 * @return array
	 */
	public function getDynamicTypes()
	{
		return $this->_dynamicTypes;
	}

	/**
	 * Checks if the variable has any of the passed dynamic
	 *
	 * @param mixed $types
	 */
	public function hasAnyDynamicType($types)
	{
		if (is_string($types)) {
			return isset($this->_dynamicTypes[$types]);
		} else {
			foreach ($types as $type) {
				if (isset($this->_dynamicTypes[$type])) {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * Check if the variable has at least one dynamic type to the ones passed in the list
	 *
	 * @param array|string $types
	 */
	public function hasDifferentDynamicType($types)
	{
		$number = 0;
		foreach ($types as $type) {
			if (isset($this->_dynamicTypes[$type])) {
				$number++;
			}
		}
		return $number == 0;
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
	 * Separates variables before being updated
	 *
	 * @param \CompilationContext $compilationContext
	 */
	public function separate(CompilationContext $compilationContext)
	{
		if ($this->getName() != 'this_ptr' && $this->getName() != 'return_value') {
			$compilationContext->codePrinter->output('ZEPHIR_SEPARATE(' . $this->getName() . ');');
		}
	}

	/**
	 * Skips variable initialization
	 *
	 * @param int $numberSkips
	 */
	public function skipInitVariant($numberSkips)
	{
		$this->_numberSkips += $numberSkips;
	}

	/**
	 * Initializes a variant variable
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function initVariant(CompilationContext $compilationContext)
	{

		if ($this->_numberSkips) {
			$this->_numberSkips--;
			return;
		}

		/**
		 * Variables are allocated for the first time using ZEPHIR_INIT_VAR
		 * the second, third, etc times are allocated using ZEPHIR_INIT_NVAR
		 * Variables initialized for the first time in a cycle are always initialized using ZEPHIR_INIT_NVAR
		 */

		if ($this->getName() != 'this_ptr' && $this->getName() != 'return_value') {

			if ($this->_initBranch === false) {
				$this->_initBranch = $compilationContext->currentBranch;
			}

			$compilationContext->headersManager->add('kernel/memory');
			if (!$this->isLocalOnly()) {
				$compilationContext->symbolTable->mustGrownStack(true);
				if ($compilationContext->insideCycle) {
					$this->_mustInitNull = true;
					$compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(' . $this->getName() . ');');
				} else {
					if ($this->_variantInits > 0) {
						if ($this->_initBranch === 1) {
							$compilationContext->codePrinter->output('ZEPHIR_INIT_BNVAR(' . $this->getName() . ');');
						} else {
							$this->_mustInitNull = true;
							$compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(' . $this->getName() . ');');
						}
					} else {
						$compilationContext->codePrinter->output('ZEPHIR_INIT_VAR(' . $this->getName() . ');');
					}
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

		if ($this->_numberSkips) {
			$this->_numberSkips--;
			return;
		}

		if ($this->getName() != 'this_ptr' && $this->getName() != 'return_value') {

			if ($this->_initBranch === false) {
				$this->_initBranch = $compilationContext->currentBranch;
			}

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

		if ($this->_numberSkips) {
			$this->_numberSkips--;
			return;
		}

		$name = $this->getName();
		if ($name != 'this_ptr' && $name != 'return_value') {

			if ($this->_initBranch === false) {
				$this->_initBranch = $compilationContext->currentBranch;
			}

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