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
 * ClassMethod
 *
 * Represents a class method
 */
class ClassMethod
{

	protected $_visibility;

	protected $_name;

	protected $_parameters;

	protected $_statements;

	protected $_docblock;

	/**
	 * ClassMethod constructor
	 *
	 * @param string $visibility
	 * @param string $name
	 */
	public function __construct($visibility, $name, $parameters, StatementsBlock $statements=null, $docblock=null)
	{
		$this->_visibility = $visibility;
		$this->_name = $name;
		$this->_parameters = $parameters;
		$this->_statements = $statements;
		$this->_docblock = $docblock;
	}

	/**
	 * Returns the method name
	 *
	 * @return string
	 */
	public function getName()
	{
		return $this->_name;
	}

	/**
	 * Returns the docblock
	 *
	 * @return string
	 */
	public function getDocBlock()
	{
		return $this->_docblock;
	}

	/**
	 * Returns the parameters
	 *
	 * @return \ClassMethodParameters
	 */
	public function getParameters()
	{
		return $this->_parameters;
	}

	/**
	 * Returns the number of parameters the method has
	 *
	 * @return int
	 */
	public function getNumberOfParameters()
	{
		if (is_object($this->_parameters)) {
			return count($this->_parameters->getParameters());
		}
		return 0;
	}

	/**
	 * Returns the number of required parameters the method has
	 *
	 * @return int
	 */
	public function getNumberOfRequiredParameters()
	{
		if (is_object($this->_parameters)) {
			$parameters = $this->_parameters->getParameters();
			if (count($parameters)) {
				$required = 0;
				foreach ($parameters as $parameter) {
					if (!isset($parameter['default'])) {
						$required++;
					}
				}
				return $required;
			}
		}
		return 0;
	}

