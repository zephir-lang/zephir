<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
	/**
	 * @var CompilerFile[]
	 */
	protected $_files = array();

	/**
	 * @var ClassDefinition[]
	 */
	protected $_definitions = array();

	/**
	 * @var array|string[]
	 */
	protected $_compiledFiles = array();

	protected $_constants = array();

	protected $_globals = array();

	/**
	 * @var StringsManager
	 */
	protected $_stringManager;

	/**
	 * @var Config
	 */
	protected $_config;

	/**
	 * @var Logger
	 */
	protected $_logger;

	/**
	 * @var array|ReflectionClass[]
	 */
	protected static $_reflections = array();

	/**
	 * @var boolean
	 */
	protected static $_loadedPrototypes = false;

	public function __construct(Config $config, Logger $logger)
	{
		$this->_config = $config;
		$this->_logger = $logger;
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
	 * @return boolean
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
	 * @return boolean
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
	 * @return boolean
	 */
	public function isInternalClass($className)
	{
		return class_exists($className, false);
	}

	/**
	 * Allows to check if a interface is part of PHP
	 *
	 * @param string $className
	 * @return boolean
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
					$this->_recursiveProcess($pathName, $dest . '/' . $fileName, $pattern, $callback);
				}
			} else {
				if (!$pattern || ($pattern && preg_match($pattern, $fileName) === 1)) {
					if (is_string($callback)) {
						$success = $success && $callback($pathName, $dest . '/' . $fileName);
					} else {
						$success = $success && call_user_func($callback, $pathName, $dest . '/' . $fileName);
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

	/**
	 * @return string
	 * @throws Exception
	 */
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

		/**
		 * Tell the user the name could be reserved by another extension
		 */
		if (extension_loaded($namespace)) {
			echo 'This extension can have conflicts with an existing loaded extension', PHP_EOL;
		}

		$this->_config->set('namespace', $namespace);
		$this->_config->set('name',      $namespace);

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
		 * Load function optimizers
		 */
		if (self::$_loadedPrototypes == false) {

			FunctionCall::addOptimizerDir(ZEPHIRPATH . 'Library/Optimizers/FunctionCall');

			$optimizerDirs = $this->_config->get('optimizer-dirs');
			if (is_array($optimizerDirs)) {
				foreach ($optimizerDirs as $directory) {
					FunctionCall::addOptimizerDir(realpath($directory));
				}
			}

			if (is_dir(ZEPHIRPATH . 'prototypes') && is_readable(ZEPHIRPATH . 'prototypes')) {

				/**
				 * Load additional extension prototypes
				 */
				foreach (new DirectoryIterator(ZEPHIRPATH . 'prototypes') as $file) {
					if (!$file->isDir()) {
						$extension = str_replace('.php', '', $file);
						if (!extension_loaded($extension)) {
							require $file->getRealPath();
						}
					}
				}
			}

			self::$_loadedPrototypes = true;
		}

		/**
		 * Round 3. compile all files to C sources
		 */
		$files = array();

		$hash = "";
		foreach ($this->_files as $compileFile) {
			$compileFile->compile($this, $this->_stringManager);
			$compiledFile = $compileFile->getCompiledFile();
			$files[] = $compiledFile;
			$hash .= '|' . $compiledFile;
		}
		$hash = md5($hash);

		$this->_compiledFiles = $files;

		/* Load extra C-sources */
		$extraSources = $this->_config->get('extra-sources');
		if (is_array($extraSources)) {
			$this->_extraFiles = $extraSources;
		} else {
			$this->_extraFiles = array();
		}

		/**
		 * Round 4. Create config.m4 and config.w32 files / Create project.c and project.h files
		 */
		$needConfigure  = $this->createConfigFiles($namespace);
		$needConfigure |= $this->createProjectFiles($namespace);
		$needConfigure |= $this->checkIfPhpized($namespace);

		/**
		 * When a new file is added or removed we need to run configure again
		 */
		if (!($command instanceof CommandGenerate)) {
			if (!file_exists('.temp/compiled-files-sum')) {
				$needConfigure = true;
				file_put_contents('.temp/compiled-files-sum', $hash);
			} else {
				if (file_get_contents('.temp/compiled-files-sum') != $hash) {
					$needConfigure = true;
					file_put_contents('.temp/compiled-files-sum', $hash);
				}
			}
		}

		/**
		 * Round 5. Generate the concatenation
		 */
		$this->_stringManager->genConcatCode();

		return $needConfigure;
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

		$needConfigure = $this->generate($command);

		if ($needConfigure) {

			exec('cd ext && make clean && phpize --clean', $output, $exit);
			$this->_logger->output('Preparing for PHP compilation...');

			exec('cd ext && phpize', $output, $exit);
			$this->_logger->output('Preparing configuration file...');

			exec('export CC="gcc" && export CFLAGS="-O2" && cd ext && ./configure --enable-' . $namespace);
		}

		$this->_logger->output('Compiling...');

		$verbose = ($this->_config->get('verbose') ? true : false);
		if ($verbose) {
			passthru('cd ext && make -j2', $exit);
		} else {
			exec('cd ext && make --silent -j2', $output, $exit);
		}

	}

	/**
	 * Compiles and installs the extension
	 *
	 * @param CommandInstall $command
	 * @throws Exception
	 */
	public function install(CommandInterface $command)
	{
		/**
		 * Get global namespace
		 */
		$namespace = $this->_checkDirectory();

		$this->compile($command);

		$this->_logger->output('Installing...');
		exec('(export CC="gcc" && export CFLAGS="-O2" && cd ext && sudo make --silent install) > /dev/null 2>&1', $output, $exit);

		$this->_logger->output('Extension installed!');
		if (!extension_loaded($namespace)) {
			$this->_logger->output('Add extension=' . $namespace . '.so to your php.ini');
		}
		$this->_logger->output('Don\'t forget to restart your web server');
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

		$this->_logger->output('Running tests...');
		system('export CC="gcc" && export CFLAGS="-O0 -g" && export NO_INTERACTION=1 && cd ext && make test', $exit);
	}

	/**
	 * Clean the extension directory
	 *
	 * @param CommandInterface $command
	 */
	public function clean(CommandInterface $command)
	{
		system('cd ext && make clean 1> /dev/null');
	}

	/**
	 * Clean the extension directory
	 *
	 * @param CommandInterface $command
	 */
	public function fullClean(CommandInterface $command)
	{
		system('cd ext && sudo make clean 1> /dev/null');
		system('cd ext && phpize --clean 1> /dev/null');
	}

	/**
	 *
	 * Compiles and installs the extension
	 * @param CommandInstall $command
	 * @throws Exception
	 */
	public function build(CommandInterface $command)
	{
		$this->compile($command);
		$this->install($command);
	}

	/**
	 * Checks if a file must be copied
	 *
	 * @param string $src
	 * @param string $dst
	 * @return boolean
	 */
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

	/**
	 * Checks which files in the base kernel must be copied
	 *
	 * @return boolean
	 */
	protected function checkKernelFiles()
	{
		$configured = $this->_recursiveProcess(realpath(__DIR__ . '/../ext/kernel'), 'ext/kernel', '@^.*\.c|h$@', array($this, '_checkKernelFile'));
		if (!$configured) {
			$this->_logger->output('Copying new kernel files...');
			exec("rm -fr ext/kernel/*");
			$this->_recursiveProcess(realpath(__DIR__ . '/../ext/kernel'), 'ext/kernel', '@^.*\.c|h$@');
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

		$content = file_get_contents(__DIR__ . '/../templates/config.m4');
		if (empty($content)) {
			throw new Exception("Template config.m4 doesn't exist");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 	 	      => strtolower($project),
			'%PROJECT_UPPER%' 		      => strtoupper($project),
			'%PROJECT_CAMELIZE%' 	      => ucfirst($project),
			'%FILES_COMPILED%' 		      => implode(' ', $this->_compiledFiles),
			'%EXTRA_FILES_COMPILED%'      => implode(' ', $this->_extraFiles),
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		$needConfigure = Utils::checkAndWriteIfNeeded($content, 'ext/config.m4');

		/**
		 * php_ext.h
		 */
		$content = file_get_contents(__DIR__ . '/../templates/php_ext.h');
		if (empty($content)) {
			throw new Exception("Template php_ext.h doesn't exist");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		Utils::checkAndWriteIfNeeded($content, 'ext/php_ext.h');

		/**
		 * ext.h
		 */
		$content = file_get_contents(__DIR__ . '/../templates/ext.h');
		if (empty($content)) {
			throw new Exception("Template ext.h doesn't exist");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		Utils::checkAndWriteIfNeeded($content, 'ext/ext.h');

		/**
		 * ext_config.h
		 */
		$content = file_get_contents(__DIR__ . '/../templates/ext_config.h');
		if (empty($content)) {
			throw new Exception("Template ext_config.h doesn't exist");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		Utils::checkAndWriteIfNeeded($content, 'ext/ext_config.h');

		/**
		 * ext_clean
		 */
		$content = file_get_contents(__DIR__ . '/../ext/clean');
		if (empty($content)) {
			throw new Exception("Clean file doesn't exist");
		}

		if (Utils::checkAndWriteIfNeeded($content, 'ext/clean')) {
			chmod('ext/clean', 0755);
		}

		/**
		 * ext_install
		 */
		$content = file_get_contents(__DIR__ . '/../templates/install');
		if (empty($content)) {
			throw new Exception("Install file doesn't exist");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		if (Utils::checkAndWriteIfNeeded($content, 'ext/install')) {
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
		Utils::checkAndWriteIfNeeded($content, 'ext/' . $project . '.c');
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

		Utils::checkAndWriteIfNeeded($content, 'ext/' . $project . '.h');
		unset($content);

		/**
		 * Round 5. Create php_project.h
		 */
		$content = file_get_contents(__DIR__ . '/../templates/php_project.h');
		if (empty($content)) {
			throw new Exception('Template php_project.h doesn`t exist');
		}

		$globalCode = '';
		$globalStruct = '';

		/**
		 * Generate the globals declaration
		 */
		$globals = $this->_config->get('globals');
		if (is_array($globals)) {
			$structures = array();
			foreach ($globals as $name => $global) {
				$parts = explode('.', $name);
				$structures[$parts[0]][$parts[1]] = $global;
			}

			foreach ($structures as $structureName => $internalStructure) {
				$structBuilder = new Code\Builder\Struct('_zephir_struct_' . $structureName);
				foreach ($internalStructure as $field => $global) {
					$structBuilder->addProperty($global['type'], $field);
				}

				$globalStruct .= $structBuilder . PHP_EOL;
			}

			$globalCode = PHP_EOL;
			foreach ($structures as $structureName => $internalStructure) {
				$globalCode .= "\t" . 'zephir_struct_' . $structureName . ' ' . $structureName . ';' . PHP_EOL . PHP_EOL;
			}

		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		     => strtolower($project),
			'%PROJECT_UPPER%' 		     => strtoupper($project),
			'%PROJECT_EXTNAME%' 	     => strtolower($project),
			'%PROJECT_NAME%'             => $this->_config->get('name'),
			'%PROJECT_VERSION%' 	     => $this->_config->get('version'),
			'%EXTENSION_GLOBALS%'        => $globalCode,
			'%EXTENSION_STRUCT_GLOBALS%' => $globalStruct
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		Utils::checkAndWriteIfNeeded($content, 'ext/php_' . $project . '.h');
		unset($content);

		return $needConfigure;
	}

	/*+
	 * Check if the project must be phpized again
	 *
	 * @param string $namespace
	 */
	public function checkIfPhpized($namespace)
	{
		return !file_exists('ext/Makefile');
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
