<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */

namespace Zephir;

use Zephir\Parser\Manager;
use Zephir\Parser\ParseException;
use Zephir\Commands\CommandGenerate;
use Zephir\Commands\CommandInterface;
use Zephir\Compiler\CompilerException;
use Zephir\Fcall\FcallManagerInterface;
use Zephir\Exception\IllegalStateException;
use Zephir\FileSystem\HardDisk as FileSystem;

/**
 * Zephir\Compiler
 *
 * The main compiler.
 *
 * @package Zephir
 */
class Compiler
{
    const VERSION = '0.10.16';

    public $parserCompiled = false;

    /**
     * @var BaseBackend
     */
    public $backend;

    /**
     * @var FunctionDefinition[]
     */
    public $functionDefinitions = array();

    /**
     * @var CompilerFile[]
     */
    protected $files = array();

    /**
     * @var array|string[]
     */
    protected $anonymousFiles = array();

    /**
     * Additional initializer code
     * used for static property initialization
     * @var array
     */
    protected $internalInitializers = array();

    /**
     * @var ClassDefinition[]
     */
    protected $definitions = array();

    /**
     * @var array|string[]
     */
    protected $compiledFiles = array();

    /**
     *
     */
    protected $constants = array();

    /**
     * Extension globals
     *
     * @var array
     */
    protected $globals = array();

    /**
     * External dependencies
     *
     * @var array
     */
    protected $externalDependencies = array();

    /**
     * @var StringsManager
     */
    protected $stringManager;

    /**
     * @var FcallManagerInterface
     */
    protected $fcallManager;

    /**
     * @var Config
     */
    protected $config;

    /**
     * @var Logger
     */
    protected $logger;

    /**
     * @var \ReflectionClass[]
     */
    protected static $internalDefinitions = array();

    /**
     * @var boolean
     */
    protected static $loadedPrototypes = false;

    /**
     * @var string
     */
    protected static $currentVersion;

    /**
     * @var array
     */
    protected $extraFiles = array();

    /**
     * The Zephir Parser Manager
     * @var Manager
     */
    protected $parserManager;

    /**
     * @var FileSystem
     */
    protected $fileSystem;

    /**
     * Compiler constructor
     *
     * @param Config $config
     * @param Logger $logger
     * @param BaseBackend $backend
     * @param Manager $manager
     *
     * @throws Exception
     */
    public function __construct(Config $config, Logger $logger, BaseBackend $backend, Manager $manager)
    {
        $this->config = $config;
        $this->logger = $logger;
        $this->fileSystem = new FileSystem();
        $this->backend = $backend;
        $this->stringManager = $this->backend->getStringsManager();
        $this->fcallManager = $this->backend->getFcallManager();
        $this->checkRequires();

        $this->parserManager = $manager;
    }

    /**
     * Gets the Zephir Parser Manager.
     *
     * @return Manager
     */
    public function getParserManager()
    {
        return $this->parserManager;
    }

    /**
     * Adds a function to the function definitions
     *
     * @param FunctionDefinition $func
     * @param array $statement
     * @throws CompilerException
     */
    public function addFunction(FunctionDefinition $func, $statement = null)
    {
        $funcName = strtolower($func->getInternalName());
        if (isset($this->functionDefinitions[$funcName])) {
            throw new CompilerException(
                "Function '" . $func->getCompleteName() . "' was defined more than one time",
                $statement
            );
        }

        $this->functionDefinitions[$funcName] = $func;
    }

    /**
     * Pre-compiles classes creating a CompilerFile definition
     *
     * @param string $filePath
     * @throws CompilerException
     * @throws IllegalStateException
     * @throws ParseException
     */
    protected function preCompile($filePath)
    {
        if (!$this->parserManager->isAvailable()) {
            throw new IllegalStateException($this->parserManager->requirements());
        }

        if (preg_match('#\.zep$#', $filePath)) {
            $className = str_replace(DIRECTORY_SEPARATOR, '\\', $filePath);
            $className = preg_replace('#.zep$#', '', $className);

            $className = join('\\', array_map(function ($i) {
                return ucfirst($i);
            }, explode('\\', $className)));

            $this->files[$className] = new CompilerFile($className, $filePath, $this->config, $this->logger, $this->backend);
            $this->files[$className]->preCompile($this);

            $this->definitions[$className] = $this->files[$className]->getClassDefinition();
        }
    }

    /**
     * Recursively pre-compiles all sources found in the given path
     *
     * @param string $path
     *
     * @throws CompilerException
     */
    protected function recursivePreCompile($path)
    {
        if (!is_string($path)) {
            throw new CompilerException('Invalid compilation path' . var_export($path, true));
        }

        /**
         * Pre compile all files
         */
        $files = array();
        $iterator = new \RecursiveIteratorIterator(
            new \RecursiveDirectoryIterator($path),
            \RecursiveIteratorIterator::SELF_FIRST
        );

        /**
         * @var $item \SplFileInfo
         */
        foreach ($iterator as $item) {
            if (!$item->isDir()) {
                $files[] = $item->getPathname();
            }
        }

        sort($files, SORT_STRING);
        foreach ($files as $file) {
            $this->preCompile($file);
        }
    }

    /**
     * Loads a class definition in an external dependency
     *
     * @param string $className
     * @param string $location
     * @return boolean
     * @throws CompilerException
     * @throws Exception
     * @throws ParseException
     */
    public function loadExternalClass($className, $location)
    {
        $filePath = $location . DIRECTORY_SEPARATOR .
                    strtolower(str_replace('\\', DIRECTORY_SEPARATOR, $className)) . '.zep';

        /**
         * Fix the class name
         */
        $className = join('\\', array_map(function ($i) {
            return ucfirst($i);
        }, explode('\\', $className)));

        if (isset($this->files[$className])) {
            return true;
        }

        if (!file_exists($filePath)) {
            return false;
        }

        $this->files[$className] = new CompilerFile($className, $filePath, $this->config, $this->logger);
        $this->files[$className]->setIsExternal(true);
        $this->files[$className]->preCompile($this);

        $this->definitions[$className] = $this->files[$className]->getClassDefinition();
        return true;
    }

    /**
     * Allows to check if a class is part of the compiled extension
     *
     * @param string $className
     * @return boolean
     */
    public function isClass($className)
    {
        foreach ($this->definitions as $key => $value) {
            if (!strcasecmp($key, $className)) {
                if ($value->getType() == 'class') {
                    return true;
                }
            }
        }

        /**
         * Try to autoload the class from an external dependency
         */
        if (count($this->externalDependencies)) {
            foreach ($this->externalDependencies as $namespace => $location) {
                if (preg_match('#^' . $namespace . '\\\\#i', $className)) {
                    return $this->loadExternalClass($className, $location);
                }
            }
        }

        return false;
    }

    /**
     * Allows to check if an interface is part of the compiled extension
     *
     * @param string $className
     *
     * @return boolean
     */
    public function isInterface($className)
    {
        foreach ($this->definitions as $key => $value) {
            if (!strcasecmp($key, $className)) {
                if ($value->getType() == 'interface') {
                    return true;
                }
            }
        }

        /**
         * Try to autoload the class from an external dependency
         */
        if (count($this->externalDependencies)) {
            foreach ($this->externalDependencies as $namespace => $location) {
                if (preg_match('#^' . $namespace . '\\\\#i', $className)) {
                    return $this->loadExternalClass($className, $location);
                }
            }
        }

        return false;
    }

    /**
     * Allows to check if a class is part of PHP
     *
     * @param string $className
     *
     * @return boolean
     */
    public function isBundledClass($className)
    {
        return class_exists($className, false);
    }

    /**
     * Allows to check if a interface is part of PHP
     *
     * @param string $className
     *
     * @return boolean
     */
    public function isBundledInterface($className)
    {
        return interface_exists($className, false);
    }

    /**
     * Returns class the class definition from a given class name
     *
     * @param string $className
     *
     * @return ClassDefinition|false returns false if no class definition is found
     */
    public function getClassDefinition($className)
    {
        foreach ($this->definitions as $key => $value) {
            if (!strcasecmp($key, $className)) {
                return $value;
            }
        }

        return false;
    }

    /**
     * Inserts an anonymous class definition in the compiler
     *
     * @param CompilerFileAnonymous $file
     * @param ClassDefinition $classDefinition
     */
    public function addClassDefinition(CompilerFileAnonymous $file, ClassDefinition $classDefinition)
    {
        $this->definitions[$classDefinition->getCompleteName()] = $classDefinition;
        $this->anonymousFiles[$classDefinition->getCompleteName()] = $file;
    }

