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
 * Compiler
 *
 * Main compiler
 */
class Compiler
{

	protected $_files;

	/*+
	 * @var ClassDefinition[]
	 */
	protected $_definitions;

	protected $_compiledFiles;

	protected $_constants = array();

	protected $_globals = array();

	protected static $_reflections = array();

	/**
	 * Pre-compiles classes creating a CompilerFile definition
	 *
	 * @param string $filePath
	 */
	protected function _preCompile($filePath)
	{
		if (preg_match('/\.zep$/', $filePath)) {
			$className = str_replace('/', '\\', $filePath);
			$className = preg_replace('/.zep$/', '', $className);
			$className = join('\\', array_map(function($i) {
				return ucfirst($i);
			}, explode('\\', $className)));
			$this->_files[$className] = new CompilerFile($className, $filePath);
			$this->_files[$className]->preCompile();
			$this->_definitions[$className] = $this->_files[$className]->getClassDefinition();
		}
	}

	/**
	 *
	 * @param string $path
	 */
	protected function _recursivePreCompile($path)
	{
		/**
		 * Pre compile all files
		 */
		$files = array();
		$iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($path), RecursiveIteratorIterator::SELF_FIRST);
		foreach ($iterator as $item) {
			if (!$item->isDir()) {
				$files[] = $item->getPathname();
			}
		}
		sort($files, SORT_STRING);
		foreach ($files as $file) {
			$this->_preCompile($file);
		}
	}

	/**
	 * Allows to check if a class is part of the compiled extension
	 *
	 * @param string $className
	 * @return bolean
	 */
	public function isClass($className)
	{
		foreach ($this->_definitions as $key => $value) {
			if (!strcasecmp($key, $className)) {
				if ($value->getType() == 'class') {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * Allows to check if an interface is part of the compiled extension
	 *
	 * @param string $className
	 * @return bolean
	 */
	public function isInterface($className)
	{
		foreach ($this->_definitions as $key => $value) {
			if (!strcasecmp($key, $className)) {
				if ($value->getType() == 'interface') {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * Allows to check if a class is part of PHP
	 *
	 * @param string $className
	 * @return bolean
	 */
	public function isInternalClass($className)
	{
		return class_exists($className, false);
	}

	/**
	 * Allows to check if a interface is part of PHP
	 *
	 * @param string $className
	 * @return bolean
	 */
	public function isInternalInterface($className)
	{
		return interface_exists($className, false);
	}

	/**
	 * Returns class the class definition from a given class name
	 *
	 * @param string $className
	 * @return ClassDefinition
	 */
	public function getClassDefinition($className)
	{
		foreach ($this->_definitions as $key => $value) {
			if (!strcasecmp($key, $className)) {
				return $value;
			}
		}
		return false;
	}

	/**
	 * Returns class the class definition from a given class name
	 *
	 * @param string $className
	 * @return ClassDefinition
	 */
	public function getInternalClassDefinition($className)
	{
		if (!isset(self::$_reflections[$className])) {
			self::$_reflections[$className] = new ReflectionClass($className);
		}
		return self::$_reflections[$className];
	}

	/**
	 * Copies the base kernel to the extension destination
	 *
	 * @param string $path
	 */
	protected function _copyBaseKernel($path)
	{
		/**
		 * Pre compile all files
		 */
		$iterator = new DirectoryIterator($path);
		foreach ($iterator as $item) {
			if ($item->isDir()) {
				if ($item->getFileName() != '.' && $item->getFileName() != '..') {
					$this->_copyBaseKernel($item->getPathname());
				}
			} else {
				if (preg_match('/\.[hc]$/', $item->getPathname())) {
					if (strpos($item->getPathName(), 'alternative') !== false) {
						copy($item->getPathname(), 'ext/kernel/alternative/' . $item->getBaseName());
					} else {
						copy($item->getPathname(), 'ext/kernel/' . $item->getBaseName());
					}
				}
			}
		}
	}

	/**
	 * Registers C-constants as PHP constants from a C-file
	 *
	 * @param array $constantsSources
	 */
	protected function _loadConstantsSources($constantsSources)
	{
		foreach ($constantsSources as $constantsSource) {

			if (!file_exists($constantsSource)) {
				throw new Exception("File '" . $constantsSource . "' with constants definitions");
			}

			foreach (file($constantsSource) as $line) {
				if (preg_match('/^\#define[ \t]+([A-Z0-9\_]+)[ \t]+([0-9]+)/', $line, $matches)) {
					$this->_constants[$matches[1]] = array('int', $matches[2]);
					continue;
				}
				if (preg_match('/^\#define[ \t]+([A-Z0-9\_]+)[ \t]+(\'(.){1}\')/', $line, $matches)) {
					$this->_constants[$matches[1]] = array('char', $matches[3]);
				}
			}
		}
	}

	/**
	 * Checks if $name is Zephir constant
	 *
	 * @param string $name
	 * @return boolean
	 */
	public function isConstant($name)
	{
		return isset($this->_constants[$name]);
	}

	/**
	 * Returns a Zephir Constant by its name
	 *
	 * @param string $name
	 */
	public function getConstant($name)
	{
		return $this->_constants[$name];
	}

	/**
	 * @param array $globals
	 */
	public function _setExtensionGlobals($globals)
	{
		foreach ($globals as $key => $value) {
			$this->_globals[$key] = $value;
		}
	}

	/**
	 * Checks if a specific extension global is defined
	 *
	 * @param string $name
	 * @return boolean
	 */
	public function isExtensionGlobal($name)
	{
		return isset($this->_globals[$name]);
	}

	/**
	 * Returns a extension global by its name
	 *
	 * @param string $name
	 * @return boolean
	 */
	public function getExtensionGlobal($name)
	{
		return $this->_globals[$name];
	}

	/**
	 * Initializes a Zephir extension
	 *
	 * @param Config $config
	 * @param Logger $logger
	 */
	public function init(Config $config, Logger $logger)
	{

		if (!is_dir('.temp')) {
			mkdir('.temp');
		}

		/**
		 * If init namespace is specified
		 */
		if (isset($_SERVER['argv'][2])) {
			$namespace = strtolower(preg_replace('/[^0-9a-zA-Z]/', '', $_SERVER['argv'][2]));
		} else {
			$namespace = strtolower(preg_replace('/[^0-9a-zA-Z]/', '', basename(getcwd())));
		}

		if (!$namespace) {
			throw new Exception("Cannot obtain a valid initial namespace for the project");
		}

		/**
		 * Using json_encode with pretty-print
		 */
		$configArray = array(
			'namespace'   => $namespace,
			'name'        => $namespace,
			'description' => '',
			'author'      => ''
		);

		/**
		 * Above PHP 5.4
		 */
		if (defined('JSON_PRETTY_PRINT')) {
			$configArray = json_encode($configArray, JSON_PRETTY_PRINT);
		} else {
			$configArray = json_encode($configArray);
		}
		file_put_contents('config.json', $configArray);

		/**
		 * Create 'kernel'
		 */
		if (!is_dir('ext')) {

			mkdir('ext');
			mkdir('ext/kernel');
			mkdir('ext/kernel/alternative');

			$this->_copyBaseKernel(__DIR__ . '/../ext/kernel/');
		}

		if (!is_dir($namespace)) {
			mkdir($namespace);
		}
	}

	protected function _checkDirectory()
	{
		if (!file_exists('config.json')) {
			throw new Exception("There is no Zephir extension initialized in this directory");
		}

		if (!is_dir('.temp')) {
			mkdir('.temp');
		}

	}

	/**
	 *
	 * @param Config $config
	 * @param Logger $logger
	 */
	public function compile(Config $config, Logger $logger)
	{

		$this->_checkDirectory();

		/**
		 * Get global namespace
		 */
		$namespace = $config->get('namespace');
		if (!$namespace) {
			throw new Exception("Extension namespace cannot be loaded");
		}

		/**
		 * Round 1. pre-compile all files in memory
		 */
		$this->_recursivePreCompile($namespace);
		if (!count($this->_files)) {
			throw new Exception("Zephir files to compile weren't found");
		}

		/**
		 * Round 2. Check 'extends' and 'implements' dependencies
		 */
		foreach ($this->_files as $compileFile) {
			$compileFile->checkDependencies($this, $config, $logger);
		}

		/**
		 * Convert C-constants into PHP constants
		 */
		$constantsSources = $config->get('constants-sources');
		if (is_array($constantsSources)) {
			$this->_loadConstantsSources($constantsSources);
		}

		/**
		 * Set extension globals
		 */
		$globals = $config->get('globals');
		if (is_array($globals)) {
			$this->_setExtensionGlobals($globals);
		}

		/**
		 * The string manager manages
		 */
		$stringManager = new StringsManager();

		/**
		 * Round 3. compile all files to C sources
		 */
		$files = array();
		foreach ($this->_files as $compileFile) {
			$compileFile->compile($this, $config, $logger, $stringManager);
			$files[] = $compileFile->getCompiledFile();
		}

		$this->_compiledFiles = $files;

		/**
		 * Round 4. Create config.m4 and config.w32 files / Create project.c and project.h files
		 */
		$this->createConfigFiles($namespace);
		$this->createProjectFiles($namespace);

		/**
		 * Round 5.
		 */
		$stringManager->genConcatCode();

	}

	/**
	 * Compiles an installs the extension
	 */
	public function install()
	{
		if (!file_exists('ext/Makefile')) {
			system('export CC="gcc" && export CFLAGS="-O0 -g" && cd ext && phpize --silent && ./configure --silent --enable-test && sudo make --silent install 1> /dev/null');
		} else {
			system('cd ext && sudo make --silent install 1> /dev/null');
		}
	}

	/**
	 * Create config.m4 and config.w32 by compiled files to test extension
	 *
	 * @param string $project
	 */
	public function createConfigFiles($project)
	{

		$content = file_get_contents(__DIR__ . '/../templates/config.m4');
		if (empty($content)) {
			throw new Exception("Template config.m4 doesn't exists");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project),
			'%PROJECT_UPPER%' 		=> strtoupper($project),
			'%PROJECT_CAMELIZE%' 	=> ucfirst($project),
			'%FILES_COMPILED%' 		=> implode(' ', $this->_compiledFiles),
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		file_put_contents('ext/config.m4', $content);

		/**
		 * php_ext.h
		 */
		$content = file_get_contents(__DIR__ . '/../templates/php_ext.h');
		if (empty($content)) {
			throw new Exception("Template php_ext.h doesn't exists");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		file_put_contents('ext/php_ext.h', $content);

		/**
		 * ext.h
		 */
		$content = file_get_contents(__DIR__ . '/../templates/ext.h');
		if (empty($content)) {
			throw new Exception("Template ext.h doesn't exists");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		file_put_contents('ext/ext.h', $content);

		/**
		 * ext_config.h
		 */
		$content = file_get_contents(__DIR__ . '/../templates/ext_config.h');
		if (empty($content)) {
			throw new Exception("Template ext_config.h doesn't exists");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		file_put_contents('ext/ext_config.h', $content);
	}

	/**
	 * Create project.c and project.h by compiled files to test extension
	 *
	 * @param string $project
	 */
	public function createProjectFiles($project)
	{

		/**
		 * project.c
		 */
		$content = file_get_contents(__DIR__ . '/../templates/project.c');
		if (empty($content)) {
			throw new Exception("Template project.c doesn't exist");
		}

		$files = $this->_files;

		/**
		 * Round 1. Calculate the dependency rank
		 * Classes are ordered according to a dependency ranking
		 * Classes that are dependencies of classes that are dependency of other classes
		 * have more weight
		 */
		foreach ($files as $file) {
			$classDefinition = $file->getClassDefinition();
			if ($classDefinition) {
				$classDefinition->calculateDependencyRank();
			}
		}

		/**
		 * Round 1.5 Make a second pass to ensure classes will have the correct weight
		 */
		foreach ($files as $file) {
			$classDefinition = $file->getClassDefinition();
			if ($classDefinition) {
				$classDefinition->calculateDependencyRank();
			}
		}

		$classEntries = array();
		$classInits = array();

		$interfaceEntries = array();
		$interfaceInits = array();

		/**
		 * Round 2. Generate the ZEPHIR_INIT calls according to the dependency rank
		 */
		foreach ($files as $file) {
			$classDefinition = $file->getClassDefinition();
			if ($classDefinition) {
				$dependencyRank = $classDefinition->getDependencyRank();
				if ($classDefinition->getType() == 'class') {
					if (!isset($classInits[$dependencyRank])) {
						$classEntries[$dependencyRank] = array();
						$classInits[$dependencyRank] = array();
					}
					$classEntries[$dependencyRank][] = 'zend_class_entry *' . $classDefinition->getClassEntry() . ';';
					$classInits[$dependencyRank][] = 'ZEPHIR_INIT(' . $classDefinition->getCNamespace() . '_' . $classDefinition->getName() . ');';
				} else {
					if (!isset($interfaceInits[$dependencyRank])) {
						$interfaceEntries[$dependencyRank] = array();
						$interfaceInits[$dependencyRank] = array();
					}
					$interfaceEntries[$dependencyRank][] = 'zend_class_entry *' . $classDefinition->getClassEntry() . ';';
					$interfaceInits[$dependencyRank][] = 'ZEPHIR_INIT(' . $classDefinition->getCNamespace() . '_' . $classDefinition->getName() . ');';
				}
			}
		}

		krsort($classInits);
		krsort($classEntries);
		krsort($interfaceInits);
		krsort($interfaceEntries);

		$completeInterfaceInits = array();
		foreach ($interfaceInits as $dependencyRank => $rankInterfaceInits) {
			asort($rankInterfaceInits, SORT_STRING);
			$completeInterfaceInits = array_merge($completeInterfaceInits, $rankInterfaceInits);
		}

		$completeInterfaceEntries = array();
		foreach ($interfaceEntries as $dependencyRank => $rankInterfaceEntries) {
			asort($rankInterfaceEntries, SORT_STRING);
			$completeInterfaceEntries = array_merge($completeInterfaceEntries, $rankInterfaceEntries);
		}

		$completeClassInits = array();
		foreach ($classInits as $dependencyRank => $rankClassInits) {
			asort($rankClassInits, SORT_STRING);
			$completeClassInits = array_merge($completeClassInits, $rankClassInits);
		}

		$completeClassEntries = array();
		foreach ($classEntries as $dependencyRank => $rankClassEntries) {
			asort($rankClassEntries, SORT_STRING);
			$completeClassEntries = array_merge($completeClassEntries, $rankClassEntries);
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project),
			'%PROJECT_UPPER%' 		=> strtoupper($project),
			'%PROJECT_CAMELIZE%' 	=> ucfirst($project),
			'%CLASS_ENTRIES%' 		=> implode(PHP_EOL, array_merge($completeInterfaceEntries, $completeClassEntries)),
			'%CLASS_INITS%'			=> implode(PHP_EOL . "\t", array_merge($completeInterfaceInits, $completeClassInits)),
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		/**
		 * Round 3. Generate and place the entry point of the project
		 */
		file_put_contents('ext/' . $project . '.c', $content);
		unset($content);

		/**
		 * Round 4. Generate the project main header
		 */
		$content = file_get_contents(__DIR__ . '/../templates/project.h');
		if (empty($content)) {
			throw new Exception("Template project.h doesn't exists");
		}

		$includeHeaders = array();
		foreach ($this->_compiledFiles as $file) {
			if ($file) {
				$fileH = str_replace(".c", ".h", $file);
				$include = '#include "' . $fileH . '"';
				$includeHeaders[] = $include;
			}
		}

		$toReplace = array(
			'%INCLUDE_HEADERS%' => implode(PHP_EOL, $includeHeaders)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		file_put_contents('ext/' . $project . '.h', $content);

		/**
		 * Round 5. Create php_project.h
		 */
		$content = file_get_contents(__DIR__ . '/../templates/php_project.h');
		if (empty($content)) {
			throw new Exception("Template php_project.h doesn't exist");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project),
			'%PROJECT_UPPER%' 		=> strtoupper($project),
			'%PROJECT_EXTNAME%' 	=> strtolower($project),
			'%PROJECT_VERSION%' 	=> '0.0.1'
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		file_put_contents('ext/php_' . $project . '.h', $content);

	}

	/**
	 * Returns a short path
	 *
	 * @param string $path
	 */
	public static function getShortPath($path)
	{
		return str_replace(ZEPHIRPATH . DIRECTORY_SEPARATOR, '', $path);
	}

	/**
	 * Returns a short user path
	 *
	 * @param string $path
	 */
	public static function getShortUserPath($path)
	{
		return str_replace(getcwd() . DIRECTORY_SEPARATOR, '', $path);
	}

}
