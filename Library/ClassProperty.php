<?php

/**
 * ClassProperty
 *
 * Represents a property class
 */
class ClassProperty
{

	protected $_visibility;

	protected $_name;

	protected $defaultValue;

	public function __construct($visibility, $name, $defaultValue=null)
	{
		$this->_visibility = $visibility;
		$this->_name = $name;
		$this->_defaultValue = $defaultValue;
	}

	public function getName()
	{
		return $this->_name;
	}

	public function getVisibilityAccesor()
	{
		return 'ZEND_ACC_PROTECTED';
	}

	public function compile(CompilationContext $compilationContext)
	{
		if (!is_array($this->_defaultValue)) {
			$compilationContext->codePrinter->output("zend_declare_property_null(" .
				$compilationContext->classDefinition->getClassEntry() .
				", SL(\"" . $this->getName() . "\"), " .
				$this->getVisibilityAccesor() . " TSRMLS_CC);");
		} else {
			switch ($this->_defaultValue['type']) {
				case 305:
					$compilationContext->codePrinter->output("zend_declare_property_bool(" .
						$compilationContext->classDefinition->getClassEntry() .
						", SL(\"" . $this->getName() . "\"), 0, " .
						$this->getVisibilityAccesor() . " TSRMLS_CC);");
					break;
				default:
					$compilationContext->codePrinter->output("zend_declare_property_null(" .
						$compilationContext->classDefinition->getClassEntry() .
						", SL(\"" . $this->getName() . "\"), " .
						$this->getVisibilityAccesor() . " TSRMLS_CC);");
			}
		}
	}

}
