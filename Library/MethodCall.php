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

class MethodCall
{

	public function compile($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
	{

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Methods cannot be called on variable type: " . $symbolVariable->getType(), $statement);
		}

		$codePrinter = $compilationContext->codePrinter;

		$expr = $resolvedExpr->getOriginal();

		/**
		 * @TODO get variable from symbol table
		 */
		if ($expr['variable'] == 'this') {
			$expr['variable'] = 'this_ptr';
		}

		if (!isset($expr['parameters'])) {
			$codePrinter->output('zephir_call_method(' . $variable . ', ' . $expr['variable'] . ', "' . strtolower($expr['name']) . '");');
		}
	}

}