	/**
	 * Checks whether the method has a specific modifier
	 *
	 * @param string $modifier
	 * @return boolean
	 */
	public function hasModifier($modifier)
	{
		foreach ($this->_visibility as $visibility) {
			if ($visibility == $modifier) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Returns the C-modifier flags
	 *
	 * @return string
	 */
	public function getModifiers()
	{
		$modifiers = array();
		foreach ($this->_visibility as $visibility) {
			switch ($visibility) {
				case 'public':
					$modifiers['ZEND_ACC_PUBLIC'] = $visibility;
					break;
				case 'protected':
					$modifiers['ZEND_ACC_PROTECTED'] = $visibility;
					break;
				case 'private':
					$modifiers['ZEND_ACC_PRIVATE'] = $visibility;
					break;
				case 'static':
					$modifiers['ZEND_ACC_STATIC'] = $visibility;
					break;
				case 'final':
					$modifiers['ZEND_ACC_FINAL'] = $visibility;
					break;
				case 'inline':
					break;
				case 'scoped':
					break;
				default:
					throw new Exception('Unknown modifier "' . $visibility . '"');
			}
		}
		return join('|', array_keys($modifiers));
	}

	/**
	 * Replace macros
	 */
	public function removeMemoryStackReferences(SymbolTable $symbolTable, $containerCode)
	{
		if (!$symbolTable->getMustGrownStack()) {
			$containerCode = str_replace('ZEPHIR_THROW_EXCEPTION_STR', 'ZEPHIR_THROW_EXCEPTION_STRW', $containerCode);
			$containerCode = str_replace('ZEPHIR_THROW_EXCEPTION_ZVAL', 'ZEPHIR_THROW_EXCEPTION_ZVALW', $containerCode);
			$containerCode = str_replace('RETURN_THIS', 'RETURN_THISW', $containerCode);
			$containerCode = str_replace('RETURN_CTOR', 'RETURN_CTORW', $containerCode);
			$containerCode = str_replace('RETURN_NCTOR', 'RETURN_NCTORW', $containerCode);
			$containerCode = str_replace('RETURN_CCTOR', 'RETURN_CCTORW', $containerCode);
			$containerCode = str_replace('RETURN_MM_NULL', 'RETURN_NULL', $containerCode);
			$containerCode = str_replace('RETURN_MM_BOOL', 'RETURN_BOOL', $containerCode);
			$containerCode = str_replace('RETURN_MM_FALSE', 'RETURN_FALSE', $containerCode);
			$containerCode = str_replace('RETURN_MM_TRUE', 'RETURN_TRUE', $containerCode);
			$containerCode = str_replace('RETURN_MM_STRING', 'RETURN_STRING', $containerCode);
			$containerCode = str_replace('RETURN_MM_LONG', 'RETURN_LONG', $containerCode);
			$containerCode = str_replace('RETURN_MM_DOUBLE', 'RETURN_DOUBLE', $containerCode);
			$containerCode = str_replace('RETURN_MM_FALSE', 'RETURN_FALSE', $containerCode);
			$containerCode = str_replace('RETURN_MM_EMPTY_STRING', 'RETURN_MM_EMPTY_STRING', $containerCode);
			$containerCode = str_replace('RETURN_MM_EMPTY_ARRAY', 'RETURN_EMPTY_ARRAY', $containerCode);
			$containerCode = str_replace('RETURN_MM()', 'return', $containerCode);
			$containerCode = preg_replace('/[ \t]+ZEPHIR_MM_RESTORE\(\);' . PHP_EOL . '/s', '', $containerCode);
		}
		return $containerCode;
	}

	/**
	 * Assigns a default value
	 *
	 * @param array $parameter
	 * @param CompilationContext $compilationContext
	 */
	public function assignDefaultValue($parameter, $compilationContext)
	{
		if (isset($parameter['data-type'])) {
			$dataType = $parameter['data-type'];
		} else {
			$dataType = 'variable';
		}

		$code = '';

		switch ($dataType) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
				switch ($parameter['default']['type']) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						$code .= "\t\t" . $parameter['name'] . ' = ' . $parameter['default']['value'] . ';';
						break;
					case 'double':
						$code .= "\t\t" . $parameter['name'] . ' = (int) ' . $parameter['default']['value'] . ';';
						break;
					default:
						throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(int)", $parameter);
				}
				break;
			case 'bool':
				switch ($parameter['default']['type']) {
					case 'bool':
						if ($parameter['default']['value'] == 'true') {
							$code .= "\t\t" . $parameter['name'] . ' = 1;' . PHP_EOL;
						} else {
							$code .= "\t\t" . $parameter['name'] . ' = 0;' . PHP_EOL;
						}
						break;
					default:
						throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(int)", $parameter);
				}
				break;
			case 'variable':
				$compilationContext->headersManager->add('kernel/memory');
				$compilationContext->symbolTable->mustGrownStack(true);
				$code .= "\t\t" . 'ZEPHIR_INIT_VAR(' . $parameter['name'] . ');' . PHP_EOL;
				switch ($parameter['default']['type']) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						$code .= "\t\t" . 'ZVAL_LONG(' . $parameter['name'] . ', ' . $parameter['default']['value'] . ');' . PHP_EOL;
						break;
					case 'double':
						$code .= "\t\t" . 'ZVAL_DOUBLE(' . $parameter['name'] . ', ' . $parameter['default']['value'] . ');' . PHP_EOL;
						break;
					case 'bool':
						if ($parameter['default']['value'] == 'true') {
							$code .= "\t\t" . 'ZVAL_BOOL' . $parameter['name'] . ', 1);' . PHP_EOL;
						} else {
							$code .= "\t\t" . 'ZVAL_BOOL' . $parameter['name'] . ', 0);' . PHP_EOL;
						}
						break;
					case 'null':
						break;
					default:
						throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(variable)", $parameter);
				}
				break;
			default:
				throw new CompilerException("Default parameter type: " . $dataType, $parameter);
		}

