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
 * DeclareStatement
 *
 * This creates variables in the current symbol table
 */
class DeclareStatement
{

	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	/**
	 * Compiles the statement
	 *
	 * @param \CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$statement = $this->_statement;

		if (!isset($statement['data-type'])) {
			throw new CompilerException("Data type is required", $this->_statement);
		}

		$typeInference = $compilationContext->typeInference;
		$symbolTable = $compilationContext->symbolTable;

		$variables = array();
		foreach ($statement['variables'] as $variable) {

			if ($symbolTable->hasVariable($variable['variable'])) {
				throw new CompilerException("Variable '" . $variable['variable'] . "' is already defined", $variable);
			}

			$currentType = $statement['data-type'];

			/**
			 * Replace original data type by the pre-processed infered type
			 */
			if ($typeInference) {
				if ($currentType == 'variable') {
					$type = $typeInference->getInferedType($variable['variable']);
					if (is_string($type)) {
						$currentType = $type;
					}
				}
			}

			/**
			 * Variables are added to the symbol table
			 */
			if (isset($variable['expr'])) {
				$symbolVariable = $symbolTable->addVariable($currentType, $variable['variable'], $compilationContext, $variable['expr']);
			} else {
				$symbolVariable = $symbolTable->addVariable($currentType, $variable['variable'], $compilationContext);
			}

			/**
			 * Set the node where the variable is declared
			 */
			$symbolVariable->setOriginal($variable);

			if (isset($variable['expr'])) {
				$defaultValue = $variable['expr']['value'];
			} else {
				$defaultValue = null;
			}

			/**
			 * Variables with a default value are initialized by default
			 */
			if ($defaultValue !== null) {

				switch ($currentType) {
					case 'int':
					case 'uint':
					case 'ulong':
					case 'long':
						switch ($variable['expr']['type']) {
							case 'int':
							case 'uint':
							case 'ulong':
							case 'int':
								break;
							default:
								throw new CompilerException('Invalid default type: ' . $variable['expr']['type'] . ' for data type: ' . $statement['data-type'], $variable);
						}
						break;
					case 'double':
						switch ($variable['expr']['type']) {
							case 'double':
								break;
							default:
								throw new CompilerException('Invalid default type: ' . $variable['expr']['type'] . ' for data type: ' . $statement['data-type'], $variable);
						}
						break;
					case 'bool':
						switch ($variable['expr']['type']) {
							case 'bool':
								if ($variable['expr']['value'] == 'true') {
									$defaultValue = 1;
								} else {
									$defaultValue = 0;
								}
								break;
							default:
								throw new CompilerException('Invalid default type: ' . $variable['expr']['type'] . ' for data type: ' . $statement['data-type'], $variable);
						}
						break;
					case 'char':
					case 'uchar':
						switch ($variable['expr']['type']) {
							case 'char':
							case 'uchar':
								$defaultValue = '\'' . $defaultValue . '\'';
								break;
							case 'int':
								break;
							default:
								throw new CompilerException('Invalid default type: ' . $variable['expr']['type'] . ' for data type: ' . $statement['data-type'], $variable);
						}
						break;
					case 'variable':
						$defaultValue = $variable['expr'];
						switch ($variable['expr']['type']) {
							case 'int':
							case 'uint':
							case 'long':
							case 'char':
							case 'uchar':
								$symbolVariable->setDynamicType('long');
								break;
							case 'double':
								$symbolVariable->setDynamicType('double');
								break;
							default:
								throw new CompilerException('Invalid default type: ' . $variable['expr']['type'] . ' for data type: ' . $statement['data-type'], $variable);
						}
						break;
					default:
						throw new CompilerException('Invalid data type: ' . $currentType, $variable);
				}

				$symbolVariable->setDefaultInitValue($defaultValue);
				$symbolVariable->setIsInitialized(true);
				$symbolVariable->increaseMutates();
			}
		}

	}
}
