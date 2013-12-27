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
 * StatementsBlock
 *
 * This represent a list of statements like the one in a method
 * or a if/else/while
 */
class StatementsBlock
{
	protected $_statements;

	protected $_unrecheable;

	protected $_debug = false;

	protected $_lastStatement;

	/**
	 *
	 */
	public function __construct(array $statements)
	{
		$this->_statements = $statements;
	}

	public function compile(CompilationContext $compilationContext, $unrecheable=false)
	{
		$compilationContext->codePrinter->increaseLevel();
		$compilationContext->currentBranch++;

		$this->_unrecheable = $unrecheable;

		$statements = $this->_statements;
		foreach ($statements as $statement) {

			/**
			 * Generate GDB hints
			 */
			if ($this->_debug) {
				if ($statement['type'] != 'declare' && $statement['type'] != 'comment') {
					$compilationContext->codePrinter->outputNoIndent('#line ' . $statement['line'] . ' "'. $statement['file'] . '"');
				}
			}

			/**
			 * Show warnings if code is generated when the 'unrecheable state' is 'on'
			 */
			if ($this->_unrecheable) {
				switch ($statement['type']) {
					case 'echo':
						$compilationContext->logger->warning('Unrecheable code', "unrecheable-code", $statement['expressions'][0]);
						break;
					case 'let':
						$compilationContext->logger->warning('Unrecheable code', "unrecheable-code", $statement['assignments'][0]);
						break;
					case 'fetch':
					case 'fcall':
					case 'mcall':
					case 'scall':
					case 'if':
					case 'while':
					case 'do-while':
					case 'switch':
					case 'for':
					case 'return':
						if (isset($statement['expr'])) {
							$compilationContext->logger->warning('Unrecheable code', "unrecheable-code", $statement['expr']);
						} else {
							$compilationContext->logger->warning('Unrecheable code', "unrecheable-code", $statement);
						}
						break;
					default:
						$compilationContext->logger->warning('Unrecheable code', "unrecheable-code", $statement);
				}
			}

			switch ($statement['type']) {
				case 'let':
					$letStatement = new LetStatement($statement);
					$letStatement->compile($compilationContext);
					break;
				case 'echo':
					$echoStatement = new EchoStatement($statement);
					$echoStatement->compile($compilationContext);
					break;
				case 'declare':
					$declareStatement = new DeclareStatement($statement);
					$declareStatement->compile($compilationContext);
					break;
				case 'if':
					$ifStatement = new IfStatement($statement);
					$ifStatement->compile($compilationContext);
					break;
				case 'while':
					$whileStatement = new WhileStatement($statement);
					$whileStatement->compile($compilationContext);
					break;
				case 'do-while':
					$whileStatement = new DoWhileStatement($statement);
					$whileStatement->compile($compilationContext);
					break;
				case 'switch':
					$switchStatement = new SwitchStatement($statement);
					$switchStatement->compile($compilationContext);
					break;
				case 'for':
					$forStatement = new ForStatement($statement);
					$forStatement->compile($compilationContext);
					break;
				case 'return':
					$returnStatement = new ReturnStatement($statement);
					$returnStatement->compile($compilationContext);
					$this->_unrecheable = true;
					break;
				case 'loop':
					$loopStatement = new LoopStatement($statement);
					$loopStatement->compile($compilationContext);
					break;
				case 'break':
					$breakStatement = new BreakStatement($statement);
					$breakStatement->compile($compilationContext);
					$this->_unrecheable = true;
					break;
				case 'continue':
					$continueStatement = new ContinueStatement($statement);
					$continueStatement->compile($compilationContext);
					$this->_unrecheable = true;
					break;
				case 'unset':
					$unsetStatement = new UnsetStatement($statement);
					$unsetStatement->compile($compilationContext);
					break;
				case 'throw':
					$throwStatement = new ThrowStatement($statement);
					$throwStatement->compile($compilationContext);
					$this->_unrecheable = true;
					break;
				case 'fetch':
					$expr = new Expression($statement['expr']);
					$expr->setExpectReturn(false);
					$expr->compile($compilationContext);
					break;
				case 'mcall':
					$methodCall = new MethodCall();
					$expr = new Expression($statement['expr']);
					$expr->setExpectReturn(false);
					$methodCall->compile($expr, $compilationContext);
					break;
				case 'fcall':
					$functionCall = new FunctionCall();
					$expr = new Expression($statement['expr']);
					$expr->setExpectReturn(false);
					$functionCall->compile($expr, $compilationContext);
					break;
				case 'scall':
					$methodCall = new StaticCall();
					$expr = new Expression($statement['expr']);
					$expr->setExpectReturn(false);
					$methodCall->compile($expr, $compilationContext);
					break;
				case 'cblock':
					$compilationContext->codePrinter->output($statement['value']);
					break;
				default:
					$compilationContext->codePrinter->output('//missing ' . $statement['type']);
			}

			if ($statement['type'] != 'comment') {
				$this->_lastStatement = $statement;
			}
		}

		/**
		 * Traverses temporal variables created in a specific branch
		 * marking them as idle
		 */
		$compilationContext->symbolTable->markTemporalVariablesIdle($compilationContext);

		$compilationContext->currentBranch--;
		$compilationContext->codePrinter->decreaseLevel();
	}

	/**
	 * Returns the statements in the block
	 *
	 * @return array
	 */
	public function getStatements()
	{
		return $this->_statements;
	}

	/**
	 * Returns the type of the last statement executed
	 *
	 * @return string
	 */
	public function getLastStatementType()
	{
		return $this->_lastStatement['type'];
	}

}
