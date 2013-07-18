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
	public function compile(CompilationContext $compilationContext)
	{
		$compilationContext->classDefinition = $this;

		$compilationContext->codePrinter->outputBlankLine();

		$compilationContext->codePrinter->output('TEST_INIT_CLASS(' . $this->getCNamespace() . '_' . $this->getName() . ') {');
		$compilationContext->codePrinter->outputBlankLine();

		$compilationContext->codePrinter->increaseLevel();

		/**
		 * Register the class
		 */
		$compilationContext->codePrinter->output('TEST_REGISTER_CLASS(' . $this->getCNamespace() . ', ' . $this->getName() . ', ' .
			strtolower($this->getName()) . ', ' . strtolower($this->getCNamespace()) . '_' .
			strtolower($this->getName()) . '_method_entry, 0);');
		$compilationContext->codePrinter->outputBlankLine();

		/**
		 * Compile properties
		 */
		foreach ($this->getProperties() as $property) {
			$property->compile($compilationContext);
		}

		$compilationContext->codePrinter->outputBlankLine();
		$compilationContext->codePrinter->output('return SUCCESS;');

		$compilationContext->codePrinter->outputBlankLine();
		$compilationContext->codePrinter->decreaseLevel();

		$compilationContext->codePrinter->output('}');
		$compilationContext->codePrinter->outputBlankLine();

		/**
		 * Compile methods
		 */
		foreach ($this->getMethods() as $method) {

			$compilationContext->codePrinter->output('PHP_METHOD(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ') {');
			$compilationContext->codePrinter->outputBlankLine();

			$method->compile($compilationContext);

			$compilationContext->codePrinter->output('}');
			$compilationContext->codePrinter->outputBlankLine();
		}

	}

}