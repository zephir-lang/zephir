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
								$compilationContext->headersManager->add('kernel/string_type');
								$codePrinter->output('zephir_str_assign(' . $variable . ', "", sizeof("")-1);');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
						}
						break;
					case 'string':
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->setMustInitNull(true);
								$compilationContext->headersManager->add('kernel/string_type');
								$codePrinter->output('zephir_str_assign(' . $variable . ', "' . $resolvedExpr->getCode() . '", sizeof("' . $resolvedExpr->getCode() . '")-1);');
								break;
							case 'concat-assign':
								$symbolVariable->setMustInitNull(true);
								$compilationContext->headersManager->add('kernel/string_type');
								$codePrinter->output('zephir_str_append(' . $variable . ', "' . $resolvedExpr->getCode() . '", sizeof("' . $resolvedExpr->getCode() . '")-1);');
								break;
							default:
								throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
						}
						break;
					case 'char':
					case 'uchar':
						switch ($statement['operator']) {
							case 'assign':
								$symbolVariable->setMustInitNull(true);
								$compilationContext->headersManager->add('kernel/string_type');
								$codePrinter->output('zephir_str_assign_char(' . $variable . ', \'' . $resolvedExpr->getCode() . '\');');
								break;
							case 'concat-assign':
								$symbolVariable->setMustInitNull(true);
								$compilationContext->headersManager->add('kernel/string_type');
								$codePrinter->output('zephir_str_append_char(' . $variable . ', \'' . $resolvedExpr->getCode() . '\');');
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
										$compilationContext->headersManager->add('kernel/string_type');
										$codePrinter->output('zephir_str_assign(' . $variable . ', ' . $itemVariable->getName() . '->str, ' . $itemVariable->getName() . '->len);');
										break;
									case 'concat-assign':
										$symbolVariable->setMustInitNull(true);
										$compilationContext->headersManager->add('kernel/string_type');
										$codePrinter->output('zephir_str_append(' . $variable . ', ' . $itemVariable->getName() . '->str, ' . $itemVariable->getName() . '->len);');
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
										$symbolVariable->setMustInitNull(true);
										$compilationContext->headersManager->add('kernel/string_type');
										$codePrinter->output('zephir_str_assign_long(' . $variable . ', ' . $itemVariable->getName() . ');');
										break;
									case 'concat-assign':
										$symbolVariable->setMustInitNull(true);
										$compilationContext->headersManager->add('kernel/string_type');
										$codePrinter->output('zephir_str_append_long(' . $variable . ', ' . $itemVariable->getName() . ');');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
								}
								break;
							case 'char':
							case 'uchar':
								switch ($statement['operator']) {
									case 'assign':
										$symbolVariable->setMustInitNull(true);
										$compilationContext->headersManager->add('kernel/string_type');
										$codePrinter->output('zephir_str_assign_char(' . $variable . ', ' . $itemVariable->getName() . ');');
										break;
									case 'concat-assign':
										$symbolVariable->setMustInitNull(true);
										$compilationContext->headersManager->add('kernel/string_type');
										$codePrinter->output('zephir_str_append_char(' . $variable . ', ' . $itemVariable->getName() . ');');
										break;
									default:
										throw new CompilerException("Operator '" . $statement['operator'] . "' is not supported for variable type: string", $statement);
								}
								break;
							case 'variable':
								switch ($statement['operator']) {
									case 'assign':
										$symbolVariable->setMustInitNull(true);
										$compilationContext->headersManager->add('kernel/string_type');
										$codePrinter->output('zephir_str_assign_long(' . $variable . ', zephir_get_intval(' . $itemVariable->getName() . '));');
										break;
									case 'concat-assign':
										$symbolVariable->setMustInitNull(true);
										$compilationContext->headersManager->add('kernel/string_type');
										$codePrinter->output('zephir_str_append_long(' . $variable . ', ' . $itemVariable->getName() . ');');
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
								$symbolVariable->setDynamicType('null');
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
								$symbolVariable->setDynamicType('long');
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
								$symbolVariable->setDynamicType('long');
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
								$symbolVariable->setDynamicType('double');
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
								$symbolVariable->setDynamicType('bool');
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
								$symbolVariable->setDynamicType('string');
								if ($symbolVariable->isLocalOnly()) {
									$codePrinter->output('ZVAL_STRING(&' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
								} else {
									$codePrinter->output('ZVAL_STRING(' . $variable . ', "' . $resolvedExpr->getCode() . '", 1);');
								}
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
										$symbolVariable->setDynamicType('long');
										if ($symbolVariable->isLocalOnly()) {
											$codePrinter->output('ZVAL_LONG(&' . $variable . ', ' . $itemVariable->getName() . ');');
										} else {
											$codePrinter->output('ZVAL_LONG(' . $variable . ', ' . $itemVariable->getName() . ');');
										}
										break;
									case 'add-assign':
										/* @todo, use temporary variable */
										$symbolVariable->initVariant($compilationContext);
										$symbolVariable->setDynamicType('long');
										if ($symbolVariable->isLocalOnly()) {
											$codePrinter->output('ZVAL_LONG(&' . $variable . ', zephir_get_numberval(&' . $variable . ') + ' . $itemVariable->getName() . ');');
										} else {
											$codePrinter->output('ZVAL_LONG(' . $variable . ', zephir_get_numberval(&' . $variable . ') + ' . $itemVariable->getName() . ');');
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
										$symbolVariable->setDynamicType('double');
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
										$symbolVariable->setDynamicType('bool');
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
											$symbolVariable->setDynamicType($itemVariable->getDynamicType());
											$symbolVariable->setClassType($itemVariable->getClassType());

											$codePrinter->output('ZEPHIR_CPY_WRT(' . $variable . ', ' . $itemVariable->getName() . ');');
											if ($itemVariable->isTemporal()) {
												$itemVariable->setIdle(true);
											}
										}
										break;
									case 'concat-assign':
										$compilationContext->symbolTable->mustGrownStack(true);
										$compilationContext->headersManager->add('kernel/operators');
										//$codePrinter->output('ZEPHIR_ADD_ASSIGN(' . $variable . ', ' . $itemVariable->getName() . ');');
										break;
									case 'add-assign':
										$compilationContext->symbolTable->mustGrownStack(true);
										$compilationContext->headersManager->add('kernel/operators');
										$codePrinter->output('ZEPHIR_ADD_ASSIGN(' . $variable . ', ' . $itemVariable->getName() . ');');
										break;
									case 'sub-assign':
										$compilationContext->symbolTable->mustGrownStack(true);
										$compilationContext->headersManager->add('kernel/operators');
										$codePrinter->output('ZEPHIR_SUB_ASSIGN(' . $variable . ', ' . $itemVariable->getName() . ');');
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

		$dynamicType = $symbolVariable->getDynamicType();
		if ($dynamicType == 'unknown') {
			throw new CompilerException("Attempt to use variable '" . $variable . "' without be initialized", $statement);
		}

		if ($dynamicType != 'undefined' && $dynamicType != 'array') {
			$compilationContext->logger->warning('Possible attempt to append elements on a non-array dynamic variable', 'non-array-append', $statement);
		}

		$codePrinter = $compilationContext->codePrinter;

		$type = $symbolVariable->getType();
		switch ($type) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
				$codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $exprVariable->getName() . ');');
				$codePrinter->output('zephir_array_append(&' . $variable . ', ' . $symbolVariable->getName() . ', PH_SEPARATE);');
				break;
			case 'variable':
				switch ($resolvedExpr->getType()) {
					case 'string':
						$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $statement);
						$codePrinter->output('ZVAL_STRING(' . $symbolVariable->getName() . ', "' . Utils::addSlaches($resolvedExpr->getCode()) . '", 1);');
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
								$codePrinter->output('zephir_array_append(&' . $variable . ', ' . $resolvedExpr->getCode() . ', PH_SEPARATE);');
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

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Variable type: '" . $symbolVariable->getType() . "' cannot be used as an array", $statement);
		}

		$dynamicType = $symbolVariable->getDynamicType();

		/**
		 * Variable is probably not initialized here
		 */
		if ($dynamicType == 'unknown') {
			throw new CompilerException("Attempt to update index on uninitialized variable", $statement);
		}

		/**
		 * Update non-array variables will throw a notice in runtime
		 */
		if ($dynamicType != 'undefined' && $dynamicType != 'array') {
			$compilationContext->logger->warning('Possible attempt to update index on a non-array dynamic variable', 'non-array-update', $statement);
		}

		$codePrinter = $compilationContext->codePrinter;

		$expression = new Expression($statement['index-expr']);
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
				throw new CompilerException("Index: " . $exprIndex->getType() . " cannot be used as array index in assigment without cast", $statement);
		}

		switch ($resolvedExpr->getType()) {
			case 'int':
			case 'uint':
			case 'long':
			case 'ulong':
				$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
				$codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $resolvedExpr->getCode() . ');');
				break;
			case 'variable':
				$variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
				switch ($variableExpr->getType()) {
					case 'int':
					case 'uint':
					case 'long':
					case 'ulong':
						$symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
						$codePrinter->output('ZVAL_LONG(' . $symbolVariable->getName() . ', ' . $variableExpr->getName() . ');');
						break;
					case 'variable':
						$symbolVariable = $variableExpr;
						break;
					default:
						throw new CompilerException("Variable: " . $exprIndex->getType() . " cannot be assigned to array index", $statement);
				}
				break;
			default:
				throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be assigned to array index", $statement);
		}

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
						$codePrinter->output('zephir_array_update_string(&' . $variable . ', ' . $variableIndex->getName() . '->str, ' . $variableIndex->getName() . '->len, &' . $symbolVariable->getName() . ', PH_COPY | PH_SEPARATE);');
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
			//throw new CompilerException("Cannot write variable '" . $variable . "' because it is read only", $statement);
			return;
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

				$dynamicType = $symbolVariable->getDynamicType();

				/**
				 * Variable is probably not initialized here
				 */
				if ($dynamicType == 'unknown') {
					throw new CompilerException("Attempt to increment uninitialized variable", $statement);
				}

				/**
				 * Update non-numeric dynamic variables could be expensive
				 */
				if ($dynamicType != 'undefined' && $dynamicType != 'long' && $dynamicType != 'double') {
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

				$dynamicType = $symbolVariable->getDynamicType();

				/**
				 * Variable is probably not initialized here
				 */
				if ($dynamicType == 'unknown') {
					throw new CompilerException("Attempt to increment uninitialized variable", $statement);
				}

				/**
				 * Decrement non-numeric variables could be expensive
				 */
				if ($dynamicType != 'undefined' && $dynamicType != 'long' && $dynamicType != 'double') {
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
		if ($variable == 'this') {
			if (!$compilationContext->classDefinition->hasProperty($propertyName)) {
				throw new CompilerException("Property '" . $propertyName . "' is not defined on class '" . $propertyName . "'", $statement);
			}
		} else {

		}

		$dynamicType = $symbolVariable->getDynamicType();

		/**
		 * Variable is probably not initialized here
		 */
		if ($dynamicType == 'unknown') {
			throw new CompilerException("Attempt to update property on uninitialized variable", $statement);
		}

		/**
		 * Trying to use a non-object dynamic variable as object
		 */
		if ($dynamicType != 'undefined' && $dynamicType != 'object') {
			$compilationContext->logger->warning('Possible attempt to update property on non-object dynamic property', 'non-valid-objectupdate', $statement);
		}

		$codePrinter = $compilationContext->codePrinter;

		$compilationContext->headersManager->add('kernel/object');

		switch ($resolvedExpr->getType()) {
			case 'null':
				$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
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
				$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
				if ($variable == 'this') {
					$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
				} else {
					$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
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
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_LONG(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
						$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						break;
					case 'bool':
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . ');');
						$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						break;
					case 'string':
						/* @todo, use ZVAL_STRINGL */
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_STRING(' . $tempVariable->getName() . ', ' . $variableVariable->getName() . '->str, 1);');
						$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $tempVariable->getName() . ' TSRMLS_CC);');
						break;
					case 'variable':
						if ($variable == 'this') {
							$codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $resolvedExpr->getCode() . ' TSRMLS_CC);');
						} else {
							$codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $resolvedExpr->getCode() . ' TSRMLS_CC);');
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
						$codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $variableExpr->getName() . ' TSRMLS_CC);');
						break;
					default:
						throw new CompilerException("Variable: " . $variableExpr->getType() . " cannot be used as object", $statement);
				}
				break;
			default:
				throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be appended to property", $statement);
		}
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

		$codePrinter = $compilationContext->codePrinter;

		$property = $statement['property'];
		$compilationContext->headersManager->add('kernel/object');

		$indexExpression = new Expression($statement['index-expr']);
		$resolvedExpression = $indexExpression->compile($compilationContext);
		if ($resolvedExpression->getType() != 'variable') {
			throw new CompilerException("Expression: " . $indexExpression->getType() . " cannot be used as index", $statement['index-expr']);
		}

		$indexVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpression->getCode(), $compilationContext, $statement['index-expr']);
		switch ($indexVariable->getType()) {
			case 'variable':
				break;
			default:
				throw new CompilerException("Variable: " . $indexVariable->getType() . " cannot be used as index", $statement);
		}

		$dynamicType = $indexVariable->getDynamicType();
		if ($dynamicType != 'undefined' && $dynamicType != 'string' && $dynamicType != 'long') {
			$compilationContext->logger->warning('Possible attempt to use non string/long dynamic variable as array index', 'invalid-array-index', $statement);
		}

		switch ($resolvedExpr->getType()) {
			case 'bool':
				$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
				$codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $resolvedExpr->getBooleanCode() . ');');
				$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
				break;
			case 'variable':
				$variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
				switch ($variableExpr->getType()) {
					case 'bool':
						$tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
						$codePrinter->output('ZVAL_BOOL(' . $tempVariable->getName() . ', ' . $variableExpr->getName() . ');');
						$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $tempVariable->getName() . ' TSRMLS_CC);');
						break;
					case 'variable':
						$codePrinter->output('zephir_update_property_array(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $indexVariable->getName() . ', ' . $variableExpr->getName() . ' TSRMLS_CC);');
						break;
					default:
						throw new CompilerException("Variable: " . $variableExpr->getType() . " cannot be updated into array property", $statement);
				}
				break;
			default:
				throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be updated into array property", $statement);
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
						$expr->setExpectReturn(true, $symbolVariable);
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
					$symbolVariable->setIsInitialized(true);
					$this->assignVariable($variable, $symbolVariable, $resolvedExpr, $readDetector, $compilationContext, $assignment);
					break;
				case 'variable-append':
					$this->assignVariableAppend($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'object-property':
					$this->assignObjectProperty($variable, $symbolVariable, $resolvedExpr, $compilationContext, $assignment);
					break;
				case 'variable-dynamic-object-property':
					/* @todo, implement this */
					break;
				case 'string-dynamic-object-property':
					/* @todo, implement this */
					break;
				case 'static-property':
					/* @todo, implement this */
					break;
				case 'static-property-append':
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
				default:
					throw new CompilerException("Unknown assignment: " . $assignment['assign-type'], $assignment);
			}
		}
	}

}
