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
 * Call
 *
 * Base class for common functionality in functions/calls
 */
class Call
{

	public function getResolvedParams($parameters, $compilationContext, $expression)
	{

		$codePrinter = $compilationContext->codePrinter;

		/**
		 * @TODO: Resolve parameters properly
		 */
		$params = array();
		foreach ($parameters as $parameter) {
			$paramExpr = new Expression($parameter);
			$compiledExpression = $paramExpr->compile($compilationContext);
			switch ($compiledExpression->getType()) {
				case 'int':
				case 'uint':
				case 'long':
				case 'ulong':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_LONG(' . $parameterVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
					$params[] = $parameterVariable->getName();
					break;
				case 'double':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_DOUBLE(' . $parameterVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
					$params[] = $parameterVariable->getName();
					break;
				case 'bool':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_BOOL(' . $parameterVariable->getName() . ', ' . $compiledExpression->getBooleanCode() . ');');
					$params[] = $parameterVariable->getName();
					break;
				case 'string':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_STRING(' . $parameterVariable->getName() . ', "' . $compiledExpression->getCode() . '", 1);');
					$params[] = $parameterVariable->getName();
					break;
				case 'variable':
					$parameterVariable = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $compilationContext, $expression);
					switch ($parameterVariable->getType()) {
						case 'int':
						case 'uint':
						case 'long':
						case 'ulong':
							$parameterTempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
							$codePrinter->output('ZVAL_LONG(' . $parameterTempVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
							$params[] = $parameterTempVariable->getName();
							break;
						case 'string':
							$parameterTempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
							$codePrinter->output('ZVAL_STRINGL(' . $parameterTempVariable->getName() . ', ' . $compiledExpression->getCode() . '->str, ' . $compiledExpression->getCode() . '->len, 1);');
							$params[] = $parameterTempVariable->getName();
							break;
						case 'variable':
							$params[] = $parameterVariable->getName();
							break;
						default:
							throw new CompilerException("Cannot use variable type: " . $parameterVariable->getType() . " as parameter", $parameter);
					}
					break;
				default:
					throw new CompilerException("Cannot use value type: " . $compiledExpression->getType() . " as parameter", $parameter);
			}
		}

		return $params;
	}

	public function getReadOnlyResolvedParams($parameters, $compilationContext, $expression)
	{

		$codePrinter = $compilationContext->codePrinter;

		/**
		 * @TODO: Resolve parameters properly
		 */
		$params = array();
		foreach ($parameters as $parameter) {
			$paramExpr = new Expression($parameter);
			$compiledExpression = $paramExpr->compile($compilationContext);
			switch ($compiledExpression->getType()) {
				case 'int':
				case 'uint':
				case 'long':
				case 'ulong':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_LONG(' . $parameterVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
					$params[] = $parameterVariable->getName();
					break;
				case 'double':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_DOUBLE(' . $parameterVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
					$params[] = $parameterVariable->getName();
					break;
				case 'bool':
					$parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_BOOL(' . $parameterVariable->getName() . ', ' . $compiledExpression->getBooleanCode() . ');');
					$params[] = $parameterVariable->getName();
					break;
				case 'string':
					$parameterVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
					$codePrinter->output('ZVAL_STRING(&' . $parameterVariable->getName() . ', "' . $compiledExpression->getCode() . '", 0);');
					$params[] = $parameterVariable->getName();
					break;
				case 'variable':
					$parameterVariable = $compilationContext->symbolTable->getVariableForRead($compiledExpression->getCode(), $compilationContext, $expression);
					switch ($parameterVariable->getType()) {
						case 'int':
						case 'uint':
						case 'long':
						case 'ulong':
							$parameterTempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
							$codePrinter->output('ZVAL_LONG(' . $parameterTempVariable->getName() . ', ' . $compiledExpression->getCode() . ');');
							$params[] = $parameterTempVariable->getName();
							break;
						case 'string':
							$parameterTempVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $expression);
							$codePrinter->output('ZVAL_STRINGL(&' . $parameterTempVariable->getName() . ', ' . $compiledExpression->getCode() . '->str, ' . $compiledExpression->getCode() . '->len, 0);');
							$params[] = '&'. $parameterTempVariable->getName();
							break;
						case 'variable':
							$params[] = $parameterVariable->getName();
							break;
						default:
							throw new CompilerException("Cannot use variable type: " . $parameterVariable->getType() . " as parameter", $parameter);
					}
					break;
				default:
					throw new CompilerException("Cannot use value type: " . $compiledExpression->getType() . " as parameter", $parameter);
			}
		}

		return $params;
	}

}