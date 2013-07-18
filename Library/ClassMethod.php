<?php

/**
 * ClassMethod
 *
 * Represents a class method
 */
class ClassMethod
{

	protected $_visibility;

	protected $_name;

	protected $_parameters;

	protected $_statements;

	/**
	 * ClassMethod constructor
	 *
	 * @param string $visibility
	 */
	public function __construct($visibility, $name, $parameters, StatementsBlock $statements=null)
	{
		$this->_visibility = $visibility;
		$this->_name = $name;
		$this->_parameters = $parameters;
		$this->_statements = $statements;
	}

	public function getName()
	{
		return $this->_name;
	}

	public function getVisibilityAccesor()
	{
		return 'ZEND_ACC_PROTECTED';
	}

	/**
	 * Compiles the method
	 *
	 */
	public function compile(CompilationContext $compilationContext)
	{

		$symbolTable = new SymbolTable();

		$compilationContext->symbolTable = $symbolTable;

		if (is_object($this->_parameters)) {
			$params = array();
			foreach ($this->_parameters->getParameters() as $parameter) {
				$params[] = '&' . $parameter['name'];
			}
			$compilationContext->codePrinter->output('test_fetch_params(' . join(', ', $params) . ');');
			$compilationContext->codePrinter->outputBlankLine();
		}

		/**
		 * <comment>Compile the block of statements if any</comment>
		 */
		if (is_object($this->_statements)) {
			$this->_statements->compile($compilationContext);
		}

		/**
		 * Check if there are unused variables
		 */
		foreach ($symbolTable->getVariables() as $variable) {
			if ($variable->getNumberUses() <= 0) {
				echo 'Warning: Variable "' . $variable->getName() . '" declared but not used in ' .
					$compilationContext->classDefinition->getName() . '::' . $this->getName(), PHP_EOL;
			}
		}

		return null;
	}

}