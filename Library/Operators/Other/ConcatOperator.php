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
 * ConcatOperator
 *
 * Perform concatenations and optimizations
 */
class ConcatOperator extends BaseOperator
{

	private function _getOptimizedConcat($expression, CompilationContext $compilationContext)
	{
		$originalExpr = $expression;

		$parts = array();
		while ($expression && isset($expression['left'])){
			$parts[] = $expression['right'];
			if ($expression['left']['type'] == 'concat') {
				$expression = $expression['left'];
			} else {
				$parts[] = $expression['left'];
				$expression = null;
			}
		}
		if ($expression) {
			$parts[] = $expression['right'];
			$parts[] = $expression['left'];
		}

		$key = '';
		$concatParts = array();
		$parts = array_reverse($parts);
		foreach ($parts as $n => $part) {

			$expr = new Expression($part);
			switch ($part['type']) {
				case 'array-access':
				case 'property-access':
					$expr->setReadOnly(true);
					break;
				default:
					$expr->setReadOnly($this->_readOnly);
					break;
			}

			$compiledExpr = $expr->compile($compilationContext);
			switch ($compiledExpr->getType()) {
				case 'variable':
					$variable = $compilationContext->symbolTable->getVariableForRead($compiledExpr->getCode(), $compilationContext, $originalExpr);
					switch ($variable->getType()) {
						case 'variable':
						case 'string':
							$key .= 'v';
							$concatParts[] = $variable->getName();
							break;
						case 'int':
						case 'long':
							$key .= 'v';
							$tempVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $originalExpr);
							$compilationContext->codePrinter->output('ZVAL_LONG(&' . $tempVariable->getName() . ', ' . $compiledExpr->getCode() . ');');
							$concatParts[] = '&' . $tempVariable->getName();
							break;
						default:
							throw new CompilerException("Variable type: " . $variable->getType() . " cannot be used in concat operation", $compiledExpr->getOriginal());
					}
					break;
				case 'string':
					$key .= 's';
					$concatParts[] = '"' . $compiledExpr->getCode() . '"';
					break;
				default:
					echo $compiledExpr->getType();
					break;
			}

		}

		$compilationContext->stringsManager->addConcatKey($key);
		return array($key, join(', ', $concatParts));
	}

	public function compile($expression, CompilationContext $compilationContext)
	{

		if (!isset($expression['left'])) {
			throw new Exception("Missing left part of the expression");
		}

		if (!isset($expression['right'])) {
			throw new Exception("Missing right part of the expression");
		}

		$compilationContext->headersManager->add('kernel/concat');

		$optimized = $this->_getOptimizedConcat($expression, $compilationContext);
		if (is_array($optimized)) {

			$expected = $this->getExpectedComplexLiteral($compilationContext, $expression);

			$compilationContext->codePrinter->output('ZEPHIR_CONCAT_' . strtoupper($optimized[0]) . '(' . $expected->getName() . ', ' . $optimized[1] . ');');

			$expected->setDynamicTypes('string');

			return new CompiledExpression('variable', $expected->getName(), $expression);
		}

		$leftExpr = new Expression($expression['left']);
		switch ($expression['left']['type']) {
			case 'array-access':
			case 'property-access':
				$leftExpr->setReadOnly(true);
				break;
			default:
				$leftExpr->setReadOnly($this->_readOnly);
				break;
		}
		$left = $leftExpr->compile($compilationContext);

		if ($left->getType() == 'variable') {
			$variableLeft = $compilationContext->symbolTable->getVariableForRead($left->getCode(), $compilationContext, $expression['right']);
		}

		$rightExpr = new Expression($expression['right']);
		switch ($expression['left']['type']) {
			case 'array-access':
			case 'property-access':
				$rightExpr->setReadOnly(true);
				break;
			default:
				$rightExpr->setReadOnly($this->_readOnly);
				break;
		}
		$right = $rightExpr->compile($compilationContext);

		if ($right->getType() == 'variable') {
			$variableLeft = $compilationContext->symbolTable->getVariableForRead($right->getCode(), $compilationContext, $expression['right']);
		}

		$expected = $this->getExpectedComplexLiteral($compilationContext, $expression);

		if ($left->getType() == 'string' && $right->getType() == 'variable') {
			$compilationContext->codePrinter->output('ZEPHIR_CONCAT_SV(' . $expected->getName() . ', "' . $left->getCode() . '", ' . $right->getCode() . ');');
		}

		if ($left->getType() == 'variable' && $right->getType() == 'string') {
			$compilationContext->codePrinter->output('ZEPHIR_CONCAT_VS(' . $expected->getName() . ', ' . $left->getCode() . ', "' . $right->getCode() . '");');
		}

		if ($left->getType() == 'variable' && $right->getType() == 'variable') {
			$compilationContext->codePrinter->output('concat_function(' . $expected->getName() . ', ' . $left->getCode() . ', ' . $right->getCode() . ' TSRMLS_CC);');
		}

		$expected->setDynamicTypes('string');

		return new CompiledExpression('variable', $expected->getName(), $expression);
	}

}
