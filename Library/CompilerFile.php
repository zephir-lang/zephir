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
 * CompilerFile
 *
 * This class represents every file compiled in a project
 * Every file may contain a class or an interface
 */
class CompilerFile
{

	protected $_namespace;

	protected $_className;

	protected $_filePath;

	protected $_ir;

	protected $_compiledFile;

	/**
	 * @var \ClassDefinition
	 */
	protected $_classDefinition;

	/**
	 * CompilerFile constructor
	 *
	 * @param string $className
	 * @param string $filePath
	 */
	public function __construct($className, $filePath)
	{
		$this->_className = $className;
		$this->_filePath = $filePath;
		$this->_compiledFilePath = preg_replace('/\.zep$/', '', $className);
		$this->_filesCompiled = array();
	}

	/**
	 * Returns the class definition related to the compiled file
	 *
	 * @return \ClassDefinition
	 */
	public function getClassDefinition()
	{
		return $this->_classDefinition;
	}

	/**
	 * Compiles the file generating a JSON intermediate representation
	 *
	 * @return array
	 */
	public function genIR()
	{
		$compilePath = '.temp/' . str_replace(DIRECTORY_SEPARATOR, '.', realpath($this->_filePath)) . ".js";
		$zepRealPath = realpath($this->_filePath);
		if (file_exists($compilePath)) {
			if (filemtime($compilePath) < filemtime($zepRealPath)) {
				system(ZEPHIRPATH . '/bin/zephir-parser ' . $zepRealPath . ' > ' . $compilePath);
			}
		} else {
			system(ZEPHIRPATH . '/bin/zephir-parser ' . $zepRealPath . ' > ' . $compilePath);
		}
		return json_decode(file_get_contents($compilePath), true);
	}

	/**
	 * Compiles the class/interface contained in the file
	 *
	 * @param \CompilationContext $compilationContext
	 * @param string $namespace
	 * @param string $topStatement
	 */
	public function compileClass(CompilationContext $compilationContext, $namespace, $topStatement)
	{
		$classDefinition = $this->_classDefinition;
		if (!$classDefinition) {
			return;
		}

		/**
		 * Do the compilation
		 */
		$classDefinition->compile($compilationContext);

		$code  = '' . PHP_EOL;
		$code .= '#ifdef HAVE_CONFIG_H' . PHP_EOL;
		$code .= '#include "config.h"' . PHP_EOL;
		$code .= '#endif' . PHP_EOL;
		$code .= '' . PHP_EOL;

		$code .= '#include "php.h"' . PHP_EOL;
		$code .= '#include "php_test.h"' . PHP_EOL;
		$code .= '#include "test.h"' . PHP_EOL;
		$code .= '' . PHP_EOL;

		$code .= '#include "Zend/zend_operators.h"' . PHP_EOL;
		$code .= '#include "Zend/zend_exceptions.h"' . PHP_EOL;
		$code .= '#include "Zend/zend_interfaces.h"' . PHP_EOL;
		$code .= '' . PHP_EOL;

		$code .= '#include "kernel/main.h"' . PHP_EOL;

		foreach ($compilationContext->headersManager->get() as $header => $one) {
			$code .= '#include "' . $header . '.h"' . PHP_EOL;
		}

		/**
		 * Prepend the required files to the header
		 */
		$compilationContext->codePrinter->preOutput($code);
	}

	/**
	 * Compiles a comment as a top-level statement
	 */
	public function compileComment(CompilationContext $compilationContext, $topStatement)
	{
		$compilationContext->codePrinter->output('/' . $topStatement['value'] . '/');
	}

	/**
	 * Creates a definition for an interface
	 *
	 * @param string $namespace
	 * @param array $topStatement
	 */
	public function preCompileInterface($namespace, $topStatement)
	{
	}

	/**
	 * Creates a definition for a class
	 *
	 * @param string $namespace
	 * @param array $topStatement
	 */
	public function preCompileClass($namespace, $topStatement)
	{

		$classDefinition = new ClassDefinition($namespace, $topStatement['name']);

		if (isset($topStatement['extends'])) {
			$classDefinition->setExtendsClass($topStatement['extends']);
		}

		if (isset($topStatement['definition'])) {

			$definition = $topStatement['definition'];

			if (isset($definition['properties'])) {
				foreach ($definition['properties'] as $property) {
					$classDefinition->addProperty(new ClassProperty(
						$property['visibility'],
						$property['name'],
						isset($property['default']) ? $property['default'] : null,
						isset($property['docblock']) ? $property['docblock'] : null
					));
				}
			}

			if (isset($definition['constants'])) {
				foreach ($definition['constants'] as $constant) {
					$classDefinition->addConstant(new ClassConstant(
						$constant['name'],
						isset($constant['default']) ? $constant['default'] : null,
						isset($constant['docblock']) ? $constant['docblock'] : null
					));
				}
			}

			if (isset($definition['methods'])) {
				foreach ($definition['methods'] as $method) {
					$classDefinition->addMethod(new ClassMethod(
						$method['visibility'],
						$method['name'],
						isset($method['parameters']) ? new ClassMethodParameters($method['parameters']) : null,
						isset($method['statements']) ? new StatementsBlock($method['statements']) : null,
						isset($method['docblock']) ? $method['docblock'] : null
					), $method);
				}
			}
		}

		$this->_classDefinition = $classDefinition;
	}

