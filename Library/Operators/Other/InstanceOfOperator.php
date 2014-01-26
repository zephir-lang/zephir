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
 * InstanceOf
 *
 * Checks if a variable is an instance of a class
 */
class InstanceOfOperator extends BaseOperator
{

	/**
	 *
	 * @param array $expression
	 * @param \CompilationContext $compilationContext
	 * @return \CompiledExpression
	 * @throws CompilerException
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{

		$left = new Expression($expression['left']);
		$resolved = $left->compile($compilationContext);

		if ($resolved->getType() != 'variable') {
			throw new CompilerException("InstanceOf requires a 'dynamic variable' in the left operand", $expression);
		}

		$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("InstanceOf requires a 'dynamic variable' in the left operand", $expression);
		}

		$right = new Expression($expression['right']);
		$resolved = $right->compile($compilationContext);
		$resolvedVariable = $resolved->getCode();

		switch ($resolved->getType()) {
			case 'string':
				$code = 'SL("' . trim($resolvedVariable, "\\") . '")';
				break;
			default:
				switch ($resolved->getType()) {
					case 'variable':
						if (!$compilationContext->symbolTable->hasVariable($resolvedVariable)) {
							$code = 'SL("' . Utils::addSlashes(trim($resolvedVariable, "\\"), true) . '")';
						}
						break;
					case 'property-access':
					case 'array-access':
						$compilationContext->headersManager->add('kernel/operators');
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('string', $compilationContext);
						$tempVariable->setMustInitNull(true);
						$tempVariableName = $tempVariable->getName();
						$compilationContext->codePrinter->output('zephir_get_strval(' . $tempVariableName . ', ' . $resolvedVariable . ');');
						$code = 'Z_STRVAL_P(' . $tempVariableName . '), Z_STRLEN_P(' . $tempVariableName . ')';
						break;
					default:
						throw new CompilerException("InstanceOf requires a 'variable' or a 'string' in the right operand", $expression);
				}
		}

		$compilationContext->headersManager->add('kernel/object');
		return new CompiledExpression('bool', 'zephir_is_instance_of(' . $symbolVariable->getName() . ', ' . $code . ' TSRMLS_CC)', $expression);
	}

}