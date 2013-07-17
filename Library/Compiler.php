<?php

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

	public function compileClass($codePrinter, $topStatement)
	{

		$classDefinition = new ClassDefinition($topStatement['name']);

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
					isset($method['statements']) ? new StatementsBlock($method['statements']) : null				
				));
			}
		}

		$classDefinition->compile($codePrinter);		
	}

	public function compileComment($codePrinter, $topStatement)
	{
		$codePrinter->output('/'. $topStatement['value'] . '/');
	}

	/**
	 *
	 */
	public function compile($ir)
	{
		if (!is_dir('.temp')) {
			mkdir('.temp');
		}

		$codePrinter = new codePrinter();

		foreach ($ir as $topStatement) {

			switch ($topStatement['type']) {
				case 'class':
					$this->compileClass($codePrinter, $topStatement);
					break;
				case 'comment':
					$this->compileComment($codePrinter, $topStatement);
					break;
			}
		}

		file_put_contents('ext/router.c', $codePrinter->getOutput());
	}
}