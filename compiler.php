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

	public function compile(ClassDefinition $classDefinition)
	{
		if (!is_array($this->_defaultValue)) {			
			return "\tzend_declare_property_null(" . 
				$classDefinition->getClassEntry() . 
				", SL(\"" . $this->getName() . "\"), " . 
				$this->getVisibilityAccesor() . " TSRMLS_CC);\n";
		} else {
			switch ($this->_defaultValue['type']) {
				case 305:
					return "\tzend_declare_property_bool(" . 
						$classDefinition->getClassEntry() . 
						", SL(\"" . $this->getName() . "\"), 0, " . 
						$this->getVisibilityAccesor() . " TSRMLS_CC);\n";
				default:
					return "\tzend_declare_property_null(" . 
						$classDefinition->getClassEntry() . 
						", SL(\"" . $this->getName() . "\"), " . 
						$this->getVisibilityAccesor() . " TSRMLS_CC);\n";
			}
		}
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

	public function compile()
	{
		$code = 'PHP_INIT_CLASS(' . $this->getName() . ') {' . PHP_EOL . PHP_EOL;

		/**
		 * Compile properties
		 */
		foreach ($this->getProperties() as $property) {
			$code .= $property->compile($this);	
		}

		$code .= '}' . PHP_EOL . PHP_EOL;

		foreach ($this->getMethods() as $method) {
			$code .= 'PHP_METHOD(' . $this->getName() . ', ' . $method->getName() . ') {' . PHP_EOL . PHP_EOL;
			$code .= '}' . PHP_EOL . PHP_EOL;
		}

		return $code;
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
			foreach ($definition['methods'] as $property) {
				$classDefinition->addMethod(new ClassMethod(
					$property['visibility'], 
					$property['name']					
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

$c = new Compiler();
$ir = $c->genIR();

echo $c->compile($ir);
