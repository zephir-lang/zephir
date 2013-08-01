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
			//throw new CompilerException("Methods cannot be called on variable type: " . $symbolVariable->getType(), $statement);
		}

		$expr = $resolvedExpr->getOriginal();

		$variableVariable = $compilationContext->symbolTable->getVariableForRead($expr['variable'], $statement);

		$codePrinter = $compilationContext->codePrinter;

		$methodName = strtolower($expr['name']);

		if (!isset($expr['parameters'])) {
			if ($variable) {
				$codePrinter->output('zephir_call_method(' . $variable . ', ' . $variableVariable->getName() . ', "' . $methodName . '");');
			}
			return;
		}

		$params = array();
		if (isset($expr['parameters'])) {
			foreach ($expr['parameters'] as $parameter) {
				$expr = new Expression($parameter);
				$compiledExpression = $expr->compile($compilationContext);
				$params[] = $compiledExpression->getCode();
			}
		}

		if (count($params)) {
			$codePrinter->output('zephir_call_method_p' . count($params) . '_noret(' . $variable . ', "' . $methodName . '", ' . join(', ', $params) . ');');
		} else {
			$codePrinter->output('zephir_call_method_noret(' . $variable . ', "' . $methodName . '");');
		}

	}

}