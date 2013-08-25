<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

/**
 * ClassDefinition
 *
 * Represents a class and their properties and methods
 */
class ClassDefinition
{

	protected $_namespace;

	protected $_name;

	protected $_extendsClass;

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

	/**
	 * Sets the extended class
	 *
	 * @param string $extendsClass
	 */
	public function setExtendsClass($extendsClass)
	{
		$this->_extendsClass = $extendsClass;
	}

	/**
	 * Adds a property to the definition
	 *
	 * @param ClassProperty $property
	 */
	public function addProperty(ClassProperty $property)
	{
		if (isset($this->_properties[$property->getName()])) {
			throw new Exception("Property '" . $property->getName() . "' was defined more than one time");
		}
		$this->_properties[$property->getName()] = $property;
	}

	/**
	 * Checks if class definition has a property
	 *
	 * @param string $name
	 */
	public function hasProperty($name)
	{
		return isset($this->_properties[$name]);
	}

	/**
	 * Adds a method to the class definition
	 */
	public function addMethod(ClassMethod $method, $statement=null)
	{
		if (isset($this->_methods[$method->getName()])) {
			throw new CompilerException("Method '" . $method->getName() . "' was defined more than one time", $statement);
		}
		$this->_methods[$method->getName()] = $method;
	}

	/**
	 * Returns all properties defined in the class
	 */
	public function getProperties()
	{
		return $this->_properties;
	}

	/**
	 * Returns all methods defined in the class
	 *
	 * @param string
	 */
	public function getMethods()
	{
		return $this->_methods;
	}

	/**
	 * Checks if the class implements an specific name
	 *
	 * @param string string
	 * @return boolean
	 */
	public function hasMethod($methodName)
	{
		foreach ($this->_methods as $name => $method) {
			if (!strcasecmp($methodName, $name)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Returns the name of the zend_class_entry according to the class name
	 *
	 * @return string
	 */
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
	 * Returns a valid namespace to be used in C-sources
	 */
	public function getNCNamespace()
	{
		return str_replace('\\', '\\\\', $this->_namespace);
	}

	public function getSCName()
	{
		return str_replace("test_", "", strtolower(str_replace('\\', '_', $this->_namespace) . '_' . $this->_name));
	}

	/**
	 * Compiles a class
	 *
	 */
	public function compile(CompilationContext $compilationContext)
	{
		$compilationContext->classDefinition = $this;

		$codePrinter = $compilationContext->codePrinter;

		/**
		 * The ZEPHIR_INIT_CLASS defines properties and constants exported by the class
		 */
		$codePrinter->output('ZEPHIR_INIT_CLASS(' . $this->getCNamespace() . '_' . $this->getName() . ') {');
		$codePrinter->outputBlankLine();

		$codePrinter->increaseLevel();

		/**
		 * Register the class with extends + interfaces
		 */
		if ($this->_extendsClass) {
			if (substr($this->_extendsClass, 0, 1) == '\\') {
				$extendsClass = substr($this->_extendsClass, 1);
			} else {
				$extendsClass = $this->_extendsClass;
			}
			$codePrinter->output('ZEPHIR_REGISTER_CLASS_EX(' . $this->getNCNamespace() . ', ' . $this->getName() . ', ' .
				strtolower($this->getSCName()) . ', ' . '"' . strtolower($extendsClass) . '", ' .
				strtolower($this->getCNamespace()) . '_' . strtolower($this->getName()) . '_method_entry, 0);');
			$codePrinter->outputBlankLine();
		} else {
			$codePrinter->output('ZEPHIR_REGISTER_CLASS(' . $this->getNCNamespace() . ', ' . $this->getName() . ', ' .
				strtolower($this->getSCName()) . ', ' . strtolower($this->getCNamespace()) . '_' .
				strtolower($this->getName()) . '_method_entry, 0);');
			$codePrinter->outputBlankLine();
		}

		/**
		 * Compile properties
		 */
		foreach ($this->getProperties() as $property) {
			$property->compile($compilationContext);
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

			$docBlock = $method->getDocBlock();
			if ($docBlock) {
				$codePrinter->outputDocBlock($docBlock);
			}

			$codePrinter->output('PHP_METHOD(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ') {');
			$codePrinter->outputBlankLine();

			$method->compile($compilationContext);

			$codePrinter->output('}');
			$codePrinter->outputBlankLine(true);
		}

		/**
		 * Create a code printer for the header file
		 */
		$codePrinter = new CodePrinter();

		$codePrinter->outputBlankLine();
		$codePrinter->output('extern zend_class_entry *' . $this->getClassEntry() . ';');
		$codePrinter->outputBlankLine();

		$codePrinter->output('ZEPHIR_INIT_CLASS(' . $this->getCNamespace() . '_' . $this->getName() . ');');
		$codePrinter->outputBlankLine();

		$methods = $this->getMethods();

		if (count($methods)) {
			foreach ($methods as $method) {
				$codePrinter->output('PHP_METHOD(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ');');
			}
			$codePrinter->outputBlankLine();
		}

		/**
		 * Create arg. info
		 */
		foreach ($methods as $method) {

			$parameters = $method->getParameters();
			if (count($parameters)) {
				$codePrinter->output('ZEND_BEGIN_ARG_INFO_EX(arginfo_' . strtolower($this->getCNamespace() . '_' . $this->getName()) . '_' . $method->getName() . ', 0, 0, 0)');
				foreach ($parameters as $parameters) {
					foreach ($parameters as $parameter) {
						$codePrinter->output('	ZEND_ARG_INFO(0, ' . $parameter['name'] . ')');
					}
				}
				$codePrinter->output('ZEND_END_ARG_INFO()');
				$codePrinter->outputBlankLine();
			}

		}

		$codePrinter->output('ZEPHIR_INIT_FUNCS(' . strtolower($this->getCNamespace() . '_' . $this->getName()) . '_method_entry) {');
		foreach ($methods as $method) {
			$parameters = $method->getParameters();
			if (count($parameters)) {
				$codePrinter->output("\t" . 'PHP_ME(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ', arginfo_' . strtolower($this->getCNamespace() . '_' . $this->getName()) . '_' . $method->getName() . ', ' . $method->getModifiers() . ')');
			} else {
				$codePrinter->output("\t" . 'PHP_ME(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ', NULL, ' . $method->getModifiers() . ')');
			}
		}
		$codePrinter->output('	PHP_FE_END');
		$codePrinter->output('};');

		$compilationContext->headerPrinter = $codePrinter;
	}

}