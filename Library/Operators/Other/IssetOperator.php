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
 * IssetOperator
 *
 * Checks if a array offset or a property is defined on a polymorphic variable
 */
class IssetOperator extends BaseOperator
{

	/**
	 * Compiles an 'isset' operator
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{

		$compilationContext->headersManager->add('kernel/array');

		switch ($expression['left']['type']) {
			case 'array-access':

				$exprVariable = new Expression($expression['left']['left']);
				$exprVariable->setReadOnly(true);
				$exprCompiledVariable = $exprVariable->compile($compilationContext);
				if ($exprCompiledVariable->getType() != 'variable') {
					throw new CompiledException("Expression type: " . $exprCompiledVariable->getType() . " cannot be used as array", $expression['left']['left']);
				}

				$variable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression['left']['left']);
				if ($variable->getType() != 'variable') {
					throw new CompiledException("Variable type: " . $variable->getType() . " cannot be used as array", $expression['left']['left']);
				}

				if ($variable->hasDifferentDynamicType(array('undefined', 'array', 'object', 'null'))) {
					$compilationContext->logger->warning('Possible attempt to use non array/object in isset operator', 'non-valid-isset', $expression);
				}

				$expr = new Expression($expression['left']['right']);
				$expr->setReadOnly(true);
				$resolvedExpr = $expr->compile($compilationContext);
				switch ($resolvedExpr->getType())	{
					case 'int':
					case 'long':
						return new CompiledExpression('bool', 'zephir_array_isset_long(' . $variable->getName() . ', ' . $resolvedExpr->getCode() . ')', $expression['left']['right']);
					case 'string':
						return new CompiledExpression('bool', 'zephir_array_isset_string(' . $variable->getName() . ', SS("' . $resolvedExpr->getCode() . '"))', $expression['left']['right']);
					case 'variable':
						$indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $expression['left']['right']);
						switch ($indexVariable->getType()) {
							case 'int':
							case 'long':
								return new CompiledExpression('bool', 'zephir_array_isset_long(' . $variable->getName() . ', ' . $indexVariable->getName() . ')', $expression['left']['right']);
							case 'variable':
							case 'string':
								return new CompiledExpression('bool', 'zephir_array_isset(' . $variable->getName() . ', ' . $indexVariable->getName() . ')', $expression['left']['right']);
							default:
								throw new CompilerException('[' . $indexVariable->getType() . ']', $expression);
						}
						break;
					default:
						throw new CompilerException('[' . $expression['left']['right']['type'] . ']', $expression);
				}
				break;
			case 'property-access':
				return new CompiledExpression('bool', '0 == 0', $expression['left']['right']);
				//return new CompiledExpression('bool', 'zephir_isset_property(' . $variable->getName() . ', SS("' . $expression['left']['right']['value'] . '") TSRMLS_CC)', $expression['left']['right']);
				break;
			case 'property-dynamic-access':
				return new CompiledExpression('bool', '0 == 0', $expression['left']['right']);
				/*$expr = new Expression($expression['left']['right']);
				$expr->setReadOnly(true);
				$resolvedExpr = $expr->compile($compilationContext);
				switch ($resolvedExpr->getType()) {
					case 'variable':
						$indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $expression['left']['right']);
						switch ($indexVariable->getType()) {
							case 'variable':
							case 'string':
								return new CompiledExpression('bool', 'zephir_isset_property_zval(' . $variable->getName() . ', ' . $indexVariable->getName() . ' TSRMLS_CC)', $expression['left']['right']);
							default:
								throw new CompilerException('[' . $indexVariable->getType() . ']', $expression);
						}
						break;
					default:
						throw new CompilerException('[' . $expression['left']['right']['type'] . ']', $expression);
				}
				break;*/
			default:
				throw new CompilerException('This expression is not valid for "isset" operator', $expression);
		}

	}

}