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
 * EchoStatement
 *
 * Produce output according to the type
 */
class EchoStatement
{
	protected $_statement;

	public function __construct($statement)
	{
		$this->_statement = $statement;
	}

	public function compile(CompilationContext $compilationContext)
	{
		foreach ($this->_statement['expressions'] as $expr) {
			switch ($expr['type']) {
				case 'string':
					$compilationContext->codePrinter->output('php_printf("' . $expr['value'] . '");');
					break;
				case 'variable':
					$variable = $compilationContext->symbolTable->getVariableForRead($expr['value'], $compilationContext, $expr);
					switch ($variable->getType()) {
						case 'long':
							$compilationContext->codePrinter->output('php_printf("%ld", ' . $expr['value'] . ');');
							break;
						case 'int':
							$compilationContext->codePrinter->output('php_printf("%d", ' . $expr['value'] . ');');
							break;
						case 'char':
							$compilationContext->codePrinter->output('php_printf("%c", ' . $expr['value'] . ');');
							break;
						case 'string':
							$compilationContext->codePrinter->output('php_printf("%s", ' . $expr['value'] . '->str);');
							break;
						case 'variable':
							$compilationContext->codePrinter->output('zend_print_zval(' . $expr['value'] . ', 0);');
							break;
						default:
							throw new CompilerException("Unknown type: " . $variable->getType(), $expr);
					}
					break;
				default:
					throw new CompilerException("Unknown type: " . $expr['type'], $expr);
			}
		}
	}

}