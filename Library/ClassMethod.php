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

		$oldCodePrinter = $compilationContext->codePrinter;

		/**
		 * Change the code printer to a single method instance
		 */
		$codePrinter = new CodePrinter();
		$compilationContext->codePrinter = $codePrinter;

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
		$usedVariables = array();
		foreach ($symbolTable->getVariables() as $variable) {
			if ($variable->getNumberUses() <= 0) {
				echo 'Warning: Variable "' . $variable->getName() . '" declared but not used in ' .
					$compilationContext->classDefinition->getName() . '::' . $this->getName(), PHP_EOL;
			} else {
				if ($variable->getName() != 'this') {
					if (!isset($usedVariables[$variable->getType()])) {
						$usedVariables[$variable->getType()] = array();
					}
					$usedVariables[$variable->getType()][] = $variable;
				}
			}
		}

		foreach ($usedVariables as $type => $variables) {

			$pointer = null;
			switch ($type) {
				case 'int':
					$code = 'int ';
					break;
				case 'double':
					$code = 'double ';
					break;
				case 'variable':
					$pointer = '*';
					$code = 'zval ';
					break;
				default:
					throw new Exception("Unsupported type in declare " . $type);
			}

			$groupVariables = array();
			foreach ($variables as $variable) {
				//var_dump($variable);
				$groupVariables[] = $pointer . $variable->getName();
			}

			$codePrinter->preOutput("\t" . $code . join(', ', $groupVariables) . ';');
		}

		/**
		 * Restore the compilation context
		 */
		$oldCodePrinter->output($codePrinter->getOutput());
		$compilationContext->codePrinter = $oldCodePrinter;

		return null;
	}

}