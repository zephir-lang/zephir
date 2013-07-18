<?php

/**
 * ClassDefinition
 *
 * Represents a class and their properties and methods 
 */
class ClassDefinition
{

	protected $_namespace;

	protected $_name;

	protected $_properties = array();

	protected $_methods = array();

	public function __construct($namespace, $name)
	{
		$this->_namespace = $namespace;
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
		return strtolower(str_replace('\\', '_', $this->_namespace) . '_' . $this->_name) . '_ce';
	}

	/**
	 * Returns a valid namespace to be used in C-sources
	 */
	public function getCNamespace()
	{
		return str_replace('\\', '_', $this->_namespace);
	}

	/**
	 * Compiles a class
	 *
	 */
	public function compile(CodePrinter $codePrinter)
	{	

		$codePrinter->outputBlankLine();

		$codePrinter->output('TEST_INIT_CLASS(' . $this->getCNamespace() . '_' . $this->getName() . ') {');
		$codePrinter->outputBlankLine();

		$codePrinter->increaseLevel();

		/**
		 * Register the class
		 */
		$codePrinter->output('TEST_REGISTER_CLASS(' . $this->getCNamespace() . ', ' . $this->getName() . ', ' . 
			strtolower($this->getName()) . ', ' . strtolower($this->getCNamespace()) . '_' . 
			strtolower($this->getName()) . '_method_entry, 0);');
		$codePrinter->outputBlankLine();		

		/**
		 * Compile properties
		 */
		foreach ($this->getProperties() as $property) {			
			$property->compile($codePrinter, $this);				
		}

		$codePrinter->outputBlankLine();
		$codePrinter->output('return SUCCESS;');

		$codePrinter->outputBlankLine();
		$codePrinter->decreaseLevel();

		$codePrinter->output('}');
		$codePrinter->outputBlankLine();

		/**
		 * Compile methods
		 */
		foreach ($this->getMethods() as $method) {			

			$codePrinter->output('PHP_METHOD(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ') {');
			$codePrinter->outputBlankLine();

			$codePrinter->increaseLevel();

			$method->compile($codePrinter, $this);

			$codePrinter->decreaseLevel();

			$codePrinter->output('}');
			$codePrinter->outputBlankLine();
		}

	}

}