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
 * ThrowStatement
 *
 * Throws exceptions
 */
class ThrowStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	public function compile(CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;

		$statement = $this->_statement;

		$compilationContext->headersManager->add('kernel/exception');

		$expr = $statement['expr'];

		/**
		 * This optimizes throw new Exception("hello")
		 */
		if (isset($expr['class'])) {
			if (count($expr['parameters']) == 1) {
				$className = $expr['class'];
				if ($compilationContext->compiler->isClass($className)) {
					if ($expr['parameters'][0]['type'] == 'string') {
						$classDefinition = $compilationContext->compiler->getClassDefinition($className);
						$codePrinter->output('ZEPHIR_THROW_EXCEPTION_STR(' . $classDefinition->getClassEntry() . ', "' . Utils::addSlaches($expr['parameters'][0]['value']) . '");');
						$codePrinter->output('return;');
						return;
					}
				}
			}
		}

		$throwExpr = new Expression($expr);
		$resolvedExpr = $throwExpr->compile($compilationContext);

		if ($resolvedExpr->getType() != 'variable') {
			throw new CompilerException("Expression '" . $resolvedExpr->getType . '" cannot be used as exception');
		}

		$variableVariable = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $expr);
		if ($variableVariable->getType() != 'variable') {
			throw new CompilerException("Variable '" . $variableVariable->getType() . "' cannot be used as exception", $expr);
		}

		$codePrinter->output('zephir_throw_exception(' . $variableVariable->getName() . ' TSRMLS_CC);');
		$codePrinter->output('return;');

		if ($variableVariable->isTemporal()) {
			$variableVariable->setIdle(true);
		}
	}

}