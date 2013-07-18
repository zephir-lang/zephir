<?php

/**
 * Compiler
 *
 *
 */
class Compiler
{

	protected $_class;

	/**
	 * Compiles the file generating a JSON
	 */
	public function genIR()
	{
		system("./xx test/router.xx 2> .temp/router.xx.js");
		return json_decode(file_get_contents(".temp/router.xx.js"), true);
	}

	public function compileClass($namespace, $codePrinter, $topStatement)
	{

		$classDefinition = new ClassDefinition($namespace, $topStatement['name']);

		$definition = $topStatement['definition'];

		if (isset($definition['properties'])) {
			foreach ($definition['properties'] as $property) {
				$classDefinition->addProperty(new ClassProperty(
					$property['visibility'],
					$property['name'],
					isset($property['default']) ? $property['default'] : null
				));
			}
		}

		if (isset($definition['methods'])) {
			foreach ($definition['methods'] as $method) {
				$classDefinition->addMethod(new ClassMethod(
					$method['visibility'],
					$method['name'],
					isset($method['parameters']) ? new ClassMethodParameters($method['parameters']) : null,
					isset($method['statements']) ? new StatementsBlock($method['statements']) : null
				));
			}
		}

		$codePrinter->outputBlankLine();
		$codePrinter->output('#ifdef HAVE_CONFIG_H');
		$codePrinter->output('#include "config.h"');
		$codePrinter->output('#endif');
		$codePrinter->outputBlankLine();

		$codePrinter->output('#include "php.h"');
		$codePrinter->output('#include "php_test.h"');
		$codePrinter->output('#include "test.h"');
		$codePrinter->outputBlankLine();

		$codePrinter->output('#include "Zend/zend_operators.h"');
		$codePrinter->output('#include "Zend/zend_exceptions.h"');
		$codePrinter->output('#include "Zend/zend_interfaces.h"');
		$codePrinter->outputBlankLine();

		$codePrinter->output('#include "kernel/main.h"');
		$codePrinter->outputBlankLine();

		$classDefinition->compile($codePrinter);
	}

	public function compileComment($codePrinter, $topStatement)
	{
		$codePrinter->output('/' . $topStatement['value'] . '/');
	}

	/**
	 *
	 */
	public function compile($ir)
	{
		if (!is_dir('.temp')) {
			mkdir('.temp');
		}

		if (!is_array($ir)) {
			throw new Exception("Cannot parse file");
		}

		/**
		 * Traverse the top level statements looking for the namespace
		 */
		$namespace = null;
		foreach ($ir as $topStatement) {

			switch ($topStatement['type']) {
				case 'namespace':
					$namespace = $topStatement['name'];
					break;
			}
		}

		if (!$namespace) {
			throw new Exception("Every file need a namespace");
		}

		$codePrinter = new codePrinter();

		$codePrinter->outputBlankLine();

		foreach ($ir as $topStatement) {

			switch ($topStatement['type']) {
				case 'class':
					$this->compileClass($namespace, $codePrinter, $topStatement);
					break;
				case 'comment':
					$this->compileComment($codePrinter, $topStatement);
					break;
			}
		}

		file_put_contents('ext/router.c', $codePrinter->getOutput());
	}
}