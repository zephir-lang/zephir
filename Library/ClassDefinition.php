<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the MIT license,       |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the MIT license and are unable      |
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

	protected $_type = 'class';

	protected $_extendsClass;

	protected $_interfaces;

	protected $_final;

	protected $_abstract;

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
	 * Set the class' type (class/interface)
	 *
	 * @param string $type
	 */
	public function setType($type)
	{
		$this->_type = $type;
	}

	/**
	 * Returns the class type
	 *
	 * @return string
	 */
	public function getType()
	{
		return $this->_type;
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
	 * Sets if the class is final
	 *
	 * @param boolean $final
	 */
	public function setIsFinal($final)
	{
		$this->_final = $final;
	}

	/**
	 * Sets if the class is final
	 *
	 * @param boolean $abstract
	 */
	public function setIsAbstract($abstract)
	{
		$this->_abstract = $abstract;
	}

	/**
	 * Checks whether the class is abstract or not
	 *
	 * @return boolean
	 */
	public function isAbstract()
	{
		return $this->_abstract;
	}

	/**
	 * Checks whether the class is abstract or not
	 *
	 * @return boolean
	 */
	public function isFinal()
	{
		return $this->_final;
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
	 * Sets the implemented interfaces
	 *
	 * @param string $implementedInterfaces
	 */
	public function setImplementsInterfaces($implementedInterfaces)
	{
		$interfaces = array();
		foreach ($implementedInterfaces as $implementedInterface) {
			if (substr($implementedInterface['value'], 0, 1) == '\\') {
				$implementedInterface['value'] = substr($implementedInterface['value'], 1);
			}
			$interfaces[] = $implementedInterface['value'];
		}
		$this->_interfaces = $interfaces;
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
	 * Returns the implemented interfaces
	 *
	 * @return array
	 */
	public function getImplementedInterfaces()
	{
		return $this->_interfaces;
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
				$classDefinition->increaseDependencyRank($this->_dependencyRank * 2);
			}
		}
	}

	/**
	 * A class definition calls this method to mark this class as a dependency of another
	 *
	 * @param int $rank
	 */
	public function increaseDependencyRank($rank)
	{
		$this->_dependencyRank += ($rank + 1);
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
			throw new CompilerException("Property '" . $property->getName() . "' was defined more than one time", $property->getOriginal());
		}
		$this->_properties[$property->getName()] = $property;
	}

	/**
	 * Adds a constant to the definition
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
	 * Checks if a class definition has a property
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
	 * Returns a method definition by its name
	 *
	 * @param string string
	 * @return boolean
	 */
	public function getProperty($propertyName)
	{

		if (isset($this->_properties[$propertyName])) {
			return $this->_properties[$propertyName];
		}

		$extendsClassDefinition = $this->_extendsClassDefinition;
		if ($extendsClassDefinition) {
			if ($extendsClassDefinition->hasProperty($propertyName)) {
				return $extendsClassDefinition->getProperty($propertyName);
			}
		}
		return false;
	}

	/**
	 * Checks if class definition has a property
	 *
	 * @param string $name
	 */
	public function hasConstant($name)
	{
		if (isset($this->_constants[$name]))
		{
			return true; 
		}
		/**
		 * @todo add code to check if constant is defined in interfaces
		 */
		return false;
	}

	/**
	 * Returns a constant definition by its name
	 *
	 * @param string string
	 * @return boolean
	 */
	public function getConstant($constantName)
	{
		if (isset($this->_constants[$constantName])) {
			return $this->_constants[$constantName];
		}
		/**
		 * @todo add code to get constant from interfaces
		 */
		return false;
	}

	/**
	 * Adds a method to the class definition
	 *
	 * @param \ClassMethod $method
	 * @param array $statement
	 */
	public function addMethod(ClassMethod $method, $statement=null)
	{
		$methodName = strtolower($method->getName());
		if (isset($this->_methods[$methodName])) {
			throw new CompilerException("Method '" . $method->getName() . "' was defined more than one time", $statement);
		}
		$this->_methods[$methodName] = $method;
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
		$methodNameLower = strtolower($methodName);
		foreach ($this->_methods as $name => $method) {
			if ($methodNameLower == $name) {
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
		$methodNameLower = strtolower($methodName);
		foreach ($this->_methods as $name => $method) {
			if ($methodNameLower == $name) {
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
	 * Checks if a class implements an interface
	 *
	 * @param ClassDefinition $classDefinition
	 * @param ClassDefinition $interfaceDefinition
	 */
	public function checkInterfaceImplements($classDefinition, $interfaceDefinition)
	{
		foreach ($interfaceDefinition->getMethods() as $method) {
			if (!$classDefinition->hasMethod($method->getName())) {
				if (method_exists($classDefinition, 'getCompleteName')) {
					throw new CompilerException("Class " . $classDefinition->getCompleteName() . " must implement method: " . $method->getName() . " defined on interface: " . $interfaceDefinition->getCompleteName());
				} else {
					throw new CompilerException("Class " . $classDefinition->getCompleteName() . " must implement method: " . $method->getName() . " defined on interface: " . $interface);
				}
			}
		}
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
		$methods = $this->_methods;
		if (count($methods)) {
			$methodEntry = strtolower($this->getCNamespace()) . '_' . strtolower($this->getName()) . '_method_entry';
		} else {
			$methodEntry = 'NULL';
		}

		$namespace = $compilationContext->config->get('namespace');

		/**
		 * Register the class with extends + interfaces
		 */
		$classExtendsDefinition = null;
		if ($this->_extendsClass) {

			$classExtendsDefinition = $this->_extendsClassDefinition;
			if ($classExtendsDefinition instanceof ClassDefinition) {
				$classEntry = $classExtendsDefinition->getClassEntry();
			} else {
				switch (strtolower($classExtendsDefinition->getName())) {
					case 'arrayaccess':
						$classEntry = 'zend_ce_arrayaccess';
						break;
					case 'exception':
						$classEntry = 'zend_exception_get_default(TSRMLS_C)';
						break;
					default:
						throw new Exception(get_class($classExtendsDefinition) . '-' . $classExtendsDefinition->getName());
				}
			}

			if ($this->getType() == 'class') {
				$codePrinter->output('ZEPHIR_REGISTER_CLASS_EX(' . $this->getNCNamespace() . ', ' . $this->getName() . ', ' . $namespace  . ', ' .
					strtolower($this->getSCName($namespace)) . ', ' . $classEntry . ', ' . $methodEntry . ', 0);');
				$codePrinter->outputBlankLine();
			} else {
				$codePrinter->output('ZEPHIR_REGISTER_INTERFACE_EX(' . $this->getNCNamespace() . ', ' . $this->getName() . ', ' . $namespace  . ', ' .
					strtolower($this->getSCName($namespace)) . ', ' . $classEntry . ', ' . $methodEntry . ');');
				$codePrinter->outputBlankLine();
			}
		} else {
			if ($this->getType() == 'class') {
				$codePrinter->output('ZEPHIR_REGISTER_CLASS(' . $this->getNCNamespace() . ', ' . $this->getName() . ', ' . $namespace . ', ' .
					strtolower($this->getSCName($namespace)) . ', ' . $methodEntry . ', 0);');
			} else {
				$codePrinter->output('ZEPHIR_REGISTER_INTERFACE(' . $this->getNCNamespace() . ', ' . $this->getName() . ', ' . $namespace . ', ' .
					strtolower($this->getSCName($namespace)) . ', ' . $methodEntry . ');');
			}
			$codePrinter->outputBlankLine();
		}

		/**
		 * Compile properties
		 */
		foreach ($this->getProperties() as $property) {
			$docBlock = $property->getDocBlock();
			if ($docBlock) {
				$codePrinter->outputDocBlock($docBlock, false);
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

		/**
		 * Implemented interfaces
		 */
		$interfaces = $this->_interfaces;
		$compiler = $compilationContext->compiler;

		if (is_array($interfaces)) {

			$interfacesEntries = array();
			$codePrinter->outputBlankLine(true);

			foreach ($interfaces as $interface) {

				if ($compiler->isInterface($interface)) {
					$classInterfaceDefinition = $compiler->getClassDefinition($interface);
					$classEntry = $classInterfaceDefinition->getClassEntry();
				} else {
					if ($compiler->isInternalInterface($interface)) {
						$classInterfaceDefinition = $compiler->getInternalClassDefinition($interface);
						switch (strtolower($classInterfaceDefinition->getName())) {
							case 'iterator':
								$classEntry = 'zend_ce_iterator';
								break;
							case 'countable':
								$classEntry = 'spl_ce_Countable';
								break;
							case 'arrayaccess':
								$classEntry = 'zend_ce_arrayaccess';
								break;
							case 'seekableiterator':
								$classEntry = 'spl_ce_SeekableIterator';
								break;
							case 'serializable':
								$classEntry = 'zend_ce_serializable';
								break;
							default:
								throw new CompilerException($classInterfaceDefinition->getName());
						}
					} else {
						throw new CompilerException("Cannot locate interface " . $interface . " when implementing interfaces on " . $this->getCompleteName());
					}
				}

				/**
				 * We dont' check if abstract classes implement the methods in their interfaces
				 */
				if (!$this->isAbstract()) {
					$this->checkInterfaceImplements($this, $classInterfaceDefinition);
				}

				$codePrinter->output('zend_class_implements(' . $this->getClassEntry() . ' TSRMLS_CC, 1, ' . $classEntry . ');');
			}
		}

		if (!$this->isAbstract()) {

			/**
			 * Interfaces in extended classes may have
			 */
			if ($classExtendsDefinition) {

				if ($classExtendsDefinition instanceof ClassDefinition) {
					$interfaces = $classExtendsDefinition->getImplementedInterfaces();
					if (is_array($interfaces)) {
						foreach ($interfaces as $interface) {

							$classInterfaceDefinition = null;
							if ($compiler->isInterface($interface)) {
								$classInterfaceDefinition = $compiler->getClassDefinition($interface);
							} else {
								if ($compiler->isInternalInterface($interface)) {
									$classInterfaceDefinition = $compiler->getInternalClassDefinition($interface);
								}
							}

							if ($classInterfaceDefinition) {
								$this->checkInterfaceImplements($this, $classInterfaceDefinition);
							}

						}
					}
				}

			}
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
		foreach ($methods as $method) {

			$docBlock = $method->getDocBlock();
			if ($docBlock) {
				$codePrinter->outputDocBlock($docBlock);
			}

			if ($this->getType() == 'class') {
				$codePrinter->output('PHP_METHOD(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ') {');
				$codePrinter->outputBlankLine();

				$method->compile($compilationContext);

				$codePrinter->output('}');
				$codePrinter->outputBlankLine();
			} else {
				$codePrinter->output('ZEPHIR_DOC_METHOD(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ');');
				$codePrinter->outputBlankLine();
			}
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

		if ($this->getType() == 'class') {
			if (count($methods)) {
				foreach ($methods as $method) {
					$codePrinter->output('PHP_METHOD(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ');');
				}
				$codePrinter->outputBlankLine();
			}
		}

		/**
		 * Create arg. info
		 */
		foreach ($methods as $method) {

			$parameters = $method->getParameters();
			if (count($parameters)) {
				$codePrinter->output('ZEND_BEGIN_ARG_INFO_EX(arginfo_' . strtolower($this->getCNamespace() . '_' . $this->getName() . '_' . $method->getName()) . ', 0, 0, ' . $method->getNumberOfRequiredParameters() . ')');
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
				if ($this->getType() == 'class') {
					if (count($parameters)) {
						$codePrinter->output("\t" . 'PHP_ME(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ', arginfo_' . strtolower($this->getCNamespace() . '_' . $this->getName() . '_' . $method->getName()) . ', ' . $method->getModifiers() . ')');
					} else {
						$codePrinter->output("\t" . 'PHP_ME(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ', NULL, ' . $method->getModifiers() . ')');
					}
				} else {
					if ($method->isStatic()) {
						if (count($parameters)) {
							$codePrinter->output("\t" . 'ZEND_FENTRY(' . $method->getName() . ', NULL, arginfo_' . strtolower($this->getCNamespace() . '_' . $this->getName() . '_' . $method->getName()) . ', ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)');
						} else {
							$codePrinter->output("\t" . 'ZEND_FENTRY(' . $method->getName() . ', NULL, NULL, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)');
						}
					} else {
						if (count($parameters)) {
							$codePrinter->output("\t" . 'PHP_ABSTRACT_ME(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ', arginfo_' . strtolower($this->getCNamespace() . '_' . $this->getName() . '_' . $method->getName()) . ')');
						} else {
							$codePrinter->output("\t" . 'PHP_ABSTRACT_ME(' . $this->getCNamespace() . '_' . $this->getName() . ', ' . $method->getName() . ', NULL)');
						}
					}
				}
			}
			$codePrinter->output('	PHP_FE_END');
			$codePrinter->output('};');
		}

		$compilationContext->headerPrinter = $codePrinter;
	}

}
