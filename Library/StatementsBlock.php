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
 * StatementsBlock
 *
 * This represent a list of statements like the one in a method
 * or a if/else/while
 */
class StatementsBlock
{
	protected $_statements;

	protected $_lastStatement;

	public function __construct(array $statements)
	{
		$this->_statements = $statements;
	}

	public function compile(CompilationContext $compilationContext)
	{

		$compilationContext->codePrinter->increaseLevel();
		$compilationContext->currentBranch++;

		$statements = $this->_statements;

		foreach ($statements as $statement) {

			/*if ($statement['type'] != 'declare' && $statement['type'] != 'comment') {
				$compilationContext->codePrinter->outputNoIndent('#line ' . $statement['line'] . ' "'. $statement['file'] . '"');
			}*/

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
					break;
				case 'loop':
					$loopStatement = new LoopStatement($statement);
					$loopStatement->compile($compilationContext);
					break;
				case 'break':
					$breakStatement = new BreakStatement($statement);
					$breakStatement->compile($compilationContext);
					break;
				case 'continue':
					$continueStatement = new ContinueStatement($statement);
					$continueStatement->compile($compilationContext);
					break;
				case 'throw':
					$throwStatement = new ThrowStatement($statement);
					$throwStatement->compile($compilationContext);
					break;
				case 'mcall':
					$methodCall = new MethodCall();
					$expr = new Expression($statement['expr']);
					$expr->setExpectReturn(false);
					$methodCall->compile($expr, $compilationContext);
					break;
				case 'scall':
					$methodCall = new StaticCall();
					$expr = new Expression($statement['expr']);
					$expr->setExpectReturn(false);
					$methodCall->compile($expr, $compilationContext);
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

	public function getStatements()
	{
		return $this->_statements;
	}

	/**
	 *
	 */
	public function getLastStatementType()
	{
		return $this->_lastStatement['type'];
	}

}
