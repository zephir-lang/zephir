<?php

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

	public function compile(CodePrinter $codePrinter, ClassDefinition $classDefinition)
	{
		if (!is_array($this->_defaultValue)) {			
			$codePrinter->output("zend_declare_property_null(" . 
				$classDefinition->getClassEntry() . 
				", SL(\"" . $this->getName() . "\"), " . 
				$this->getVisibilityAccesor() . " TSRMLS_CC);");
		} else {
			switch ($this->_defaultValue['type']) {
				case 305:
					$codePrinter->output("zend_declare_property_bool(" . 
						$classDefinition->getClassEntry() . 
						", SL(\"" . $this->getName() . "\"), 0, " . 
						$this->getVisibilityAccesor() . " TSRMLS_CC);");
				default:
					$codePrinter->output("zend_declare_property_null(" . 
						$classDefinition->getClassEntry() . 
						", SL(\"" . $this->getName() . "\"), " . 
						$this->getVisibilityAccesor() . " TSRMLS_CC);");
			}
		}
	}
}