    /**
     * Returns class the class definition from a given class name
     *
     * @param string $className
     *
     * @return ClassDefinition
     */
    public function getInternalClassDefinition($className)
    {
        if (!isset(self::$internalDefinitions[$className])) {
            $reflection = new \ReflectionClass($className);
            self::$internalDefinitions[$className] = ClassDefinition::buildFromReflection($reflection);
        }

        return self::$internalDefinitions[$className];
    }

    /**
     * Copies the base kernel to the extension destination
     *
     * @param        $src
     * @param        $dest
     * @param string $pattern
     * @param mixed  $callback
     *
     * @return bool
     */
    protected function recursiveProcess($src, $dest, $pattern = null, $callback = "copy")
    {
        $success = true;
        $iterator = new \DirectoryIterator($src);
        foreach ($iterator as $item) {
            $pathName = $item->getPathname();
            if (!is_readable($pathName)) {
                $this->logger->output('File is not readable :' . $pathName);
                continue;
            }

            $fileName = $item->getFileName();

            if ($item->isDir()) {
                if ($fileName != '.' && $fileName != '..' && $fileName != '.libs') {
                    if (!is_dir($dest . DIRECTORY_SEPARATOR . $fileName)) {
                        mkdir($dest . DIRECTORY_SEPARATOR . $fileName, 0755, true);
                    }
                    $this->recursiveProcess($pathName, $dest . DIRECTORY_SEPARATOR . $fileName, $pattern, $callback);
                }
            } elseif (!$pattern || ($pattern && preg_match($pattern, $fileName) === 1)) {
                $path = $dest . DIRECTORY_SEPARATOR . $fileName;
                $success = $success && call_user_func($callback, $pathName, $path);
            }
        }

        return $success;
    }

    /**
     * Recursively deletes files in a specified location
     *
     * @param string $path Directory to deletes files
     * @param string $mask Regular expression to deletes files
     *
     * @return void
     */
    protected function recursiveDeletePath($path, $mask)
    {
        if (!file_exists($path) || !is_dir($path) || !is_readable($path)) {
            $this->logger->output("Directory {$path} does not exist or it is not readble. Skip...");
            return;
        }

        $objects = new \RecursiveIteratorIterator(
            new \RecursiveDirectoryIterator($path),
            \RecursiveIteratorIterator::SELF_FIRST
        );

        foreach ($objects as $name => $object) {
            if (preg_match($mask, $name)) {
                @unlink($name);
            }
        }
    }

    /**
     * Registers C-constants as PHP constants from a C-file
     *
     * @param array $constantsSources
     *
     * @throws Exception
     */
    protected function loadConstantsSources($constantsSources)
    {
        foreach ($constantsSources as $constantsSource) {
            if (!file_exists($constantsSource)) {
                throw new Exception("File '" . $constantsSource . "' with constants definitions");
            }

            foreach (file($constantsSource) as $line) {
                if (preg_match('/^\#define[ \t]+([A-Z0-9\_]+)[ \t]+([0-9]+)/', $line, $matches)) {
                    $this->constants[$matches[1]] = array('int', $matches[2]);
                    continue;
                }
                if (preg_match('/^\#define[ \t]+([A-Z0-9\_]+)[ \t]+(\'(.){1}\')/', $line, $matches)) {
                    $this->constants[$matches[1]] = array('char', $matches[3]);
                }
            }
        }
    }

    /**
     * Checks if $name is a Zephir constant
     *
     * @param string $name
     *
     * @return boolean
     */
    public function isConstant($name)
    {
        return isset($this->constants[$name]);
    }

    /**
     * Returns a Zephir Constant by its name
     *
     * @param string $name
     */
    public function getConstant($name)
    {
        return $this->constants[$name];
    }

    /**
     * Sets extensions globals
     *
     * @param array $globals
     */
    public function setExtensionGlobals(array $globals)
    {
        foreach ($globals as $key => $value) {
            $this->globals[$key] = $value;
        }
    }

    /**
     * Checks if a specific extension global is defined
     *
     * @param string $name
     *
     * @return boolean
     */
    public function isExtensionGlobal($name)
    {
        return isset($this->globals[$name]);
    }

    /**
     * Returns a extension global by its name
     *
     * @param string $name
     *
     * @return array
     */
    public function getExtensionGlobal($name)
    {
        return $this->globals[$name];
    }

    /**
     * Returns the current version + the ident if available
     *
     * @return string
     */
    public static function getCurrentVersion()
    {
        $version = '$Id$';
        if (strlen($version) != 4) {
            return self::VERSION . '-' . substr($version, 0, 10);
        }

        if (!Utils::isWindows()) {
            if (self::$currentVersion === null) {
                if (file_exists(__DIR__ . '/../.git')) {
                    exec('cd ' . __DIR__ . '/.. && git log --format="%H" -n 1', $xversion);
                    if (isset($xversion[0]) && strlen($xversion[0]) > 10) {
                        self::$currentVersion = substr($xversion[0], 0, 10);
                    } else {
                        self::$currentVersion = false;
                    }
                }
            }

            if (self::$currentVersion) {
                return self::VERSION . '-' . self::$currentVersion;
            }
        }

        return self::VERSION;
    }

    /**
     * Returns GCC flags for current compilation
     *
     * @param boolean $development
     * @return string
     */
    public function getGccFlags($development = false)
    {
        if (!Utils::isWindows()) {
            $gccFlags = getenv('CFLAGS');
            if (!is_string($gccFlags)) {
                if (!$development) {
                    $gccVersion = $this->getGccVersion();
                    if (version_compare($gccVersion, '4.6.0', '>=')) {
                        $gccFlags = '-O2 -fvisibility=hidden -Wparentheses -flto -DZEPHIR_RELEASE=1';
                    } else {
                        $gccFlags = '-O2 -fvisibility=hidden -Wparentheses -DZEPHIR_RELEASE=1';
                    }
                } else {
                    $gccFlags = '-O0 -g3';
                }
            }
            return $gccFlags;
        }
    }

    /**
     * Returns the php include directories returned by php-config
     *
     * @return string
     */
    public function getPhpIncludeDirs()
    {
        $version = self::getCurrentVersion();

        if (!Utils::isWindows()) {
            $this->fileSystem->system('php-config --includes', 'stdout', $version . '/php-includes');
        }

        return trim($this->fileSystem->read($version . '/php-includes'));
    }

