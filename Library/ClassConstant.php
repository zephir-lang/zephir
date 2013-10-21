<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

/**
 * ClassConstant
 *
 * Represents a constant class
 */
class ClassConstant
{

	protected $_name;

	protected $value;

	protected $_docblock;

	public function __construct($name, $value, $docBlock)
	{
		$this->_name = $name;
		$this->_value = $value;
		$this->_docblock = $docBlock;
	}

	/**
	 * Returns the constant's name
	 *
	 * @return string
	 */
	public function getName()
	{
		return $this->_name;
	}

	/**
	 * Returns the constant's value
	 *
	 * @return array
	 */
	public function getValue()
	{
		return $this->_value;
	}

	/**
	 * Returns the docblock related to the constant
	 *
	 * @return string
	 */
	public function getDocBlock()
	{
		return $this->_docblock;
	}

	/**
	 * Produce the code to register a property
	 *
	 * @param CompilationContext $compilationContext
	 */
	public function compile(CompilationContext $compilationContext)
	{

		switch ($this->_value['type']) {
			case 'long':
			case 'int':
				$compilationContext->codePrinter->output("zend_declare_class_constant_long(" .
					$compilationContext->classDefinition->getClassEntry() .
					", SL(\"" . $this->getName() . "\"), " . $this->_value['value'] . " TSRMLS_CC);");
				break;
			case 'double':
				$compilationContext->codePrinter->output("zend_declare_class_constant_double(" .
					$compilationContext->classDefinition->getClassEntry() .
					", SL(\"" . $this->getName() . "\"), " . $this->_value['value'] . " TSRMLS_CC);");
				break;
			case 'bool':
				if ($this->_value['value'] == 'false') {
					$compilationContext->codePrinter->output("zend_declare_class_constant_bool(" .
						$compilationContext->classDefinition->getClassEntry() .
						", SL(\"" . $this->getName() . "\"), 0 TSRMLS_CC);");
				} else {
					$compilationContext->codePrinter->output("zend_declare_class_constant_bool(" .
						$compilationContext->classDefinition->getClassEntry() .
						", SL(\"" . $this->getName() . "\"), 1 TSRMLS_CC);");
				}
				break;
			case 'string':
				$compilationContext->codePrinter->output("zend_declare_class_constant_string(" .
					$compilationContext->classDefinition->getClassEntry() .
					", SL(\"" . $this->getName() . "\"), \"" . Utils::addSlashes($this->_value['value']) . "\" TSRMLS_CC);");
				break;
			default:
				$compilationContext->codePrinter->output("zend_declare_class_constant_null(" .
					$compilationContext->classDefinition->getClassEntry() .
					", SL(\"" . $this->getName() . "\") TSRMLS_CC);");
		}
	}

}
