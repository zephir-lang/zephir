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

		if ($compilationContext->compiler->isClass($statement['domain'])) {

			$classCe = strtolower(str_replace('\\', '_', $statement['domain'])) . '_ce';

			if ($statement['parameters'][0]['type'] == 'string') {
				$codePrinter->output('ZEPHIR_THROW_EXCEPTION_STR(' . $classCe . ', "' . $statement['parameters'][0]['value'] . '");');
				$codePrinter->output('return;');
			}


		}

		//print_R();
	}

}