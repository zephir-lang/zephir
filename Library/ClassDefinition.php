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
 * Represents a class/interface and their properties and methods
 */
class ClassDefinition
{

	protected $_namespace;

	protected $_name;

	protected $_extendsClass;

	protected $_extendsClassDefinition;

	protected $_properties = array();

	protected $_constants = array();

	protected $_methods = array();

	protected $_dependencyRank = 0;

	/**
	 * ClassDefinition
	 *
	 * @param string $namespace
	 * @param string $name
	 */
	public function __construct($namespace, $name)
	{
		$this->_namespace = $namespace;
		$this->_name = $name;
	}

	/**
	 * Returns the class name without namespace
	 *
	 * @return string
	 */
	public function getName()
	{
		return $this->_name;
	}

	/**
	 * Returns the class name including its namespace
	 *
	 * @return string
	 */
	public function getCompleteName()
	{
		return $this->_namespace . '\\' . $this->_name;
	}

	/**
	 * Sets the extended class
	 *
	 * @param string $extendsClass
	 */
	public function setExtendsClass($extendsClass)
	{
		if (substr($extendsClass, 0, 1) == '\\') {
			$extendsClass = substr($extendsClass, 1);
		}
		$this->_extendsClass = $extendsClass;
	}

	/**
	 * Returns the extended class
	 *
	 * @return string
	 */
	public function getExtendsClass()
	{
		return $this->_extendsClass;
	}

	/**
	 * Sets the class definition for the extended class
	 *
	 * @param \ClassDefinition $classDefinition
	 */
	public function setExtendsClassDefinition($classDefinition)
	{
		$this->_extendsClassDefinition = $classDefinition;
	}

	/**
	 * Returns the class definition related to the extended class
	 *
	 * @return \ClassDefinition
	 */
	public function getExtendsClassDefinition()
	{
		return $this->_extendsClassDefinition;
	}

	/**
	 * Calculate the dependency rank of the class based on its dependencies
	 *
	 */
	public function calculateDependencyRank()
	{
		if ($this->_extendsClassDefinition) {
			$classDefinition = $this->_extendsClassDefinition;
			if (method_exists($classDefinition, 'increaseDependencyRank')) {
				$classDefinition->increaseDependencyRank();
			}
		}
	}

	/**
	 * A class definition calls this method to mark this class as a dependency of another
	 *
	 */
	public function increaseDependencyRank()
	{
		$this->_dependencyRank++;
	}

