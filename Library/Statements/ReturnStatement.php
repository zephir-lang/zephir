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
 * ReturnStatement
 *
 * Return statement is used to assign variables
 */
class ReturnStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	public function compile(CompilationContext $compilationContext)
	{

		$statement = $this->_statement;

		$codePrinter = $compilationContext->codePrinter;

		$codePrinter->outputBlankLine(true);

		/**
		 * Use return member for properties on this
		 */
		if ($statement['expr']['type'] == 'property-access') {
			if ($statement['expr']['left']['type'] == 'variable') {
				if ($statement['expr']['left']['value'] == 'this') {
					if ($statement['expr']['right']['type'] == 'variable') {
						$codePrinter->output('RETURN_MEMBER(this_ptr, "' . $statement['expr']['right']['value'] . '");');
						return;
					}
				}
			}
		}

		$expr = new Expression($statement['expr']);
		$expr->setExpectReturn(true);
		$resolvedExpr = $expr->compile($compilationContext);

		switch ($resolvedExpr->getType()) {
			case 'int':
				$codePrinter->output('RETURN_MM_LONG(' . $resolvedExpr->getCode() . ');');
				break;
			case 'bool':
				$codePrinter->output('RETURN_MM_BOOL(' . $resolvedExpr->getBooleanCode() . ');');
				break;
			case 'double':
				$codePrinter->output('RETURN_MM_DOUBLE(' . $resolvedExpr->getCode() . ');');
				break;
			case 'string':
				$codePrinter->output('RETURN_MM_STRING("' . $resolvedExpr->getCode() . '", 1);');
				break;
			case 'variable':
				$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode());
				switch ($symbolVariable->getType()) {
					case 'int':
						$codePrinter->output('RETURN_MM_LONG(' . $symbolVariable->getName() . ');');
						break;
					case 'double':
						$codePrinter->output('RETURN_MM_DOUBLE(' . $symbolVariable->getName() . ');');
						break;
					case 'bool':
						$codePrinter->output('RETURN_MM_BOOL(' . $symbolVariable->getName() . ');');
						break;
					case 'variable':
						if ($symbolVariable->getName() == 'this_ptr') {
							$codePrinter->output('RETURN_THIS();');
						} else {
							if ($symbolVariable->isLocalOnly()) {
								$codePrinter->output('RETURN_CCTOR(&' . $symbolVariable->getName() . ');');
							} else {
								$codePrinter->output('RETURN_CCTOR(' . $symbolVariable->getName() . ');');
							}
						}
						break;
					default:
						throw new CompilerException("Cannot return variable '" . $symbolVariable->getType() . "'", $statement['expr']);
				}
				break;
			case 'null':
				$codePrinter->output('RETURN_MM_NULL();');
				break;
			default:
				throw new CompilerException("Cannot return '" . $resolvedExpr->getType() . "'", $statement['expr']);
		}

		$codePrinter->outputBlankLine(true);
	}

}