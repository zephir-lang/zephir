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
 * UnsetStatement
 *
 * Unset statement
 */
class UnsetStatement
	extends StatementAbstract
{
	/**
	 * @param CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{

		$compilationContext->headersManager->add('kernel/array');

		$expression = $this->_statement['expr'];

		switch ($expression['type']) {
			case 'array-access':
				break;
			default:
				throw new CompilerException('Cannot use expression type: ' . $expression['type'] . ' in "unset"', $expression);
		}

		$expr = new Expression($expression['left']);
		$expr->setReadOnly(true);
		$exprVar = $expr->compile($compilationContext);

		$variable = $compilationContext->symbolTable->getVariableForWrite($exprVar->getCode(), $compilationContext, $this->_statement);
		if ($variable->getType() != 'variable') {
			throw new CompilerException('Cannot use variable type: ' . $variable->gettype() . ' in "unset"', $expression['left']);
		}

		if ($variable->hasDifferentDynamicType(array('undefined', 'array', 'object', 'null'))) {
			$compilationContext->logger->warning('Possible attempt to use non array/object in unset operator', 'non-valid-unset', $expression['left']);
		}

		$expr = new Expression($expression['right']);
		$expr->setReadOnly(true);
		$exprIndex = $expr->compile($compilationContext);

		$flags = 'PH_SEPARATE';

		switch ($exprIndex->getType()) {
			case 'int':
			case 'uint':
			case 'long':
				$compilationContext->headersManager->add('kernel/array');
				$compilationContext->codePrinter->output('zephir_array_unset_long(&' . $variable->getName() . ', ' . $exprIndex->getCode() . ', ' . $flags . ');');
				break;
			case 'string':
				$compilationContext->codePrinter->output('zephir_array_unset_string(&' . $variable->getName() . ', SS("' . $exprIndex->getCode() . '"), ' . $flags . ');');
				break;
			case 'long':
				break;
			case 'variable':
				$variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $compilationContext, $expression['right']);
				switch ($variableIndex->getType()) {
					case 'int':
					case 'uint':
					case 'long':
						$compilationContext->headersManager->add('kernel/array');
						$compilationContext->codePrinter->output('zephir_array_unset_long(&' . $variable->getName() . ', ' . $variableIndex->getName() . ', ' . $flags . ');');
						break;
					case 'string':
					case 'variable':
						$compilationContext->headersManager->add('kernel/array');
						$compilationContext->codePrinter->output('zephir_array_unset(&' . $variable->getName() . ', ' . $variableIndex->getName() . ', ' . $flags . ');');
						break;
					default:
						throw new CompilerException("Variable type: " . $variableIndex->getType() . " cannot be used as array index without cast", $expression['right']);
				}
				break;
			default:
				throw new CompilerException("Cannot use expression: " . $exprIndex->getType() . " as array index without cast", $expression['right']);
		}
	}

}
