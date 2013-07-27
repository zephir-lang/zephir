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
		if (isset($this->_properties[$property->getName()])) {
			throw new Exception("Property '" . $property->getName() . "' was defined more than one time");
		}
		$this->_properties[$property->getName()] = $property;
	}

	public function hasProperty($name)
	{
		return isset($this->_properties[$name]);
	}

	public function addMethod(ClassMethod $method, $statement=null)
	{
		if (isset($this->_methods[$method->getName()])) {
			throw new CompilerException("Method '" . $method->getName() . "' was defined more than one time", $statement);
		}
		$this->_methods[$method->getName()] = $method;
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
		 * Register the class
		 */
		$codePrinter->output('ZEPHIR_REGISTER_CLASS(' . $this->getCNamespace() . ', ' . $this->getName() . ', ' .
			strtolower($this->getSCName()) . ', ' . strtolower($this->getCNamespace()) . '_' .
			strtolower($this->getName()) . '_method_entry, 0);');
		$codePrinter->outputBlankLine();

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

			if ($method->getDocBlock()) {
				$codePrinter->outputDocBlock($method->getDocBlock());
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
				$codePrinter->output("\t" . 'PHP_ME(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ', arginfo_' . strtolower($this->getCNamespace() . '_' . $this->getName()) . '_' . $method->getName() . ', ZEND_ACC_PUBLIC)');
			} else {
				$codePrinter->output("\t" . 'PHP_ME(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ', NULL, ZEND_ACC_PUBLIC)');
			}
		}
		$codePrinter->output('	PHP_FE_END');
		$codePrinter->output('};');

		$compilationContext->headerPrinter = $codePrinter;
	}

}