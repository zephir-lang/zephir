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
 * LetStatement
 *
 * Let statement is used to assign variables
 */
class LetStatement
{
	protected $_statement;

	/**
	 * LetStatement constructor
	 *
	 * @param array $statement
	 */
	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	/**
	 * Compiles foo = {expr}
	 * Changes the value of a mutable variable
	 *
	 * @param string $variable
	 * @param \Variable $symbolVariable
	 * @param \CompiledExpression $resolvedExpr
	 */
	public function assignVariable($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
			ReadDetector $readDetector, CompilationContext $compilationContext, $statement)
	{

		if ($symbolVariable->isReadOnly())  {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is read only", $statement);
		}

		$codePrinter = $compilationContext->codePrinter;

		/**
		 * Only initialize variables if it's direct assignment
		 */
		if ($statement['operator'] == 'assign') {
			$symbolVariable->setIsInitialized(true);
		} else {
			if (!$symbolVariable->isInitialized()) {
				throw new CompilerException("Cannot write variable '" . $variable . "' because it is not initialized", $statement);
			}
		}

		$type = $symbolVariable->getType();
		switch ($type) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
			case 'char':
			case 'uchar':
				switch ($resolvedExpr->getType()) {
					case 'null':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = 0;');
								break;
							case 'add-assign':
								$codePrinter->output($variable . ' += 0;');
								break;
							case 'sub-assign':
								$codePrinter->output($variable . ' -= 0;');
								break;
							case 'mul-assign':
								$codePrinter->output($variable . ' *= 0;');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
						}
						break;
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = ' . $resolvedExpr->getCode() . ';');
								break;
							case 'add-assign':
								$codePrinter->output($variable . ' += ' . $resolvedExpr->getCode() . ';');
								break;
							case 'sub-assign':
								$codePrinter->output($variable . ' -= ' . $resolvedExpr->getCode() . ';');
								break;
							case 'mul-assign':
								$codePrinter->output($variable . ' *= ' . $resolvedExpr->getCode() . ';');
								break;
							case 'div-assign':
								$codePrinter->output($variable . ' /= ' . $resolvedExpr->getCode() . ';');
								break;
							case 'mod-assign':
								$codePrinter->output($variable . ' %= ' . $resolvedExpr->getCode() . ';');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
						}
						break;
					case 'char':
					case 'uchar':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = \'' . $resolvedExpr->getCode() . '\';');
								break;
							case 'add-assign':
								$codePrinter->output($variable . ' += \'' . $resolvedExpr->getCode() . '\';');
								break;
							case 'sub-assign':
								$codePrinter->output($variable . ' -= \'' . $resolvedExpr->getCode() . '\';');
								break;
							case 'mul-assign':
								$codePrinter->output($variable . ' *= \'' . $resolvedExpr->getCode() . '\';');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
						}
						break;
					case 'double':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = (long) (' . $resolvedExpr->getCode() . ');');
								break;
							case 'add-assign':
								$codePrinter->output($variable . ' += (long) (' . $resolvedExpr->getCode() . ');');
								break;
							case 'sub-assign':
								$codePrinter->output($variable . ' -= (long) (' . $resolvedExpr->getCode() . ');');
								break;
							case 'mul-assign':
								$codePrinter->output($variable . ' *= (long) (' . $resolvedExpr->getCode() . ');');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
						}
						break;
					case 'bool':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
								break;
							case 'add-assign':
								$codePrinter->output($variable . ' += ' . $resolvedExpr->getBooleanCode() . ';');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
						}
						break;
					case 'variable':
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
						switch ($itemVariable->getType()) {
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
							case 'bool':
							case 'char':
							case 'uchar':
								switch ($statement['operator']) {
									case 'assign':
										$codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
										break;
									case 'add-assign':
										$codePrinter->output($variable . ' += ' . $itemVariable->getName() . ';');
										break;
									case 'sub-assign':
										$codePrinter->output($variable . ' -= ' . $itemVariable->getName() . ';');
										break;
									case 'mul-assign':
										$codePrinter->output($variable . ' *= ' . $itemVariable->getName() . ';');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
								}
								break;
							case 'double':
								switch ($statement['operator']) {
									case 'assign':
										$codePrinter->output($variable . ' = (long) ' . $itemVariable->getName() . ';');
										break;
									case 'add-assign':
										$codePrinter->output($variable . ' += (long) ' . $itemVariable->getName() . ';');
										break;
									case 'sub-assign':
										$codePrinter->output($variable . ' -= (long) ' . $itemVariable->getName() . ';');
										break;
									case 'mul-assign':
										$codePrinter->output($variable . ' *= (long) ' . $itemVariable->getName() . ';');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
								}
								break;
							case 'variable':
								$compilationContext->headersManager->add('kernel/operators');
								switch ($statement['operator']) {
									case 'assign':
										$codePrinter->output($variable . ' = zephir_get_numberval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
										break;
									case 'add-assign':
										$codePrinter->output($variable . ' += zephir_get_numberval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
										break;
									case 'sub-assign':
										$codePrinter->output($variable . ' -= zephir_get_numberval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
										break;
									case 'mul-assign':
										$codePrinter->output($variable . ' *= zephir_get_numberval(' . $resolvedExpr->resolve(null, $compilationContext) . ');');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: int", $statement);
								}
								break;
							default:
								throw new CompilerException("Unknown type: " . $itemVariable->getType(), $statement);
						}
						break;
					default:
						throw new CompilerException("Value type '" . $resolvedExpr->getType() . "' cannot be assigned to variable: int", $statement);
				}
				break;
			case 'double':
				switch ($resolvedExpr->getType()) {
					case 'null':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = 0.0;');
								break;
							case 'add-assign':
								$codePrinter->output($variable . ' += 0.0;');
								break;
							case 'sub-assign':
								$codePrinter->output($variable . ' -= 0.0;');
								break;
							case 'mul-assign':
								$codePrinter->output($variable . ' *= 0.0;');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
						}
						break;
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = (double) (' . $resolvedExpr->getCode() . ');');
								break;
							case 'add-assign':
								$codePrinter->output($variable . ' += (double) (' . $resolvedExpr->getCode() . ');');
								break;
							case 'sub-assign':
								$codePrinter->output($variable . ' -= (double) (' . $resolvedExpr->getCode() . ');');
								break;
							case 'mul-assign':
								$codePrinter->output($variable . ' *= (double) (' . $resolvedExpr->getCode() . ');');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
						}
						break;
					case 'double':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = ' . $resolvedExpr->getCode() . ';');
								break;
							case 'add-assign':
								$codePrinter->output($variable . ' += ' . $resolvedExpr->getCode() . ';');
								break;
							case 'sub-assign':
								$codePrinter->output($variable . ' -= ' . $resolvedExpr->getCode() . ';');
								break;
							case 'mul-assign':
								$codePrinter->output($variable . ' *= ' . $resolvedExpr->getCode() . ';');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
						}
						break;
					case 'bool':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
								break;
							case 'add-assign':
								$codePrinter->output($variable . ' += ' . $resolvedExpr->getBooleanCode() . ';');
								break;
							case 'sub-assign':
								$codePrinter->output($variable . ' -= ' . $resolvedExpr->getBooleanCode() . ';');
								break;
							case 'mul-assign':
								$codePrinter->output($variable . ' *= ' . $resolvedExpr->getBooleanCode() . ';');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
						}
						break;
					case 'variable':
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
						switch ($itemVariable->getType()) {
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
							case 'bool':
								switch ($statement['operator']) {
									case 'assign':
										$codePrinter->output($variable . ' = (double) ' . $itemVariable->getName() . ';');
										break;
									case 'add-assign':
										$codePrinter->output($variable . ' += (double) ' . $itemVariable->getName() . ';');
										break;
									case 'sub-assign':
										$codePrinter->output($variable . ' -= (double) ' . $itemVariable->getName() . ';');
										break;
									case 'mul-assign':
										$codePrinter->output($variable . ' *= (double) ' . $itemVariable->getName() . ';');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
								}
								break;
							case 'double':
								switch ($statement['operator']) {
									case 'assign':
										$codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
										break;
									case 'add-assign':
										$codePrinter->output($variable . ' += ' . $itemVariable->getName() . ';');
										break;
									case 'sub-assign':
										$codePrinter->output($variable . ' -= ' . $itemVariable->getName() . ';');
										break;
									case 'mul-assign':
										$codePrinter->output($variable . ' *= ' . $itemVariable->getName() . ';');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
								}
								break;
							case 'variable':
								$compilationContext->headersManager->add('kernel/operators');
								switch ($statement['operator']) {
									case 'assign':
										$codePrinter->output($variable . ' = zephir_get_numberval(' . $itemVariable->getName() . ');');
										break;
									case 'add-assign':
										$codePrinter->output($variable . ' += zephir_get_numberval(' . $itemVariable->getName() . ');');
										break;
									case 'sub-assign':
										$codePrinter->output($variable . ' -= zephir_get_numberval(' . $itemVariable->getName() . ');');
										break;
									case 'mul-assign':
										$codePrinter->output($variable . ' *= zephir_get_numberval(' . $itemVariable->getName() . ');');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: double", $statement);
								}
								break;
							default:
								throw new CompilerException("Unknown type: " . $itemVariable->getType(), $statement);
						}
						break;
					default:
						throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
				}
				break;
			case 'string':
				switch ($resolvedExpr->getType()) {
					case 'null':
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->initVariant($compilationContext);
								$codePrinter->output('ZVAL_EMPTY_STRING(' . $variable . ');');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
						}
						break;
					case 'string':
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->initVariant($compilationContext);
								if ($resolvedExpr->getCode()) {
									$codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
								} else {
									$codePrinter->output('ZVAL_EMPTY_STRING(' . $variable . ');');
								}
								break;
							case 'concat-assign':
								$codePrinter->output('zephir_concat_self_str(&' . $variable . ', "' . $resolvedExpr->getCode() . '", sizeof("' . $resolvedExpr->getCode() . '")-1 TSRMLS_CC);');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
						}
						break;
					case 'char':
					case 'uchar':
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->initVariant($compilationContext);
								if ($resolvedExpr->getCode()) {
									$codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
								} else {
									$codePrinter->output('ZVAL_EMPTY_STRING(' . $variable . ');');
								}
								break;
							case 'concat-assign':
								$compilationContext->headersManager->add('kernel/operators');
								$codePrinter->output('zephir_concat_self_str(&' . $variable . ', "' . $resolvedExpr->getCode() . '", sizeof("' . $resolvedExpr->getCode() . '")-1 TSRMLS_CC);');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
						}
						break;
					case 'variable':
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
						switch ($itemVariable->getType()) {
							case 'string':
								switch ($statement['operator']) {
									case 'assign':
										$symbolVariable->setMustInitNull(true);
										$codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $itemVariable->getName() . ');');
										break;
									case 'concat-assign':
										$compilationContext->headersManager->add('kernel/operators');
										$codePrinter->output('zephir_concat_self(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
								}
								break;
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
								switch ($statement['operator']) {
									case 'assign':
										$symbolVariable->initVariant($compilationContext);
										$compilationContext->headersManager->add('kernel/string');
										$codePrinter->output('Z_STRLEN_P(' . $variable . ') = zephir_spprintf(&Z_STRVAL_P(' . $variable . '), 0, "%ld", ' . $itemVariable->getName() . ');');
										$codePrinter->output('Z_TYPE_P(' . $variable . ') = IS_STRING;');
										break;
									case 'concat-assign':
										$compilationContext->headersManager->add('kernel/operators');
										$codePrinter->output('zephir_concat_self_long(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
								}
								break;
							case 'char':
							case 'uchar':
								switch ($statement['operator']) {
									case 'assign':
										$symbolVariable->initVariant($compilationContext);
										$codePrinter->output('Z_STRLEN_P(' . $variable . ') = zend_spprintf(&Z_STRVAL_P(' . $variable . '), 0, "%c", ' . $itemVariable->getName() . ');');
										$codePrinter->output('Z_TYPE_P(' . $variable . ') = IS_STRING;');
										break;
									case 'concat-assign':
										$compilationContext->headersManager->add('kernel/operators');
										$codePrinter->output('zephir_concat_self_char(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
								}
								break;
							case 'variable':
								switch ($statement['operator']) {
									case 'assign':
										$symbolVariable->initVariant($compilationContext);
										//$compilationContext->headersManager->add('kernel/string_type');
										//$codePrinter->output('zephir_str_assign_long(' . $variable . ', zephir_get_intval(' . $itemVariable->getName() . '));');
										break;
									case 'concat-assign':
										//$compilationContext->headersManager->add('kernel/string_type');
										//$codePrinter->output('zephir_str_append_long(' . $variable . ', ' . $itemVariable->getName() . ');');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
								}
								break;
							default:
								throw new CompilerException("Unknown type: " . $itemVariable->getType(), $statement);
						}
						break;
					default:
						throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
				}
				break;
			case 'bool':
				switch ($resolvedExpr->getType()) {
					case 'null':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = 0;');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: null", $statement);
						}
						break;
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
					case 'double':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = (' . $resolvedExpr->getCode() . ') ? 1 : 0;');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $statement);
						}
						break;
					case 'bool':
						switch ($statement['operator']) {
							case 'assign':
								$codePrinter->output($variable . ' = ' . $resolvedExpr->getBooleanCode() . ';');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $statement);
						}
						break;
					case 'variable':
						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
						switch ($itemVariable->getType()) {
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
							case 'double':
								switch ($statement['operator']) {
									case 'assign':
										$codePrinter->output($variable . ' = (' . $itemVariable->getName() . ') ? 1 : 0;');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
								}
								break;
							case 'bool':
								switch ($statement['operator']) {
									case 'assign':
										$codePrinter->output($variable . ' = ' . $itemVariable->getName() . ';');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
								}
								break;
							case 'variable':
								switch ($statement['operator']) {
									case 'assign':
										$codePrinter->output($variable . ' = zephir_get_boolval(' . $itemVariable->getName() . ');');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
								}
								break;
							default:
								throw new CompilerException("Unknown type: " . $itemVariable->getType(), $statement);
						}
						break;
					default:
						throw new CompilerException("Unknown type: " . $resolvedExpr->getType(), $statement);
				}
				break;

			case 'variable':
				switch ($resolvedExpr->getType()) {
					case 'null':
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->initVariant($compilationContext);
								$symbolVariable->setDynamicTypes('null');
								if ($symbolVariable->isLocalOnly()) {
									$codePrinter->output('ZVAL_NULL(&' . $variable . ');');
								} else {
									$codePrinter->output('ZVAL_NULL(' . $variable . ');');
								}
								break;
						}
						break;
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						if ($symbolVariable->isLocalOnly()) {
							$symbol = '&' . $variable;
						} else {
							$symbol = $variable;
						}
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->setDynamicTypes('long');
								if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
									$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('int', $compilationContext);
									$codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getCode() . ';');
									$symbolVariable->initVariant($compilationContext);
									$codePrinter->output('ZVAL_LONG(' . $symbol . ', ' . $tempVariable->getName() . ');');
								} else {
									$symbolVariable->initVariant($compilationContext);
									$codePrinter->output('ZVAL_LONG(' . $symbol . ', ' . $resolvedExpr->getCode() . ');');
								}
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
						}
						break;
					case 'char':
					case 'uchar':
						if ($symbolVariable->isLocalOnly()) {
							$symbol = '&' . $variable;
						} else {
							$symbol = $variable;
						}
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->setDynamicTypes('long');
								if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
									$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('char', $compilationContext);
									$codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getCode() . ';');
									$symbolVariable->initVariant($compilationContext);
									$codePrinter->output('ZVAL_LONG(' . $symbol . ', ' . $tempVariable->getName() . ');');
								} else {
									$symbolVariable->initVariant($compilationContext);
									$codePrinter->output('ZVAL_LONG(' . $symbol . ', \'' . $resolvedExpr->getCode() . '\');');
								}
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
						}
						break;
					case 'double':
						if ($symbolVariable->isLocalOnly()) {
							$symbol = '&' . $variable;
						} else {
							$symbol = $variable;
						}
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->setDynamicTypes('double');
								if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
									$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('double', $compilationContext);
									$codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getCode() . ';');
									$symbolVariable->initVariant($compilationContext);
									$codePrinter->output('ZVAL_DOUBLE(' . $symbol . ', ' . $tempVariable->getName() . ');');
								} else {
									$symbolVariable->initVariant($compilationContext);
									$codePrinter->output('ZVAL_DOUBLE(' . $symbol . ', ' . $resolvedExpr->getCode() . ');');
								}
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
						}
						break;
					case 'bool':
						if ($symbolVariable->isLocalOnly()) {
							$symbol = '&' . $variable;
						} else {
							$symbol = $variable;
						}
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->setDynamicTypes('bool');
								if ($resolvedExpr->getCode() == 'true') {
									$symbolVariable->initVariant($compilationContext);
									$codePrinter->output('ZVAL_BOOL(' . $symbol . ', 1);');
								} else {
									if ($resolvedExpr->getCode() == 'false') {
										$symbolVariable->initVariant($compilationContext);
										$codePrinter->output('ZVAL_BOOL(' . $symbol . ', 0);');
									} else {
										if ($readDetector->detect($variable, $resolvedExpr->getOriginal())) {
											$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('double', $compilationContext);
											$codePrinter->output($tempVariable->getName() . ' = ' . $resolvedExpr->getBooleanCode() . ';');
											$symbolVariable->initVariant($compilationContext);
											$codePrinter->output('ZVAL_BOOL(' . $symbol . ', ' . $tempVariable->getName() . ');');
										} else {
											$symbolVariable->initVariant($compilationContext);
											$codePrinter->output('ZVAL_BOOL(' . $symbol . ', ' . $resolvedExpr->getBooleanCode() . ');');
										}
									}
								}
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
						}
						break;
					case 'string':
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->initVariant($compilationContext);
								$symbolVariable->setDynamicTypes('string');
								if ($symbolVariable->isLocalOnly()) {
									$codePrinter->output('ZVAL_STRING(&' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
								} else {
									$codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
								}
								break;
							case 'concat-assign':
								$compilationContext->headersManager->add('kernel/operators');
								$codePrinter->output('zephir_concat_self_str(&' . $variable . ', SL("' . $resolvedExpr->getCode() . '") TSRMLS_CC);');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
						}
						break;
					case 'variable':

						$itemVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $resolvedExpr->getOriginal());
						switch ($itemVariable->getType()) {
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
							case 'char':
							case 'uchar':
								switch ($statement['operator']) {
									case 'assign':
										$symbolVariable->initVariant($compilationContext);
										$symbolVariable->setDynamicTypes('long');
										if ($symbolVariable->isLocalOnly()) {
											$codePrinter->output('ZVAL_LONG(&' . $variable . ', ' . $itemVariable->getName() . ');');
										} else {
											$codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $itemVariable->getName() . ');');
										}
										break;
									case 'add-assign':
										$compilationContext->headersManager->add('kernel/operators');
										$symbolVariable->initVariant($compilationContext);
										$symbolVariable->setDynamicTypes('long');
										$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite($itemVariable->getType(), $compilationContext);
										if ($symbolVariable->isLocalOnly()) {
											$codePrinter->output($tempVariable->getName() . ' = zephir_get_numberval(&' . $variable . ');');
											$codePrinter->output('ZVAL_LONG(&' . $variable . ', ' . $tempVariable->getName() . ' + ' . $itemVariable->getName() . ');');
										} else {
											$codePrinter->output($tempVariable->getName() . ' = zephir_get_numberval(' . $variable . ');');
											$codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $tempVariable->getName() . ' + ' . $itemVariable->getName() . ');');
										}
										break;
									case 'sub-assign':
										$compilationContext->headersManager->add('kernel/operators');
										$symbolVariable->initVariant($compilationContext);
										$symbolVariable->setDynamicTypes('long');
										$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite($itemVariable->getType(), $compilationContext);
										if ($symbolVariable->isLocalOnly()) {
											$codePrinter->output($tempVariable->getName() . ' = zephir_get_numberval(&' . $variable . ');');
											$codePrinter->output('ZVAL_LONG(&' . $variable . ', ' . $tempVariable->getName() . ' - ' . $itemVariable->getName() . ');');
										} else {
											$codePrinter->output($tempVariable->getName() . ' = zephir_get_numberval(' . $variable . ');');
											$codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $tempVariable->getName() . ' - ' . $itemVariable->getName() . ');');
										}
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
								}
								break;
							case 'double':
								switch ($statement['operator']) {
									case 'assign':
										$symbolVariable->initVariant($compilationContext);
										$symbolVariable->setDynamicTypes('double');
										if ($symbolVariable->isLocalOnly()) {
											$codePrinter->output('ZVAL_DOUBLE(&' . $variable . ', ' . $itemVariable->getName() . ');');
										} else {
											$codePrinter->output('ZVAL_DOUBLE(' . $variable . ', ' . $itemVariable->getName() . ');');
										}
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
								}
								break;
							case 'bool':
								switch ($statement['operator']) {
									case 'assign':
										$symbolVariable->initVariant($compilationContext);
										$symbolVariable->setDynamicTypes('bool');
										if ($symbolVariable->isLocalOnly()) {
											$codePrinter->output('ZVAL_BOOL(&' . $variable . ', ' . $itemVariable->getName() . ');');
										} else {
											$codePrinter->output('ZVAL_BOOL(' . $variable . ', ' . $itemVariable->getName() . ');');
										}
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
								}
								break;
							case 'variable':
								switch ($statement['operator']) {
									case 'assign':
										if ($itemVariable->getName() != $variable) {

											$symbolVariable->setMustInitNull(true);
											$compilationContext->symbolTable->mustGrownStack(true);

											/* Inherit the dynamic type data from the assigned value */
											$symbolVariable->setDynamicTypes($itemVariable->getDynamicTypes());
											$symbolVariable->setClassTypes($itemVariable->getClassTypes());

											$codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $itemVariable->getName() . ');');
											if ($itemVariable->isTemporal()) {
												$itemVariable->setIdle(true);
											}
										}
										break;
									case 'concat-assign':
										$compilationContext->headersManager->add('kernel/operators');
										$codePrinter->output('zephir_concat_self(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
										break;
									case 'add-assign':
										$compilationContext->symbolTable->mustGrownStack(true);
										$compilationContext->headersManager->add('kernel/operators');
										$codePrinter->output('ZEPHIR_ADD_ASSIGN(' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
										break;
									case 'sub-assign':
										$compilationContext->symbolTable->mustGrownStack(true);
										$compilationContext->headersManager->add('kernel/operators');
										$codePrinter->output('ZEPHIR_SUB_ASSIGN(' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
								}
								break;
							case 'string':
								switch ($statement['operator']) {
									case 'assign':
										if ($itemVariable->getName() != $variable) {

											$symbolVariable->setMustInitNull(true);
											$compilationContext->symbolTable->mustGrownStack(true);

											/* Inherit the dynamic type data from the assigned value */
											$symbolVariable->setDynamicTypes($itemVariable->getDynamicTypes());
											$symbolVariable->setClassTypes($itemVariable->getClassTypes());

											$codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $itemVariable->getName() . ');');
											if ($itemVariable->isTemporal()) {
												$itemVariable->setIdle(true);
											}
										}
										break;
									case 'concat-assign':
										$compilationContext->headersManager->add('kernel/operators');
										$codePrinter->output('zephir_concat_self(&' . $variable . ', ' . $itemVariable->getName() . ' TSRMLS_CC);');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: " . $itemVariable->getType(), $statement);
								}
								break;
							default:
								throw new CompilerException("Unknown type: " . $itemVariable->getType(), $resolvedExpr->getOriginal());
						}
						break;
					default:
						throw new CompilerException("Unknown type: " . $resolvedExpr->getType(), $resolvedExpr->getOriginal());
				}
				break;
			default:
				throw new CompilerException("Unknown type: " . $type, $statement);
		}
	}

	/**
	 * Compiles foo[] = {expr}
	 *
	 * @param string $variable
	 * @param Variable $symbolVariable
	 * @param CompiledExpression $resolvedExpr
	 * @param CompilationContext $compilationContext
	 * @param array $statement
	 */
	public function assignVariableAppend($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		if (!$symbolVariable->isInitialized()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is not initialized", $statement);
		}

		if ($symbolVariable->isReadOnly())  {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is read only", $statement);
		}

		if ($symbolVariable->isLocalOnly()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is local only", $statement);
		}

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Cannot use variable type: '" . $symbolVariable->getType() . "' as array", $statement);
		}

		if ($symbolVariable->hasDifferentDynamicType(array('undefined', 'array'))) {
			$compilationContext->logger->warning('Possible attempt to append elements on a non-array dynamic variable', 'non-array-append', $statement);
		}

		$codePrinter = $compilationContext->codePrinter;

		$compilationContext->headersManager->add('kernel/array');

		$type = $symbolVariable->getType();
		switch ($type) {
			/*case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
				$codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $exprVariable->getName() . ');');
				$codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE);');
				break;*/
			case 'variable':
				switch ($resolvedExpr->getType()) {
					case 'null':
						$codePrinter->output('zephir_array_append(&' . $variable . ', ZEPHIR_GLOBAL(global_null), PH_SEPARATE);');
						break;
					case 'string':
						$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
						$codePrinter->output('ZVAL_STRING(' . $symbolVariable->getName() . ', "' . Utils::addSlashes($resolvedExpr->getCode()) . '", 1);');
						$codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE);');
						break;
					case 'variable':
						$exprVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
						switch ($exprVariable->getType()) {
							case 'int':
							case 'uint':
							case 'long':
							case 'ulong':
								$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
								$codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $exprVariable->getName() . ');');
								$codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE);');
								break;
							case 'variable':
								$codePrinter->output('zephir_array_append(&' . $variable . ', ' . $exprVariable->getName() . ', PH_SEPARATE);');
								break;
							default:
								throw new CompilerException("Unknown type " . $exprVariable->getType(), $statement);
						}
						break;
					default:
						throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
				}
				break;
			default:
				throw new CompilerException("Unknown type", $statement);
		}
	}

	/**
	 * Resolves an item that will be assigned to an array offset
	 *
	 * @param CompiledExpr $resolvedExpr
	 * @param CompilationContext $compilationContext
	 */
	protected function _getResolvedArrayItem($resolvedExpr, $compilationContext)
	{
		$codePrinter = $compilationContext->codePrinter;

		switch ($resolvedExpr->getType()) {
			case 'null':
				$symbolVariable = new GlobalConstant('ZEPHIR_GLOBAL(global_null)');
				break;
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
				$codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $resolvedExpr->getCode() . ');');
				break;
			case 'bool':
				if ($resolvedExpr->getBooleanCode() == '1') {
					$symbolVariable = new GlobalConstant('ZEPHIR_GLOBAL(global_true)');
				} else {
					if ($resolvedExpr->getBooleanCode() == '0') {
						$symbolVariable = new GlobalConstant('ZEPHIR_GLOBAL(global_false)');
					} else {
						$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
						$codePrinter->output('ZVAL_BOOL(' . $symbolVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
					}
				}
				break;
			case 'string':
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
				$codePrinter->output('ZVAL_STRING(' . $symbolVariable->getName() . ', "' . $resolvedExpr->getCode() . '", 1);');
				break;
			case 'variable':
				$variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $resolvedExpr->getOriginal());
				switch ($variableExpr->getType()) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
						$codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $variableExpr->getName() . ');');
						break;
					case 'bool':
						$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $resolvedExpr->getOriginal());
						$codePrinter->output('ZVAL_BOOL(' . $symbolVariable->getName() . ', ' . $variableExpr->getName() . ');');
						break;
					case 'variable':
						$symbolVariable = $variableExpr;
						break;
					default:
						throw new CompilerException("Variable: " . $exprIndex->getType() . " cannot be assigned to array offset", $resolvedExpr->getOriginal());
				}
				break;
			default:
				throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be assigned to array offset", $resolvedExpr->getOriginal());
		}

		return $symbolVariable;
	}

	/**
	 * Compiles foo[y] = {expr} (one offset)
	 *
	 * @param string $variable
	 * @param Variable $symbolVariable
	 * @param CompiledExpression $resolvedExpr
	 * @param CompilationContext $compilationContext
	 * @param array $statement
	 */
	protected function _assignArrayIndexSingle($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		$expression = new Expression($statement['index-expr'][0]);
		$exprIndex = $expression->compile($compilationContext);

		switch ($exprIndex->getType()) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
			case 'string':
			case 'variable':
				break;
			default:
				throw new CompilerException("Index: " . $exprIndex->getType() . " cannot be used as array offset in assigment without cast", $statement['index-expr'][0]);
		}

		$codePrinter = $compilationContext->codePrinter;

		/**
		 * Create a temporal zval (if needed)
		 */
		$symbolVariable = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

		$compilationContext->headersManager->add('kernel/array');
		switch ($exprIndex->getType()) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
				$codePrinter->output('zephir_array_update_long(&' . $variable . ', ' . $exprIndex->getCode() . ', &' . $symbolVariable->getName() . ', PH_COPY | PH_SEPARATE);');
				break;
			case 'string':
				$codePrinter->output('zephir_array_update_string(&' . $variable . ', SL("' . $exprIndex->getCode() . '"), &' . $symbolVariable->getName() . ', PH_COPY | PH_SEPARATE);');
				break;
			case 'variable':
				$variableIndex = $compilationContext->symbolTable->getVariableForRead($exprIndex->getCode(), $compilationContext, $statement);
				switch ($variableIndex->getType()) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						$codePrinter->output('zephir_array_update_long(&' . $variable . ', ' . $variableIndex->getName() . ', &' . $symbolVariable->getName() . ', PH_COPY | PH_SEPARATE);');
						break;
					case 'string':
						$codePrinter->output('zephir_array_update_zval(&' . $variable . ', ' . $variableIndex->getName() . ', &' . $symbolVariable->getName() . ', PH_COPY | PH_SEPARATE);');
						break;
					case 'variable':
						$codePrinter->output('zephir_array_update_zval(&' . $variable . ', ' . $variableIndex->getName() . ', &' . $symbolVariable->getName() . ', PH_COPY | PH_SEPARATE);');
						break;
					default:
						throw new CompilerException("Variable: " . $variableIndex->getType() . " cannot be used as array index", $statement);
				}
				break;
			default:
				throw new CompilerException("Value: " . $exprIndex->getType() . " cannot be used as array index", $statement);
		}

	}

	/**
	 * Compiles foo[y][x] = {expr} (multiple offset)
	 *
	 * @param string $variable
	 * @param Variable $symbolVariable
	 * @param CompiledExpression $resolvedExpr
	 * @param CompilationContext $compilationContext
	 * @param array $statement
	 */
	protected function _assignArrayIndexMultiple($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		$offsetExprs = array();
		foreach ($statement['index-expr'] as $indexExpr) {

			$expression = new Expression($indexExpr);
			$exprIndex = $expression->compile($compilationContext);

			switch ($exprIndex->getType()) {
				case 'int':
				case 'uint':
				case 'long':
				case 'ulong':
				case 'string':
				case 'variable':
					break;
				default:
					throw new CompilerException("Index: " . $exprIndex->getType() . " cannot be used as array index in assigment without cast", $indexExpr);
			}

			$offsetExprs[] = $exprIndex;
		}

		$compilationContext->headersManager->add('kernel/array');

		/**
		 * Create a temporal zval (if needed)
		 */
		$symbolVariable = $this->_getResolvedArrayItem($resolvedExpr, $compilationContext);

		$keys = '';
		$numberParams = 0;
		$offsetItems = array();
		foreach ($offsetExprs as $offsetExpr) {
			switch ($offsetExpr->getType()) {
				case 'int':
				case 'uint':
				case 'long':
				case 'ulong':
					$keys .= 'l';
					$offsetItems[] = $offsetExpr->getCode();
					$numberParams++;
					break;
				case 'string':
					$keys .= 's';
					$offsetItems[] = 'SL("' . $offsetExpr->getCode() . '")';
					$numberParams += 2;
					break;
				case 'variable':
					$variableIndex = $compilationContext->symbolTable->getVariableForRead($offsetExpr->getCode(), $compilationContext, $statement);
					switch ($variableIndex->getType()) {
						case 'int':
						case 'uint':
						case 'long':
						case 'ulong':
							$keys .= 'l';
							$offsetItems[] = $variableIndex->getName();
							$numberParams++;
							break;
						case 'string':
						case 'variable':
							$keys .= 'z';
							$offsetItems[] = $variableIndex->getName();
							$numberParams++;
							break;
						default:
							throw new CompilerException("Variable: " . $variableIndex->getType() . " cannot be used as array index", $statement);
					}
					break;
				default:
					throw new CompilerException("Value: " . $offsetExpr->getType() . " cannot be used as array index", $statement);
			}
		}

		$codePrinter->output('zephir_array_update_multi(&' .
			$variable .
			', &' . $symbolVariable->getName() . ' TSRMLS_CC, SL("' . $keys . '"), ' .
			$numberParams . ', ' . join(', ', $offsetItems) .
		');');
	}

	/**
	 * Compiles foo[y] = {expr}
	 *
	 * @param string $variable
	 * @param Variable $symbolVariable
	 * @param CompiledExpression $resolvedExpr
	 * @param CompilationContext $compilationContext
	 * @param array $statement
	 */
	public function assignArrayIndex($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		/**
		 * Arrays must be stored in the HEAP
		 */
		if ($symbolVariable->isLocalOnly()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is local only", $statement);
		}

		if (!$symbolVariable->isInitialized()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is not initialized", $statement);
		}

		if ($symbolVariable->isReadOnly())  {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is read only", $statement);
		}

		if ($symbolVariable->isLocalOnly()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is local only", $statement);
		}

		/**
		 * Only dynamic variables can be used as arrays
		 */
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Cannot use variable type: '" . $symbolVariable->getType() . "' as array", $statement);
		}

		if ($symbolVariable->hasAnyDynamicType('unknown')) {
			throw new CompilerException("Cannot use non-initialized variable as an object", $statement);
		}

		/**
		 * Trying to use a non-object dynamic variable as object
		 */
		if ($symbolVariable->hasDifferentDynamicType(array('undefined', 'array', 'null'))) {
			$compilationContext->logger->warning('Possible attempt to update index on a non-array dynamic variable', 'non-array-update', $statement);
		}

		/**
		 * Choose one-offset or multiple-offset functions
		 */
		if (count($statement['index-expr']) == 1) {
			$this->_assignArrayIndexSingle($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
		} else {
			$this->_assignArrayIndexMultiple($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
		}

	}

	/**
	 * Compiles x++
	 *
	 * @param string $variable
	 * @param Variable $symbolVariable
	 * @param CompilationContext $compilationContext
	 * @param array $statement
	 */
	public function assignIncr($variable, Variable $symbolVariable, CompilationContext $compilationContext, $statement)
	{

		if (!$symbolVariable->isInitialized()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is not initialized", $statement);
		}

		if ($symbolVariable->isReadOnly()) {
			/**
			 * @TODO implement increment of objects members
			 */
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is read only", $statement);
		}

		$codePrinter = $compilationContext->codePrinter;

		switch ($symbolVariable->getType()) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
			case 'double':
			case 'char':
			case 'uchar':
				$codePrinter->output($variable . '++;');
				break;
			case 'variable':

				/**
				 * Update non-numeric dynamic variables could be expensive
				 */
				if (!$symbolVariable->hasAnyDynamicType(array('undefined', 'long', 'double'))) {
					$compilationContext->logger->warning('Possible attempt to increment non-numeric dynamic variable', 'non-valid-increment', $statement);
				}

				$compilationContext->headersManager->add('kernel/operators');
				if ($symbolVariable->isLocalOnly()) {
					$codePrinter->output('zephir_increment(&' . $variable . ');');
				} else {
					$symbolVariable->separate($compilationContext);
					$codePrinter->output('zephir_increment(' . $variable . ');');
				}
				break;
			default:
				throw new CompilerException("Cannot increment: " . $symbolVariable->getType(), $statement);
		}
	}

	/**
	 * Compiles x--
	 *
	 * @param string $variable
	 * @param Variable $symbolVariable
	 * @param CompilationContext $compilationContext
	 * @param array $statement
	 */
	public function assignDecr($variable, Variable $symbolVariable, CompilationContext $compilationContext, $statement)
	{

		if (!$symbolVariable->isInitialized()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is not initialized", $statement);
		}

		if ($symbolVariable->isReadOnly()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is read only", $statement);
		}

		$codePrinter = $compilationContext->codePrinter;

		switch ($symbolVariable->getType()) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
			case 'double':
			case 'char':
			case 'uchar':
				$codePrinter->output($variable . '--;');
				break;
			case 'variable':

				/**
				 * Variable is probably not initialized here
				 */
				if ($symbolVariable->hasAnyDynamicType('unknown')) {
					throw new CompilerException("Attempt to increment uninitialized variable", $statement);
				}

				/**
				 * Decrement non-numeric variables could be expensive
				 */
				if (!$symbolVariable->hasAnyDynamicType(array('undefined', 'int', 'long', 'double', 'uint'))) {
					$compilationContext->logger->warning('Possible attempt to decrement non-numeric dynamic variable', 'non-valid-decrement', $statement);
				}

				$compilationContext->headersManager->add('kernel/operators');
				if ($symbolVariable->isLocalOnly()) {
					$codePrinter->output('zephir_decrement(&' . $variable . ');');
				} else {
					$symbolVariable->separate($compilationContext);
					$codePrinter->output('zephir_decrement(' . $variable . ');');
				}
				break;
			default:
				throw new CompilerException("Cannot decrement variable: " . $symbolVariable->getType(), $statement);
		}
	}

	/**
	 * Compiles foo[] = {expr}
	 *
	 * @param string $variable
	 * @param Variable $symbolVariable
	 * @param CompiledExpression $resolvedExpr
	 * @param CompilationContext $compilationContext
	 * @param array $statement
	 */
	public function assignObjectProperty($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		if (!$symbolVariable->isInitialized()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is not initialized", $statement);
		}

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Variable type '" . $symbolVariable->getType() . "' cannot be used as object", $statement);
		}

		$propertyName = $statement['property'];

		if (!$symbolVariable->isInitialized()) {
			throw new CompilerException("Cannot write static property '" . $classDefinition->getCompleteName() . "::" . $property . "' because it is not initialized", $statement);
		}

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Cannot use variable type: " . $symbolVariable->geType() . " as an object", $statement);
		}

		if ($symbolVariable->hasAnyDynamicType('unknown')) {
			throw new CompilerException("Cannot use non-initialized variable as an object", $statement);
		}

		/**
		 * Trying to use a non-object dynamic variable as object
		 */
		if ($symbolVariable->hasDifferentDynamicType(array('undefined', 'object'))) {
			$compilationContext->logger->warning('Possible attempt to update property on non-object dynamic property', 'non-valid-objectupdate', $statement);
		}

		/**
		 * Try to check if property is implemented on related object
		 */
		if ($variable == 'this') {
			if (!$compilationContext->classDefinition->hasProperty($propertyName)) {
				throw new CompilerException("Property '" . $propertyName . "' is not defined on class '" . $propertyName . "'", $statement);
			}
		}

		$codePrinter = $compilationContext->codePrinter;

		$compilationContext->headersManager->add('kernel/object');

		switch ($resolvedExpr->getType()) {
			case 'null':
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
				}
				break;
			case 'int':
			case 'long':
			case 'uint':
				$tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
				$codePrinter->output('ZEPHIR_INIT_ZVAL_NREF(' . $tempVariable->getName() . ');');
				$codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				}
				$tempVariable->setIdle(true);
				break;
			case 'string':
				$tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
				$codePrinter->output('ZEPHIR_INIT_ZVAL_NREF(' . $tempVariable->getName() . ');');
				$codePrinter->output('ZVAL_STRING(' . $tempVariable->getName() . ', "' . $resolvedExpr->getCode() . '", 1);');
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				}
				$tempVariable->setIdle(true);
				break;
			case 'bool':
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), (' . $resolvedExpr->getBooleanCode() . ') ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), (' . $resolvedExpr->getBooleanCode() . ') ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
				}
				break;
			case 'empty-array': /* unrecheable code */
				$tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
				$codePrinter->output('ZEPHIR_INIT_ZVAL_NREF(' . $tempVariable->getName() . ');');
				$codePrinter->output('array_init(' . $tempVariable->getName() . ');');
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				}
				$tempVariable->setIdle(true);
				break;
			case 'variable':
				$variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
				switch ($variableVariable->getType()) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
					case 'char':
					case 'uchar':
						$tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
						$codePrinter->output('ZEPHIR_INIT_ZVAL_NREF(' . $tempVariable->getName() . ');');
						$codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
						$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						$tempVariable->setIdle(true);
						break;
					case 'bool':
						if ($variable == 'this') {
							$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $variableVariable->getName() . ' ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
						} else {
							$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $variableVariable->getName() . ' ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
						}
						break;
					case 'string':
					case 'variable':
						if ($variable == 'this') {
							$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $variableVariable->getName() . ' TSRMLS_CC);');
						} else {
							$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $variableVariable->getName() . ' TSRMLS_CC);');
						}
						if ($symbolVariable->isTemporal()) {
							$symbolVariable->setIdle(true);
						}
						break;
					default:
						throw new CompilerException("Unknown type " . $variableVariable->getType(), $statement);
				}
				break;
			default:
				throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
		}

	}

	/**
	 * Compiles foo[] = {expr}
	 *
	 * @param string $variable
	 * @param Variable $symbolVariable
	 * @param CompiledExpression $resolvedExpr
	 * @param CompilationContext $compilationContext
	 * @param array $statement
	 */
	public function assignObjectDynamicProperty($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		if (!$symbolVariable->isInitialized()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is not initialized", $statement);
		}

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Variable type '" . $symbolVariable->getType() . "' cannot be used as object", $statement);
		}

		$propertyName = $statement['property'];

		$propertyVariable = $compilationContext->symbolTable->getVariableForRead($propertyName, $compilationContext, $statement);
		if ($propertyVariable->getType() != 'variable' && $propertyVariable->getType() != 'string') {
			throw new CompilerException("Cannot use variable type '" . $propertyVariable->getType() . "' to update object property", $statement);
		}

		if (!$symbolVariable->isInitialized()) {
			throw new CompilerException("Cannot write static property '" . $classDefinition->getCompleteName() . "::" . $property . "' because it is not initialized", $statement);
		}

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Cannot use variable type: " . $symbolVariable->geType() . " as an object", $statement);
		}

		if ($symbolVariable->hasAnyDynamicType('unknown')) {
			throw new CompilerException("Cannot use non-initialized variable as an object", $statement);
		}

		/**
		 * Trying to use a non-object dynamic variable as object
		 */
		if ($symbolVariable->hasDifferentDynamicType(array('undefined', 'object', 'null'))) {
			$compilationContext->logger->warning('Possible attempt to update property on non-object dynamic property', 'non-valid-objectupdate', $statement);
		}

		$codePrinter = $compilationContext->codePrinter;

		$compilationContext->headersManager->add('kernel/object');

		switch ($resolvedExpr->getType()) {
			/*case 'null':
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
				}
				break;
			case 'int':
			case 'long':
				$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				}
				break;
			case 'string':
				$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_STRING(' . $tempVariable->getName() . ', "' . $resolvedExpr->getCode() . '", 1);');
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				}
				break;
			case 'bool':
				if ($variable == 'this') {
					if ($resolvedExpr->getBooleanCode() == '1') {
						$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ZEPHIR_GLOBAL(global_true) TSRMLS_CC);');
					} else {
						$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
					}
				} else {
					if ($resolvedExpr->getBooleanCode() == '1') {
						$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ZEPHIR_GLOBAL(global_true) TSRMLS_CC);');
					} else {
						$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
					}
				}
				break;
			case 'empty-array':
				$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('array_init(' . $tempVariable->getName() . ');');
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				}
				break;*/
			case 'variable':
				$variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
				switch ($variableVariable->getType()) {
					/*case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
					case 'char':
					case 'uchar':
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
						$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						break;
					case 'bool':
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
						if ($variable == 'this') {
							$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						} else {
							$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						}
						break;*/
					case 'string':
					case 'variable':
						$codePrinter->output('zephir_update_property_zval_zval(' . $symbolVariable->getName() . ', ' . $propertyVariable->getName() . ', ' . $resolvedExpr->getCode() . ' TSRMLS_CC);');
						if ($symbolVariable->isTemporal()) {
							$symbolVariable->setIdle(true);
						}
						break;
					default:
						throw new CompilerException("Unknown type " . $variableVariable->getType(), $statement);
				}
				break;
			default:
				throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
		}

	}

	/**
	 * Compiles x->y[] = foo
	 *
	 * @param string $variable
	 * @param \Variable $symbolVariable
	 * @param \CompiledExpression $resolvedExpr
	 * @param \CompilationContext $compilationContext,
	 * @param array $statement
	 */
	public function assignPropertyAppend($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		if (!$symbolVariable->isInitialized()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is not initialized", $statement);
		}

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Attempt to use variable type: " . $symbolVariable->getType() . " as object", $statement);
		}

		$codePrinter = $compilationContext->codePrinter;

		$property = $statement['property'];
		$compilationContext->headersManager->add('kernel/object');

		switch ($resolvedExpr->getType()) {
			case 'variable':
				$variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
				switch ($variableExpr->getType()) {
					case 'variable':
					case 'string':
						$codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $variableExpr->getName() . ' TSRMLS_CC);');
						if ($variableExpr->isTemporal()) {
							$variableExpr->setIdle(true);
						}
						break;
					default:
						throw new CompilerException("Variable: " . $variableExpr->getType() . " cannot be appended to property array", $statement);
				}
				break;
			default:
				throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be appended to property", $statement);
		}
	}

	/**
	 * Compiles x->y[z] = {expr} (single offset assignment)
	 *
	 * @param string $variable
	 * @param \Variable $symbolVariable
	 * @param \CompiledExpression $resolvedExpr
	 * @param \CompilationContext $compilationContext,
	 * @param array $statement
	 */
	protected function _assignPropertyArraySingleIndex($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		$codePrinter = $compilationContext->codePrinter;

		$property = $statement['property'];
		$compilationContext->headersManager->add('kernel/object');

		/**
		 * Only string/variable/int
		 */
		$indexExpression = new Expression($statement['index-expr'][0]);
		$resolvedIndex = $indexExpression->compile($compilationContext);

		switch ($resolvedIndex->getType()) {
			case 'string':
			case 'int':
			case 'uint':
			case 'ulong':
			case 'long':
			case 'variable':
				break;
			default:
				throw new CompilerException("Expression: " . $resolvedIndex->getType() . " cannot be used as index without cast", $statement['index-expr']);
		}

		if ($resolvedIndex->getType() == 'variable') {

			$indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedIndex->getCode(), $compilationContext, $statement['index-expr']);
			switch ($indexVariable->getType()) {
				case 'string':
				case 'int':
				case 'uint':
				case 'ulong':
				case 'long':
				case 'variable':
					break;
				default:
					throw new CompilerException("Variable: " . $indexVariable->getType() . " cannot be used as index without cast", $statement);
			}

			if ($indexVariable->getType() == 'variable') {
				if ($indexVariable->hasDifferentDynamicType(array('undefined', 'int', 'string'))) {
					$compilationContext->logger->warning('Possible attempt to use non string/long dynamic variable as array index', 'invalid-array-offset', $statement);
				}
			}

		}

		switch ($resolvedIndex->getType()) {
			case 'string':
				$codePrinter->output('//missing');
				break;
			case 'variable':
				switch ($indexVariable->getType()) {
					case 'int':
						$codePrinter->output('//missing');
						break;
					case 'variable':
					case 'string':
						switch ($resolvedExpr->getType()) {
							case 'null':
								$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
								break;
							case 'bool':
								if ($resolvedExpr->getBooleanCode() == '1') {
									$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ZEPHIR_GLOBAL(global_true) TSRMLS_CC);');
								} else {
									$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
								}
								break;
							case 'variable':
								$variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement['index-expr']);
								switch ($variableExpr->getType()) {
									case 'bool':
										$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
										$codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $variableExpr->getName() . ');');
										$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
										break;
									case 'variable':
									case 'string':
										$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $variableExpr->getName() . ' TSRMLS_CC);');
										if ($variableExpr->isTemporal()) {
											$variableExpr->setIdle(true);
										}
										break;
									default:
										throw new CompilerException("Cannot update variable type: " . $variableExpr->getType(), $statement);
								}
								break;
						}
						break;
					default:
						throw new CompilerException("Variable index: " . $indexVariable->getType() . " cannot be updated into array property", $statement);
				}
				break;
			default:
				throw new CompilerException("Index: " . $resolvedIndex->getType() . " cannot be updated into array property", $statement);
		}
	}

	/**
	 * Compiles x->y[a][b] = {expr} (multiple offset assignment)
	 *
	 * @param string $variable
	 * @param \Variable $symbolVariable
	 * @param \CompiledExpression $resolvedExpr
	 * @param \CompilationContext $compilationContext,
	 * @param array $statement
	 */
	protected function _assignPropertyArrayMultipleIndex($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		//print_r($statement);

		/**
		 * We read the variable member in read-only mode, this must return a reference
		 * to the real property in the object
		 *
		 * This could introduce a bug because the variable will be separated
		 * However null values are not converted implicity to objects or arrays
		 *
		 * @todo, use a builder
		 */
		$propertyExpr = new Expression(array(
			'type' => 'property-access',
			'left' => array(
				'type' => 'variable',
				'value' => $statement['variable']
			),
			'right' => array(
				'type' => 'variable',
				'value' => $statement['property']
			)
		));

		$propertyExpr->setReadOnly(true);
		$property = $propertyExpr->compile($compilationContext);

		$propertyVariable = $compilationContext->symbolTable->getVariableForRead($property->getCode(), $compilationContext, $statement);
		switch ($propertyVariable->getType()) {

		}

		$this->assignArrayIndex($propertyVariable->getName(), $propertyVariable, $resolvedExpr, $compilationContext, $statement);
	}

	/**
	 * Compiles x->y[z] = foo
	 *
	 * @param string $variable
	 * @param \Variable $symbolVariable
	 * @param \CompiledExpression $resolvedExpr
	 * @param \CompilationContext $compilationContext,
	 * @param array $statement
	 */
	public function assignPropertyArrayIndex($variable, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{

		if (!$symbolVariable->isInitialized()) {
			throw new CompilerException("Cannot write variable '" . $variable . "' because it is not initialized", $statement);
		}

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Attempt to use variable type: " . $symbolVariable->getType() . " as object", $statement);
		}

		/**
		 * Update the property according to the number of array-offsets
		 */
		if (count($statement['index-expr']) == 1) {
			$this->_assignPropertyArraySingleIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
		} else {
			$this->_assignPropertyArrayMultipleIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $statement);
		}

	}

	/**
	 * Compiles ClassName::foo = {expr}
	 *
	 * @param                    $className
	 * @param                    $property
	 * @param Variable           $symbolVariable
	 * @param CompiledExpression $resolvedExpr
	 * @param CompilationContext $compilationContext
	 * @param array              $statement
	 *
	 * @throws CompilerException
	 * @internal param string $variable
	 */
	public function assignStaticProperty($className, $property, Variable $symbolVariable, CompiledExpression $resolvedExpr,
		CompilationContext $compilationContext, $statement)
	{
		$compiler = $compilationContext->compiler;
		if ($className != 'self' && $className != 'parent') {
			if ($compiler->isClass($className)) {
				$classDefinition = $compiler->getClassDefinition($className);
			} else {
				if ($compiler->isInternalClass($className)) {
					$classDefinition = $compiler->getInternalClassDefinition($className);
				} else {
					throw new CompilerException("Cannot locate class '" . $className . "'", $statement);
				}
			}
		} else {
			if ($className == 'self') {
				$classDefinition = $compilationContext->classDefinition;
			} else {
				if ($className == 'parent') {
					$classDefinition = $compilationContext->classDefinition;
					$extendsClass = $classDefinition->getExtendsClass();
					if (!$extendsClass) {
						throw new CompilerException('Cannot assign static property "' . $property . '" on parent because class ' .
							$classDefinition->getCompleteName() . ' does not extend any class', $statement);
					} else {
						$classDefinition = $classDefinition->getExtendsClassDefinition();
					}
				}
			}
		}

		if (!$classDefinition->hasProperty($property)) {
			throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a property called: '" . $property . "'", $statement);
		}

		/** @var $propertyDefinition ClassProperty */
		$propertyDefinition = $classDefinition->getProperty($property);
		if (!$propertyDefinition->isStatic()) {
			throw new CompilerException("Cannot access non-static property '" . $classDefinition->getCompleteName() . '::' . $property . "'", $statement);
		}

		if ($propertyDefinition->isPrivate()) {
			if ($classDefinition != $compilationContext->classDefinition) {
				throw new CompilerException("Cannot access private static property '" . $classDefinition->getCompleteName() . '::' . $property . "' out of its declaring context", $statement);
			}
		}

		$codePrinter = $compilationContext->codePrinter;

		$compilationContext->headersManager->add('kernel/object');
		$classEntry = $classDefinition->getClassEntry();

		switch ($resolvedExpr->getType()) {
			case 'null':
				$codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);');
				break;
			case 'int':
			case 'long':
				$tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
				$codePrinter->output('ZEPHIR_INIT_ZVAL_NREF(' . $tempVariable->getName() . ');');
				$codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
				$codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				break;
			case 'string':
				$tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
				$codePrinter->output('ZEPHIR_INIT_ZVAL_NREF(' . $tempVariable->getName() . ');');
				$codePrinter->output('ZVAL_STRING(' . $tempVariable->getName() . ', "' . $resolvedExpr->getCode() . '", 1);');
				$codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				break;
			case 'bool':
				if ($resolvedExpr->getBooleanCode() == '1') {
					$codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), ZEPHIR_GLOBAL(global_true) TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), ZEPHIR_GLOBAL(global_false) TSRMLS_CC);');
				}
				break;
			case 'empty-array':
				$tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
				$codePrinter->output('ZEPHIR_INIT_ZVAL_NREF(' . $tempVariable->getName() . ');');
				$codePrinter->output('array_init(' . $tempVariable->getName() . ');');
				$codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				break;
			case 'variable':
				$variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
				switch ($variableVariable->getType()) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
					case 'char':
					case 'uchar':
						$tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
						$codePrinter->output('ZEPHIR_INIT_ZVAL_NREF(' . $tempVariable->getName() . ');');
						$codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
						$codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						break;
					case 'bool':
						$tempVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext);
						$codePrinter->output('ZEPHIR_INIT_ZVAL_NREF(' . $tempVariable->getName() . ');');
						$codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
						$codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						break;
					case 'string':
					case 'variable':
						$codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), ' . $variableVariable->getName() . ' TSRMLS_CC);');
						if ($variableVariable->isTemporal()) {
							$variableVariable->setIdle(true);
						}
						break;
					default:
						throw new CompilerException("Unknown type " . $variableVariable->getType(), $statement);
				}
				break;
			default:
				throw new CompilerException("Unknown type " . $resolvedExpr->getType(), $statement);
		}

	}

	/**
	 * Compiles the let statement
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$statement = $this->_statement;

		foreach ($statement['assignments'] as $assignment) {

			$variable = $assignment['variable'];

			/**
			 * Get the symbol from the symbol table if necessary
			 */
			switch ($assignment['assign-type']) {
				case 'static-property':
				case 'static-property-append':
				case 'static-property-array-index':
					$symbolVariable = $compilationContext->symbolTable->getTempNonTrackedVariable('variable', $compilationContext, $assignment);
					break;
				default:
					$symbolVariable = $compilationContext->symbolTable->getVariableForWrite($variable, $compilationContext, $assignment);
					break;
			}

			/**
			 * Incr/Decr assignments don't require an expression
			 */
			if (isset($assignment['expr'])) {

				$readDetector = new ReadDetector($assignment['expr']);
				$expr = new Expression($assignment['expr']);
				switch ($assignment['assign-type']) {
					case 'variable':
						if (isset($assignment['operator'])) {
							if ($assignment['operator'] == 'assign') {
								$expr->setExpectReturn(true, $symbolVariable);
							}
						} else {
							$expr->setExpectReturn(true, $symbolVariable);
						}
						break;
				}

				switch ($assignment['expr']['type']) {
					case 'property-access':
					case 'array-access':
					case 'type-hint':
						$expr->setReadOnly(true);
						break;
				}
				$resolvedExpr = $expr->compile($compilationContext);

				/**
				 * Bad implemented operators could return values different than objects
				 */
				if (!is_object($resolvedExpr)) {
					throw new CompilerException("Resolved expression is not valid", $assignment['expr']);
				}
			}

			$codePrinter = $compilationContext->codePrinter;

			/**
			 * There are four types of assignments
			 */
			switch ($assignment['assign-type']) {
				case 'variable':
					$this->assignVariable($variable, $symbolVariable, $resolvedExpr, $readDetector, $compilationContext, $assignment);
					break;
				case 'variable-append':
					$this->assignVariableAppend($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'object-property':
					$this->assignObjectProperty($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'variable-dynamic-object-property':
					$this->assignObjectDynamicProperty($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'string-dynamic-object-property':
					/* @todo, implement this */
					break;
				case 'static-property':
					$this->assignStaticProperty($variable, $assignment['property'], $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'static-property-append':
					/* @todo, implement this */
					break;
				case 'static-property-array-index':
					/* @todo, implement this */
					break;
				case 'array-index':
					$this->assignArrayIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'object-property-append':
					$this->assignPropertyAppend($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'object-property-array-index':
					$this->assignPropertyArrayIndex($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'incr':
					$this->assignIncr($variable, $symbolVariable, $compilationContext, $assignment);
					break;
				case 'decr':
					$this->assignDecr($variable, $symbolVariable, $compilationContext, $assignment);
					break;
				case 'object-property-incr':
					/* @todo, implement this */
					break;
				case 'object-property-decr':
					/* @todo, implement this */
					break;
				default:
					throw new CompilerException("Unknown assignment: " . $assignment['assign-type'], $assignment);
			}
		}
	}

}
