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

	public function compileClass($topStatement)
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

		$code = $classDefinition->compile();

		return $code;
	}

	public function compileComment($topStatement)
	{
		return '/'. $topStatement['value'] . '/' . PHP_EOL;
	}

	/**
	 *
	 */
	public function compile($ir)
	{
		//echo $ir['name'], PHP_EOL;

		if (!is_dir('.temp')) {
			mkdir('.temp');
		}

		$code = '';
		foreach ($ir as $topStatement) {

			switch ($topStatement['type']) {
				case 'class':
					$code .= $this->compileClass($topStatement);
					break;
				case 'comment':
					$code .= $this->compileComment($topStatement);
					break;
			}
		}

		return $code;

	}
}