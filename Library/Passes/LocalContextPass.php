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
 * LocalContextPass
 *
 * This pass tries to check whether variables only do exist in the local context of the method block
 * or if they're used externally which will unallow variables to be placed in the stack
 *
 * This pass also tracks the number of initializations a variable may have, this allows
 * to mark variables as read-only after their last initialization. The number of
 * mutations is relative, since assignments inside cycles/loops may perform a n-number of
 * mutations
 */
class LocalContextPass
{
	protected $_variables = array();

	protected $_mutations = array();

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
		if (isset($statement['data-type'])) {
			if ($statement['data-type'] != 'variable') {
				return;
			}
		}
		foreach ($statement['variables'] as $variable) {
			if (isset($variable['expr'])) {
				if ($variable['expr']['type'] == 'string') {
					$this->_variables[$variable['variable']] = false;
					continue;
				}
			}
			if (!isset($this->_variables[$variable['variable']])) {
				$this->_variables[$variable['variable']] = true;
			}
		}
	}

	/**
	 * Marks a variable to mandatory be stored in the heap
	 *
	 * @param string $variable
	 */
	public function markVariableNoLocal($variable)
	{
		if (isset($this->_variables[$variable])) {
			$this->_variables[$variable] = false;
		}
	}

	/**
	 * Asks the local context information whether a variable can be stored in the stack instead of the heap
	 *
	 * @param string $variable
	 * @return boolean
	 */
	public function shouldBeLocal($variable)
	{
		if (isset($this->_variables[$variable])) {
			if ($this->_variables[$variable]) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Increase the number of mutations a variable has inside a statement block
	 *
	 * @param string $variable
	 */
	public function increaseMutations($variable)
	{
		if (isset($this->_mutations[$variable])) {
			$this->_mutations[$variable]++;
		} else {
			$this->_mutations[$variable] = 1;
		}
	}

	/**
	 * Returns the number of assignment instructions that mutated a variable
	 *
	 * @param string $variable
	 * @return int
	 */
	public function getNumberOfMutations($variable)
	{
		if (isset($this->_mutations[$variable])) {
			return $this->_mutations[$variable];
		}
		return 0;
	}

	public function passLetStatement(array $statement)
	{
		foreach ($statement['assignments'] as $assigment) {

			if (isset($assigment['expr'])) {
				$this->passExpression($assigment['expr']);
			}
			$this->increaseMutations($assigment['variable']);

			switch ($assigment['assign-type']) {
				case 'variable':
					switch ($assigment['expr']['type']) {
						case 'property-access':
						case 'array-access':
						case 'static-property-access':
						case 'string':
						case 'array':
						case 'empty-array':
						case 'new':
						case 'fcall':
						case 'mcall':
						case 'scall':
						case 'concat':
						case 'clone':
						case 'require':
						case 'type-hint':
							$this->markVariableNoLocal($assigment['variable']);
							break;
						case 'constant':
							if (defined($assigment['expr']['value'])) {
								if (gettype(constant($assigment['expr']['value'])) == 'string') {
									$this->markVariableNoLocal($assigment['variable']);
								}
							}
							break;
						case 'variable':
							$this->markVariableNoLocal($assigment['expr']['value']);
							$this->markVariableNoLocal($assigment['variable']);
							break;
						default:
							//echo '[', $assigment['expr']['type'], ']';
					}
					break;
				case 'object-property':
				case 'array-index':
				case 'object-property-array-index':
				case 'object-property-append':
					switch ($assigment['expr']['type']) {
						case 'variable':
							$this->markVariableNoLocal($assigment['expr']['value']);
							break;
					}
					$this->markVariableNoLocal($assigment['variable']);
					break;
				case 'variable-append':
					$this->markVariableNoLocal($assigment['variable']);
					switch ($assigment['expr']['type']) {
						case 'variable':
							$this->markVariableNoLocal($assigment['expr']['value']);
							break;
						default:
							//echo '[', $assigment['assign-type'], ']';
					}
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
					$this->markVariableNoLocal($parameter['value']);
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
				$this->markVariableNoLocal($item['value']['value']);
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
					$this->markVariableNoLocal($parameter['value']);
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
			case 'string':
			case 'null':
			case 'char':
			case 'uchar':
			case 'empty-array':
			case 'variable':
			case 'constant':
			case 'static-constant-access':
				break;
			case 'sub':
			case 'add':
			case 'div':
			case 'mul':
			case 'mod':
			case 'and':
			case 'or':
			case 'concat':
			case 'equals':
			case 'identical':
			case 'not-identical':
			case 'not-equals':
			case 'less':
			case 'greater':
			case 'greater-equal':
			case 'less-equal':
				$this->passExpression($expression['left']);
				$this->passExpression($expression['right']);
				break;
			case 'typeof':
			case 'not':
				$this->passExpression($expression['left']);
				break;
			case 'mcall':
			case 'fcall':
			case 'scall':
				$this->passCall($expression);
				break;
			case 'array':
				$this->passArray($expression);
				break;
			case 'new':
				$this->passNew($expression);
				break;
			case 'property-access':
			case 'property-dynamic-access':
			case 'static-property-access':
			case 'array-access':
				$this->passExpression($expression['left']);
				break;
			case 'isset':
			case 'empty':
			case 'instanceof':
			case 'require':
			case 'clone':
			case 'likely':
			case 'unlikely':
				$this->passExpression($expression['left']);
				break;
			case 'fetch':
				$this->increaseMutations($expression['left']['value']);
				$this->markVariableNoLocal($expression['left']['value']);
				$this->passExpression($expression['right']);
				break;
			case 'list':
				$this->passExpression($expression['left']);
				break;
			case 'cast':
			case 'type-hint':
				$this->passExpression($expression['right']);
				break;
			default:
				echo 'Type=', $expression['type'], PHP_EOL;
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
						$this->markVariableNoLocal($statement['value']);
						$this->increaseMutations($statement['value']);
					}
					if (isset($statement['key'])) {
						$this->markVariableNoLocal($statement['key']);
						$this->increaseMutations($statement['key']);
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
				case 'fetch':
					$this->passExpression($statement['expr']);
					break;
				case 'mcall':
				case 'scall':
				case 'fcall':
				case 'require':
					$this->passCall($statement['expr']);
					break;
				case 'unset':
					if ($statement['expr']['left']['type'] == 'variable') {
						$this->increaseMutations($statement['expr']['left']['value']);
					}
					break;
				case 'break':
				case 'continue':
				case 'empty':
				case 'cblock':
					break;
				default:
					echo 'Statement=', $statement['type'], PHP_EOL;
			}
		}
	}

}
