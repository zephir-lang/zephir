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

	public function compile(CodePrinter $codePrinter)
	{		
		$codePrinter->outputBlankLine();
		$codePrinter->output('PHP_INIT_CLASS(' . $this->getName() . ') {');

		$codePrinter->increaseLevel();
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