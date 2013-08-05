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

		foreach ($this->_statements as $statement) {

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
					$callExpr = new Expression($statement['expr']);
					$methodCall = new MethodCall();
					$methodCall->compile($callExpr, $compilationContext);
					break;
				default:
					$compilationContext->codePrinter->output('//missing ' . $statement['type']);
			}

			if ($statement['type'] != 'comment') {
				$this->_lastStatement = $statement;
			}
		}

		$compilationContext->codePrinter->decreaseLevel();
	}

	/**
	 *
	 */
	public function getLastStatementType()
	{
		return $this->_lastStatement['type'];
	}

}
