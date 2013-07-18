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

	public function compileClass(CompilationContext $compilationContext, $namespace, $topStatement)
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

		$compilationContext->codePrinter->outputBlankLine();
		$compilationContext->codePrinter->output('#ifdef HAVE_CONFIG_H');
		$compilationContext->codePrinter->output('#include "config.h"');
		$compilationContext->codePrinter->output('#endif');
		$compilationContext->codePrinter->outputBlankLine();

		$compilationContext->codePrinter->output('#include "php.h"');
		$compilationContext->codePrinter->output('#include "php_test.h"');
		$compilationContext->codePrinter->output('#include "test.h"');
		$compilationContext->codePrinter->outputBlankLine();

		$compilationContext->codePrinter->output('#include "Zend/zend_operators.h"');
		$compilationContext->codePrinter->output('#include "Zend/zend_exceptions.h"');
		$compilationContext->codePrinter->output('#include "Zend/zend_interfaces.h"');
		$compilationContext->codePrinter->outputBlankLine();

		$compilationContext->codePrinter->output('#include "kernel/main.h"');
		$compilationContext->codePrinter->outputBlankLine();

		$classDefinition->compile($compilationContext);
	}

	/**
	 * Compiles a comment as a top-level statement
	 */
	public function compileComment(CompilationContext $compilationContext, $topStatement)
	{
		$compilationContext->codePrinter->output('/' . $topStatement['value'] . '/');
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

		$compilationContext = new CompilationContext;

		$codePrinter = new codePrinter();

		$compilationContext->codePrinter = $codePrinter;

		$codePrinter->outputBlankLine();

		foreach ($ir as $topStatement) {

			switch ($topStatement['type']) {
				case 'class':
					$this->compileClass($compilationContext, $namespace, $topStatement);
					break;
				case 'comment':
					$this->compileComment($compilationContext, $topStatement);
					break;
			}
		}

		file_put_contents('ext/router.c', $codePrinter->getOutput());
	}

}