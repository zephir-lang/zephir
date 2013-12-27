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
 * ForStatement
 *
 * For statement
 */
class ForStatement
	extends StatementAbstract
{
	/**
	 * Compiles a for statement that use a 'range' as expression
	 *
	 * @param array $exprRaw
	 * @param \CompilationContext $compilationContext
	 * @return boolean
	 */
	public function compileRange($exprRaw, $compilationContext)
	{

		if (!count($exprRaw['parameters'])) {
			return false;
		}

		if (count($exprRaw['parameters']) > 3) {
			return false;
		}

		$functionCall = new FunctionCall();
		$parameters = $functionCall->getResolvedParamsAsExpr($exprRaw['parameters'], $compilationContext, $exprRaw);

		if ($parameters[0]->getType() != 'variable') {
			if (!$parameters[0]->isIntCompatibleType()) {
				return false;
			}
		}

		if ($parameters[1]->getType() != 'variable') {
			if (!$parameters[1]->isIntCompatibleType()) {
				return false;
			}
		}

		$codePrinter = $compilationContext->codePrinter;

		/**
		 * Initialize 'key' variable
		 */
		if (isset($this->_statement['key'])) {

			/**
			 * This variable is used to check if the loop is in its first iteration
			 */
			$keyVariable = $compilationContext->symbolTable->getTempVariableForWrite('int', $compilationContext);
		}

		/**
		 * This variable is used to check if the loop is in its first iteration
		 */
		$flagVariable = $compilationContext->symbolTable->getTempVariableForWrite('bool', $compilationContext);

		if ($parameters[0]->getType() != 'variable') {
			$tempVariable = $compilationContext->symbolTable->addTemp($parameters[0]->getType(), $compilationContext);
		} else {
			$rangeVariable = $compilationContext->symbolTable->getVariableForRead($parameters[0]->getCode());
			$tempVariable = $compilationContext->symbolTable->addTemp($rangeVariable->getType(), $compilationContext);
		}

		/**
		 * Create a copy of the current value in the end of the range to avoid modify the range
		 * inside the cycle
		 */
		if ($parameters[1]->getType() != 'variable') {
			$upperBoundVariable = $compilationContext->symbolTable->getTempVariable($parameters[1]->getType(), $compilationContext);
		} else {
			$rangeVariable = $compilationContext->symbolTable->getVariableForRead($parameters[1]->getCode());
			$upperBoundVariable = $compilationContext->symbolTable->getTempVariable($rangeVariable->getType(), $compilationContext);
		}

		/**
		 * Create an implicit 'let' operation to set the current value in the upper bound of the range
		 */
		$statement = new LetStatement(array(
			'type' => 'let',
			'assignments' => array(
				array(
					'assign-type' => 'variable',
					'variable' => $upperBoundVariable->getName(),
					'operator' => 'assign',
					'expr' => array(
						'type' => $parameters[1]->getType(),
						'value' => $parameters[1]->getCode(),
					),
				)
			)
		));
		$statement->compile($compilationContext);

		if ($this->_statement['reverse']) {

			/**
			 * Create an implicit 'let' operation for the initialize expression, @TODO use a builder
			 */
			$statement = new LetStatement(array(
				'type' => 'let',
				'assignments' => array(
					array(
						'assign-type' => 'variable',
						'variable' => $tempVariable->getName(),
						'operator' => 'assign',
						'expr' => array(
							'type' => 'variable',
							'value' => $upperBoundVariable->getName(),
							'file' => $this->_statement['file'],
							'line' => $this->_statement['line'],
							'char' => $this->_statement['char']
						),
						'file' => $this->_statement['file'],
						'line' => $this->_statement['line'],
						'char' => $this->_statement['char']
					)
				)
			));

		} else {

			/**
			 * Create an implicit 'let' operation for the initialize expression, @TODO use a builder
			 */
			$statement = new LetStatement(array(
				'type' => 'let',
				'assignments' => array(
					array(
						'assign-type' => 'variable',
						'variable' => $tempVariable->getName(),
						'operator' => 'assign',
						'expr' => array(
							'type' => $parameters[0]->getType(),
							'value' => $parameters[0]->getCode(),
							'file' => $this->_statement['file'],
							'line' => $this->_statement['line'],
							'char' => $this->_statement['char']
						),
						'file' => $this->_statement['file'],
						'line' => $this->_statement['line'],
						'char' => $this->_statement['char']
					)
				)
			));
		}

		$statement->compile($compilationContext);

		/**
		 * Initialize 'key' variable
		 */
		if (isset($this->_statement['key'])) {
			$codePrinter->output($keyVariable->getName() . ' = 0;');
		}
		$codePrinter->output($flagVariable->getName() . ' = 0;');

		if ($this->_statement['reverse']) {
			$conditionExpr = array(
				'type' => 'greater-equal',
				'left' => array('type' => 'variable', 'value' => $tempVariable->getName()),
				'right' => array('type' => $parameters[0]->getType(), 'value' => $parameters[0]->getCode())
			);
		} else {
			$conditionExpr = array(
				'type' => 'less-equal',
				'left' => array('type' => 'variable', 'value' => $tempVariable->getName()),
				'right' => array('type' => 'variable', 'value' => $upperBoundVariable->getName())
			);
		}

		$expr = new EvalExpression();
		$condition = $expr->optimize($conditionExpr, $compilationContext);

		$codePrinter->output('if (' . $condition . ') {');
		$codePrinter->increaseLevel();

		/**
		 * Inside a cycle
		 */
		$compilationContext->insideCycle++;

		$codePrinter->output('while (1) {');
		$codePrinter->increaseLevel();

		$codePrinter->output('if (' . $flagVariable->getName() . ') {');

		$codePrinter->increaseLevel();

		if (isset($this->_statement['key'])) {
			$codePrinter->output($keyVariable->getName() . '++;');
		}

		if ($this->_statement['reverse']) {
			if (!isset($parameters[2])) {
				$statement = new LetStatement(array(
					'type' => 'let',
					'assignments' => array(
						array(
							'assign-type' => 'decr',
							'variable' => $tempVariable->getName(),
							'file' => $this->_statement['file'],
							'line' => $this->_statement['line'],
							'char' => $this->_statement['char']
						)
					)
				));
			} else {
				$statement = new LetStatement(array(
					'type' => 'let',
					'assignments' => array(
						array(
							'assign-type' => 'variable',
							'operator' => 'sub-assign',
							'variable' => $tempVariable->getName(),
							'expr' => array(
								'type' => $parameters[2]->getType(),
								'value' => $parameters[2]->getCode(),
								'file' => $this->_statement['file'],
								'line' => $this->_statement['line'],
								'char' => $this->_statement['char']
							),
							'file' => $this->_statement['file'],
							'line' => $this->_statement['line'],
							'char' => $this->_statement['char']
						)
					)
				));
			}
		} else {
			if (!isset($parameters[2])) {
				$statement = new LetStatement(array(
					'type' => 'let',
					'assignments' => array(
						array(
							'assign-type' => 'incr',
							'variable' => $tempVariable->getName(),
							'file' => $this->_statement['file'],
							'line' => $this->_statement['line'],
							'char' => $this->_statement['char']
						)
					)
				));
			} else {
				$statement = new LetStatement(array(
					'type' => 'let',
					'assignments' => array(
						array(
							'assign-type' => 'variable',
							'operator' => 'add-assign',
							'variable' => $tempVariable->getName(),
							'expr' => array(
								'type' => $parameters[2]->getType(),
								'value' => $parameters[2]->getCode(),
								'file' => $this->_statement['file'],
								'line' => $this->_statement['line'],
								'char' => $this->_statement['char']
							),
							'file' => $this->_statement['file'],
							'line' => $this->_statement['line'],
							'char' => $this->_statement['char']
						)
					)
				));
			}
		}
		$statement->compile($compilationContext);

		/**
		 * Multi-line conditions would need to be regenerated here
		 */
		$condition = $expr->optimize($conditionExpr, $compilationContext);
		$codePrinter->output('if (!(' . $condition . ')) {');
		$codePrinter->output("\t" . "break;");
		$codePrinter->output('}');

		$codePrinter->decreaseLevel();

		$codePrinter->output('} else {');
		$codePrinter->output("\t" . $flagVariable->getName() . ' = 1;');
		$codePrinter->output('}');

		/**
		 * Initialize 'key' variable
		 */
		if (isset($this->_statement['key'])) {

			/**
			 * Create an implicit 'let' operation, @TODO use a builder
			 */
			$statement = new LetStatement(array(
				'type' => 'let',
				'assignments' => array(
					array(
						'assign-type' => 'variable',
						'variable' => $this->_statement['key'],
						'operator' => 'assign',
						'expr' => array(
							'type' => 'variable',
							'value' => $keyVariable->getName(),
							'file' => $this->_statement['file'],
							'line' => $this->_statement['line'],
							'char' => $this->_statement['char']
						),
						'file' => $this->_statement['file'],
						'line' => $this->_statement['line'],
						'char' => $this->_statement['char']
					)
				)
			));

			$statement->compile($compilationContext);
		}

		/**
		 * Initialize 'value' variable
		 */
		if (isset($this->_statement['value'])) {

			/**
			 * Create an implicit 'let' operation, @TODO use a builder
			 */
			$statement = new LetStatement(array(
				'type' => 'let',
				'assignments' => array(
					array(
						'assign-type' => 'variable',
						'variable' => $this->_statement['value'],
						'operator' => 'assign',
						'expr' => array(
							'type' => 'variable',
							'value' => $tempVariable->getName(),
							'file' => $this->_statement['file'],
							'line' => $this->_statement['line'],
							'char' => $this->_statement['char']
						),
						'file' => $this->_statement['file'],
						'line' => $this->_statement['line'],
						'char' => $this->_statement['char']
					)
				)
			));

			$statement->compile($compilationContext);
		}

		$codePrinter->decreaseLevel();

		/**
		 * Compile statements in the 'for' block
		 */
		if (isset($this->_statement['statements'])) {
			$st = new StatementsBlock($this->_statement['statements']);
			$st->compile($compilationContext);
		}

		/**
		 * Restore the cycle counter
		 */
		$compilationContext->insideCycle--;

		$codePrinter->output('}');

		$codePrinter->decreaseLevel();

		$codePrinter->output('}');

	}

	/**
	 * Compiles traversing of string values
	 * - Evaluated expression must be a string
	 * - Every key must be an integer or compatible
	 * - Every value must be a char/integer or compatible
	 *
	 * @param array $expression
	 * @param CompilationContext $compilationContext
	 */
	public function compileStringTraverse($expression, $compilationContext)
	{
		$codePrinter = $compilationContext->codePrinter;

		/**
		 * Initialize 'key' variable
		 */
		if (isset($this->_statement['key'])) {
			$keyVariable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['key'], $compilationContext, $compilationContext, $this->_statement['expr']);
			switch ($keyVariable->getType()) {
				case 'int':
				case 'uint':
				case 'long':
				case 'ulong':
				case 'char':
				case 'uchar':
					break;
				default:
					throw new CompilerException("Cannot use variable: " . $this->_statement['key'] . " type: " . $keyVariable->getType() . " as key in string traversal", $this->_statement['expr']);
			}
			$keyVariable->setMustInitNull(true);
			$keyVariable->setIsInitialized(true);
		}

		/**
		 * Initialize 'value' variable
		 */
		if (isset($this->_statement['value'])) {
			$variable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['value'], $compilationContext, $compilationContext, $this->_statement['expr']);
			switch ($variable->getType()) {
				case 'int':
				case 'uint':
				case 'long':
				case 'ulong':
				case 'char':
				case 'uchar':
					break;
				default:
					throw new CompilerException("Cannot use variable: " . $this->_statement['value'] . " type: " . $variable->getType() . " as value in string traversal", $this->_statement['expr']);
			}
			$variable->setMustInitNull(true);
			$variable->setIsInitialized(true);
		}

		$tempVariable = $compilationContext->symbolTable->addTemp('int', $compilationContext);

		if ($this->_statement['reverse']) {
			$codePrinter->output('for (' . $tempVariable->getName() . ' = Z_STRLEN_P(' . $expression->getCode() . '); ' .
				$tempVariable->getName() . ' >= 0; ' .
				$tempVariable->getName() . '--) {');
		} else {
			$codePrinter->output('for (' . $tempVariable->getName() . ' = 0; ' .
				$tempVariable->getName() . ' < Z_STRLEN_P(' . $expression->getCode() . '); ' .
				$tempVariable->getName() . '++) {');
		}

		if (isset($this->_statement['key'])) {
			$codePrinter->output("\t" . $keyVariable->getName() . ' = ' . $tempVariable->getName() . '; ');
		}

		$codePrinter->output("\t" . $variable->getName() . ' = Z_STRVAL_P(' . $expression->getCode() . ')[' . $tempVariable->getName() . ']; ');

		/**
		 * Variables are initialized in a different way inside cycle
		 */
		$compilationContext->insideCycle++;

		/**
		 * Compile statements in the 'for' block
		 */
		if (isset($this->_statement['statements'])) {
			$st = new StatementsBlock($this->_statement['statements']);
			$st->compile($compilationContext);
		}

		$compilationContext->insideCycle--;

		$codePrinter->output('}');

	}

	/**
	 * Compiles traversing of hash values
	 * - Evaluated expression must be a zval
	 * - Every key must be a zval
	 * - Every value must be a zval
	 *
	 * @param array $expression
	 * @param CompilationContext $compilationContext
	 */
	public function compileHashTraverse($expression, $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;

		/**
		 * Initialize 'key' variable
		 */
		if (isset($this->_statement['key'])) {
			$keyVariable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['key'], $compilationContext, $this->_statement['expr']);
			if ($keyVariable->getType() != 'variable') {
				throw new CompilerException("Cannot use variable: " . $this->_statement['key'] . " type: " . $keyVariable->getType() . " as key in hash traversal", $this->_statement['expr']);
			}
			$keyVariable->setMustInitNull(true);
			$keyVariable->setIsInitialized(true);
			$keyVariable->setDynamicTypes('undefined');
		}

		/**
		 * Initialize 'value' variable
		 */
		if (isset($this->_statement['value'])) {
			$variable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['value'], $compilationContext, $this->_statement['expr']);
			if ($variable->getType() != 'variable') {
				throw new CompilerException("Cannot use variable: " . $this->_statement['value'] . " type: " . $variable->getType() . " as value in hash traversal", $this->_statement['expr']);
			}
			$variable->setMustInitNull(true);
			$variable->setIsInitialized(true);
			$variable->setDynamicTypes('undefined');
		}

		/**
		 * Variables are initialized in a different way inside cycle
		 */
		$compilationContext->insideCycle++;

		/**
		 * Create a hash table and hash pointer temporary variables
		 */
		$arrayPointer = $compilationContext->symbolTable->addTemp('HashPosition', $compilationContext);
		$arrayHash = $compilationContext->symbolTable->addTemp('HashTable', $compilationContext);

		/**
		 * Create a temporary zval to fetch the items from the hash
		 */
		$tempVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
		$tempVariable->setIsDoublePointer(true);

		if ($this->_statement['reverse']) {
			$codePrinter->output('zephir_is_iterable(' . $expression->getCode() . ', &' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ', 0, 1);');
		} else {
			$codePrinter->output('zephir_is_iterable(' . $expression->getCode() . ', &' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ', 0, 0);');
		}

		$codePrinter->output('for (');
		$codePrinter->output('	; zend_hash_get_current_data_ex(' . $arrayHash->getName() . ', (void**) &' . $tempVariable->getName() . ', &' . $arrayPointer ->getName() . ') == SUCCESS');
		if ($this->_statement['reverse']) {
			$codePrinter->output('	; zend_hash_move_backwards_ex(' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ')');
		} else {
			$codePrinter->output('	; zend_hash_move_forward_ex(' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ')');
		}
		$codePrinter->output(') {');

		if (isset($this->_statement['key'])) {
			$compilationContext->headersManager->add('kernel/hash');
			$codePrinter->output("\t" . 'ZEPHIR_GET_HMKEY(' . $this->_statement['key'] . ', ' . $arrayHash->getName() . ', ' . $arrayPointer ->getName() . ');');
		}

		if (isset($this->_statement['value'])) {
			$codePrinter->output("\t" . 'ZEPHIR_GET_HVALUE(' . $this->_statement['value'] . ', ' . $tempVariable->getName() . ');');
		}

		/**
		 * Compile statements in the 'for' block
		 */
		if (isset($this->_statement['statements'])) {
			$st = new StatementsBlock($this->_statement['statements']);
			$st->compile($compilationContext);
		}

		/**
		 * Restore the cycle counter
		 */
		$compilationContext->insideCycle--;

		$codePrinter->output('}');
	}

	/**
	 * @param CompilationContext $compilationContext
	 * @throws CompilerException
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$exprRaw = $this->_statement['expr'];

		/**
		 * @TODO implement optimizers here
		 */
		if ($exprRaw['type'] == 'fcall') {
			if ($exprRaw['name'] == 'range') {
				$status = $this->compileRange($exprRaw, $compilationContext);
				if ($status !== false) {
					return;
				}
			}
		}

		$expr = new Expression($exprRaw);
		$expr->setReadOnly(true);
		$expression = $expr->compile($compilationContext);

		if ($expression->getType() != 'variable') {
			throw new CompilerException("Unknown type: " . $variable->getType(), $exprRaw);
		}

		$exprVariable = $compilationContext->symbolTable->getVariableForRead($expression->getCode(), $compilationContext, $this->_statement['expr']);
		switch ($exprVariable->getType()) {
			case 'variable':
				$this->compileHashTraverse($expression, $compilationContext);
				break;
			case 'string':
				$this->compileStringTraverse($expression, $compilationContext);
				break;
			default:
				throw new CompilerException("Cannot traverse value type: " . $exprVariable->getType(), $exprRaw);
		}

	}

}
