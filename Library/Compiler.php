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

	/**
	 * @var ClassDefinition[]
	 */
	protected $_definitions;

	protected $_compiledFiles;

	protected $_constants = array();

	protected $_globals = array();

	protected $_stringManager = null;

	protected $_config = null;

	protected $_logger = null;

	protected static $_reflections = array();

	public function __construct(Config $config = null, Logger $logger = null)
	{
		$this->_config = $config;
		$this->_logger = $logger;
		/**
		 * The string manager manages
		 */
		$this->_stringManager = new StringsManager();
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
			$className = join('\\', array_map(function($i) {
				return ucfirst($i);
			}, explode('\\', $className)));
			$this->_files[$className] = new CompilerFile($className, $filePath, $this->_config, $this->_logger);
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
	 * @param string $destination
	 */
	protected function _recursiveProcess($src, $dest, $pattern=null, $callback="copy")
	{
		$success = true;
		$iterator = new DirectoryIterator($src);
		foreach ($iterator as $item) {
			$pathName = $item->getPathname();
			if (!is_readable($pathName)) {
				continue;
			}
			$fileName = $item->getFileName();
			if ($item->isDir()) {
				if ($fileName != '.' && $fileName != '..') {
					if (!is_dir($dest . '/' . $fileName)) {
						mkdir($dest . '/' . $fileName, 0755, true);
					}
					$this->_recursiveProcess($pathName, $dest.'/'.$fileName, $pattern, $callback);
				}
			} else {
			  	if (!$pattern || ($pattern && preg_match($pattern, $fileName) === 1)) {
					if (is_string($callback)) {
						$success = $success && $callback($pathName, $dest.'/'.$fileName);
					} else {
						$success = $success && call_user_func($callback, $pathName, $dest.'/'.$fileName);
					}
				}
			}
		}
		return $success;
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

	protected function _checkDirectory()
	{
		$namespace = $this->_config->get('namespace');
		if (!$namespace) {
			throw new Exception("Extension namespace cannot be loaded");
		}

		if (!is_dir('.temp')) {
			mkdir('.temp');
		}

		return $namespace;
	}

	/**
	 * Initializes a Zephir extension
	 *
	 * @param CommandInitialize $command
	 */
	public function init(CommandInterface $command)
	{
		/**
		 * If init namespace is specified
		 */
		$namespace = $command->getParameter('namespace');
		if (!$namespace) {
			throw new Exception("Cannot obtain a valid initial namespace for the project");
		}

		$this->_config->set('namespace', $namespace);
		$this->_config->set('name', $namespace);

		if (!is_dir($namespace)) {
			mkdir($namespace);
		}

		chdir($namespace);
		if (!is_dir($namespace)) {
			mkdir($namespace);
		}

		/**
		 * Create 'kernel'
		 */
		if (!is_dir('ext/kernel')) {
			mkdir('ext/kernel', 0755, true);
		}
		// Copy the latest kernel files
		$this->_recursiveProcess(realpath(__DIR__ . '/../ext/kernel'), 'ext/kernel');
	}

	/**
	 *
	 * @param CommandGenerate $command
	 * @throws Exception
	 */
	public function generate(CommandInterface $command)
	{
		/**
		 * Get global namespace
		 */
		$namespace = $this->_checkDirectory();

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
			$compileFile->checkDependencies($this);
		}

		/**
		 * Convert C-constants into PHP constants
		 */
		$constantsSources = $this->_config->get('constants-sources');
		if (is_array($constantsSources)) {
			$this->_loadConstantsSources($constantsSources);
		}

		/**
		 * Set extension globals
		 */
		$globals = $this->_config->get('globals');
		if (is_array($globals)) {
			$this->_setExtensionGlobals($globals);
		}

		/**
		 * Round 3. compile all files to C sources
		 */
		$files = array();
		foreach ($this->_files as $compileFile) {
			$compileFile->compile($this, $this->_stringManager);
			$files[] = $compileFile->getCompiledFile();
		}

		$this->_compiledFiles = $files;
	}

	/**
	 *
	 * @param CommandCompile $command
	 */
	public function compile(CommandInterface $command)
	{

		/**
		 * Get global namespace
		 */
		$namespace = $this->_checkDirectory();

		$this->generate($command);

		/**
		 * Round 4. Create config.m4 and config.w32 files / Create project.c and project.h files
		 */
		$needConfigure  = $this->createConfigFiles($namespace);
		$needConfigure |= $this->createProjectFiles($namespace);

		/**
		 * Round 5. Generate the concatenation cubrid_error_code(oid)
		 */
		$this->_stringManager->genConcatCode();

		$verbose = ($this->_config->get('verbose') ? true : false);
		if ($needConfigure) {
			exec('cd ext && make clean', $output, $exit);
			echo "Preparing for PHP compilation...\n";
			exec('cd ext && phpize', $output, $exit);
			echo "Preparing configuration file...\n";
			exec('export CC="gcc" && export CFLAGS="-O2" && cd ext && ./configure --enable-' . $namespace);
		}

		echo "Compiling...\n";
		if ($verbose) {
			passthru('cd ext && make', $exit);
		} else {
			exec('cd ext && make' . $verbose, $output, $exit);
		}
	}

	/**
	 *
	 * Compiles and installs the extension
	 * @param CommandInstall $command
	 * @throws Exception
	 */
	public function install(CommandInstall $command)
	{
		/**
		 * Get global namespace
		 */
		$namespace = $this->_checkDirectory();

		$this->compile($command);

		echo "Installing...\n";
		exec('(export CC="gcc" && export CFLAGS="-O2" && cd ext && sudo make install) > /dev/null 2>&1', $output, $exit);
		echo "Don't forget to restart your web server\n";
	}

	/**
	 * Run tests
	 * @param CommandInterface $command
	 */
	public function test(CommandInterface $command)
	{
		/**
		 * Get global namespace
		 */
		$namespace = $this->_checkDirectory();

		echo "Running tests...\n";
		system('export CC="gcc" && export CFLAGS="-O0 -g" && export NO_INTERACTION=1 && cd ext && make test', $exit);
	}

	/**
	 * Clean the extension directory
	 *
	 * @param CommandClean $command
	 */
	public function clean(CommandClean $command)
	{
		system('cd ext && make clean 1> /dev/null');
	}

	/**
	 * Checks if the content of the file on the disk is the same as the content.
	 * Returns true if the file has been written
	 *
	 * @param string $content
	 * @param string $path
	 * @return boolean
	 */
	protected function _checkAndWriteIfNeeded($content, $path)
	{
		if (file_exists($path)) {
			$contentMd5 = md5($content);
			$existingMd5 = md5_file($path);
			if ($contentMd5 != $existingMd5) {
				file_put_contents($path, $content);
				return true;
			}
		} else {
			file_put_contents($path, $content);
			return true;
		}
		return false;
	}

	protected function _checkKernelFile($src, $dst)
	{
		if (strstr($src, 'ext/kernel/concat.') !== false) {
			return true;
		}
		if (!file_exists($dst)) {
			return false;
		}
		return md5_file($src) == md5_file($dst);
	}

	protected function checkKernelFiles()
	{
		$configured = $this->_recursiveProcess(realpath(__DIR__ . '/../ext/kernel'), 'ext/kernel', '@^.*\.c|h$@', array($this, '_checkKernelFile'));
		if (!$configured) {
			echo "Copying new kernel files...\n";
			exec("rm -fr ext/kernel/*");
			$this->_recursiveProcess(realpath(__DIR__ . '/../ext/kernel'), 'ext/kernel', '@^[^\.]@');
		}
		return !$configured;
	}

	/**
	 * Create config.m4 and config.w32 by compiled files to test extension
	 *
	 * @param string $project
	 * @return bool true if need to run configure
	 */
	public function createConfigFiles($project)
	{

		$need_configure = false;
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

		$needConfigure = $this->_checkAndWriteIfNeeded($content, 'ext/config.m4');

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

		$this->_checkAndWriteIfNeeded($content, 'ext/php_ext.h');

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

		$this->_checkAndWriteIfNeeded($content, 'ext/ext.h');

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

		$this->_checkAndWriteIfNeeded($content, 'ext/ext_config.h');

		/**
		 * ext_clean
		 */
		$content = file_get_contents(__DIR__ . '/../ext/clean');
		if (empty($content)) {
			throw new Exception("clean file doesn't exists");
		}

		if ($this->_checkAndWriteIfNeeded($content, 'ext/clean')) {
			chmod('ext/clean', 0755);
		}

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

		if ($this->_checkAndWriteIfNeeded($content, 'ext/install')) {
			chmod('ext/install', 0755);
		}

		return $needConfigure;
	}

	/**
	 * Create project.c and project.h by compiled files to test extension
	 *
	 * @param string $project
	 * @return boolean
	 */
	public function createProjectFiles($project)
	{

		$needConfigure = $this->checkKernelFiles();

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
		$this->_checkAndWriteIfNeeded($content, 'ext/' . $project . '.c');
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

		$this->_checkAndWriteIfNeeded($content, 'ext/' . $project . '.h');
		unset($content);

		/**
		 * Round 5. Create php_project.h
		 */
		$content = file_get_contents(__DIR__ . '/../templates/php_project.h');
		if (empty($content)) {
			throw new Exception("Template php_project.h doesn't exist");
		}

		/**
		 * Generate the globals declaration
		 */
		$globals = $this->_config->get('globals');
		if (is_array($globals)) {

			$globalStruct = "";

			$structures = array();
			foreach ($globals as $name => $global) {
				$parts = explode(".", $name);
				$structures[$parts[0]][$parts[1]] = $global;
			}

			foreach ($structures as $structureName => $internalStructure) {
				$globalStruct .= 'typedef struct _zephir_struct_' . $structureName . ' {' . PHP_EOL;
				foreach ($internalStructure as $field => $global) {
					switch ($global['type']) {
						case 'bool':
							$globalStruct .= "\t" . 'zend_bool ' . $field . ';' . PHP_EOL;
							break;
						default:
							throw new Exception('Unknown global type: ' . $global['type']);
					}
				}
				$globalStruct .= '} zephir_struct_' . $structureName . ';' . PHP_EOL;
			}

			$globalCode = PHP_EOL;
			foreach ($structures as $structureName => $internalStructure) {
				$globalCode .= "\t" . 'zephir_struct_' . $structureName . ' ' . $structureName . ';' . PHP_EOL . PHP_EOL;
			}

		} else {
			$globalCode = null;
			$globalStruct = null;
		}

		$version = $this->_config->get('version');
		if (!$version) {
			$version = '0.0.1';
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		     => strtolower($project),
			'%PROJECT_UPPER%' 		     => strtoupper($project),
			'%PROJECT_EXTNAME%' 	     => strtolower($project),
			'%PROJECT_VERSION%' 	     => $version,
			'%EXTENSION_GLOBALS%'        => $globalCode,
			'%EXTENSION_STRUCT_GLOBALS%' => $globalStruct
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		$this->_checkAndWriteIfNeeded($content, 'ext/php_' . $project . '.h');
		unset($content);

		return $needConfigure;
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
