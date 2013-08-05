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
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

/**
 * ForStatement
 *
 * For statement, the same as in PHP/C
 */
class ForStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	/**
	 * Perform the compilation of code
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$exprRaw = $this->_statement['expr'];

		$codePrinter = $compilationContext->codePrinter;

		$expr = new Expression($exprRaw);
		$expression = $expr->compile($compilationContext);

		if ($expression->getType() != 'variable') {
			throw new CompilerException("Unknown type: " . $variable->getType(), $expr);
		}

		/**
		 * Initialize 'key' variable
		 */
		if (isset($this->_statement['key'])) {
			$keyVariable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['key'], $this->_statement['expr']);
			$keyVariable->setIsInitialized(true);
		}

		/**
		 * Initialize 'value' variable
		 */
		if (isset($this->_statement['value'])) {
			$variable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['value'], $this->_statement['expr']);
			$variable->setIsInitialized(true);
		}

		/**
		 * Variables are initialized in a different way inside cycle
		 */
		$compilationContext->insideCycle++;

		$codePrinter->output('zephir_is_iterable(' . $expression->getCode() . ', &ah0, &hp0, 0, 1);');
        $codePrinter->output('while (zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) == SUCCESS) {');

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

}
