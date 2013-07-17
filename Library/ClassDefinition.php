<?php

/**
 * ClassDefinition
 *
 * Represents a class and their properties and methods 
 */
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

	/**
	 * Compiles a class
	 *
	 */
	public function compile(CodePrinter $codePrinter)
	{		
		$codePrinter->outputBlankLine();

		$codePrinter->output('TEST_INIT_CLASS(' . $this->getName() . ') {');
		$codePrinter->outputBlankLine();

		$codePrinter->increaseLevel();

		/**
		 * Register the class
		 */
		$codePrinter->output('TEST_REGISTER_CLASS(Test, Router, router, router_method_entry, 0);');
		$codePrinter->outputBlankLine();		

		/**
		 * Compile properties
		 */
		foreach ($this->getProperties() as $property) {			
			$property->compile($codePrinter, $this);				
		}

		$codePrinter->outputBlankLine();
		$codePrinter->decreaseLevel();

		$codePrinter->output('}');
		$codePrinter->outputBlankLine();

		/**
		 * Compile methods
		 */
		foreach ($this->getMethods() as $method) {			

			$codePrinter->output('PHP_METHOD(' . $this->getName() . ', ' . $method->getName() . ') {');
			$codePrinter->outputBlankLine();

			$codePrinter->increaseLevel();

			$method->compile($codePrinter, $this);

			$codePrinter->decreaseLevel();

			$codePrinter->output('}');
			$codePrinter->outputBlankLine();
		}

	}

}