<?php

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
			$code .= $method->compile($this);
			$code .= '}' . PHP_EOL . PHP_EOL;
		}

		return $code;
	}

}