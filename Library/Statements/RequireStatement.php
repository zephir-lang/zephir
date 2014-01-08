<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
 * RequireStatement
 *
 * Require statement is used to execute PHP scripts in a given path
 */
class RequireStatement
	extends StatementAbstract
{
	/**
	 * @param CompilationContext $compilationContext
	 * @throws CompilerException
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$compilationContext->headersManager->add('kernel/require');

		$statement = $this->_statement;

		$codePrinter = $compilationContext->codePrinter;

		/**
		 * Resolve path
		 */
		$expr = new Expression($statement['expr']);
		$expr->setReadOnly(true);
		$resolvedExpr = $expr->compile($compilationContext);

		$codePrinter->output('if (zephir_require(' . $resolvedExpr->getCode() . ' TSRMLS_CC) == FAILURE) {');
		$codePrinter->output("\t" . 'RETURN_MM_NULL();');
		$codePrinter->output('}');

	}

}