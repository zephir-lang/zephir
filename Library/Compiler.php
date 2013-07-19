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
		system("./xx test/router.zep 2> .temp/router.zep.js");
		return json_decode(file_get_contents(".temp/router.zep.js"), true);
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
					isset($method['statements']) ? new StatementsBlock($method['statements']) : null,
					isset($method['docblock']) ? $method['docblock'] : null
				));
			}
		}

		/**
		 * Do the compilation
		 */
		$classDefinition->compile($compilationContext);

		$code  = '' . PHP_EOL;
		$code .= '#ifdef HAVE_CONFIG_H' . PHP_EOL;
		$code .= '#include "config.h"' . PHP_EOL;
		$code .= '#endif' . PHP_EOL;
		$code .= '' . PHP_EOL;

		$code .= '#include "php.h"' . PHP_EOL;
		$code .= '#include "php_test.h"' . PHP_EOL;
		$code .= '#include "test.h"' . PHP_EOL;
		$code .= '' . PHP_EOL;

		$code .= '#include "Zend/zend_operators.h"' . PHP_EOL;
		$code .= '#include "Zend/zend_exceptions.h"' . PHP_EOL;
		$code .= '#include "Zend/zend_interfaces.h"' . PHP_EOL;
		$code .= '' . PHP_EOL;

		$code .= '#include "kernel/main.h"' . PHP_EOL;

		foreach ($compilationContext->headersManager->get() as $header => $one) {
			$code .= '#include "' . $header . '.h"' . PHP_EOL;
		}

		/**
		 * Prepend the required files to the header
		 */
		$compilationContext->codePrinter->preOutput($code);
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
					if ($namespace !== null) {
						throw new Exception("The namespace must be defined just one time");
					}
					$namespace = $topStatement['name'];
					break;
			}
		}

		if (!$namespace) {
			throw new Exception("Every file need a namespace");
		}

		/**
		 * Compilation context stores common objects required by compilation entities
		 */
		$compilationContext = new CompilationContext;

		/**
		 * Headers manager
		 */
		$headersManager = new HeadersManager();
		$compilationContext->headersManager = $headersManager;

		/**
		 * Main code-printer for the file
		 */
		$codePrinter = new codePrinter();
		$compilationContext->codePrinter = $codePrinter;

		$codePrinter->outputBlankLine();

		$class = false;
		foreach ($ir as $topStatement) {

			switch ($topStatement['type']) {
				case 'class':
					if ($class) {
						throw new Exception("More than one class defined in the same file");
					}
					$class = true;
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