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

namespace Zephir;

use Zephir\Commands\CommandInterface;
use Zephir\Commands\CommandGenerate;

/**
 * Compiler
 *
 * Main compiler
 */
class Compiler
{
    const VERSION = '0.4.5a';

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
     * @var \ReflectionClass[]
     */
    protected static $_internalDefinitions = array();

    /**
     * @var boolean
     */
    protected static $_loadedPrototypes = false;

    /**
     * @var array
     */
    protected $_extraFiles = array();

    /**
     * Compiler constructor
     *
     * @param Config $config
     * @param Logger $logger
     */
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

            $className = join('\\', array_map(function ($i) {
                return ucfirst($i);
            }, explode('\\', $className)));

            $this->_files[$className] = new CompilerFile($className, $filePath, $this->_config, $this->_logger);
            $this->_files[$className]->preCompile();

            $this->_definitions[$className] = $this->_files[$className]->getClassDefinition();
        }
    }

    /**
     * @param string $path
     *
     * @throws CompilerException
     */
    protected function _recursivePreCompile($path)
    {
        if (!is_string($path)) {
            throw new CompilerException('Invalid compilation path' . var_export($path, true));
        }

        /**
         * Pre compile all files
         */
        $files = array();
        $iterator = new \RecursiveIteratorIterator(new \RecursiveDirectoryIterator($path), \RecursiveIteratorIterator::SELF_FIRST);

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
            $this->_preCompile($file);
        }
    }

    /**
     * Allows to check if a class is part of the compiled extension
     *
     * @param string $className
     *
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
     *
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
     *
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
     *
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
     *
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
     *
     * @return ClassDefinition
     */
    public function getInternalClassDefinition($className)
    {
        if (!isset(self::$_internalDefinitions[$className])) {
            $reflection = new \ReflectionClass($className);
            self::$_internalDefinitions[$className] = ClassDefinition::buildFromReflection($reflection);
        }

        return self::$_internalDefinitions[$className];
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
    protected function _recursiveProcess($src, $dest, $pattern = null, $callback = "copy")
    {
        $success = true;
        $iterator = new \DirectoryIterator($src);
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
     *
     * @throws Exception
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
     *
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
     *
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
     *
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

        if (!is_string($namespace)) {
            throw new Exception("Extension namespace is invalid");
        }

        if (!is_dir('.temp')) {
            mkdir('.temp');
        }

        if (!file_exists('.temp/' . self::VERSION)) {
            if (file_exists('ext/Makefile')) {
                $this->_logger->output('Zephir version has changed, use "zephir fullclean" to perform a full clean of the project');
            }
            mkdir('.temp/' . self::VERSION);
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
        if (file_exists('.temp/' . self::VERSION . '/gcc-version')) {
            return file_get_contents('.temp/' . self::VERSION . '/gcc-version');
        }

        system('gcc -v 2> .temp/' . self::VERSION . '/gcc-version-temp');
        $lines = file('.temp/' . self::VERSION . '/gcc-version-temp');
        foreach ($lines as $line) {
            if (strpos($line, 'LLVM') !== false) {
                file_put_contents('.temp/' . self::VERSION . '/gcc-version', '4.8.0');

                return '4.8.0';
            }
        }

        $lastLine = $lines[count($lines) - 1];
        if (preg_match('/[0-9]+\.[0-9]+\.[0-9]+/', $lastLine, $matches)) {
            file_put_contents('.temp/' . self::VERSION . '/gcc-version', $matches[0]);

            return $matches[0];
        }

        return '0.0.0';
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
            $this->_logger->output('This extension can have conflicts with an existing loaded extension');
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
     * @param CommandInterface $command
     *
     * @return bool
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
        $this->_recursivePreCompile(str_replace('\\', DIRECTORY_SEPARATOR, $namespace));
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
                 * @var $file \DirectoryIterator
                 */
                foreach (new \DirectoryIterator(ZEPHIRPATH . 'prototypes') as $file) {
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

            $methods = array();
            $classDefinition = $compileFile->getClassDefinition();
            foreach ($classDefinition->getMethods() as $method) {
                $methods[] = '[' . $method->getName() . ':' . join('-', $method->getVisibility()) . ']';
            }

            $files[] = $compiledFile;

            $hash .= '|' . $compiledFile . ':' . $classDefinition->getClassEntry() . '[' . join('|', $methods) . ']';
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
        $namespace = str_replace('\\', '_', $namespace);
        $needConfigure = $this->createConfigFiles($namespace);
        $needConfigure |= $this->createProjectFiles($namespace);
        $needConfigure |= $this->checkIfPhpized($namespace);

        /**
         * When a new file is added or removed we need to run configure again
         */
        if (!($command instanceof CommandGenerate)) {
            if (!file_exists('.temp/' . self::VERSION . '/compiled-files-sum')) {
                $needConfigure = true;
                file_put_contents('.temp/' . self::VERSION . '/compiled-files-sum', $hash);
            } else {
                if (file_get_contents('.temp/' . self::VERSION . '/compiled-files-sum') != $hash) {
                    $needConfigure = true;
                    file_put_contents('.temp/' . self::VERSION . '/compiled-files-sum', $hash);
                }
            }
        }

        /**
         * Round 5. Generate the concatenation
         */
        $this->_stringManager->genConcatCode();

        if ($this->_config->get('stubs-run-after-generate', 'stubs')) {
            $this->stubs($command, true);
        }

        return $needConfigure;
    }

    /**
     * Compiles the extension without installing it
     *
     * @param CommandInterface $command
     */
    public function compile(CommandInterface $command)
    {
        /**
         * Get global namespace
         */
        $namespace = str_replace('\\', '_', $this->_checkDirectory());
        $needConfigure = $this->generate($command);
        if ($needConfigure) {

            exec('cd ext && make clean && phpize --clean', $output, $exit);

            $this->_logger->output('Preparing for PHP compilation...');
            exec('cd ext && phpize', $output, $exit);

            $this->_logger->output('Preparing configuration file...');

            $gccFlags = getenv('CFLAGS');
            if (!is_string($gccFlags)) {
                $gccVersion = $this->getGccVersion();
                if (version_compare($gccVersion, '4.6.0', '>=')) {
                    $gccFlags = '-O2 -fvisibility=hidden -Wparentheses -flto';
                } else {
                    $gccFlags = '-O2 -fvisibility=hidden -Wparentheses';
                }
            }

            exec(
                'cd ext && export CC="gcc" && export CFLAGS="' . $gccFlags . '" && ./configure --enable-' . $namespace
            );
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
     * Generate ide stubs
     *
     * @param CommandInterface $command
     * @param bool             $fromGenerate
     */
    public function stubs(CommandInterface $command, $fromGenerate = false)
    {
        if (!$fromGenerate) {
            $this->generate($command);
        }
        $this->_logger->output('Generating stubs...');
        $stubsGenerator = new Stubs\Generator($this->_files, $this->_config);
        $path = $this->_config->get('path', 'stubs');
        $path = str_replace('%version%', $this->_config->get('version'), $path);
        $path = str_replace('%namespace%', ucfirst($this->_config->get('namespace')), $path);

        $stubsGenerator->generate($path);
    }

    /**
     * Compiles and installs the extension
     *
     * @param CommandInterface $command
     *
     * @throws Exception
     */
    public function install(CommandInterface $command)
    {
        /**
         * Get global namespace
         */
        $namespace = str_replace('\\', '_', $this->_checkDirectory());

        $this->compile($command);

        $this->_logger->output('Installing...');

        $gccFlags = getenv('CFLAGS');
        if (!is_string($gccFlags)) {
            $gccVersion = $this->getGccVersion();
            if (version_compare($gccVersion, '4.6.0', '>=')) {
                $gccFlags = '-O2 -fvisibility=hidden -Wparentheses -flto';
            } else {
                $gccFlags = '-O2 -fvisibility=hidden -Wparentheses';
            }
        }

        exec('(cd ext && export CC="gcc" && export CFLAGS="' . $gccFlags . '" && sudo make --silent install) > /dev/null 2>&1', $output, $exit);

        $this->_logger->output('Extension installed!');
        if (!extension_loaded($namespace)) {
            $this->_logger->output('Add extension=' . $namespace . '.so to your php.ini');
        }
        $this->_logger->output('Don\'t forget to restart your web server');
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
        system('rm -fr .temp');
        system('cd ext && sudo make clean 1> /dev/null');
        system('cd ext && sudo phpize --clean 1> /dev/null');
    }

    /**
     * Compiles and installs the extension
     *
     * @param CommandInterface $command
     */
    public function build(CommandInterface $command)
    {
        $this->install($command);
    }

    /**
     * Checks if a file must be copied
     *
     * @param string $src
     * @param string $dst
     *
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
     *
     * @throws Exception
     * @return bool true if need to run configure
     */
    public function createConfigFiles($project)
    {
        $content = file_get_contents(__DIR__ . '/../templates/config.m4');
        if (empty($content)) {
            throw new Exception("Template config.m4 doesn't exist");
        }
        $compiledFiles = array_map(function ($file) {
            return str_replace('.c', '.zep.c', $file);
        }, $this->_compiledFiles);

        $toReplace = array(
            '%PROJECT_LOWER%'        => strtolower($project),
            '%PROJECT_UPPER%'        => strtoupper($project),
            '%PROJECT_CAMELIZE%'     => ucfirst($project),
            '%FILES_COMPILED%'       => implode("\n\t", $compiledFiles),
            '%EXTRA_FILES_COMPILED%' => implode("\n\t", $this->_extraFiles),
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
            '%PROJECT_LOWER%' => strtolower($project)
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
            '%PROJECT_LOWER%' => strtolower($project)
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
            '%PROJECT_LOWER%' => strtolower($project)
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
        $globalsDefault = '';
        $initEntries = '';

        /**
         * Generate the extensions globals declaration
         */
        $globals = $this->_config->get('globals');
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

                    $globalsDefault .= $structBuilder->getCDefault($field, $global) . PHP_EOL;
                }

                $globalStruct .= $structBuilder . PHP_EOL;
            }

            $globalCode = PHP_EOL;
            foreach ($structures as $structureName => $internalStructure) {
                $globalCode .= "\t" . 'zephir_struct_' . $structureName . ' ' . $structureName . ';' . PHP_EOL . PHP_EOL;
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
                $type = $global['type'];
                switch ($global['type']) {
                    case 'boolean':
                    case 'bool':
                        $type = 'zend_bool';
                        if ($global['default'] === true) {
                            $globalsDefault .= "\t" . 'zephir_globals->' . $name . ' = 1;' . PHP_EOL;
                        } else {
                            $globalsDefault .= "\t" . 'zephir_globals->' . $name . ' = 0;' . PHP_EOL;
                        }
                        break;
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'double':
                        $globalsDefault
                            .= "\t" . 'zephir_globals->' . $name . ' = ' . $global['default'] . ';' . PHP_EOL;
                        break;
                    case 'char':
                    case 'uchar':
                        $globalsDefault
                            .= "\t" . 'zephir_globals->' . $name . ' = \'' . $global['default'] . '\'";' . PHP_EOL;
                        break;
                    default:
                        throw new Exception("Unknown type '" . $global['type'] . "'");
                }

                $globalCode .= "\t" . $type . ' ' . $name . ';' . PHP_EOL . PHP_EOL;

                if (isset($global['ini-entry'])) {

                    $iniEntry = $global['ini-entry'];

                    if (!isset($iniEntry['name'])) {
                        $iniName = $iniEntry['name'];
                    } else {
                        $iniName = $iniEntry['name'];
                    }

                    if (!isset($iniEntry['scope'])) {
                        $scope = $iniEntry['scope'];
                    } else {
                        $scope = $iniEntry['scope'];
                    }

                    switch ($global['type']) {

                        case 'boolean':
                        case 'bool':
                            if ($global['default'] === true) {
                                $initEntries .= 'STD_PHP_INI_BOOLEAN("' . $iniName . '", "1", ' . $scope . ', OnUpdateBool, ' . $name . ', zend_' . $namespace . '_globals, ' . $namespace . '_globals)';
                            } else {
                                $initEntries .= 'STD_PHP_INI_BOOLEAN("' . $iniName . '", "0", ' . $scope . ', OnUpdateBool, ' . $name . ', zend_' . $namespace . '_globals, ' . $namespace . '_globals)';
                            }
                            break;
                    }
                }
            }
        }

        return array($globalCode, $globalStruct, $globalsDefault);
    }

    /**
     * Generates phpinfo() sections showing information about the extension
     */
    public function processExtensionInfo()
    {
        $phpinfo = '';

        $info = $this->_config->get('info');
        if (is_array($info)) {
            foreach ($info as $table) {
                $phpinfo .= "\t" . 'php_info_print_table_start();' . PHP_EOL;
                if (isset($table['header'])) {
                    $headerArray = array();
                    foreach ($table['header'] as $header) {
                        $headerArray[] = '"' . htmlentities($header) . '"';
                    }
                    $phpinfo .= "\t" . 'php_info_print_table_header(' . count($headerArray) . ', ' . join(', ', $headerArray) . ');' . PHP_EOL;
                }
                if (isset($table['rows'])) {
                    foreach ($table['rows'] as $row) {
                        $rowArray = array();
                        foreach ($row as $field) {
                            $rowArray[] = '"' . htmlentities($field) . '"';
                        }
                        $phpinfo .= "\t" . 'php_info_print_table_row(' . count($rowArray) . ', ' . join(', ', $rowArray) . ');' . PHP_EOL;
                    }
                }
                $phpinfo .= "\t" . 'php_info_print_table_end();' . PHP_EOL;
            }
        }

        return $phpinfo;
    }

    /**
     * Create project.c and project.h by compiled files to test extension
     *
     * @param string $project
     *
     * @throws Exception
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
        list($globalCode, $globalStruct, $globalsDefault) = $this->processExtensionGlobals($project);

        /**
         * Round 4. Process extension info
         */
        $phpInfo = $this->processExtensionInfo();

        $toReplace = array(
            '%PROJECT_LOWER%'    => strtolower($project),
            '%PROJECT_UPPER%'    => strtoupper($project),
            '%PROJECT_CAMELIZE%' => ucfirst($project),
            '%CLASS_ENTRIES%'    => implode(PHP_EOL, array_merge($completeInterfaceEntries, $completeClassEntries)),
            '%CLASS_INITS%'      => implode(PHP_EOL . "\t", array_merge($completeInterfaceInits, $completeClassInits)),
            '%INIT_GLOBALS%'     => $globalsDefault,
            '%EXTENSION_INFO%'   => $phpInfo
        );
        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        /**
         * Round 5. Generate and place the entry point of the project
         */
        Utils::checkAndWriteIfNeeded($content, 'ext/' . $project . '.c');
        unset($content);

        /**
         * Round 6. Generate the project main header
         */
        $content = file_get_contents(__DIR__ . '/../templates/project.h');
        if (empty($content)) {
            throw new Exception("Template project.h doesn't exists");
        }

        $includeHeaders = array();
        foreach ($this->_compiledFiles as $file) {
            if ($file) {
                $fileH = str_replace(".c", ".zep.h", $file);
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
         * Round 7. Create php_project.h
         */
        $content = file_get_contents(__DIR__ . '/../templates/php_project.h');
        if (empty($content)) {
            throw new Exception('Template php_project.h doesn`t exist');
        }

        $toReplace = array(
            '%PROJECT_LOWER%'            => strtolower($project),
            '%PROJECT_UPPER%'            => strtoupper($project),
            '%PROJECT_EXTNAME%'          => strtolower($project),
            '%PROJECT_NAME%'             => utf8_decode($this->_config->get('name')),
            '%PROJECT_AUTHOR%'           => utf8_decode($this->_config->get('author')),
            '%PROJECT_VERSION%'          => utf8_decode($this->_config->get('version')),
            '%PROJECT_DESCRIPTION%'      => utf8_decode($this->_config->get('description')),
            '%PROJECT_ZEPVERSION%'       => self::VERSION,
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
        return str_replace(getcwd() . DIRECTORY_SEPARATOR, '', $path);
    }
}
