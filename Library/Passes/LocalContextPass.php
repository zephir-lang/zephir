<?php

/**
 * LocalContextPass
 *
 * This pass try to check if variables only live in the local context of the block
 * or it's used externally which will unallow variables to be placed in the heap
 */
class LocalContextPass
{
	protected $_variables;

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
			if (!isset($this->_variables[$variable['variable']])) {
				$this->_variables[$variable['variable']] = true;
			}
		}
	}

	public function markVariableNoLocal($variable)
	{
		if (isset($this->_variables[$variable])) {
			$this->_variables[$variable] = false;
		}
	}

	/**
	 * Asks the local context information if a variable can be static instead of allocated
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

	public function passLetStatement(array $statement)
	{
		foreach ($statement['assignments'] as $assigment) {
			if (isset($assigment['expr'])) {
				$this->passExpression($assigment['expr']);
			}
			switch ($assigment['assign-type']) {
				case 'variable':
					switch ($assigment['expr']['type']) {
						case 'property-access':
						case 'array-access':
						case 'string':
						case 'array':
						case 'empty-array':
						case 'new':
						case 'fcall':
						case 'mcall':
							$this->markVariableNoLocal($assigment['variable']);
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
					switch ($assigment['expr']['type']) {
						case 'variable':
							$this->markVariableNoLocal($assigment['variable']);
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
			case 'string':
			case 'null':
			case 'empty-array':
			case 'variable':
				break;
			case 'sub':
			case 'add':
			case 'concat':
			case 'equals':
			case 'identical':
			case 'not-identical':
			case 'not-equals':
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
			default:
				//echo $expression['type'], PHP_EOL;
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
					if (isset($statement['clauses'])) {
						foreach ($statement['clauses'] as $clause) {
							if (isset($clause['expr'])) {
								$this->passExpression($clause['expr']);
							}
							$this->passStatementBlock($clause['statements']);
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
					}
					if (isset($statement['key'])) {
						$this->markVariableNoLocal($statement['key']);
					}
					if (isset($statement['statements'])) {
						$this->passStatementBlock($statement['statements']);
					}
					break;
				case 'return':
					break;
				case 'loop':
					if (isset($statement['statements'])) {
						$this->passStatementBlock($statement['statements']);
					}
					break;
				case 'throw':
					break;
				case 'mcall':
				case 'scall':
				case 'fcall':
					$this->passCall($statement['expr']);
					break;
				default:

			}
		}
	}

}