		return $code;
	}

	/**
	 * Assigns a zval value to a static type
	 *
	 * @param array $parameter
	 * @param \CompilationContext $compilationContext
	 * @return string
	 */
	public function assignZvalValue($parameter, $compilationContext)
	{
		if (isset($parameter['data-type'])) {
			$dataType = $parameter['data-type'];
		} else {
			$dataType = 'variable';
		}

		switch ($dataType) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
				$compilationContext->headersManager->add('kernel/operators');
				return "\t\t" . $parameter['name'] . ' = zephir_get_intval(' . $parameter['name'] . '_param);' . PHP_EOL;
			case 'bool':
				$compilationContext->headersManager->add('kernel/operators');
				return "\t\t" . $parameter['name'] . ' = zephir_get_boolval(' . $parameter['name'] . '_param);' . PHP_EOL;
			case 'double':
				$compilationContext->headersManager->add('kernel/operators');
				return "\t\t" . $parameter['name'] . ' = zephir_get_doubleval(' . $parameter['name'] . '_param);' . PHP_EOL;
			case 'string':
				$compilationContext->headersManager->add('kernel/operators');
				return "\t\t" . 'zephir_get_strval(' . $parameter['name'] . '_param, &' . $parameter['name'] . ');' . PHP_EOL;
			default:
				throw new CompilerException("Parameter type: " . $dataType, $parameter);
		}
	}

	/**
	 * Compiles the method
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{

		if (is_object($this->_statements)) {

			/**
			 * This pass checks for zval variables than can be potentally
			 * used without allocate memory and memory tracking
			 * these variables are stored in the stack
			 */
			if ($compilationContext->config->get('local-context-pass')) {
				$localContext = new LocalContextPass();
				$localContext->pass($this->_statements);
			} else {
				$localContext = null;
			}

			/**
			 * This pass tries to infer types for dynamic variables
			 * replacing them by low level variables
			 */
			if ($compilationContext->config->get('static-type-inference')) {
				$typeInference = new StaticTypeInference();
				$typeInference->pass($this->_statements);
				if ($compilationContext->config->get('static-type-inference-second-pass')) {
					$typeInference->reduce();
					$typeInference->pass($this->_statements);
				}
			} else {
				$typeInference = null;
			}

		} else {
			$localContext = null;
			$typeInference = null;
		}

		/**
		 * Every method has its own symbol table
		 */
		$symbolTable = new SymbolTable($compilationContext);
		if ($localContext) {
			$symbolTable->setLocalContext($localContext);
		}

		$compilationContext->typeInference = $typeInference;
		$compilationContext->symbolTable = $symbolTable;

		$oldCodePrinter = $compilationContext->codePrinter;

		/**
		 * Change the code printer to a single method instance
		 */
		$codePrinter = new CodePrinter();
		$compilationContext->codePrinter = $codePrinter;

		if (is_object($this->_parameters)) {

			/**
			 * Round 1. Create variables in parameters in the symbol table
			 */
			foreach ($this->_parameters->getParameters() as $parameter) {

				/**
				 * Change dynamic variables by low level types
				 */
				if ($typeInference) {
					if (isset($parameter['data-type'])) {
						if ($parameter['data-type'] == 'variable') {
							$type = $typeInference->getInferedType($parameter['name']);
							if (is_string($type)) {
								//echo $parameter['name'], ' ', $type, ' ', $parameter['file'], ' ', $parameter['line'], PHP_EOL;
								//$statement['data-type'] = $type;
							}
						}
					} else {
						$type = $typeInference->getInferedType($parameter['name']);
						if (is_string($type)) {
							//echo $parameter['name'], ' ', $type, ' ', $parameter['file'], ' ', $parameter['line'], PHP_EOL;
							//$statement['data-type'] = $type;
						}
					}
				}

				$symbolParam = null;
				if (isset($parameter['data-type'])) {
					if ($parameter['data-type'] == 'variable') {
						$symbol = $symbolTable->addVariable($parameter['data-type'], $parameter['name'], $compilationContext);
					} else {
						$symbol = $symbolTable->addVariable($parameter['data-type'], $parameter['name'], $compilationContext);
						$symbolParam = $symbolTable->addVariable('variable', $parameter['name'] . '_param', $compilationContext);
						if ($parameter['data-type'] == 'string') {
							$symbol->setMustInitNull(true);
						}
					}
				} else {
					$symbol = $symbolTable->addVariable('variable', $parameter['name'], $compilationContext);
				}

				if (is_object($symbolParam)) {

					/**
					 * Parameters are marked as 'external'
					 */
					$symbolParam->setIsExternal(true);

					/**
					 * Assuming they're initialized
					 */
					$symbolParam->setIsInitialized(true);

					/**
					 * Initialize auxiliar parameter zvals to null
					 */
					$symbolParam->setMustInitNull(true);

					/**
					 * Increase uses
					 */
					$symbolParam->increaseUses();

				} else {
					if (isset($parameter['default'])) {
						if (isset($parameter['data-type'])) {
							if ($parameter['data-type'] == 'variable') {
								$symbol->setMustInitNull(true);
							}
						} else {
							$symbol->setMustInitNull(true);
						}
					}
				}

				/**
				 * Original node where the variable was declared
				 */
				$symbol->setOriginal($parameter);

				/**
				 * Parameters are marked as 'external'
				 */
				$symbol->setIsExternal(true);

				/**
				 * Assuming they're initialized
				 */
				$symbol->setIsInitialized(true);

				/**
				 * Variables with class/type must be objects across the execution
				 */
				if (isset($parameter['cast'])) {
					$symbol->setDynamicType('object');
					$symbol->setClassType($parameter['cast']['value']);
				} else {
					if (isset($parameter['data-type'])) {
						if ($parameter['data-type'] == 'variable') {
							$symbol->setDynamicType('undefined');
						}
					} else {
						$symbol->setDynamicType('undefined');
					}
				}
			}
		}

		/**
		 * Compile the block of statements if any
		 */
		if (is_object($this->_statements)) {
			if ($this->hasModifier('static')) {
				$compilationContext->staticContext = true;
			} else {
				$compilationContext->staticContext = false;
			}
			$this->_statements->compile($compilationContext);
		}

		/**
		 * Initialize default values in dynamic variables
		 */
		$initVarCode = "";
		foreach ($symbolTable->getVariables() as $variable) {
			if ($variable->getType() == 'variable') {
				if ($variable->getNumberUses() > 0) {
					if ($variable->getName() != 'this_ptr' && $variable->getName() != 'return_value') {
						$defaultValue = $variable->getDefaultInitValue();
						if (is_array($defaultValue)) {
							$symbolTable->mustGrownStack(true);
							switch ($defaultValue['type']) {
								case 'int':
								case 'uint':
								case 'long':
								case 'char':
								case 'uchar':
									$initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
									$initVarCode .= "\t" . 'ZVAL_LONG(' . $variable->getName() . ', ' . $defaultValue['value'] . ');' . PHP_EOL;
									break;
								case 'double':
									$initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
									$initVarCode .= "\t" . 'ZVAL_DOUBLE(' . $variable->getName() . ', ' . $defaultValue['value'] . ');' . PHP_EOL;
									break;
								default:
									throw new CompilerException('Invalid default type: ' . $variable['expr']['type'] . ' for data type: ' . $statement['data-type'], $variable);
							}
						}
					}
				}
			}
		}

		/**
		 * Fetch parameters from vm-top
		 */
		$initCode = "";
		$code = "";
		if (is_object($this->_parameters)) {

			/**
			 * Round 2. Fetch the parameters in the method
			 */
			$params = array();
			$requiredParams = array();
			$optionalParams = array();
			$numberRequiredParams = 0;
			$numberOptionalParams = 0;
			foreach ($this->_parameters->getParameters() as $parameter) {

				if (isset($parameter['data-type'])) {
					$dataType = $parameter['data-type'];
				} else {
					$dataType = 'variable';
				}

				if ($dataType == 'variable') {
					$params[] = '&' . $parameter['name'];
				} else {
					$params[] = '&' . $parameter['name'] . '_param';
				}

				if (isset($parameter['default'])) {
					$optionalParams[] = $parameter;
					$numberOptionalParams++;
				} else {
					$requiredParams[] = $parameter;
					$numberRequiredParams++;
				}
			}

			/**
			 * Pass the write detector to the method statement block to check if the parameter
			 * variable is modified so as do the proper separation
			 */
			$parametersToSeparate = array();
			if (is_object($this->_statements)) {
				$writeDetector = new WriteDetector();
				foreach ($this->_parameters->getParameters() as $parameter) {
					if (isset($parameter['data-type'])) {
						$dataType = $parameter['data-type'];
					} else {
						$dataType = 'variable';
					}
					if ($dataType == 'variable') {
						$name = $parameter['name'];
						if ($writeDetector->detect($name, $this->_statements->getStatements())) {
							$parametersToSeparate[$name] = true;
						}
					}
				}
			}

			/**
			 * Initialize required parameters
			 */
			foreach ($requiredParams as $parameter) {

				if (isset($parameter['data-type'])) {
					$dataType = $parameter['data-type'];
				} else {
					$dataType = 'variable';
				}

				if ($dataType != 'variable') {

					/**
					 * Assign value from zval to low level type
					 */
					$initCode .= $this->assignZvalValue($parameter, $compilationContext);
				}

				if ($dataType == 'variable') {
					if (isset($parametersToSeparate[$name])) {
						$initCode .= "\t" . "ZEPHIR_SEPARATE_PARAM(" . $name . ");" . PHP_EOL;
					}
				}

			}

			/**
			 * Initialize optional parameters
			 */
			foreach ($optionalParams as $parameter) {

				if (isset($parameter['data-type'])) {
					$dataType = $parameter['data-type'];
				} else {
					$dataType = 'variable';
				}

				if ($dataType == 'variable') {
					$name = $parameter['name'];
				} else {
					$name = $parameter['name'] . '_param';
				}

				/**
				 * Assign the default value according to the variable's type
				 */
				$initCode .= "\t" . 'if (!' . $name . ') {' . PHP_EOL;
				$initCode .= $this->assignDefaultValue($parameter, $compilationContext);
				if ($dataType == 'variable') {
					$initCode .= "\t" . '}' . PHP_EOL;
				} else {
					$initCode .= "\t" . '} else {' . PHP_EOL;
					$initCode .= $this->assignZvalValue($parameter, $compilationContext);
					$initCode .= "\t" . '}' . PHP_EOL;
				}

				if ($dataType == 'variable') {
					if (isset($parametersToSeparate[$name])) {
						$initCode .= "\t" . "ZEPHIR_SEPARATE_PARAM(" . $name . ");" . PHP_EOL;
					}
				}
			}

			/**
			 * Fetch the parameters to zval pointers
			 */
			$codePrinter->preOutputBlankLine();
			$compilationContext->headersManager->add('kernel/memory');
			if ($symbolTable->getMustGrownStack()) {
				$code .= "\t" . 'zephir_fetch_params(1, ' . $numberRequiredParams . ', ' . $numberOptionalParams . ', ' . join(', ', $params) . ');' . PHP_EOL;
			} else {
				$code .= "\t" . 'zephir_fetch_params(0, ' . $numberRequiredParams . ', ' . $numberOptionalParams . ', ' . join(', ', $params) . ');' . PHP_EOL;
			}
			$code .= PHP_EOL;
		}

		$code .= $initCode . $initVarCode;
		$codePrinter->preOutput($code);

		/**
		 * Grow the stack if needed
		 */
		if ($symbolTable->getMustGrownStack()) {
			$compilationContext->headersManager->add('kernel/memory');
			$codePrinter->preOutput("\t" . 'ZEPHIR_MM_GROW();');
		}

		/**
		 * Check if there are unused variables
		 */
		$usedVariables = array();
		foreach ($symbolTable->getVariables() as $variable) {

			if ($variable->getNumberUses() <= 0) {
				$compilationContext->logger->warning('Variable "' . $variable->getName() . '" declared but not used in ' . $compilationContext->classDefinition->getName() . '::' . $this->getName(), "unused-variable", $variable->getOriginal());
				if ($variable->isExternal() == false) {
					continue;
				}
			}

			if ($variable->getName() != 'this_ptr' && $variable->getName() != 'return_value') {
				$type = $variable->getType();
				if (!isset($usedVariables[$type])) {
					$usedVariables[$type] = array();
				}
				$usedVariables[$type][] = $variable;
			}
		}

		if (count($usedVariables)) {
			$codePrinter->preOutputBlankLine();
		}

		/**
		 * Generate the variable definition for variables used
		 */
		foreach ($usedVariables as $type => $variables) {

			$pointer = null;
			switch ($type) {
				case 'int':
					$code = 'int ';
					break;
				case 'uint':
					$code = 'unsigned int ';
					break;
				case 'char':
					$code = 'char ';
					break;
				case 'uchar':
					$code = 'unsigned char ';
					break;
				case 'long':
					$code = 'long ';
					break;
				case 'ulong':
					$code = 'unsigned long ';
					break;
				case 'bool':
					$code = 'zend_bool ';
					break;
				case 'double':
					$code = 'double ';
					break;
				case 'string':
					$compilationContext->headersManager->add('kernel/string_type');
					$pointer = '*';
					$code = 'zephir_str ';
					break;
				case 'variable':
					$pointer = '*';
					$code = 'zval ';
					break;
				case 'HashTable':
					$pointer = '*';
					$code = 'HashTable ';
					break;
				case 'HashPosition':
					$code = 'HashPosition ';
					break;
				case 'zend_class_entry':
					$pointer = '*';
					$code = 'zend_class_entry ';
					break;
				default:
					throw new CompilerException("Unsupported type in declare: " . $type);
			}

			$groupVariables = array();
			$defaultValues = array();
			foreach ($variables as $variable) {
				if (($type == 'variable' || $type == 'string') && $variable->mustInitNull()) {
					if ($variable->isLocalOnly()) {
						$groupVariables[] = $variable->getName() . ' = zval_used_for_init';
					} else {
						if ($variable->isDoublePointer()) {
							$groupVariables[] = $pointer . $pointer . $variable->getName() . ' = NULL';
						} else {
							$groupVariables[] = $pointer . $variable->getName() . ' = NULL';
						}
					}
				} else {
					if ($variable->isLocalOnly()) {
						$groupVariables[] = $variable->getName();
					} else {
						if ($variable->isDoublePointer()) {
							$groupVariables[] = $pointer . $pointer . $variable->getName();
						} else {
							$defaultValue = $variable->getDefaultInitValue();
							if ($defaultValue !== null) {
								if ($type == 'variable') {
									$groupVariables[] = $pointer . $variable->getName();
								} else {
									$groupVariables[] = $pointer . $variable->getName() . ' = '. $defaultValue;
								}
							} else {
								$groupVariables[] = $pointer . $variable->getName();
							}
						}
					}
				}
			}

			$codePrinter->preOutput("\t" . $code . join(', ', $groupVariables) . ';');
		}

		/**
		 * Compile the block of statements if any
		 */
		if (is_object($this->_statements)) {
			if ($symbolTable->getMustGrownStack()) {
				$lastType = $this->_statements->getLastStatementType();
				if ($lastType != 'return' && $lastType != 'throw') {
					$compilationContext->headersManager->add('kernel/memory');
					$codePrinter->output("\t" . 'ZEPHIR_MM_RESTORE();');
				}
			}
		}

		/**
		 * Remove macros that restore the memory stack if it wasn't used
		 */
		$code = $this->removeMemoryStackReferences($symbolTable, $codePrinter->getOutput());

		/**
		 * Restore the compilation context
		 */
		$oldCodePrinter->output($code);
		$compilationContext->codePrinter = $oldCodePrinter;

		return null;
	}

}
