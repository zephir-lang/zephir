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
 * Compiler
 *
 * Main compiler
 */
class Compiler
{

	protected $_files;

	protected $_definitions;

	protected $_compiledFiles;

	protected static $_reflections = array();

	const VERSION = '0.2.0a';

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
			$className = join('\\', array_map(function($i) { return ucfirst($i); }, explode('\\', $className)));
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
		$iterator = new DirectoryIterator($path);
		foreach ($iterator as $item) {
			if ($item->isDir()) {
				if ($item->getFileName() != '.' && $item->getFileName() != '..') {
					$this->_recursivePreCompile($item->getPathname());
				}
			} else {
				$this->_preCompile($item->getPathname());
			}
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
		$classes = array_keys($this->_definitions);
		foreach ($classes as $value) {
			if (!strcasecmp($value, $className)) {
				return true;
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
	 * Initializes a zephir extension
	 */
	public function init($config , $logger)
	{

		if (!is_dir('.temp')) {
			mkdir('.temp');
		}

		$namespace = strtolower(preg_replace('/[^0-9a-zA-Z]/', '', basename(getcwd())));
		if (!$namespace) {
			throw new Exception("Cannot obtain a valid initial namespace for the project");
		}

		file_put_contents('config.json', '{"namespace": "' . $namespace . '"}');

		/**
		 * Create 'kernel'
		 */
		if (!is_dir('ext')) {
			mkdir('ext');
			mkdir('ext/kernel');
			mkdir('ext/kernel/alternative');
			$this->_copyBaseKernel(__DIR__ . '/../ext/kernel/');
			copy(__DIR__ . '/../ext/install', 'ext/install');
			chmod('ext/install', 0755);
			copy(__DIR__ . '/../ext/php_test.h', 'ext/php_test.h');
		}

		if (!is_dir($namespace)) {
			mkdir($namespace);
		}
	}

	/**
	 *
	 */
	public function compile($config , $logger)
	{

		if (!file_exists('config.json')) {
			throw new Exception("Zephir extension is not initialized in this directory");
		}

		if (!is_dir('.temp')) {
			mkdir('.temp');
		}

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
		 * Round 3. compile all files to C sources
		 */
		$files = array();
		foreach ($this->_files as $compileFile) {
			$compileFile->compile($this, $config, $logger);
			$files[] = $compileFile->getCompiledFile();
		}

		$this->_compiledFiles = $files;

		/**
		 * Round 4. create config.m4 and config.w32 files
		 */
		$this->createConfigFiles($namespace);

		/**
		 * Round 5. create project.c and project.h files
		 */
		$this->createProjectFiles($namespace);
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
		 */
		foreach ($files as $file) {
			$classDefinition = $file->getClassDefinition();
			if ($classDefinition) {
				$classDefinition->calculateDependencyRank();
			}
		}

		$classEntries = array();
		$classInits = array();

		/**
		 * Round 2. Generate the ZEPHIR_INIT calls according to the dependency rank
		 */
		foreach ($files as $file) {
			$classDefinition = $file->getClassDefinition();
			if ($classDefinition) {
				$dependencyRank = $classDefinition->getDependencyRank();
				if (!isset($classInits[$dependencyRank])) {
					$classEntries[$dependencyRank] = array();
					$classInits[$dependencyRank] = array();
				}
				$classEntries[$dependencyRank][] = 'zend_class_entry *' . $classDefinition->getClassEntry() . ';';
				$classInits[$dependencyRank][] = 'ZEPHIR_INIT(' . $classDefinition->getCNamespace() . '_' . $classDefinition->getName() . ');';
			}
		}

		asort($classInits);
		asort($classEntries);

		$completeClassInits = array();
		foreach ($classInits as $dependencyRank => $rankClassInits) {
			$completeClassInits = array_merge($completeClassInits, $rankClassInits);
		}

		$completeClassEntries = array();
		foreach ($classEntries as $dependencyRank => $rankClassEntries) {
			$completeClassEntries = array_merge($completeClassEntries, $rankClassEntries);
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project),
			'%PROJECT_UPPER%' 		=> strtoupper($project),
			'%PROJECT_CAMELIZE%' 	=> ucfirst($project),
			'%CLASS_ENTRIES%' 		=> implode(PHP_EOL, $completeClassEntries),
			'%CLASS_INITS%'			=> implode(PHP_EOL . "\t", $completeClassInits),
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
			$fileH = str_replace(".c", ".h", $file);
			$include = '#include "' . $fileH . '"';
			$includeHeaders[] = $include;
		}

		$toReplace = array(
			'%INCLUDE_HEADERS%' => implode(PHP_EOL, $includeHeaders)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		file_put_contents('ext/' . $project . '.h', $content);
	}

	protected static function showException($e)
	{
		echo get_class($e), ': ', $e->getMessage(), PHP_EOL;
		if (method_exists($e, 'getExtra')) {
			$extra = $e->getExtra();
			if (is_array($extra)) {
				if (isset($extra['file'])) {
					echo PHP_EOL;
					$lines = file($extra['file']);
					if (isset($lines[$extra['line'] - 1])) {
					$line = $lines[$extra['line'] - 1];
					echo "\t", str_replace("\t", " ", $line);
					if (($extra['char'] - 1) > 0) {
						echo "\t", str_repeat("-", $extra['char'] - 1), "^", PHP_EOL;
					}
					}
				}
			}
		}
		echo PHP_EOL;
		$pwd = getcwd();
		echo 'at ', str_replace($pwd, '', $e->getFile()), '(', $e->getLine(), ')', PHP_EOL;
		echo str_replace($pwd, '', $e->getTraceAsString()), PHP_EOL;
		exit(1);
	}

	/**
	 * Boots the compiler executing the specified action
	 */
	public static function boot()
	{
		try {

			$c = new Compiler();

			/**
			 * Global config
			 */
			$config = new Config();

			/**
			 * Global logger
			 */
			$logger = new Logger();

			if (isset($_SERVER['argv'][1])) {
				$action = $_SERVER['argv'][1];
			} else {
				$action = 'compile';
			}

			/**
			 * Change configurations flags
			 */
			if ($_SERVER['argc'] >= 2) {
				for ($i = 2; $i < $_SERVER['argc']; $i++) {
					if (preg_match('/^-fno-([a-z0-9\-]+)/', $_SERVER['argv'][$i], $matches)) {
						$config->set($matches[1], false);
					}
				}
			}

			switch ($action) {
				case 'init':
					$c->init($config , $logger);
					break;
				case 'compile-only':
					$c->compile($config , $logger);
					break;
				case 'compile':
					$c->compile($config , $logger);
					$c->install($config , $logger);
					break;
				case 'version':
					echo self::VERSION, PHP_EOL;
					break;
				default:
					throw new Exception('Unrecognized action "' . $action . '"');
			}

		} catch (Exception $e) {
			self::showException($e);
		}
	}

}
