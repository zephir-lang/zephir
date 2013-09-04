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

	/**
	 *
	 *
	 * @param \CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{

		$statement = $this->_statement;

		$codePrinter = $compilationContext->codePrinter;

		if (isset($statement['expr'])) {

			/**
			 * Use return member for properties on this
			 */
			if ($statement['expr']['type'] == 'property-access') {
				if ($statement['expr']['left']['type'] == 'variable') {
					if ($statement['expr']['left']['value'] == 'this') {
						if ($statement['expr']['right']['type'] == 'variable') {

							/**
							 * If the property is accessed on 'this', we check if the property does exist
							 */
							$property = $statement['expr']['right']['value'];
							$classDefinition = $compilationContext->classDefinition;
							if (!$classDefinition->hasProperty($property)) {
								throw new CompilerException("Class '" . $classDefinition->getCompleteName() . "' does not have a property called: '" . $property . "'", $statement['expr']['right']);
							}

							$compilationContext->headersManager->add('kernel/object');
							$codePrinter->output('RETURN_MEMBER(this_ptr, "' . $property . '");');
							return;
						}
					}
				}
			}

			$variable = $compilationContext->symbolTable->getVariable('return_value');

			$expr = new Expression($statement['expr']);
			$expr->setExpectReturn(true, $variable);
			$resolvedExpr = $expr->compile($compilationContext);

			switch ($resolvedExpr->getType()) {
				case 'int':
				case 'uint':
				case 'long':
				case 'char':
				case 'uchar':
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
					$symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement['expr']);
					switch ($symbolVariable->getType()) {
						case 'int':
						case 'uint':
						case 'long':
						case 'char':
						case 'uchar':
							$codePrinter->output('RETURN_MM_LONG(' . $symbolVariable->getName() . ');');
							break;
						case 'double':
							$codePrinter->output('RETURN_MM_DOUBLE(' . $symbolVariable->getName() . ');');
							break;
						case 'string':
							$codePrinter->output('RETURN_MM_STRING(' . $resolvedExpr->getCode() . '->str, 1);');
							break;
						case 'bool':
							$codePrinter->output('RETURN_MM_BOOL(' . $symbolVariable->getName() . ');');
							break;
						case 'variable':
							if ($symbolVariable->getName() == 'this_ptr') {
								$codePrinter->output('RETURN_THIS();');
							} else {
								if ($symbolVariable->getName() != 'return_value') {
									if ($symbolVariable->isLocalOnly()) {
										$codePrinter->output('RETURN_CCTOR(&' . $symbolVariable->getName() . ');');
									} else {
										$codePrinter->output('RETURN_CCTOR(' . $symbolVariable->getName() . ');');
									}
								} else {
									$codePrinter->output('RETURN_MM();');
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

			return;
		}

		/**
		 * Return without an expression
		 */
		$codePrinter->output('RETURN_MM_NULL();');
	}

}