	/**
	 *
	 *
	 */
	public function preCompile()
	{

		$ir = $this->genIR();
		if (!is_array($ir)) {
			throw new Exception("Cannot parse file: " . realpath($this->_filePath));
		}

		if (isset($ir['type']) && $ir['type'] == 'error') {
			throw new ParseException($ir['message'], $ir);
		}

		/**
		 * Traverse the top level statements looking for the namespace
		 */
		$namespace = null;
		foreach ($ir as $topStatement) {
			switch ($topStatement['type']) {
				case 'namespace':
					if ($namespace !== null) {
						throw new CompilerException("The namespace must be defined just one time", $topStatement);
					}
					$namespace = $topStatement['name'];
					$this->_namespace = $namespace;
					break;
			}
		}

		if (!$namespace) {
			throw new CompilerException("A namespace is required", $topStatement);
		}

		$class = false;
		$interface = false;
		foreach ($ir as $topStatement) {
			switch ($topStatement['type']) {
				case 'class':
					if ($class || $interface) {
						throw new CompilerException("More than one class/interface defined in the same file", $topStatement);
					}
					$class = true;
					$name = $topStatement['name'];
					$this->preCompileClass($namespace, $topStatement);
					break;
				case 'interface':
					if ($class || $interface) {
						throw new CompilerException("More than one class/interface defined in the same file", $topStatement);
					}
					$interface = true;
					$name = $topStatement['name'];
					$this->preCompileInterface($namespace, $topStatement);
					break;
			}
		}

		if (!$class && !$interface) {
			throw new CompilerException("Every file must contain at least a class or an interface", $topStatement);
		}

		if ($this->_filePath != strtolower(str_replace('\\', '/', $namespace) . '/' . $name) . '.zep') {
			throw new CompilerException('Unexpected class name ' . str_replace('\\', '/', $namespace) . '\\' . $name . ' in file: ' . $this->_filePath);
		}

		$this->_ir = $ir;
	}

	/**
	 * Returns the path to the compiled file
	 *
	 * @return string
	 */
	public function getCompiledFile()
	{
		return $this->_compiledFile;
	}

	/**
	 * Check dependencies
	 *
	 * @param \Compiler $compiler
	 * @param \Config $config
	 * @param \Logger $logger
	 */
	public function checkDependencies(Compiler $compiler, Config $config, Logger $logger)
	{
		$classDefinition = $this->_classDefinition;
		if (!$classDefinition) {
			return;
		}
		$extendedClass = $classDefinition->getExtendsClass();
		if ($extendedClass) {
			if ($compiler->isClass($extendedClass)) {
				$extendedDefinition = $compiler->getClassDefinition($extendedClass);
				$classDefinition->setExtendsClassDefinition($extendedDefinition);
			} else {
				if ($compiler->isInternalClass($extendedClass)) {
					$extendedDefinition = $compiler->getInternalClassDefinition($extendedClass);
					$classDefinition->setExtendsClassDefinition($extendedDefinition);
				} else {
					throw new CompilerException('Cannot locate class "' . $extendedClass . '" when extending class "' . $classDefinition->getCompleteName() . '"');
				}
			}
		}
	}

	/**
	 * Compiles the file
	 *
	 * @param \Compiler $compiler
	 * @param \Config $config
	 * @param \Logger $logger
	 */
	public function compile(Compiler $compiler, Config $config, Logger $logger)
	{

		/**
		 * Compilation context stores common objects required by compilation entities
		 */
		$compilationContext = new CompilationContext;

		/**
		 * Set global compiler in the compilation context
		 */
		$compilationContext->compiler = $compiler;

		/**
		 * Set global config in the compilation context
		 */
		$compilationContext->config = $config;

		/**
		 * Set global logger in the compilation context
		 */
		$compilationContext->logger = $logger;

		/**
		 * Headers manager
		 */
		$headersManager = new HeadersManager();
		$compilationContext->headersManager = $headersManager;

		/**
		 * Main code-printer for the file
		 */
		$codePrinter = new codePrinter();
		$compilationContext->codePrinter = $codePrinter;

		$codePrinter->outputBlankLine();

		$class = false;
		$interface = false;
		foreach ($this->_ir as $topStatement) {

			switch ($topStatement['type']) {
				case 'class':
					if ($interface || $class) {
						throw new CompilerException("More than one class defined in the same file", $topStatement);
					}
					$class = true;
					$this->compileClass($compilationContext, $this->_namespace, $topStatement);
					break;
				case 'interface':
					if ($interface || $class) {
						throw new CompilerException("More than one class defined in the same file", $topStatement);
					}
					$class = true;
					$this->compileClass($compilationContext, $this->_namespace, $topStatement);
					break;
				case 'comment':
					$this->compileComment($compilationContext, $topStatement);
					break;
			}
		}

		$path = str_replace('\\', DIRECTORY_SEPARATOR, strtolower(preg_replace('#^' . $config->get('namespace') . '\\\\#i', '', $this->_compiledFilePath)));
		if (!is_dir('ext/' . $path)) {
			mkdir('ext/' . $path, 0777, true);
		}

		if ($codePrinter) {
			file_put_contents('ext/' . $path . '.c', $codePrinter->getOutput());
			if ($compilationContext->headerPrinter) {
				file_put_contents('ext/' . $path . '.h', $compilationContext->headerPrinter->getOutput());
			}
		}

		/**
		 * Add to file compiled
		 */
		$this->_compiledFile = $path . '.c';
	}

}
