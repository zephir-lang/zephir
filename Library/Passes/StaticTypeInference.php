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
 * StaticTypeInference
 *
 * This pass try to infer typing on dynamic variables so the compiler
 * can replace them by low level types automatically
 */
class StaticTypeInference
{
	protected $_variables = array();

	protected $_infered = array();

	/**
	 * Do the compilation pass
	 *
	 * @param StatementsBlock $block
	 */
	public function pass(StatementsBlock $block)
	{
		$this->passStatementBlock($block->getStatements());
	}

	public function declareVariables(array $statement)
	{
		foreach ($statement['variables'] as $variable) {
			if (!isset($this->_variables[$variable['variable']])) {
				if (isset($variable['expr']['type'])) {
					$this->markVariable($variable['variable'], $variable['expr']['type']);
				}
			}
		}
	}

	/**
	 * Marks a variable to mandatory be stored in the heap
	 *
	 * @param string $variable
	 * @param string $type
	 */
	public function markVariable($variable, $type)
	{
		if (isset($this->_variables[$variable])) {
			$currentType = $this->_variables[$variable];
			if ($currentType == 'undefined') {
				return;
			}
		} else {
			$this->_variables[$variable] = $type;
			return;
		}
		switch ($currentType) {
			case 'bool':
				switch ($type) {
					case 'bool':
					case 'numeric':
						break;
					default:
						$this->_variables[$variable] = 'undefined';
						break;
				}
				break;
			case 'double':
				switch ($type) {
					case 'double':
					case 'numeric':
						break;
					default:
						$this->_variables[$variable] = 'undefined';
						break;
				}
				break;
			case 'string':
				switch ($type) {
					case 'string':
						break;
					default:
						$this->_variables[$variable] = 'undefined';
						break;
				}
				break;
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
			case 'char':
			case 'uchar':
				switch ($type) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
					case 'char':
					case 'uchar':
					case 'numeric':
						break;
					default:
						$this->_variables[$variable] = 'undefined';
						break;
				}
				break;
			case 'null':
				if ($type != 'null') {
					$this->_variables[$variable] = 'undefined';
				}
				break;
			case 'variable':
				$this->_variables[$variable] = 'undefined';
				break;
			default:
				echo $type;
				break;
		}
	}

	/**
	 * Process the found infered types and schedule a new pass
	 *
	 * @return boolean
	 */
	public function reduce()
	{
		$pass = false;
		foreach ($this->_variables as $variable => $type) {
			if ($type == 'variable' || $type == 'undefined' || $type == 'string' || $type == 'null' || $type == 'numeric') {
				unset($this->_variables[$variable]);
			} else {
				$pass = true;
				$this->_infered[$variable] = $type;
			}
		}
		return $pass;
	}

	/**
	 * Asks the local context information whether a variable can be stored in the stack instead of the heap
	 *
	 * @param string $variable
	 * @return boolean
	 */
	public function getInferedType($variable)
	{
		if (isset($this->_variables[$variable])) {
			$type = $this->_variables[$variable];
			if ($type != 'variable' && $type != 'undefined' && $type != 'string' && $type != 'null' && $type != 'numeric') {
				//echo $variable, ' ', $type, PHP_EOL;
				return $type;
			}
		}
		return false;
	}

	public function passLetStatement(array $statement)
	{
		foreach ($statement['assignments'] as $assigment) {
			switch ($assigment['assign-type']) {
				case 'variable':
					$type = $this->passExpression($assigment['expr']);
					if (is_string($type)) {
						$this->markVariable($assigment['variable'], $type);
					}
					break;
				case 'object-property':
				case 'array-index':
				case 'object-property-array-index':
				case 'object-property-append':
					$this->markVariable($assigment['variable'], 'variable');
					break;
				case 'variable-append':
					$this->markVariable($assigment['variable'], 'variable');
					break;
				default:
					//echo $assigment['assign-type'];
			}
		}
	}

	public function passCall(array $expression)
	{
		if (isset($expression['parameters'])) {
			foreach ($expression['parameters'] as $parameter) {
				if ($parameter['type'] == 'variable') {
					//$this->markVariable($parameter['value']);
				} else {
					$this->passExpression($parameter);
				}
			}
		}
	}

	public function passArray(array $expression)
	{
		foreach ($expression['left'] as $item) {
			if ($item['value']['type'] == 'variable') {
				//$this->markVariable($item['value']['value'], 'dynamical');
			} else {
				$this->passExpression($item['value']);
			}
		}
	}

	public function passNew(array $expression)
	{
		if (isset($expression['parameters'])) {
			foreach ($expression['parameters'] as $parameter) {
				if ($parameter['type'] == 'variable') {
					//$this->markVariable($parameter['value'], 'dynamical');
				} else {
					$this->passExpression($parameter);
				}
			}
		}
	}

	public function passExpression(array $expression)
	{
		switch ($expression['type']) {
			case 'bool':
			case 'double':
			case 'int':
			case 'uint';
			case 'long':
			case 'ulong':
			case 'null':
			case 'char':
			case 'uchar':
				return $expression['type'];
			case 'string':
				if (ctype_digit($expression['value'])) {
					return 'int';
				}
				return 'string';
			case 'empty-array':
			case 'static-constant-access':
				return 'variable';
			case 'div':
				return 'double';
			case 'sub':
			case 'add':
			case 'mul':
				$left = $this->passExpression($expression['left']);
				$right = $this->passExpression($expression['right']);
				if ($left == 'int' && $right == 'int') {
					return 'int';
				}
				if ($left == 'uint' && $right == 'uint') {
					return 'uint';
				}
				if ($left == 'long' && $right == 'long') {
					return 'long';
				}
				if ($left == 'double' && $right == 'double') {
					return 'double';
				}
				if ($left == 'double' && $right == 'int') {
					return 'double';
				}
				if ($left == 'double' && $right == 'long') {
					return 'double';
				}
				if ($left == 'int' && $right == 'double') {
					return 'double';
				}
				if ($left == 'int' && $right == 'bool') {
					return 'int';
				}
				if ($left == 'bool' && $right == 'int') {
					return 'int';
				}
				if ($left == $right) {
					return $left;
				}
				return 'numeric';
			case 'mod':
				$left = $this->passExpression($expression['left']);
				$right = $this->passExpression($expression['right']);
				if ($left == 'long' && $right == 'long') {
					return 'long';
				}
				if ($left == 'ulong' && $right == 'ulong') {
					return 'ulong';
				}
				return 'int';
			case 'and':
			case 'or':
				return 'bool';
			case 'concat':
				return 'string';
			case 'equals':
			case 'identical':
			case 'not-identical':
			case 'not-equals':
			case 'less':
			case 'greater':
			case 'greater-equal':
			case 'less-equal':
				$left = $this->passExpression($expression['left']);
				$right = $this->passExpression($expression['right']);
				return 'bool';
			case 'typeof':
				$this->passExpression($expression['left']);
				return 'string';
			case 'not':
				$this->passExpression($expression['left']);
				return 'bool';
			case 'mcall':
			case 'fcall':
			case 'scall':
				$this->passCall($expression);
				return 'variable';
			case 'array':
				$this->passArray($expression);
				return 'variable';
			case 'new':
				$this->passNew($expression);
				return 'variable';
			case 'property-access':
			case 'array-access':
				$this->passExpression($expression['left']);
				return 'variable';
			case 'fetch':
			case 'isset':
				$this->passExpression($expression['left']);
				return 'bool';
			case 'list':
				return $this->passExpression($expression['left']);
			case 'cast':
				return $expression['left']['value'];
			case 'type-hint':
				return $this->passExpression($expression['right']);
			case 'variable':
				if (isset($this->_infered[$expression['value']])) {
					return $this->_infered[$expression['value']];
				}
				return null;
			case 'constant':
				return null;
			default:
				echo $expression['type'], PHP_EOL;
				break;
		}
	}

	public function passStatementBlock(array $statements)
	{
		foreach ($statements as $statement) {

			switch ($statement['type']) {
				case 'let':
					$this->passLetStatement($statement);
					break;
				case 'echo':
					if (isset($statement['expressions'])) {
						foreach ($statement['expressions'] as $expr) {
							$this->passExpression($expr);
						}
					}
					break;
				case 'declare':
					$this->declareVariables($statement);
					break;
				case 'if':
					if (isset($statement['expr'])) {
						$this->passExpression($statement['expr']);
					}
					if (isset($statement['statements'])) {
						$this->passStatementBlock($statement['statements']);
					}
					if (isset($statement['else_statements'])) {
						$this->passStatementBlock($statement['else_statements']);
					}
					break;
				case 'switch':
					if (isset($statement['expr'])) {
						$this->passExpression($statement['expr']);
					}
					if (isset($statement['clauses'])) {
						foreach ($statement['clauses'] as $clause) {
							if (isset($clause['expr'])) {
								$this->passExpression($clause['expr']);
							}
							if (isset($clause['statements'])) {
								$this->passStatementBlock($clause['statements']);
							}
						}
					}
					break;
				case 'while':
				case 'do-while':
					if (isset($statement['expr'])) {
						$this->passExpression($statement['expr']);
					}
					if (isset($statement['statements'])) {
						$this->passStatementBlock($statement['statements']);
					}
					break;
				case 'for':
					if (isset($statement['expr'])) {
						$this->passExpression($statement['expr']);
					}
					if (isset($statement['value'])) {
						//$this->markVariable($statement['value']);
					}
					if (isset($statement['key'])) {
						//$this->markVariable($statement['key']);
					}
					if (isset($statement['statements'])) {
						$this->passStatementBlock($statement['statements']);
					}
					break;
				case 'return':
					if (isset($statement['expr'])) {
						$this->passExpression($statement['expr']);
					}
					break;
				case 'loop':
					if (isset($statement['statements'])) {
						$this->passStatementBlock($statement['statements']);
					}
					break;
				case 'throw':
					if (isset($statement['expr'])) {
						$this->passExpression($statement['expr']);
					}
					break;
				case 'mcall':
				case 'scall':
				case 'fcall':
					$this->passCall($statement['expr']);
					break;
				case 'break':
				case 'continue':
				case 'unset':
					break;
				default:
					echo $statement['type'];
			}
		}
	}

}