	/**
	 * Returns the dependency rank for this class
	 *
	 * @return int
	 */
	public function getDependencyRank()
	{
		return $this->_dependencyRank;
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
	 * Adds a property to the definition
	 *
	 * @param ClassConstant $constant
	 */
	public function addConstant(ClassConstant $constant)
	{
		if (isset($this->_constants[$constant->getName()])) {
			throw new Exception("Constant '" . $constant->getName() . "' was defined more than one time");
		}
		$this->_constants[$constant->getName()] = $constant;
	}

	/**
	 * Checks if class definition has a property
	 *
	 * @param string $name
	 */
	public function hasProperty($name)
	{
		$exists = isset($this->_properties[$name]);
		if ($exists) {
			return true;
		} else {
			$extendsClassDefinition = $this->_extendsClassDefinition;
			if ($extendsClassDefinition) {
				if ($extendsClassDefinition->hasProperty($name)) {
					return true;
				}
			}
			return false;
		}
	}

	/**
	 * Checks if class definition has a property
	 *
	 * @param string $name
	 */
	public function hasConstant($name)
	{
		return isset($this->_constants[$name]);
	}

	/**
	 * Adds a method to the class definition
	 *
	 * @param \ClassMethod $method
	 * @param array $statement
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
	 *
	 * @return \ClassProperties[]
	 */
	public function getProperties()
	{
		return $this->_properties;
	}

	/**
	 * Returns all constants defined in the class
	 *
	 * @return \ClassConstant[]
	 */
	public function getConstants()
	{
		return $this->_constants;
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
		$extendsClassDefinition = $this->_extendsClassDefinition;
		if ($extendsClassDefinition) {
			if ($extendsClassDefinition->hasMethod($methodName)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Returns a method by its name
	 *
	 * @param string string
	 * @return boolean
	 */
	public function getMethod($methodName)
	{
		foreach ($this->_methods as $name => $method) {
			if (!strcasecmp($methodName, $name)) {
				return $method;
			}
		}

		$extendsClassDefinition = $this->_extendsClassDefinition;
		if ($extendsClassDefinition) {
			if ($extendsClassDefinition->hasMethod($methodName)) {
				return $extendsClassDefinition->getMethod($methodName);
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
	 *
	 * @return string
	 */
	public function getCNamespace()
	{
		return str_replace('\\', '_', $this->_namespace);
	}

	/**
	 * Returns a valid namespace to be used in C-sources
	 *
	 * @return string
	 */
	public function getNCNamespace()
	{
		return str_replace('\\', '\\\\', $this->_namespace);
	}

	/**
	 * Class name without namespace prefix for class registration
	 *
	 * @param string $namespace
	 * @return string
	 */
	public function getSCName($namespace)
	{
		return str_replace($namespace . "_", "", strtolower(str_replace('\\', '_', $this->_namespace) . '_' . $this->_name));
	}

	/**
	 * Compiles a class/interface
	 *
	 * @param CompilationContext $compilationContext
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
		 * Method entry
		 */
		if (count($this->_methods)) {
			$methodEntry = strtolower($this->getCNamespace()) . '_' . strtolower($this->getName()) . '_method_entry';
		} else {
			$methodEntry = 'NULL';
		}

		$namespace = $compilationContext->config->get('namespace');

		/**
		 * Register the class with extends + interfaces
		 */
		if ($this->_extendsClass) {

			if (substr($this->_extendsClass, 0, 1) == '\\') {
				$extendsClass = substr($this->_extendsClass, 1);
			} else {
				$extendsClass = $this->_extendsClass;
			}
			$extendsClass = str_replace("\\", "\\\\", $extendsClass);

			$codePrinter->output('ZEPHIR_REGISTER_CLASS_EX(' . $this->getNCNamespace() . ', ' . $this->getName() . ', ' .
				strtolower($this->getSCName($namespace)) . ', ' . '"' . strtolower($extendsClass) . '", ' . $methodEntry . ', 0);');
			$codePrinter->outputBlankLine();
		} else {
			$codePrinter->output('ZEPHIR_REGISTER_CLASS(' . $this->getNCNamespace() . ', ' . $this->getName() . ', ' .
				strtolower($this->getSCName($namespace)) . ', ' . $methodEntry . ', 0);');
			$codePrinter->outputBlankLine();
		}

		/**
		 * Compile properties
		 */
		foreach ($this->getProperties() as $property) {
			$docBlock = $property->getDocBlock();
			if ($docBlock) {
				$codePrinter->outputDocBlock($docBlock);
			}
			$property->compile($compilationContext);
		}

		/**
		 * Compile constants
		 */
		foreach ($this->getConstants() as $constant) {
			$docBlock = $constant->getDocBlock();
			if ($docBlock) {
				$codePrinter->outputDocBlock($docBlock);
			}
			$constant->compile($compilationContext);
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
				$codePrinter->output('ZEND_BEGIN_ARG_INFO_EX(arginfo_' . strtolower($this->getCNamespace() . '_' . $this->getName() . '_' . $method->getName()) . ', 0, 0, 0)');
				foreach ($parameters as $parameters) {
					foreach ($parameters as $parameter) {
						$codePrinter->output('	ZEND_ARG_INFO(0, ' . $parameter['name'] . ')');
					}
				}
				$codePrinter->output('ZEND_END_ARG_INFO()');
				$codePrinter->outputBlankLine();
			}

		}

		if (count($methods)) {
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
		}

		$compilationContext->headerPrinter = $codePrinter;
	}

}