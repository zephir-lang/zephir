<?php

error_reporting(E_ALL);

class ClassProperty
{

	protected $_visibility;

	protected $_name;

	protected $defaultValue;

	public function __construct($visibility, $name, $defaultValue=null)
	{
		$this->_visibility = $visibility;
		$this->_name = $name;
		$this->_defaultValue = $defaultValue;
	}

	public function getName()
	{
		return $this->_name;
	}

	public function getVisibilityAccesor()
	{
		return 'ZEND_ACC_PROTECTED';
	}
}

class ClassMethod
{

	protected $_visibility;

	protected $_name;

	public function __construct($visibility, $name)
	{
		$this->_visibility = $visibility;
		$this->_name = $name;		
	}

	public function getName()
	{
		return $this->_name;
	}

	public function getVisibilityAccesor()
	{
		return 'ZEND_ACC_PROTECTED';
	}
}

class ClassDefinition
{
	protected $_name;

	protected $_properties = array();

	protected $_methods = array();

	public function __construct($name)
	{
		$this->_name = $name;		
	}

	public function getName()
	{
		return $this->_name;
	}

	public function addProperty(ClassProperty $property)
	{
		$this->_properties[] = $property;
	}

	public function addMethod(ClassMethod $method)
	{
		$this->_methods[] = $method;
	}

	public function getProperties()
	{
		return $this->_properties;
	}

	public function getMethods()
	{
		return $this->_methods;
	}

	public function getClassEntry()
	{
		return strtolower($this->_name) . '_ce';
	}

}

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

	/**
	 *
	 */
	public function compileClass($ir)
	{
		//echo $ir['name'], PHP_EOL;

		$definition = $ir['definition'];

		$classDefinition = new ClassDefinition($ir['name']);

		if (isset($definition['properties'])) {
			foreach ($definition['properties'] as $property) {
				$classDefinition->addProperty(new ClassProperty(
					$property['visibility'], 
					$property['name'], 
					isset($property['defaultValue']) ? $property['defaultValue'] : null
				));
			}
		}

		if (isset($definition['methods'])) {
			foreach ($definition['methods'] as $property) {
				$classDefinition->addMethod(new ClassMethod(
					$property['visibility'], 
					$property['name']					
				));
			}
		}

		$code = 'PHP_INIT_CLASS(' . $classDefinition->getName() . ') {' . PHP_EOL . PHP_EOL;

		/**
		 * Compile properties
		 */
		foreach ($classDefinition->getProperties() as $property) {
			$code .= "\tzend_declare_property_null(" . 
				$classDefinition->getClassEntry() . 
				", SL(\"" . $property->getName() . "\"), " . 
				$property->getVisibilityAccesor() . " TSRMLS_CC);\n";
		}

		$code .= '}' . PHP_EOL . PHP_EOL;

		foreach ($classDefinition->getMethods() as $method) {
			$code .= 'PHP_METHOD(' . $classDefinition->getName() . ', ' . $method->getName() . ') {' . PHP_EOL . PHP_EOL;
			$code .= '}' . PHP_EOL . PHP_EOL;
		}

		echo $code;

	}
}

$c = new Compiler();
$ir = $c->genIR();

$c->compileClass($ir);
