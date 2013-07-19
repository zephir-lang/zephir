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

	protected $_docblock;

	/**
	 * ClassMethod constructor
	 *
	 * @param string $visibility
	 * @param string $name
	 */
	public function __construct($visibility, $name, $parameters, StatementsBlock $statements=null, $docblock=null)
	{
		$this->_visibility = $visibility;
		$this->_name = $name;
		$this->_parameters = $parameters;
		$this->_statements = $statements;
		$this->_docblock = $docblock;
	}

	public function getName()
	{
		return $this->_name;
	}

	public function getDocBlock()
	{
		return $this->_docblock;
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

				if (isset($parameter['default_value'])) {
					//memory_grow, required_params, optional_params
				}

				//$params[] = '&' . $parameter['name'];

				if (isset($parameter['data-type'])) {
					$symbol = $symbolTable->addVariable($parameter['data-type'], $parameter['name']);
				} else {
					$symbol = $symbolTable->addVariable('variable', $parameter['name']);
				}

				$symbol->setIsExternal(true);
				$symbol->setIsInitialized(true);
			}
			//$compilationContext->codePrinter->output('zephir_fetch_params(' . join(', ', $params) . ');');
			//$compilationContext->codePrinter->outputBlankLine();
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