    /**
     * Pre-compile headers to speed up compilation
     */
    public function preCompileHeaders()
    {
        if (!Utils::isWindows()) {
            $version = self::getCurrentVersion();
            $phpIncludes = $this->getPhpIncludeDirs();

            foreach (new \DirectoryIterator('ext/kernel') as $file) {
                if (!$file->isDir()) {
                    if (preg_match('/\.h$/', $file)) {
                        $path = $file->getRealPath();
                        if (!file_exists($path . '.gch')) {
                            $this->fileSystem->system(
                                'cd ext && gcc -c kernel/' . $file->getBaseName() .
                                ' -I. ' . $phpIncludes . ' -o kernel/' . $file->getBaseName() . '.gch',
                                'stdout',
                                $version . '/compile-header'
                            );
                        } else {
                            if (filemtime($path) > filemtime($path . '.gch')) {
                                $this->fileSystem->system(
                                    'cd ext && gcc -c kernel/' . $file->getBaseName() .
                                    ' -I. ' . $phpIncludes . ' -o kernel/' . $file->getBaseName() . '.gch',
                                    'stdout',
                                    $version . '/compile-header'
                                );
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Initializes a Zephir extension
     *
     * @param CommandInterface $command
     *
     * @throws Exception
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
            $this->logger->output('This extension can have conflicts with an existing loaded extension');
        }

        $this->config->set('namespace', $namespace);
        $this->config->set('name', $namespace);

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
        $this->recursiveProcess($this->backend->getInternalKernelPath(), 'ext/kernel');
    }

    /**
     * Generates the C sources from Zephir without compiling them
     *
     * @param CommandInterface $command
     * @return bool
     * @throws Exception
     */
    public function generate(CommandInterface $command)
    {
        /**
         * Get global namespace
         */
        $namespace = $this->checkDirectory();

        /**
         * Check whether there are external dependencies
         */
        $externalDependencies = $this->config->get('external-dependencies');
        if (is_array($externalDependencies)) {
            foreach ($externalDependencies as $dependencyNs => $location) {
                if (!file_exists($location)) {
                    throw new CompilerException(
                        'Location of dependency "' .
                        $dependencyNs .
                        '" does not exist. Check the config.json for more information'
                    );
                }

                $this->addExternalDependency($dependencyNs, $location);
            }
        }

        /**
         * Check if there are module/request/global destructors
         */
        $includes = '';
        $destructors = $this->config->get('destructors');
        if (is_array($destructors)) {
            $invokeRequestDestructors = $this->processCodeInjection($destructors, 'request');
            $includes .= PHP_EOL . $invokeRequestDestructors[0];
            $reqDestructors = $invokeRequestDestructors[1];

            $invokePostRequestDestructors = $this->processCodeInjection($destructors, 'post-request');
            $includes .= PHP_EOL . $invokePostRequestDestructors[0];
            $prqDestructors = $invokePostRequestDestructors[1];

            $invokeModuleDestructors = $this->processCodeInjection($destructors, 'module');
            $includes .= PHP_EOL . $invokeModuleDestructors[0];
            $modDestructors = $invokeModuleDestructors[1];

            $invokeGlobalsDestructors = $this->processCodeInjection($destructors, 'globals');
            $includes .= PHP_EOL . $invokeGlobalsDestructors[0];
            $glbDestructors = $invokeGlobalsDestructors[1];
        }

        /**
         * Check if there are module/request/global initializers
         */
        $initializers = $this->config->get('initializers');
        if (is_array($initializers)) {
            $invokeRequestInitializers = $this->processCodeInjection($initializers, 'request');
            $includes .= PHP_EOL . $invokeRequestInitializers[0];
            $reqInitializers = $invokeRequestInitializers[1];

            $invokeModuleInitializers = $this->processCodeInjection($initializers, 'module');
            $includes .= PHP_EOL . $invokeModuleInitializers[0];
            $modInitializers = $invokeModuleInitializers[1];

            $invokeGlobalsInitializers = $this->processCodeInjection($initializers, 'globals');
            $includes .= PHP_EOL . $invokeGlobalsInitializers[0];
            $glbInitializers = $invokeGlobalsInitializers[1];
        }

        /**
         * Round 1. pre-compile all files in memory
         */
        $this->recursivePreCompile(str_replace('\\', DIRECTORY_SEPARATOR, $namespace));
        if (!count($this->files)) {
            throw new Exception(
                "Zephir files to compile couldn't be found. Did you add a first class to the extension?"
            );
        }

        /**
         * Round 2. Check 'extends' and 'implements' dependencies
         */
        foreach ($this->files as $compileFile) {
            $compileFile->checkDependencies($this);
        }

        /**
         * Sort the files by dependency ranking
         */
        $files = array();
        $rankedFiles = array();
        $this->calculateDependencies($this->files);

        foreach ($this->files as $rankFile) {
            $rank = $rankFile->getClassDefinition()->getDependencyRank();
            $rankedFiles[$rank][] = $rankFile;
        }

        krsort($rankedFiles);
        foreach ($rankedFiles as $rank => $rankFiles) {
            $files = array_merge($files, $rankFiles);
        }
        $this->files = $files;

        /**
         * Convert C-constants into PHP constants
         */
        $constantsSources = $this->config->get('constants-sources');
        if (is_array($constantsSources)) {
            $this->loadConstantsSources($constantsSources);
        }

        /**
         * Set extension globals
         */
        $globals = $this->config->get('globals');
        if (is_array($globals)) {
            $this->setExtensionGlobals($globals);
        }

        /**
         * Load function optimizers
         */
        if (self::$loadedPrototypes === false) {
            FunctionCall::addOptimizerDir(ZEPHIRPATH . '/Library/Optimizers/FunctionCall');

            $optimizerDirs = $this->config->get('optimizer-dirs');
            if (is_array($optimizerDirs)) {
                foreach ($optimizerDirs as $directory) {
                    FunctionCall::addOptimizerDir(realpath($directory));
                }
            }

            if (is_dir(ZEPHIRPATH . '/prototypes') && is_readable(ZEPHIRPATH . '/prototypes')) {
                /**
                 * Load additional extension prototypes
                 * @var $file \DirectoryIterator
                 */
                foreach (new \DirectoryIterator(ZEPHIRPATH . '/prototypes') as $file) {
                    if (!$file->isDir()) {
                        $extension = str_replace('.php', '', $file);
                        if (!extension_loaded($extension)) {
                            require $file->getRealPath();
                        }
                    }
                }
            }

            /**
             * Load customer additional extension prototypes
             */
            $prototypeDirs = $this->config->get('prototype-dir');
            if (is_array($prototypeDirs)) {
                foreach ($prototypeDirs as $prototype => $prototypeDir) {
                    /**
                     * Check if the extension is installed
                     */
                    if (!extension_loaded($prototype)) {
                        $prototypeRealpath = realpath($prototypeDir);
                        if ($prototypeRealpath) {
                            foreach (new \DirectoryIterator($prototypeRealpath) as $file) {
                                if (!$file->isDir()) {
                                    require $file->getRealPath();
                                }
                            }
                        }
                    }
                }
            }

            self::$loadedPrototypes = true;
        }

        /**
         * Round 3. Compile all files to C sources
         */
        $files = array();

        $hash = "";
        foreach ($this->files as $compileFile) {
            /**
             * Only compile classes in the local extension, ignore external classes
             */
            if (!$compileFile->isExternal()) {
                $compileFile->compile($this, $this->stringManager);
                $compiledFile = $compileFile->getCompiledFile();

                $methods = array();
                $classDefinition = $compileFile->getClassDefinition();
                foreach ($classDefinition->getMethods() as $method) {
                    $methods[] = '[' . $method->getName() . ':' . join('-', $method->getVisibility()) . ']';
                    if ($method->isInitializer() && $method->isStatic()) {
                        $this->internalInitializers[] = "\t" . $method->getName() . '(TSRMLS_C);';
                    }
                }

                $files[] = $compiledFile;

                $hash .= '|' . $compiledFile . ':' . $classDefinition->getClassEntry() .
                        '[' . join('|', $methods) . ']';
            }
        }

        /**
         * Round 3.2. Compile anonymous classes
         */
        foreach ($this->anonymousFiles as $compileFile) {
            $compileFile->compile($this, $this->stringManager);
            $compiledFile = $compileFile->getCompiledFile();

            $methods = array();
            $classDefinition = $compileFile->getClassDefinition();
            foreach ($classDefinition->getMethods() as $method) {
                $methods[] = '[' . $method->getName() . ':' . join('-', $method->getVisibility()) . ']';
            }

            $files[] = $compiledFile;

            $hash .= '|' . $compiledFile . ':' . $classDefinition->getClassEntry() . '[' . join('|', $methods) . ']';
        }

        $hash = md5($hash);
        $this->compiledFiles = $files;

        /**
         * Round 3.3. Load extra C-sources
         */
        $extraSources = $this->config->get('extra-sources');
        if (is_array($extraSources)) {
            $this->extraFiles = $extraSources;
        } else {
            $this->extraFiles = array();
        }

        /**
         * Round 3.4. Load extra classes sources
         */
        $extraClasses = $this->config->get('extra-classes');
        if (is_array($extraClasses)) {
            foreach ($extraClasses as $value) {
                if (isset($value['source'])) {
                    $this->extraFiles[] = $value['source'];
                }
            }
        }

        /**
         * Round 4. Create config.m4 and config.w32 files / Create project.c and project.h files
         */
        $namespace      = str_replace('\\', '_', $namespace);
        $extensionName  = $this->config->get('extension-name');
        if (empty($extensionName) || !is_string($extensionName)) {
            $extensionName = $namespace;
        }

        $needConfigure  = $this->createConfigFiles($extensionName);
        $needConfigure |= $this->createProjectFiles($extensionName);
        $needConfigure |= $this->checkIfPhpized();

        $version = self::getCurrentVersion();

        /**
         * When a new file is added or removed we need to run configure again
         */
        if (!($command instanceof CommandGenerate)) {
            if (!$this->fileSystem->exists($version . '/compiled-files-sum')) {
                $needConfigure = true;
                $this->fileSystem->write($version . '/compiled-files-sum', $hash);
            } else {
                if ($this->fileSystem->read($version . '/compiled-files-sum') != $hash) {
                    $needConfigure = true;
                    $this->fileSystem->write($version . '/compiled-files-sum', $hash);
                }
            }
        }

        /**
         * Round 5. Generate concatenation functions
         */
        $this->stringManager->genConcatCode();
        $this->fcallManager->genFcallCode();

        if ($this->config->get('stubs-run-after-generate', 'stubs')) {
            $this->stubs($command, true);
        }

        return $needConfigure;
    }

    /**
     * Compiles the extension without installing it
     *
     * @param CommandInterface $command
     * @param bool             $development
     *
     * @throws Exception|CompilerException
     */
    public function compile(CommandInterface $command, $development = false)
    {
        /**
         * Get global namespace
         */
        $namespace = str_replace('\\', '_', $this->checkDirectory());
        $extensionName  = $this->config->get('extension-name');
        if (empty($extensionName) || !is_string($extensionName)) {
            $extensionName = $namespace;
        }
        $needConfigure = $this->generate($command);

        if ($needConfigure) {
            if (Utils::isWindows()) {
                exec('cd ext && %PHP_DEVPACK%\\phpize --clean', $output, $exit);

                $releaseFolder = Utils::resolveWindowsReleaseFolder();
                if (file_exists($releaseFolder)) {
                    exec('rd /s /q ' . $releaseFolder, $output, $exit);
                }
                $this->logger->output('Preparing for PHP compilation...');
                exec('cd ext && %PHP_DEVPACK%\\phpize', $output, $exit);

                /* fix until https://github.com/php/php-src/commit/9a3af83ee2aecff25fd4922ef67c1fb4d2af6201 hits all supported PHP builds */
                $fixMarker = "/* zephir_phpize_fix */";

                $configureFile = file_get_contents('ext\\configure.js');
                $configureFix = ["var PHP_ANALYZER = 'disabled';", "var PHP_PGO = 'no';", "var PHP_PGI = 'no';"];
                $hasChanged = false;

                if (strpos($configureFile, $fixMarker) === false) {
                    $configureFile = $fixMarker . PHP_EOL . implode($configureFix, PHP_EOL) . PHP_EOL . $configureFile;
                    $hasChanged = true;
                }

                /* fix php's broken phpize pathing ... */
                $marker = 'var build_dir = (dirname ? dirname : "").replace(new RegExp("^..\\\\\\\\"), "");';
                $pos = strpos($configureFile, $marker);
                if ($pos !== false) {
                    $spMarker = "if (MODE_PHPIZE) {";
                    $sp = strpos($configureFile, $spMarker, $pos - 200);
                    if ($sp === false) {
                        throw new CompilerException("outofdate... phpize seems broken again");
                    }
                    $configureFile = substr($configureFile, 0, $sp) . 'if (false) {' . substr($configureFile, $sp + strlen($spMarker));
                    $hasChanged = true;
                }

                if ($hasChanged) {
                    file_put_contents('ext\\configure.js', $configureFile);
                }

                $this->logger->output('Preparing configuration file...');
                exec('cd ext && configure --enable-' . $extensionName);
            } else {
                exec('cd ext && make clean && phpize --clean', $output, $exit);

                $this->logger->output('Preparing for PHP compilation...');
                exec('cd ext && phpize', $output, $exit);

                $this->logger->output('Preparing configuration file...');

                $gccFlags = $this->getGccFlags($development);

                exec(
                    'cd ext && export CC="gcc" && export CFLAGS="' .
                    $gccFlags .
                    '" && ./configure --enable-' .
                    $extensionName
                );
            }
        }

        $currentDir = getcwd();
        $this->logger->output('Compiling...');
        if (Utils::isWindows()) {
            exec(
                'cd ext && nmake 2>' . $currentDir . '\compile-errors.log 1>' .
                $currentDir . '\compile.log',
                $output,
                $exit
            );
        } else {
            $this->preCompileHeaders();
            exec(
                'cd ext && (make -s -j2 2>' . $currentDir . '/compile-errors.log 1>' .
                $currentDir .
                '/compile.log)',
                $output,
                $exit
            );
        }
    }

    /**
     * Generate a HTML API
     *
     * @param CommandInterface $command
     * @param bool             $fromGenerate
     */
    public function api(CommandInterface $command, $fromGenerate = false)
    {
        if (!$fromGenerate) {
            $this->generate($command);
        }

        $documentator = new Documentation($this->files, $this->config, $this->logger, $command);
        $this->logger->output('Generating API into ' . $documentator->getOutputDirectory());
        $documentator->build();
    }

    /**
     * Generate IDE stubs
     *
     * @param CommandInterface $command
     * @param bool             $fromGenerate
     */
    public function stubs(CommandInterface $command, $fromGenerate = false)
    {
        if (!$fromGenerate) {
            $this->generate($command);
        }

        $this->logger->output('Generating stubs...');
        $stubsGenerator = new Stubs\Generator($this->files, $this->config);

        $path = $this->config->get('path', 'stubs');
        $path = str_replace('%version%', $this->config->get('version'), $path);
        $path = str_replace('%namespace%', ucfirst($this->config->get('namespace')), $path);

        $stubsGenerator->generate($path);
    }


    /**
     * Compiles and installs the extension
     *
     * @param CommandInterface $command
     * @param boolean $development
     *
     * @throws CompilerException
     */
    public function install(CommandInterface $command, $development = false)
    {
        /**
         * Get global namespace
         */
        $namespace = str_replace('\\', '_', $this->checkDirectory());

        @unlink("compile.log");
        @unlink("compile-errors.log");
        @unlink("ext/modules/" . $namespace . ".so");

        $this->compile($command, $development);
        if (Utils::isWindows()) {
            $this->logger->output("Installation is not implemented for windows yet! Aborting!");
            exit();
        }

        $this->logger->output('Installing...');

        $gccFlags = $this->getGccFlags($development);

        $currentDir = getcwd();
        exec(
            '(cd ext && export CC="gcc" && export CFLAGS="' . $gccFlags . '" && ' .
            'make 2>>"' . $currentDir . '/compile-errors.log" 1>>"' . $currentDir . '/compile.log" && ' .
            'sudo make install 2>>"' . $currentDir . '/compile-errors.log" 1>>"' . $currentDir . '/compile.log")',
            $output,
            $exit
        );

        $fileName = $this->getConfig()->get('extension-name') ?: $namespace;

        if (!file_exists("ext/modules/" . $fileName . ".so")) {
            throw new CompilerException(
                "Internal extension compilation failed. Check compile-errors.log for more information"
            );
        }

        $this->logger->output('Extension installed!');
        if (!extension_loaded($namespace)) {
            $this->logger->output('Add extension=' . $namespace . '.so to your php.ini');
        }
        $this->logger->output('Don\'t forget to restart your web server');
    }

    /**
     * Run tests
     *
     * @param CommandInterface $command
     */
    public function test(CommandInterface $command)
    {
        /**
         * Get global namespace
         */
        $namespace = $this->checkDirectory();

        $this->logger->output('Running tests...');

        if (!Utils::isWindows()) {
            system(
                'export CC="gcc" && export CFLAGS="-O0 -g" && export NO_INTERACTION=1 && cd ext && make test',
                $exit
            );
        }
    }

    /**
     * Clean the extension directory
     *
     * @param CommandInterface $command
     */
    public function clean(CommandInterface $command)
    {
        $this->fileSystem->clean();
        if (Utils::isWindows()) {
            system('cd ext && nmake clean-all');
        } else {
            system('cd ext && make clean > /dev/null');
        }
    }

    /**
     * Clean the extension directory
     *
     * @param CommandInterface $command
     */
    public function fullClean(CommandInterface $command)
    {
        $this->fileSystem->clean();
        if (Utils::isWindows()) {
            system('cd ext && nmake clean-all');
            system('cd ext && phpize --clean');
            system('cd ext && ./clean');
        } else {
            system('cd ext && make clean > /dev/null');
            system('cd ext && phpize --clean > /dev/null');
            system('cd ext && ./clean > /dev/null');
        }
    }

    /**
     * Compiles and installs the extension
     *
     * @param CommandInterface $command
     */
    public function build(CommandInterface $command)
    {
        $this->install($command, false);
    }

    /**
     * Compiles and installs the extension in development mode (debug symbols and no optimizations)
     *
     * @param CommandInterface $command
     */
    public function buildDev(CommandInterface $command)
    {
        $this->install($command, true);
    }

    /**
     * Process config.w32 sections
     *
     * @param array $sources
     * @param string $project
     * @return array
     */
    protected function processAddSources($sources, $project)
    {
        $groupSources = array();
        foreach ($sources as $source) {
            $dirName = str_replace(DIRECTORY_SEPARATOR, '/', dirname($source));
            if (!isset($groupSources[$dirName])) {
                $groupSources[$dirName] = array();
            }
            $groupSources[$dirName][] = basename($source);
        }
        $groups = array();
        foreach ($groupSources as $dirname => $files) {
            $groups[] = 'ADD_SOURCES(configure_module_dirname + "/' . $dirname . '", "' .
                        join(' ', $files) . '", "' . $project . '");';
        }
        return $groups;
    }

    /**
     * Create config.m4 and config.w32 for the extension
     *
     * @param string $project
     *
     * @throws Exception
     * @return bool true if need to run configure
     * TODO: move this to backend?
     */
    public function createConfigFiles($project)
    {
        $contentM4 = $this->backend->getTemplateFileContents('config.m4');
        if (empty($contentM4)) {
            throw new Exception("Template config.m4 doesn't exist");
        }

        $contentW32 = $this->backend->getTemplateFileContents('config.w32');
        if (empty($contentW32)) {
            throw new Exception("Template config.w32 doesn't exist");
        }

        if ($project == 'zend') {
            $safeProject = 'zend_';
        } else {
            $safeProject = $project;
        }

        $compiledFiles = array_map(function ($file) {
            return str_replace('.c', '.zep.c', $file);
        }, $this->compiledFiles);

        /**
         * If export-classes is enabled all headers are copied to include/php/ext
         */
        $exportClasses = $this->config->get('export-classes', 'extra');
        if ($exportClasses) {
            $compiledHeaders = array_map(function ($file) {
                return str_replace('.c', '.zep.h', $file);
            }, $this->compiledFiles);
        } else {
            $compiledHeaders = array('php_' . strtoupper($project) . '.h');
        }

        /*
         * Check extra-libs, extra-cflags, package-dependencies exists
         */
        $extraLibs = $this->config->get('extra-libs');
        $extraCflags = $this->config->get('extra-cflags');
        $contentM4 = $this->generatePackageDependenciesM4($contentM4);

        /**
         * Generate config.m4
         */
        $toReplace = array(
            '%PROJECT_LOWER_SAFE%'   => strtolower($safeProject),
            '%PROJECT_LOWER%'        => strtolower($project),
            '%PROJECT_UPPER%'        => strtoupper($project),
            '%PROJECT_CAMELIZE%'     => ucfirst($project),
            '%FILES_COMPILED%'       => implode("\n\t", $compiledFiles),
            '%HEADERS_COMPILED%'     => implode(" ", $compiledHeaders),
            '%EXTRA_FILES_COMPILED%' => implode("\n\t", $this->extraFiles),
            '%PROJECT_EXTRA_LIBS%' => $extraLibs,
            '%PROJECT_EXTRA_CFLAGS%' => $extraCflags,
        );

        foreach ($toReplace as $mark => $replace) {
            $contentM4 = str_replace($mark, $replace, $contentM4);
        }

        $needConfigure = Utils::checkAndWriteIfNeeded($contentM4, 'ext/config.m4');

        /**
         * Generate config.w32
         */
        $toReplace = array(
            '%PROJECT_LOWER_SAFE%'   => strtolower($safeProject),
            '%PROJECT_LOWER%'        => strtolower($project),
            '%PROJECT_UPPER%'        => strtoupper($project),
            '%FILES_COMPILED%'       => implode(
                "\r\n\t",
                $this->processAddSources($compiledFiles, strtolower($project))
            ),
            '%EXTRA_FILES_COMPILED%' => implode(
                "\r\n\t",
                $this->processAddSources($this->extraFiles, strtolower($project))
            ),
        );

        foreach ($toReplace as $mark => $replace) {
            $contentW32 = str_replace($mark, $replace, $contentW32);
        }

        $needConfigure = Utils::checkAndWriteIfNeeded($contentW32, 'ext/config.w32');

        /**
         * php_ext.h
         */
        $content = $this->backend->getTemplateFileContents('php_ext.h');
        if (empty($content)) {
            throw new Exception("Template php_ext.h doesn't exist");
        }

        $toReplace = array(
            '%PROJECT_LOWER_SAFE%' => strtolower($safeProject)
        );

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        Utils::checkAndWriteIfNeeded($content, 'ext/php_ext.h');

        /**
         * ext.h
         */
        $content = $this->backend->getTemplateFileContents('ext.h');
        if (empty($content)) {
            throw new Exception("Template ext.h doesn't exist");
        }

        $toReplace = array(
            '%PROJECT_LOWER_SAFE%' => strtolower($safeProject)
        );

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        Utils::checkAndWriteIfNeeded($content, 'ext/ext.h');

        /**
         * ext_config.h
         */
        $content = $this->backend->getTemplateFileContents('ext_config.h');
        if (empty($content)) {
            throw new Exception("Template ext_config.h doesn't exist");
        }

        $toReplace = array(
            '%PROJECT_LOWER%' => strtolower($project)
        );

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        Utils::checkAndWriteIfNeeded($content, 'ext/ext_config.h');

        /**
         * ext_clean
         */
        $content = $this->backend->getTemplateFileContents('clean');
        if (empty($content)) {
            throw new Exception("Clean file doesn't exist");
        }

        if (Utils::checkAndWriteIfNeeded($content, 'ext/clean')) {
            chmod('ext/clean', 0755);
        }

        /**
         * ext_install
         */
        $content = $this->backend->getTemplateFileContents('install');
        if (empty($content)) {
            throw new Exception("Install file doesn't exist");
        }

        $toReplace = array(
            '%PROJECT_LOWER%' => strtolower($project)
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
     * Process extension globals
     *
     * @param string $namespace
     *
     * @throws Exception
     * @return array
     */
    public function processExtensionGlobals($namespace)
    {
        $globalCode = '';
        $globalStruct = '';
        $globalsDefault = array(array(), array());
        $initEntries = array();

        /**
         * Generate the extensions globals declaration
         */
        $globals = $this->config->get('globals');
        if (is_array($globals)) {
            $structures = array();
            $variables = array();
            foreach ($globals as $name => $global) {
                $parts = explode('.', $name);
                if (isset($parts[1])) {
                    $structures[$parts[0]][$parts[1]] = $global;
                } else {
                    $variables[$parts[0]] = $global;
                }
            }

            /**
             * Process compound structures
             */
            foreach ($structures as $structureName => $internalStructure) {
                if (preg_match('/^[0-9a-zA-Z\_]$/', $structureName)) {
                    throw new Exception("Struct name: '" . $structureName . "' contains invalid characters");
                }

                $structBuilder = new Code\Builder\Struct('_zephir_struct_' . $structureName, $structureName);
                foreach ($internalStructure as $field => $global) {
                    if (preg_match('/^[0-9a-zA-Z\_]$/', $field)) {
                        throw new Exception("Struct field name: '" . $field . "' contains invalid characters");
                    }

                    $structBuilder->addProperty($field, $global);

                    $isModuleGlobal = (int) !empty($global['module']);
                    $globalsDefault[$isModuleGlobal][] = $structBuilder->getCDefault($field, $global, $namespace) . PHP_EOL;
                    $initEntries[] = $structBuilder->getInitEntry($field, $global, $namespace);
                }

                $globalStruct .= $structBuilder . PHP_EOL;
            }

            $globalCode = PHP_EOL;
            foreach ($structures as $structureName => $internalStructure) {
                $globalCode .= "\t" . 'zephir_struct_' . $structureName . ' ' .
                                $structureName . ';' . PHP_EOL . PHP_EOL;
            }
            /**
             * Process single variables
             */
            foreach ($variables as $name => $global) {
                if (preg_match('/^[0-9a-zA-Z\_]$/', $name)) {
                    throw new Exception("Extension global variable name: '" . $name . "' contains invalid characters");
                }

                if (!isset($global['default'])) {
                    throw new Exception("Extension global variable name: '" . $name . "' contains invalid characters");
                }

                $isModuleGlobal = (int) !empty($global['module']);
                $type = $global['type'];
                // @todo Add support for 'string', 'hash'
                // @todo Zephir\Optimizers\FunctionCall\GlobalsSetOptimizer
                switch ($global['type']) {
                    case 'boolean':
                    case 'bool':
                        $type = 'zend_bool';
                        if ($global['default'] === true) {
                            $globalsDefault[$isModuleGlobal][] = "\t" . $namespace . '_globals->' . $name . ' = 1;' . PHP_EOL;
                        } else {
                            $globalsDefault[$isModuleGlobal][] = "\t" . $namespace . '_globals->' . $name . ' = 0;' . PHP_EOL;
                        }
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'double':
                        $globalsDefault[$isModuleGlobal][]
                            = "\t" . $namespace . '_globals->' . $name . ' = ' .
                            $global['default'] . ';' . PHP_EOL;
                        break;

                    case 'char':
                    case 'uchar':
                        $globalsDefault[$isModuleGlobal][]
                            = "\t" . $namespace . '_globals->' . $name . ' = \'' .
                            $global['default'] . '\';' . PHP_EOL;
                        break;
                    default:
                        throw new Exception(
                            "Unknown type '" . $global['type'] . "' for extension global '" . $name . "'"
                        );
                }

                $globalCode .= "\t" . $type . ' ' . $name . ';' . PHP_EOL . PHP_EOL;
                $iniEntry = array();
                if (isset($global['ini-entry'])) {
                    $iniEntry = $global['ini-entry'];
                }

                if (!isset($iniEntry['name'])) {
                    $iniName = $name;
                } else {
                    $iniName = $iniEntry['name'];
                }

                if (!isset($iniEntry['scope'])) {
                    $scope = 'PHP_INI_ALL';
                } else {
                    $scope = $iniEntry['scope'];
                }

                switch ($global['type']) {
                    case 'boolean':
                    case 'bool':
                        $initEntries[] =
                        'STD_PHP_INI_BOOLEAN("' .
                        $iniName .
                        '", "' .
                        (int) ($global['default'] === true) .
                        '", ' .
                        $scope .
                        ', OnUpdateBool, ' .
                        $name .
                        ', zend_' .
                        $namespace .
                        '_globals, ' .
                        $namespace . '_globals)';
                        break;
                }
            }
        }
        $globalsDefault[0] = implode('', $globalsDefault[0]);
        $globalsDefault[1] = implode('', $globalsDefault[1]);

        return array($globalCode, $globalStruct, $globalsDefault, $initEntries);
    }

    /**
     * Generates phpinfo() sections showing information about the extension
     *
     * @return string
     */
    public function processExtensionInfo()
    {
        $phpinfo = '';

        $info = $this->config->get('info');
        if (is_array($info)) {
            foreach ($info as $table) {
                $phpinfo .= "\t" . 'php_info_print_table_start();' . PHP_EOL;
                if (isset($table['header'])) {
                    $headerArray = array();
                    foreach ($table['header'] as $header) {
                        $headerArray[] = '"' . htmlentities($header) . '"';
                    }

                    $phpinfo .= "\t" . 'php_info_print_table_header(' . count($headerArray) . ', ' .
                            join(', ', $headerArray) . ');' . PHP_EOL;
                }
                if (isset($table['rows'])) {
                    foreach ($table['rows'] as $row) {
                        $rowArray = array();
                        foreach ($row as $field) {
                            $rowArray[] = '"' . htmlentities($field) . '"';
                        }

                        $phpinfo .= "\t" . 'php_info_print_table_row(' . count($rowArray) . ', ' .
                                join(', ', $rowArray) . ');' . PHP_EOL;
                    }
                }
                $phpinfo .= "\t" . 'php_info_print_table_end();' . PHP_EOL;
            }
        }

        return $phpinfo;
    }

    /**
     * Process extension code injection
     *
     * @param array $entries
     * @return array
     */
    public function processCodeInjection(array $entries, $section = 'request')
    {
        $codes = array();
        $includes = array();

        if (isset($entries[$section])) {
            foreach ($entries[$section] as $entry) {
                if (isset($entry['code']) && !empty($entry['code'])) {
                    $codes[] = $entry['code'] . ';';
                }
                if (isset($entry['include']) && !empty($entry['include'])) {
                    $includes[] = "#include \"" . $entry['include'] . "\"";
                }
            }
        }

        return array(join(PHP_EOL, $includes), join("\n\t", $codes));
    }

    /**
     * Adds an external dependency to the compiler
     *
     * @param string $namespace
     * @param string $location
     */
    public function addExternalDependency($namespace, $location)
    {
        $this->externalDependencies[$namespace] = $location;
    }

    public function calculateDependencies($files, $_dependency = null)
    {
        /**
         * Classes are ordered according to a dependency ranking
         * Classes with higher rank, need to be initialized first
         * We first build a dependency tree and then set the rank accordingly
         */
        if ($_dependency == null) {
            $dependencyTree = array();
            foreach ($files as $file) {
                if (!$file->isExternal()) {
                    $classDefinition = $file->getClassDefinition();
                    $dependencyTree[$classDefinition->getCompleteName()] = $classDefinition->getDependencies();
                }
            }

            /* Make sure the dependencies are loaded first (recursively) */
            foreach ($dependencyTree as $className => $dependencies) {
                foreach ($dependencies as $dependency) {
                    $dependency->increaseDependencyRank(0);
                    $this->calculateDependencies($dependencyTree, $dependency);
                }
            }
            return;
        }

        $dependencyTree = $files;
        if (isset($dependencyTree[$_dependency->getCompleteName()])) {
            foreach ($dependencyTree[$_dependency->getCompleteName()] as $dependency) {
                $dependency->increaseDependencyRank(0);
                $this->calculateDependencies($dependencyTree, $dependency);
            }
        }
    }

    /**
     * Create project.c and project.h according to the current extension
     *
     * @param string $project
     *
     * @throws Exception
     * @return boolean
     * TODO: Move the part of the logic which depends on templates (backend-specific) to backend?
     */
    public function createProjectFiles($project)
    {
        $needConfigure = $this->checkKernelFiles();

        /**
         * project.c
         */
        $content = $this->backend->getTemplateFileContents('project.c');
        if (empty($content)) {
            throw new Exception("Template project.c doesn't exist");
        }

        $includes = '';
        $reqInitializers = '';
        $reqDestructors = '';
        $prqDestructors = '';
        $modInitializers = '';
        $modDestructors = '';
        $glbInitializers = '';
        $glbDestructors = '';
        $files = array_merge($this->files, $this->anonymousFiles);

        /**
         * Round 1. Calculate the dependency rank
         */
        $this->calculateDependencies($files);

        $classEntries = array();
        $classInits = array();

        $interfaceEntries = array();
        $interfaceInits = array();

        /**
         * Round 2. Generate the ZEPHIR_INIT calls according to the dependency rank
         */
        foreach ($files as $file) {
            /** @var \Zephir\Compiler\FileInterface $file */
            if (!$file->isExternal()) {
                $classDefinition = $file->getClassDefinition();
                if ($classDefinition) {
                    $dependencyRank = $classDefinition->getDependencyRank();
                    if ($classDefinition->getType() == 'class') {
                        if (!isset($classInits[$dependencyRank])) {
                            $classEntries[$dependencyRank] = array();
                            $classInits[$dependencyRank] = array();
                        }
                        $classEntries[$dependencyRank][] =
                            'zend_class_entry *' . $classDefinition->getClassEntry() . ';';
                        $classInits[$dependencyRank][] =
                            'ZEPHIR_INIT(' . $classDefinition->getCNamespace() . '_' .
                             $classDefinition->getName() . ');';
                    } else {
                        if (!isset($interfaceInits[$dependencyRank])) {
                            $interfaceEntries[$dependencyRank] = array();
                            $interfaceInits[$dependencyRank] = array();
                        }
                        $interfaceEntries[$dependencyRank][] =
                            'zend_class_entry *' . $classDefinition->getClassEntry() . ';';
                        $interfaceInits[$dependencyRank][] =
                            'ZEPHIR_INIT(' . $classDefinition->getCNamespace() . '_' .
                            $classDefinition->getName() . ');';
                    }
                }
            }
        }

        krsort($classInits);
        krsort($classEntries);
        krsort($interfaceInits);
        krsort($interfaceEntries);

        $completeInterfaceInits = array();
        foreach ($interfaceInits as $rankInterfaceInits) {
            asort($rankInterfaceInits, SORT_STRING);
            $completeInterfaceInits = array_merge($completeInterfaceInits, $rankInterfaceInits);
        }

        $completeInterfaceEntries = array();
        foreach ($interfaceEntries as $rankInterfaceEntries) {
            asort($rankInterfaceEntries, SORT_STRING);
            $completeInterfaceEntries = array_merge($completeInterfaceEntries, $rankInterfaceEntries);
        }

        $completeClassInits = array();
        foreach ($classInits as $rankClassInits) {
            asort($rankClassInits, SORT_STRING);
            $completeClassInits = array_merge($completeClassInits, $rankClassInits);
        }

        $completeClassEntries = array();
        foreach ($classEntries as $rankClassEntries) {
            asort($rankClassEntries, SORT_STRING);
            $completeClassEntries = array_merge($completeClassEntries, $rankClassEntries);
        }

        /**
         * Round 3. Process extension globals
         */
        list($globalCode, $globalStruct, $globalsDefault, $initEntries) = $this->processExtensionGlobals($project);
        if ($project == 'zend') {
            $safeProject = 'zend_';
        } else {
            $safeProject = $project;
        }

        /**
         * Round 4. Process extension info
         */
        $phpInfo = $this->processExtensionInfo();

        /**
         * Round 5. Generate Function entries (FE)
         */
        list($feHeader, $feEntries) = $this->generateFunctionInformation();

        /**
         * Check if there are module/request/global destructors
         */
        $destructors = $this->config->get('destructors');
        if (is_array($destructors)) {
            $invokeRequestDestructors = $this->processCodeInjection($destructors, 'request');
            $includes .= PHP_EOL . $invokeRequestDestructors[0];
            $reqDestructors = $invokeRequestDestructors[1];

            $invokePostRequestDestructors = $this->processCodeInjection($destructors, 'post-request');
            $includes .= PHP_EOL . $invokePostRequestDestructors[0];
            $prqDestructors = $invokePostRequestDestructors[1];

            $invokeModuleDestructors = $this->processCodeInjection($destructors, 'module');
            $includes .= PHP_EOL . $invokeModuleDestructors[0];
            $modDestructors = $invokeModuleDestructors[1];

            $invokeGlobalsDestructors = $this->processCodeInjection($destructors, 'globals');
            $includes .= PHP_EOL . $invokeGlobalsDestructors[0];
            $glbDestructors = $invokeGlobalsDestructors[1];
        }

        /**
         * Check if there are module/request/global initializers
         */
        $initializers = $this->config->get('initializers');
        if (is_array($initializers)) {
            $invokeRequestInitializers = $this->processCodeInjection($initializers, 'request');
            $includes .= PHP_EOL . $invokeRequestInitializers[0];
            $reqInitializers = $invokeRequestInitializers[1];

            $invokeModuleInitializers = $this->processCodeInjection($initializers, 'module');
            $includes .= PHP_EOL . $invokeModuleInitializers[0];
            $modInitializers = $invokeModuleInitializers[1];

            $invokeGlobalsInitializers = $this->processCodeInjection($initializers, 'globals');
            $includes .= PHP_EOL . $invokeGlobalsInitializers[0];
            $glbInitializers = $invokeGlobalsInitializers[1];
        }

        /**
         * Append extra details
         */
        $extraClasses = $this->config->get('extra-classes');
        if (is_array($extraClasses)) {
            foreach ($extraClasses as $value) {
                if (isset($value['init'])) {
                    $completeClassInits[] = 'ZEPHIR_INIT(' . $value['init'] . ')';
                }

                if (isset($value['entry'])) {
                    $completeClassEntries[] = 'zend_class_entry *' . $value['entry'] . ';';
                }
            }
        }

        $toReplace = array(
            '%PROJECT_LOWER_SAFE%'  => strtolower($safeProject),
            '%PROJECT_LOWER%'       => strtolower($project),
            '%PROJECT_UPPER%'       => strtoupper($project),
            '%PROJECT_CAMELIZE%'    => ucfirst($project),
            '%CLASS_ENTRIES%'       => implode(
                PHP_EOL,
                array_merge($completeInterfaceEntries, $completeClassEntries)
            ),
            '%CLASS_INITS%'         => implode(
                PHP_EOL . "\t",
                array_merge($completeInterfaceInits, $completeClassInits)
            ),
            '%INIT_GLOBALS%'        => implode(
                PHP_EOL . "\t",
                array_merge((array)$globalsDefault[0], array($glbInitializers))
            ),
            '%INIT_MODULE_GLOBALS%' => $globalsDefault[1],
            '%DESTROY_GLOBALS%'     => $glbDestructors,
            '%EXTENSION_INFO%'      => $phpInfo,
            '%EXTRA_INCLUDES%'      => implode(
                PHP_EOL,
                array_unique(explode(PHP_EOL, $includes))
            ),
            '%MOD_INITIALIZERS%'    => $modInitializers,
            '%MOD_DESTRUCTORS%'     => $modDestructors,
            '%REQ_INITIALIZERS%'    => implode(
                PHP_EOL . "\t",
                array_merge($this->internalInitializers, array($reqInitializers))
            ),
            '%REQ_DESTRUCTORS%'     => $reqDestructors,
            '%POSTREQ_DESTRUCTORS%' => empty($prqDestructors) ? '' : implode(
                PHP_EOL,
                array(
                    '#define ZEPHIR_POST_REQUEST 1',
                    'static PHP_PRSHUTDOWN_FUNCTION(' . strtolower($project) . ')',
                    '{',
                    "\t" . implode(
                        PHP_EOL . "\t",
                        explode(PHP_EOL, $prqDestructors)
                    ),
                    '}'
                )
            ),
            '%FE_HEADER%'           => $feHeader,
            '%FE_ENTRIES%'          => $feEntries,
            '%PROJECT_INI_ENTRIES%' => implode(PHP_EOL . "\t", $initEntries)
        );
        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        /**
         * Round 5. Generate and place the entry point of the project
         */
        Utils::checkAndWriteIfNeeded($content, 'ext/' . $safeProject . '.c');
        unset($content);

        /**
         * Round 6. Generate the project main header
         */
        $content = $this->backend->getTemplateFileContents('project.h');
        if (empty($content)) {
            throw new Exception("Template project.h doesn't exists");
        }

        $includeHeaders = array();
        foreach ($this->compiledFiles as $file) {
            if ($file) {
                $fileH = str_replace(".c", ".zep.h", $file);
                $include = '#include "' . $fileH . '"';
                $includeHeaders[] = $include;
            }
        }

        /**
         * Append extra headers
         */
        $extraClasses = $this->config->get('extra-classes');
        if (is_array($extraClasses)) {
            foreach ($extraClasses as $value) {
                if (isset($value['header'])) {
                    $include = '#include "' . $value['header'] . '"';
                    $includeHeaders[] = $include;
                }
            }
        }

        $toReplace = array(
            '%INCLUDE_HEADERS%' => implode(PHP_EOL, $includeHeaders)
        );

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        Utils::checkAndWriteIfNeeded($content, 'ext/' . $safeProject . '.h');
        unset($content);

        /**
         * Round 7. Create php_project.h
         */
        $content = $this->backend->getTemplateFileContents('php_project.h');
        if (empty($content)) {
            throw new Exception("Template php_project.h doesn't exist");
        }

        $version = self::getCurrentVersion();

        $toReplace = array(
            '%PROJECT_LOWER_SAFE%'       => strtolower($safeProject),
            '%PROJECT_LOWER%'            => strtolower($project),
            '%PROJECT_UPPER%'            => strtoupper($project),
            '%PROJECT_EXTNAME%'          => strtolower($project),
            '%PROJECT_NAME%'             => utf8_decode($this->config->get('name')),
            '%PROJECT_AUTHOR%'           => utf8_decode($this->config->get('author')),
            '%PROJECT_VERSION%'          => utf8_decode($this->config->get('version')),
            '%PROJECT_DESCRIPTION%'      => utf8_decode($this->config->get('description')),
            '%PROJECT_ZEPVERSION%'       => $version,
            '%EXTENSION_GLOBALS%'        => $globalCode,
            '%EXTENSION_STRUCT_GLOBALS%' => $globalStruct
        );

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        Utils::checkAndWriteIfNeeded($content, 'ext/php_' . $safeProject . '.h');
        unset($content);

        return $needConfigure;
    }

    public function generateFunctionInformation()
    {
        $headerPrinter = new CodePrinter();
        $entryPrinter = new CodePrinter();

        /**
         * Create argument info
         */
        foreach ($this->functionDefinitions as $func) {
            $funcName = $func->getInternalName();
            $argInfoName = 'arginfo_' . strtolower($funcName);

            $headerPrinter->output('PHP_FUNCTION(' . $funcName . ');');
            $parameters = $func->getParameters();
            if ($parameters != null && count($parameters->getParameters())) {
                $headerPrinter->output(
                    'ZEND_BEGIN_ARG_INFO_EX(' . $argInfoName . ', 0, 0, ' .
                    $func->getNumberOfRequiredParameters() . ')'
                );

                foreach ($parameters->getParameters() as $parameter) {
                    switch ($parameter['data-type']) {
                        case 'array':
                            $headerPrinter->output(
                                "\t" . 'ZEND_ARG_ARRAY_INFO(0, ' . $parameter['name'] . ', ' .
                                (isset($parameter['default']) ? 1 : 0) . ')'
                            );
                            break;

                        case 'variable':
                            if (isset($parameter['cast'])) {
                                switch ($parameter['cast']['type']) {
                                    case 'variable':
                                        $compilationContext = $func->getCallGathererPass()->getCompilationContext();
                                        $value = $parameter['cast']['value'];

                                        $headerPrinter->output(
                                            "\t" . 'ZEND_ARG_OBJ_INFO(0, ' .
                                            $parameter['name'] . ', ' .
                                            Utils::escapeClassName($compilationContext->getFullName($value)) . ', ' .
                                            (isset($parameter['default']) ? 1 : 0) . ')'
                                        );
                                        break;

                                    default:
                                        throw new Exception('Unexpected exception');
                                }
                            } else {
                                $headerPrinter->output("\t" . 'ZEND_ARG_INFO(0, ' . $parameter['name'] . ')');
                            }
                            break;

                        default:
                            $headerPrinter->output("\t" . 'ZEND_ARG_INFO(0, ' . $parameter['name'] . ')');
                            break;
                    }
                }
                $headerPrinter->output('ZEND_END_ARG_INFO()');
                $headerPrinter->outputBlankLine();
            }
            /** Generate FE's */
            $paramData = (count($parameters) ? $argInfoName : 'NULL');

            if ($func->isGlobal()) {
                $entryPrinter->output(
                    'ZEND_NAMED_FE(' . $func->getName() . ', ZEND_FN('. $funcName . '), ' . $paramData . ')'
                );
            } else {
                $entryPrinter->output(
                    'ZEND_NS_NAMED_FE("' . str_replace('\\', '\\\\', $func->getNamespace()) . '", '.
                    $func->getName() .
                    ', ZEND_FN('. $funcName . '), ' .
                    $paramData . ')'
                );
            }
        }
        $entryPrinter->output('ZEND_FE_END');

        return array($headerPrinter->getOutput(), $entryPrinter->getOutput());
    }

    /**
     * Check if the project must be phpized again
     *
     * @return boolean
     */
    public function checkIfPhpized()
    {
        return !file_exists('ext/Makefile');
    }

    /**
     * Returns the internal logger
     *
     * @return Logger
     */
    public function getLogger()
    {
        return $this->logger;
    }

    /**
     * Returns the internal config
     *
     * @return Config
     */
    public function getConfig()
    {
        return $this->config;
    }

    /**
     * Returns the internal filesystem handler
     *
     * @return FileSystem
     */
    public function getFileSystem()
    {
        return $this->fileSystem;
    }

    /**
     * Returns a short path
     *
     * @param string $path
     *
     * @return string
     */
    public static function getShortPath($path)
    {
        return str_replace(ZEPHIRPATH . DIRECTORY_SEPARATOR, '', $path);
    }

    /**
     * Returns a short user path
     *
     * @param string $path
     *
     * @return string
     */
    public static function getShortUserPath($path)
    {
        return str_replace('\\', '/', str_replace(getcwd() . DIRECTORY_SEPARATOR, '', $path));
    }

    /**
     * Generate package-dependencies config for m4
     *
     * @param $contentM4
     * @throws Exception
     * @return string
     * TODO: Move the template depending part to backend?
     */
    public function generatePackageDependenciesM4($contentM4)
    {
        $packageDependencies = $this->config->get('package-dependencies');
        if (is_array($packageDependencies)) {
            $pkgconfigM4 = $this->backend->getTemplateFileContents('pkg-config.m4');
            $pkgconfigCheckM4 = $this->backend->getTemplateFileContents('pkg-config-check.m4');
            $extraCFlags = '';

            foreach ($packageDependencies as $pkg => $version) {
                $pkgM4Buf = $pkgconfigCheckM4;

                $operator = '=';
                $operatorCmd = '--exact-version';
                $ar = explode("=", $version);
                if (count($ar) == 1) {
                    if ($version == '*') {
                        $version = '0.0.0';
                        $operator = '>=';
                        $operatorCmd = '--atleast-version';
                    }
                } else {
                    switch ($ar[0]) {
                        case '<':
                            $operator = '<=';
                            $operatorCmd = '--max-version';
                            $version = trim($ar[1]);
                            break;
                        case '>':
                            $operator = '>=';
                            $operatorCmd = '--atleast-version';
                            $version = trim($ar[1]);
                            break;
                        default:
                            $version = trim($ar[1]);
                            break;
                    }
                }

                $toReplace = array(
                    '%PACKAGE_LOWER%'        => strtolower($pkg),
                    '%PACKAGE_UPPER%'        => strtoupper($pkg),
                    '%PACKAGE_REQUESTED_VERSION%'        => $operator . ' ' . $version,
                    '%PACKAGE_PKG_CONFIG_COMPARE_VERSION%'        => $operatorCmd . '=' . $version,
                );

                foreach ($toReplace as $mark => $replace) {
                    $pkgM4Buf = str_replace($mark, $replace, $pkgM4Buf);
                }

                $pkgconfigM4 .= $pkgM4Buf;
                $extraCFlags .= '$PHP_' . strtoupper($pkg) . '_INCS ';
            }
            $contentM4 = str_replace('%PROJECT_EXTRA_CFLAGS%', '%PROJECT_EXTRA_CFLAGS% '.$extraCFlags, $contentM4);

            $contentM4 = str_replace('%PROJECT_PACKAGE_DEPENDENCIES%', $pkgconfigM4, $contentM4);

            return $contentM4;
        }

        $contentM4 = str_replace('%PROJECT_PACKAGE_DEPENDENCIES%', '', $contentM4);
        return $contentM4;
    }

    /**
     * Check require extensions orther when build your extension
     */
    protected function checkRequires()
    {
        $extensionRequires = $this->config->get("extensions", "requires");
        if ($extensionRequires) {
            $collectionError = PHP_EOL . "\tCould not load extension : ";
            foreach ($extensionRequires as $key => $value) {
                if (!extension_loaded($value)) {
                    $collectionError .= $value . ", ";
                }
            }

            if ($collectionError != PHP_EOL . "\tCould not load extension : ") {
                $collectionError .= PHP_EOL . "\tYou must add extensions above before build this extension";
                throw new Exception($collectionError);
            }
        }
    }

    /**
     * Checks if a file must be copied
     *
     * @param string $src
     * @param string $dst
     *
     * @return boolean
     */
    protected function checkKernelFile($src, $dst)
    {
        if (preg_match('#kernels/ZendEngine[2-9]/concat\.#', $src)) {
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
     * @throws Exception
     */
    protected function checkKernelFiles()
    {
        $kernelPath = 'ext' . DIRECTORY_SEPARATOR . 'kernel';

        if (!file_exists($kernelPath)) {
            if (!mkdir($kernelPath, 0775, true)) {
                throw new Exception("Cannot create kernel directory: {$kernelPath}");
            }
        }

        $kernelPath = realpath($kernelPath);
        $sourceKernelPath = $this->backend->getInternalKernelPath();

        $configured = $this->recursiveProcess(
            $sourceKernelPath,
            $kernelPath,
            '@.*\.[ch]$@',
            [$this, 'checkKernelFile']
        );

        if (!$configured) {
            $this->logger->output('Cleaning old kernel files...');
            $this->recursiveDeletePath($kernelPath, '@^.*\.[lcho]$@');

            @mkdir($kernelPath);

            $this->logger->output('Copying new kernel files...');
            $this->recursiveProcess($sourceKernelPath, $kernelPath, '@^.*\.[ch]$@');
        }

        return !$configured;
    }

    /**
     * Checks if the current directory is a valid Zephir project
     *
     * @return string
     * @throws Exception
     */
    protected function checkDirectory()
    {
        $namespace = $this->config->get('namespace');
        if (!$namespace) {
            throw new Exception("Extension namespace cannot be loaded");
        }

        if (!is_string($namespace)) {
            throw new Exception("Extension namespace is invalid");
        }

        if (!$this->fileSystem->isInitialized()) {
            $this->fileSystem->initialize();
        }

        $version = self::getCurrentVersion();

        if (!$this->fileSystem->exists($version)) {
            if (!$this->checkIfPhpized()) {
                $this->logger->output(
                    'Zephir version has changed, use "zephir fullclean" to perform a full clean of the project'
                );
            }
            $this->fileSystem->makeDirectory($version);
        }

        return $namespace;
    }

    /**
     * Returns current GCC version
     *
     * @return string
     */
    protected function getGccVersion()
    {
        $version = self::getCurrentVersion();

        if (!Utils::isWindows()) {
            if ($this->fileSystem->exists($version . '/gcc-version')) {
                return $this->fileSystem->read($version . '/gcc-version');
            }

            $this->fileSystem->system('gcc -v', 'stderr', $version . '/gcc-version-temp');
            $lines = $this->fileSystem->file($version . '/gcc-version-temp');

            foreach ($lines as $line) {
                if (strpos($line, 'LLVM') !== false) {
                    $this->fileSystem->write($version . '/gcc-version', '4.8.0');
                    return '4.8.0';
                }
            }

            $lastLine = $lines[count($lines) - 1];
            if (preg_match('/[0-9]+\.[0-9]+\.[0-9]+/', $lastLine, $matches)) {
                $this->fileSystem->write($version . '/gcc-version', $matches[0]);
                return $matches[0];
            }
        }

        return '0.0.0';
    }
}
