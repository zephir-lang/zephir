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
 * ForStatement
 *
 * For statement
 */
class ForStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	public function compileStringTraverse($expression, $compilationContext)
	{
		$codePrinter = $compilationContext->codePrinter;

		/**
		 * Initialize 'key' variable
		 */
		if (isset($this->_statement['key'])) {
			$keyVariable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['key'], $this->_statement['expr']);
			if ($keyVariable->getType() != 'int') {
				throw new CompilerException("Cannot use variable: " . $this->_statement['key'] . " type: " . $keyVariable->getType() . " as key in string traversal", $this->_statement['expr']);
			}
			$keyVariable->setMustInitNull(true);
			$keyVariable->setIsInitialized(true);
		}

		/**
		 * Initialize 'value' variable
		 */
		if (isset($this->_statement['value'])) {
			$variable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['value'], $this->_statement['expr']);
			if ($variable->getType() != 'int') {
				throw new CompilerException("Cannot use variable: " . $this->_statement['value'] . " type: " . $variable->getType() . " as value in string traversal", $this->_statement['expr']);
			}
			$variable->setMustInitNull(true);
			$variable->setIsInitialized(true);
		}

		$tempVariable = $compilationContext->symbolTable->addTemp('int', $compilationContext);

		$codePrinter->output('for (' . $tempVariable->getName() . ' = 0; ' .
			$tempVariable->getName() . ' < ' . $expression->getCode() . '->len; ' .
			$tempVariable->getName() . '++) {');

		if (isset($this->_statement['key'])) {
			$codePrinter->output("\t" . $keyVariable->getName() . ' = ' . $tempVariable->getName() . '; ');
		}

		$codePrinter->output("\t" . $variable->getName() . ' = ' . $expression->getCode() . '->str[' . $tempVariable->getName() . ']; ');

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
	 * Compiles the traversal of a hash key
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
			$keyVariable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['key'], $this->_statement['expr']);
			if ($keyVariable->getType() != 'variable') {
				throw new CompilerException("Cannot use variable: " . $this->_statement['key'] . " type: " . $keyVariable->getType() . " as key in hash traversal", $this->_statement['expr']);
			}
			$keyVariable->setMustInitNull(true);
			$keyVariable->setIsInitialized(true);
		}

		/**
		 * Initialize 'value' variable
		 */
		if (isset($this->_statement['value'])) {
			$variable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['value'], $this->_statement['expr']);
			if ($variable->getType() != 'variable') {
				throw new CompilerException("Cannot use variable: " . $this->_statement['value'] . " type: " . $variable->getType() . " as value in hash traversal", $this->_statement['expr']);
			}
			$variable->setMustInitNull(true);
			$variable->setIsInitialized(true);
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

		$codePrinter->output('zephir_is_iterable(' . $expression->getCode() . ', &' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ', 0, 0);');

        $codePrinter->output('for (');
		$codePrinter->output('	; zend_hash_get_current_data_ex(' . $arrayHash->getName() . ', (void**) &' . $tempVariable->getName() . ', &' . $arrayPointer ->getName() . ') == SUCCESS');
		$codePrinter->output('	; zend_hash_move_forward_ex(' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ')');
		$codePrinter->output(') {');

        if (isset($this->_statement['key'])) {
        	$codePrinter->output("\t" . 'ZEPHIR_GET_HKEY(' . $this->_statement['key'] . ', ' . $arrayHash->getName() . ', ' . $arrayPointer ->getName() . ');');
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
	 * Perform the compilation of code
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$exprRaw = $this->_statement['expr'];

		$expr = new Expression($exprRaw);
		$expression = $expr->compile($compilationContext);

		if ($expression->getType() != 'variable') {
			throw new CompilerException("Unknown type: " . $variable->getType(), $exprRaw);
		}

		$exprVariable = $compilationContext->symbolTable->getVariableForRead($expression->getCode(), $this->_statement['expr']);
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
