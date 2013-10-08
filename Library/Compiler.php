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

	protected $_config;
	
	protected static $_reflections = array();

	const VERSION = '0.2.1a';

	/**
	 * Compiler constructor
	 *
	 * @param Config $config
	 */
	public function __construct(Config $config)
	{
		$this->_config = $config;
	}

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
			$className = ucfirst($this->_config->get('namespace')) . '\\' . $className;
			$this->_files[$className] = new CompilerFile($className, $filePath, $this->_config);
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
		$path = '../'.$path;
		$pathLen = strlen($path);
		$iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($path), RecursiveIteratorIterator::SELF_FIRST);
		foreach ($iterator as $item) {
			if (!$item->isDir()) {
				$files[] = substr($item->getPathname(), $pathLen + 1);
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
	 * Copies the base extension folder to the extension destination
	 *
	 * @param string $path
	 */
	protected function _copyBaseKernel($src, $dest)
	{
			foreach (scandir($src) as $file) {
		    if (!is_readable($src.'/'.$file)) {
		    	continue;
		    }
		    if (is_dir($src.'/'.$file)) {
		    	if ($file != '.' && $file != '..') {
		    		if (!is_dir($dest . '/' . $file)) {
			        	mkdir($dest . '/' . $file);
			    	}
			      $this->_copyBaseKernel($src.'/'.$file, $dest.'/'.$file);
			  	}
		    } else {
		  	copy($src.'/'.$file, $dest.'/'.$file);
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
	 * Initializes a Zephir extension
	 *
	 * @param Config $config
	 * @param Logger $logger
	 */
	public function init(Config $config, Logger $logger)
	{
		$namespace = $config->get('namespace');
		if (!$namespace) {
			throw new Exception("Cannot obtain a valid initial namespace for the project");
		}

		if ($config->get('skip-create-namespace') !== true) {
			if (!is_dir($namespace)) {
				mkdir($namespace);
			}
			chdir($namespace);
		}
		// Create the config.json file
		file_put_contents('config.json', '{"namespace": "' . $namespace . '"}');
		// Create the ext and ext/kernel directories
		if (!is_dir('ext/kernel')) {
			mkdir('ext/kernel', 0755, true);
		}
		// Copy the latest kernel files
		$this->_copyBaseKernel(realpath(__DIR__ . '/../ext/kernel'), 'ext/kernel');
	}

	/**
	 *
	 * @param Config $config
	 * @param Config $logger
	 */
	public function generate($config, $logger)
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
		 * Convert C-constants into PHP constants
		 */
		$constantsSources = $config->get('constants-sources');
		if (is_array($constantsSources)) {
			$this->_loadConstantsSources($constantsSources);
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
	}

	/**
	 *
	 * @param Config $config
	 * @param Logger $logger
	 */
	public function compile(Config $config, Logger $logger)
	{
		$namespace = $config->get('namespace');
		if (!$namespace) {
			throw new Exception("Extension namespace cannot be loaded");
		}

		/**
		 * Round 4. create config.m4 and config.w32 files
		 */
		$this->createConfigFiles($namespace);

		/**
		 * Round 5. create project.c and project.h files
		 */
		$this->createProjectFiles($namespace);

		echo "Compiling...\n";
		exec('(export CC="gcc" && export CFLAGS="-O0 -g" && cd ext && phpize && ./configure --enable-' . $namespace . ' && make) > /dev/null 2>&1', $output, $exit);
	}

	/**
	 * Compiles an installs the extension
	 */
	public function install($config, $logger)
	{
		$namespace = $config->get('namespace');
		if (!$namespace) {
			throw new Exception("Extension namespace cannot be loaded");
		}

		echo "Installing...\n";
		exec('export CC="gcc" && export CFLAGS="-O0 -g" && cd ext && sudo make install', $output, $exit);
	}

	/**
	 * Create config.m4 and config.w32 by compiled files to test extension
	 *
	 * @param string $project
	 */
	public function createConfigFiles($project)
	{

		$content = file_get_contents(__DIR__ . '/../ext/config.m4');
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
		$content = file_get_contents(__DIR__ . '/../ext/php_ext.h');
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
		$content = file_get_contents(__DIR__ . '/../ext/ext.h');
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
		$content = file_get_contents(__DIR__ . '/../ext/ext_config.h');
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

		/**
		 * ext_clean
		 */
		$content = file_get_contents(__DIR__ . '/../ext/clean');
		if (empty($content)) {
			throw new Exception("clean file doesn't exists");
		}

		file_put_contents('ext/clean', $content);
		chmod('ext/clean', 0755);

		/**
		 * ext_install
		 */
		$content = file_get_contents(__DIR__ . '/../ext/install');
		if (empty($content)) {
			throw new Exception("install file doesn't exists");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		file_put_contents('ext/install', $content);
		chmod('ext/install', 0755);
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
		$content = file_get_contents(__DIR__ . '/../ext/project.c');
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
		$content = file_get_contents(__DIR__ . '/../ext/project.h');
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
		$content = file_get_contents(__DIR__ . '/../ext/php_project.h');
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
	 * Shows an exception opening the file and highlighing the wrong part
	 *
	 * @param Exception $e
	 * @param Config $config
	 */
	protected static function showException(Exception $e, Config $config=null)
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
		if ($config && $config->get('verbose')) {
			echo 'at ', str_replace(ZEPHIRPATH, '', $e->getFile()), '(', $e->getLine(), ')', PHP_EOL;
			echo str_replace(ZEPHIRPATH, '', $e->getTraceAsString()), PHP_EOL;
		}
		exit(1);
	}

	/**
	 * Boots the compiler executing the specified action
	 */
	public static function boot()
	{
		try {

			/**
			 * Global config
			 */
			$config = new Config();

			/**
			 * Global compiler
			 */
			$c = new Compiler($config);

			/**
			 * Global logger
			 */
			$logger = new Logger($config);

			$argcoffset = 2;
			if (isset($_SERVER['argv'][1])) {
				$action = $_SERVER['argv'][1];
				if ($action == 'init') {
					if (isset($_SERVER['argv'][2]) && !$_SERVER['argv'][2][0] == '-') {
						$namespace = $_SERVER['argv'][2];
						$argcoffset = 3;
					}
					else {
						$namespace = basename(getcwd());
						$config->set('skip-create-namespace', true);
					}
					$config->set('namespace', strtolower(preg_replace('/[^0-9a-zA-Z]/', '', $namespace)));
				}
							} else {
				if (!file_exists('config.json')) {
					$action = 'help';
				}else {
					$action = 'compile';
				}
			}

			/**
			 * Change configurations flags
			 */
			if ($_SERVER['argc'] >= $argcoffset) {
				for ($i = $argcoffset; $i < $_SERVER['argc']; $i++) {

					$parameter = $_SERVER['argv'][$i];
					if (preg_match('/^-fno-([a-z0-9\-]+)/', $parameter, $matches)) {
						$config->set($matches[1], false);
						continue;
					}

					if (preg_match('/^-W([a-z0-9\-]+)/', $parameter, $matches)) {
						$logger->set($matches[1], false);
						continue;
					}

					switch ($parameter) {
						case '-w':
							$config->set('silent', true);
							break;
						case '-v':
							$config->set('verbose', true);
							break;
						default:
							break;
					}
				}
			}

			switch ($action) {
				case 'init':
					$c->init($config , $logger);
					break;
				case 'generate':
					$c->generate($config, $logger);
					break;
				case 'compile':
					$c->generate($config, $logger);
					$c->compile($config, $logger);
					break;
				case 'install':
					$c->generate($config, $logger);
					$c->compile($config, $logger);
					$c->install($config, $logger);
					break;
				case 'version':
					echo self::VERSION, PHP_EOL;
					break;
				case 'help':
					$c->help();
					break;
				default:
					throw new Exception('Unrecognized action "' . $action . '"');
			}

		} catch (Exception $e) {
			self::showException($e, isset($config) ? $config : null);
		}
	}


	/**
	 * Display zephir commands help
	 */
	public function help() {

		echo "zephir version " , self::VERSION,  PHP_EOL, PHP_EOL;
		echo "Usage: ", PHP_EOL;
		echo "\tcommand [options]", PHP_EOL;
		echo PHP_EOL;
		echo "Available commands:", PHP_EOL;
		echo sprintf("\t%-20s%s\n","init namespace", "Initializes a Zephir extension");
		echo sprintf("\t%-20s%s\n","generate", "Generate the C code only");
		echo sprintf("\t%-20s%s\n","compile", "Generate and Compile the extension");
		echo sprintf("\t%-20s%s\n","install", "Generate, Compile and install the extension");
		echo sprintf("\t%-20s%s\n","version", "Display zephir version");
		echo sprintf("\t%-20s%s\n","help", "Displays help");
		echo PHP_EOL;
		echo "Options:", PHP_EOL;
		echo sprintf("\t%-20s%s\n","-fno-([a-z0-9\-]+)", "Setting options to Compiler");
		echo sprintf("\t%-20s%s\n","-W([a-z0-9\-]+)", "Setting warning options to Compiler");

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
