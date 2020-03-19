<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Psr\Log\LoggerAwareTrait;
use Psr\Log\NullLogger;
use Zephir\Compiler\CompilerFileFactory;
use Zephir\Exception\CompilerException;
use Zephir\Exception\IllegalStateException;
use Zephir\Exception\InvalidArgumentException;
use Zephir\Exception\NotImplementedException;
use Zephir\Exception\ParseException;
use Zephir\Exception\RuntimeException;
use Zephir\Fcall\FcallManagerInterface;
use Zephir\FileSystem\FileSystemInterface;

final class Compiler
{
    use LoggerAwareTrait;

    /** @var BaseBackend */
    public $backend;

    /** @var FunctionDefinition[] */
    public $functionDefinitions = [];

    /** @var CompilerFile[] */
    private $files = [];

    /** @var string[] */
    private $anonymousFiles = [];

    /**
     * Additional initializer code.
     * Used for static property initialization.
     *
     * @var array
     */
    private $internalInitializers = [];

    /** @var ClassDefinition[] */
    private $definitions = [];

    /** @var string[] */
    private $compiledFiles = [];

    private $constants = [];

    private $globals = [];

    private $externalDependencies = [];

    /** @var ClassDefinition[] */
    private static $internalDefinitions = [];

    private static $loadedPrototypes = false;

    private $extraFiles = [];

    /** @var Config */
    private $config;

    /** @var Parser\Manager */
    private $parserManager;

    /** @var StringsManager */
    private $stringManager;

    /** @var FcallManagerInterface */
    private $fcallManager;

    /** @var string|null */
    private $prototypesPath;

    /** @var string|null */
    private $optimizersPath;

    /** @var string|null */
    private $templatesPath;

    /** @var FileSystemInterface */
    private $filesystem;

    /** @var CompilerFileFactory */
    private $compilerFileFactory;

    /**
     * Compiler constructor.
     *
     * @param Config              $config
     * @param BaseBackend         $backend
     * @param Parser\Manager      $manager
     * @param FileSystemInterface $filesystem
     * @param CompilerFileFactory $compilerFileFactory
     *
     * @throws RuntimeException
     */
    public function __construct(
        Config $config,
        BaseBackend $backend,
        Parser\Manager $manager,
        FileSystemInterface $filesystem,
        CompilerFileFactory $compilerFileFactory
    ) {
        $this->config = $config;
        $this->backend = $backend;
        $this->parserManager = $manager;
        $this->filesystem = $filesystem;
        $this->compilerFileFactory = $compilerFileFactory;
        $this->logger = new NullLogger();

        $this->stringManager = $this->backend->getStringsManager();
        $this->fcallManager = $this->backend->getFcallManager();

        try {
            $this->assertRequiredExtensionsIsPresent();
        } catch (RuntimeException $e) {
            fwrite(STDERR, trim($e->getMessage()).PHP_EOL);
            exit(1);
        }
    }

    /**
     * @internal
     *
     * @param string $prototypesPath
     */
    public function setPrototypesPath($prototypesPath)
    {
        $this->prototypesPath = $prototypesPath;
    }

    /**
     * Resolves path to the internal prototypes.
     *
     * @return string
     *
     * @throws IllegalStateException in case of absence internal prototypes directory
     */
    private function resolvePrototypesPath()
    {
        $prototypesPath = $this->prototypesPath;

        // fallback
        if (empty($prototypesPath)) {
            $prototypesPath = \dirname(__DIR__).'/prototypes';
        }

        if (!is_dir($prototypesPath) || !is_readable($prototypesPath)) {
            throw new IllegalStateException('Unable to resolve internal prototypes directory.');
        }

        return $prototypesPath;
    }

    /**
     * @internal
     *
     * @param string $optimizersPath
     */
    public function setOptimizersPath($optimizersPath)
    {
        $this->optimizersPath = $optimizersPath;
    }

    /**
     * Resolves path to the internal optimizers.
     *
     * @return string
     *
     * @throws IllegalStateException in case of absence internal optimizers directory
     */
    private function resolveOptimizersPath()
    {
        $optimizersPath = $this->optimizersPath;

        // fallback
        if (empty($optimizersPath)) {
            $optimizersPath = __DIR__.'/Optimizers';
        }

        if (!is_dir($optimizersPath) || !is_readable($optimizersPath)) {
            throw new IllegalStateException('Unable to resolve internal optimizers directory.');
        }

        return $optimizersPath;
    }

    /**
     * @internal
     *
     * @param string $templatesPath
     */
    public function setTemplatesPath($templatesPath)
    {
        $this->templatesPath = $templatesPath;
    }

    /**
     * Gets the Zephir Parser Manager.
     *
     * @deprecated
     *
     * @return Parser\Manager
     */
    public function getParserManager()
    {
        return $this->parserManager;
    }

    /**
     * Adds a function to the function definitions.
     *
     * @param FunctionDefinition $func
     * @param array              $statement
     *
     * @throws CompilerException
     */
    public function addFunction(FunctionDefinition $func, $statement = null)
    {
        $funcName = strtolower($func->getInternalName());
        if (isset($this->functionDefinitions[$funcName])) {
            // @todo Cover by test
            throw new CompilerException(
                "Function '".$func->getCompleteName()."' was defined more than one time",
                $statement
            );
        }

        $this->functionDefinitions[$funcName] = $func;
    }

    /**
     * Loads a class definition in an external dependency.
     *
     * @param string $className
     * @param string $location
     *
     * @return bool
     *
     * @throws CompilerException
     * @throws IllegalStateException
     * @throws ParseException
     */
    public function loadExternalClass($className, $location)
    {
        $filePath = $location.\DIRECTORY_SEPARATOR.
                    strtolower(str_replace('\\', \DIRECTORY_SEPARATOR, $className)).'.zep';

        /**
         * Fix the class name.
         */
        $className = implode('\\', array_map('ucfirst', explode('\\', $className)));

        if (isset($this->files[$className])) {
            return true;
        }

        if (!file_exists($filePath)) {
            return false;
        }

        /** @var CompilerFile|CompilerFileAnonymous $compilerFile */
        $compilerFile = $this->compilerFileFactory->create($className, $filePath);
        $compilerFile->setIsExternal(true);
        $compilerFile->preCompile($this);

        $this->files[$className] = $compilerFile;
        $this->definitions[$className] = $compilerFile->getClassDefinition();

        return true;
    }

    /**
     * Allows to check if a class is part of the compiled extension.
     *
     * @param string $className
     *
     * @return bool
     */
    public function isClass($className)
    {
        foreach ($this->definitions as $key => $value) {
            if (!strcasecmp($key, $className) && 'class' === $value->getType()) {
                return true;
            }
        }

        /*
         * Try to autoload the class from an external dependency
         */
        if (\count($this->externalDependencies)) {
            foreach ($this->externalDependencies as $namespace => $location) {
                if (preg_match('#^'.$namespace.'\\\\#i', $className)) {
                    return $this->loadExternalClass($className, $location);
                }
            }
        }

        return false;
    }

    /**
     * Allows to check if an interface is part of the compiled extension.
     *
     * @param string $className
     *
     * @return bool
     *
     * @throws CompilerException
     * @throws IllegalStateException
     * @throws ParseException
     */
    public function isInterface($className)
    {
        foreach ($this->definitions as $key => $value) {
            if (!strcasecmp($key, $className) && 'interface' === $value->getType()) {
                return true;
            }
        }

        /*
         * Try to autoload the class from an external dependency
         */
        if (\count($this->externalDependencies)) {
            foreach ($this->externalDependencies as $namespace => $location) {
                if (preg_match('#^'.$namespace.'\\\\#i', $className)) {
                    return $this->loadExternalClass($className, $location);
                }
            }
        }

        return false;
    }

    /**
     * Allows to check if a class is part of PHP.
     *
     * @param string $className
     *
     * @return bool
     */
    public function isBundledClass($className)
    {
        return class_exists($className, false);
    }

    /**
     * Allows to check if a interface is part of PHP.
     *
     * @param string $className
     *
     * @return bool
     */
    public function isBundledInterface($className)
    {
        return interface_exists($className, false);
    }

    /**
     * Returns class the class definition from a given class name.
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
     * Inserts an anonymous class definition in the compiler.
     *
     * @param CompilerFileAnonymous $file
     * @param ClassDefinition       $classDefinition
     */
    public function addClassDefinition(CompilerFileAnonymous $file, ClassDefinition $classDefinition)
    {
        $this->definitions[$classDefinition->getCompleteName()] = $classDefinition;
        $this->anonymousFiles[$classDefinition->getCompleteName()] = $file;
    }

    /**
     * Returns class the class definition from a given class name.
     *
     * @param string $className
     *
     * @return ClassDefinition
     *
     * @throws \ReflectionException
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
     * Checks if $name is a Zephir constant.
     *
     * @param string $name
     *
     * @return bool
     */
    public function isConstant($name)
    {
        return isset($this->constants[$name]);
    }

    /**
     * Returns a Zephir Constant by its name.
     *
     * @param string $name
     */
    public function getConstant($name)
    {
        return $this->constants[$name];
    }

    /**
     * Sets extensions globals.
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
     * Checks if a specific extension global is defined.
     *
     * @param string $name
     *
     * @return bool
     */
    public function isExtensionGlobal($name)
    {
        return isset($this->globals[$name]);
    }

    /**
     * Returns a extension global by its name.
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
     * Returns GCC flags for current compilation.
     *
     * @param bool $development
     *
     * @return string
     */
    public function getGccFlags($development = false)
    {
        if (is_windows()) {
            // TODO
            return '';
        }

        $gccFlags = getenv('CFLAGS');

        if (!\is_string($gccFlags)) {
            if (false === $development) {
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

    /**
     * Returns the php include directories returned by php-config.
     *
     * @return string
     */
    public function getPhpIncludeDirs()
    {
        $this->filesystem->system('php-config --includes', 'stdout', 'php-includes');

        return trim($this->filesystem->read('php-includes'));
    }

    /**
     * Pre-compile headers to speed up compilation.
     */
    public function preCompileHeaders()
    {
        if (is_windows()) {
            // TODO: Add Windows support
            return;
        }

        $phpIncludes = $this->getPhpIncludeDirs();

        foreach (new \DirectoryIterator('ext/kernel') as $file) {
            if ($file->isDir()) {
                continue;
            }

            if (preg_match('/\.h$/', $file)) {
                $path = $file->getRealPath();

                $command = sprintf(
                    'cd ext && gcc -c kernel/%s  -I. %s  -o kernel/%s.gch',
                    $file->getBaseName(),
                    $phpIncludes,
                    $file->getBaseName()
                );

                if (!file_exists($path.'.gch') || filemtime($path) > filemtime($path.'.gch')) {
                    $this->filesystem->system($command, 'stdout', 'compile-header');
                }
            }
        }
    }

    /**
     * Generates the C sources from Zephir without compiling them.
     *
     * @param bool $fromGenerate
     *
     * @return bool
     *
     * @throws CompilerException
     * @throws Exception
     * @throws IllegalStateException
     * @throws InvalidArgumentException
     */
    public function generate($fromGenerate = false)
    {
        /*
         * Get global namespace.
         */
        $namespace = $this->checkDirectory();

        /*
         * Check whether there are external dependencies.
         */
        $externalDependencies = $this->config->get('external-dependencies');
        if (\is_array($externalDependencies)) {
            foreach ($externalDependencies as $dependencyNs => $location) {
                if (!file_exists($location)) {
                    throw new CompilerException(
                        sprintf(
                            'Location of dependency "%s" does not exist. Check the config.json for more information.',
                            $dependencyNs
                        )
                    );
                }

                $this->addExternalDependency($dependencyNs, $location);
            }
        }

        /*
         * Round 1. pre-compile all files in memory
         */
        $this->recursivePreCompile(str_replace('\\', \DIRECTORY_SEPARATOR, $namespace));
        if (!\count($this->files)) {
            throw new Exception(
                "Zephir files to compile couldn't be found. Did you add a first class to the extension?"
            );
        }

        /*
         * Round 2. Check 'extends' and 'implements' dependencies
         */
        foreach ($this->files as $compileFile) {
            $compileFile->checkDependencies($this);
        }

        /*
         * Sort the files by dependency ranking.
         */
        $files = [];
        $rankedFiles = [];
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

        /*
         * Convert C-constants into PHP constants.
         */
        $constantsSources = $this->config->get('constants-sources');
        if (\is_array($constantsSources)) {
            $this->loadConstantsSources($constantsSources);
        }

        /*
         * Set extension globals.
         */
        $globals = $this->config->get('globals');
        if (\is_array($globals)) {
            $this->setExtensionGlobals($globals);
        }

        /*
         * Load function optimizers
         */
        if (false === self::$loadedPrototypes) {
            $optimizersPath = $this->resolveOptimizersPath();
            FunctionCall::addOptimizerDir("{$optimizersPath}/FunctionCall");

            $customOptimizersPaths = $this->config->get('optimizer-dirs');
            if (\is_array($customOptimizersPaths)) {
                foreach ($customOptimizersPaths as $directory) {
                    FunctionCall::addOptimizerDir(realpath($directory));
                }
            }

            /*
             * Load additional extension prototypes.
             */
            $prototypesPath = $this->resolvePrototypesPath();
            foreach (new \DirectoryIterator($prototypesPath) as $file) {
                if ($file->isDir() || $file->isDot()) {
                    continue;
                }

                // Do not use $file->getRealPath() because it does not work inside phar
                $realPath = "{$file->getPath()}/{$file->getFilename()}";
                $extension = $file->getBasename(".{$file->getExtension()}");

                if (!\extension_loaded($extension)) {
                    require_once $realPath;
                }
            }

            /**
             * Load customer additional extension prototypes.
             */
            $prototypeDirs = $this->config->get('prototype-dir');
            if (\is_array($prototypeDirs)) {
                foreach ($prototypeDirs as $prototype => $prototypeDir) {
                    /*
                     * Check if the extension is installed
                     */
                    if (!\extension_loaded($prototype)) {
                        $prototypeRealpath = realpath($prototypeDir);
                        if ($prototypeRealpath) {
                            foreach (new \RecursiveDirectoryIterator($prototypeRealpath) as $file) {
                                if ($file->isDir()) {
                                    continue;
                                }

                                require_once $file->getRealPath();
                            }
                        }
                    }
                }
            }

            self::$loadedPrototypes = true;
        }

        /**
         * Round 3. Compile all files to C sources.
         */
        $files = [];

        $hash = '';
        foreach ($this->files as $compileFile) {
            /*
             * Only compile classes in the local extension, ignore external classes
             */
            if (!$compileFile->isExternal()) {
                $compileFile->compile($this, $this->stringManager);
                $compiledFile = $compileFile->getCompiledFile();

                $methods = [];
                $classDefinition = $compileFile->getClassDefinition();
                foreach ($classDefinition->getMethods() as $method) {
                    $methods[] = '['.$method->getName().':'.implode('-', $method->getVisibility()).']';
                    if ($method->isInitializer() && $method->isStatic()) {
                        $this->internalInitializers[] = "\t".$method->getName().'(TSRMLS_C);';
                    }
                }

                $files[] = $compiledFile;

                $hash .= '|'.$compiledFile.':'.$classDefinition->getClassEntry().
                        '['.implode('|', $methods).']';
            }
        }

        /*
         * Round 3.2. Compile anonymous classes
         */
        foreach ($this->anonymousFiles as $compileFile) {
            $compileFile->compile($this, $this->stringManager);
            $compiledFile = $compileFile->getCompiledFile();

            $methods = [];
            $classDefinition = $compileFile->getClassDefinition();
            foreach ($classDefinition->getMethods() as $method) {
                $methods[] = '['.$method->getName().':'.implode('-', $method->getVisibility()).']';
            }

            $files[] = $compiledFile;

            $hash .= '|'.$compiledFile.':'.$classDefinition->getClassEntry().'['.implode('|', $methods).']';
        }

        $hash = md5($hash);
        $this->compiledFiles = $files;

        /**
         * Round 3.3. Load extra C-sources.
         */
        $extraSources = $this->config->get('extra-sources');
        if (\is_array($extraSources)) {
            $this->extraFiles = $extraSources;
        } else {
            $this->extraFiles = [];
        }

        /**
         * Round 3.4. Load extra classes sources.
         */
        $extraClasses = $this->config->get('extra-classes');
        if (\is_array($extraClasses)) {
            foreach ($extraClasses as $value) {
                if (isset($value['source'])) {
                    $this->extraFiles[] = $value['source'];
                }
            }
        }

        /**
         * Round 4. Create config.m4 and config.w32 files / Create project.c and project.h files.
         */
        $namespace = str_replace('\\', '_', $namespace);
        $extensionName = $this->config->get('extension-name');
        if (empty($extensionName) || !\is_string($extensionName)) {
            $extensionName = $namespace;
        }

        $needConfigure = $this->createConfigFiles($extensionName);
        $needConfigure |= $this->createProjectFiles($extensionName);
        $needConfigure |= $this->checkIfPhpized();

        /*
         * When a new file is added or removed we need to run configure again
         */
        if (!$fromGenerate) {
            if (false === $this->filesystem->exists('compiled-files-sum')) {
                $needConfigure = true;
                $this->filesystem->write('compiled-files-sum', $hash);
            } else {
                if ($this->filesystem->read('compiled-files-sum') != $hash) {
                    $needConfigure = true;
                    $this->filesystem->delete('compiled-files-sum');
                    $this->filesystem->write('compiled-files-sum', $hash);
                }
            }
        }

        /*
         * Round 5. Generate concatenation functions
         */
        $this->stringManager->genConcatCode();
        $this->fcallManager->genFcallCode();

        if ($this->config->get('stubs-run-after-generate', 'stubs')) {
            $this->stubs($fromGenerate);
        }

        return $needConfigure;
    }

    /**
     * Compiles the extension without installing it.
     *
     * @param bool $development
     *
     * @throws CompilerException|Exception
     */
    public function compile($development = false)
    {
        /**
         * Get global namespace.
         */
        $namespace = str_replace('\\', '_', $this->checkDirectory());
        $extensionName = $this->config->get('extension-name');
        if (empty($extensionName) || !\is_string($extensionName)) {
            $extensionName = $namespace;
        }

        $needConfigure = $this->generate();

        if ($needConfigure) {
            if (is_windows()) {
                // TODO(klay): Make this better. Looks like it is non standard Env. Var
                exec('cd ext && %PHP_DEVPACK%\\phpize --clean', $output, $exit);

                $releaseFolder = windows_release_dir();
                if (file_exists($releaseFolder)) {
                    exec('rd /s /q '.$releaseFolder, $output, $exit);
                }
                $this->logger->info('Preparing for PHP compilation...');
                // TODO(klay): Make this better. Looks like it is non standard Env. Var
                exec('cd ext && %PHP_DEVPACK%\\phpize', $output, $exit);

                /**
                 * fix until patch hits all supported PHP builds.
                 *
                 * @see https://github.com/php/php-src/commit/9a3af83ee2aecff25fd4922ef67c1fb4d2af6201
                 */
                $fixMarker = '/* zephir_phpize_fix */';

                $configureFile = file_get_contents('ext\\configure.js');
                $configureFix = ["var PHP_ANALYZER = 'disabled';", "var PHP_PGO = 'no';", "var PHP_PGI = 'no';"];
                $hasChanged = false;

                if (false === strpos($configureFile, $fixMarker)) {
                    $configureFile = $fixMarker.PHP_EOL.implode($configureFix, PHP_EOL).PHP_EOL.$configureFile;
                    $hasChanged = true;
                }

                /* fix php's broken phpize patching ... */
                $marker = 'var build_dir = (dirname ? dirname : "").replace(new RegExp("^..\\\\\\\\"), "");';
                $pos = strpos($configureFile, $marker);
                if (false !== $pos) {
                    $spMarker = 'if (MODE_PHPIZE) {';
                    $sp = strpos($configureFile, $spMarker, $pos - 200);
                    if (false === $sp) {
                        throw new CompilerException('outofdate... phpize seems broken again');
                    }
                    $configureFile = substr($configureFile, 0, $sp).
                        'if (false) {'.substr($configureFile, $sp + \strlen($spMarker));
                    $hasChanged = true;
                }

                if ($hasChanged) {
                    file_put_contents('ext\\configure.js', $configureFile);
                }

                $this->logger->info('Preparing configuration file...');
                exec('cd ext && configure --enable-'.$extensionName);
            } else {
                exec('cd ext && make clean && phpize --clean', $output, $exit);

                $this->logger->info('Preparing for PHP compilation...');
                exec('cd ext && phpize', $output, $exit);

                $this->logger->info('Preparing configuration file...');

                $gccFlags = $this->getGccFlags($development);

                exec(
                    'cd ext && export CC="gcc" && export CFLAGS="'.
                    $gccFlags.
                    '" && ./configure --enable-'.
                    $extensionName
                );
            }
        }

        $currentDir = getcwd();
        $this->logger->info('Compiling...');
        if (is_windows()) {
            exec(
                'cd ext && nmake 2>'.$currentDir.'\compile-errors.log 1>'.
                $currentDir.'\compile.log',
                $output,
                $exit
            );
        } else {
            $this->preCompileHeaders();
            exec(
                'cd ext && (make -s -j2 2>'.$currentDir.'/compile-errors.log 1>'.
                $currentDir.
                '/compile.log)',
                $output,
                $exit
            );
        }
    }

    /**
     * Generate a HTML API.
     *
     * @param array $options
     * @param bool  $fromGenerate
     *
     * @throws ConfigException
     * @throws Exception
     */
    public function api(array $options = [], $fromGenerate = false)
    {
        if (!$fromGenerate) {
            $this->generate();
        }

        $templatesPath = $this->templatesPath;
        if (null === $templatesPath) {
            // fallback
            $templatesPath = \dirname(__DIR__).'/templates';
        }

        $documentator = new Documentation($this->files, $this->config, $templatesPath, $options);
        $documentator->setLogger($this->logger);

        $this->logger->info('Generating API into '.$documentator->getOutputDirectory());
        $documentator->build();
    }

    /**
     * Generate IDE stubs.
     *
     * @param bool $fromGenerate
     *
     * @throws Exception
     */
    public function stubs(bool $fromGenerate = false)
    {
        if (!$fromGenerate) {
            $this->generate();
        }

        $this->logger->info('Generating stubs...');
        $stubsGenerator = new Stubs\Generator($this->files);

        $path = str_replace(
            [
                '%version%',
                '%namespace%',
            ],
            [
                $this->config->get('version'),
                ucfirst($this->config->get('namespace')),
            ],
            $this->config->get('path', 'stubs')
        );

        $stubsGenerator->generate(
            $this->config->get('namespace'),
            $path,
            $this->config->get('indent', 'extra'),
            $this->config->get('banner', 'stubs') ?? ''
        );
    }

    /**
     * Compiles and installs the extension.
     *
     * TODO: Move to the separated installer
     *
     * @param bool $development
     *
     * @throws Exception
     * @throws NotImplementedException
     * @throws CompilerException
     */
    public function install($development = false)
    {
        // Get global namespace
        $namespace = str_replace('\\', '_', $this->checkDirectory());
        $currentDir = getcwd();

        if (is_windows()) {
            throw new NotImplementedException('Installation is not implemented for Windows yet. Aborting.');
        }

        if (file_exists("{$currentDir}/compile.log")) {
            unlink("{$currentDir}/compile.log");
        }

        if (file_exists("{$currentDir}/compile-errors.log")) {
            unlink("{$currentDir}/compile-errors.log");
        }

        if (file_exists("{$currentDir}/ext/modules/{$namespace}.so")) {
            unlink("{$currentDir}/ext/modules/{$namespace}.so");
        }

        $this->compile($development);

        $this->logger->info('Installing...');

        $gccFlags = $this->getGccFlags($development);

        $command = strtr(
            // TODO: Sort out with sudo
            'cd ext && export CC="gcc" && export CFLAGS=":cflags" && '.
            'make 2>> ":stderr" 1>> ":stdout" && '.
            'sudo make install 2>> ":stderr" 1>> ":stdout"',
            [
                ':cflags' => $gccFlags,
                ':stderr' => "{$currentDir}/compile-errors.log",
                ':stdout' => "{$currentDir}/compile.log",
            ]
        );

        array_map(function ($entry) {
            if (!empty($entry)) {
                $this->logger->debug(trim($entry));
            }
        }, explode('&&', $command));

        exec($command, $output, $exit);
        $fileName = $this->config->get('extension-name') ?: $namespace;

        if (false === file_exists("{$currentDir}/ext/modules/{$fileName}.so")) {
            throw new CompilerException(
                'Internal extension compilation failed. Check compile-errors.log for more information.'
            );
        }
    }

    /**
     * Create config.m4 and config.w32 for the extension.
     *
     * @param string $project
     *
     * @throws Exception
     *
     * @return bool true if need to run configure
     *              TODO: move this to backend?
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

        if ('zend' == $project) {
            $safeProject = 'zend_';
        } else {
            $safeProject = $project;
        }

        $compiledFiles = array_map(function ($file) {
            return str_replace('.c', '.zep.c', $file);
        }, $this->compiledFiles);

        /**
         * If export-classes is enabled all headers are copied to include/php/ext.
         */
        $exportClasses = $this->config->get('export-classes', 'extra');
        if ($exportClasses) {
            $compiledHeaders = array_map(function ($file) {
                return str_replace('.c', '.zep.h', $file);
            }, $this->compiledFiles);
        } else {
            $compiledHeaders = ['php_'.strtoupper($project).'.h'];
        }

        /*
         * Check extra-libs, extra-cflags, package-dependencies exists
         */
        $extraLibs = $this->config->get('extra-libs');
        $extraCflags = $this->config->get('extra-cflags');
        $contentM4 = $this->generatePackageDependenciesM4($contentM4);

        /**
         * Generate config.m4.
         */
        $toReplace = [
            '%PROJECT_LOWER_SAFE%' => strtolower($safeProject),
            '%PROJECT_LOWER%' => strtolower($project),
            '%PROJECT_UPPER%' => strtoupper($project),
            '%PROJECT_CAMELIZE%' => ucfirst($project),
            '%FILES_COMPILED%' => implode("\n\t", $compiledFiles),
            '%HEADERS_COMPILED%' => implode(' ', $compiledHeaders),
            '%EXTRA_FILES_COMPILED%' => implode("\n\t", $this->extraFiles),
            '%PROJECT_EXTRA_LIBS%' => $extraLibs,
            '%PROJECT_EXTRA_CFLAGS%' => $extraCflags,
        ];

        foreach ($toReplace as $mark => $replace) {
            $contentM4 = str_replace($mark, $replace, $contentM4);
        }

        file_put_contents_ex($contentM4, 'ext/config.m4');

        /**
         * Generate config.w32.
         */
        $toReplace = [
            '%PROJECT_LOWER_SAFE%' => strtolower($safeProject),
            '%PROJECT_LOWER%' => strtolower($project),
            '%PROJECT_UPPER%' => strtoupper($project),
            '%FILES_COMPILED%' => implode(
                "\r\n\t",
                $this->processAddSources($compiledFiles, strtolower($project))
            ),
            '%EXTRA_FILES_COMPILED%' => implode(
                "\r\n\t",
                $this->processAddSources($this->extraFiles, strtolower($project))
            ),
        ];

        foreach ($toReplace as $mark => $replace) {
            $contentW32 = str_replace($mark, $replace, $contentW32);
        }

        $needConfigure = file_put_contents_ex($contentW32, 'ext/config.w32');

        /**
         * php_ext.h.
         */
        $content = $this->backend->getTemplateFileContents('php_ext.h');
        if (empty($content)) {
            throw new Exception("Template php_ext.h doesn't exist");
        }

        $toReplace = [
            '%PROJECT_LOWER_SAFE%' => strtolower($safeProject),
        ];

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        file_put_contents_ex($content, 'ext/php_ext.h');

        /**
         * ext.h.
         */
        $content = $this->backend->getTemplateFileContents('ext.h');
        if (empty($content)) {
            throw new Exception("Template ext.h doesn't exist");
        }

        $toReplace = [
            '%PROJECT_LOWER_SAFE%' => strtolower($safeProject),
        ];

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        file_put_contents_ex($content, 'ext/ext.h');

        /**
         * ext_config.h.
         */
        $content = $this->backend->getTemplateFileContents('ext_config.h');
        if (empty($content)) {
            throw new Exception("Template ext_config.h doesn't exist");
        }

        $toReplace = [
            '%PROJECT_LOWER%' => strtolower($project),
        ];

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        file_put_contents_ex($content, 'ext/ext_config.h');

        /**
         * ext_clean.
         */
        $content = $this->backend->getTemplateFileContents('clean');
        if (empty($content)) {
            throw new Exception("Clean file doesn't exist");
        }

        if (file_put_contents_ex($content, 'ext/clean')) {
            chmod('ext/clean', 0755);
        }

        /**
         * ext_install.
         */
        $content = $this->backend->getTemplateFileContents('install');
        if (empty($content)) {
            throw new Exception("Install file doesn't exist");
        }

        $toReplace = [
            '%PROJECT_LOWER%' => strtolower($project),
        ];

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        if (file_put_contents_ex($content, 'ext/install')) {
            chmod('ext/install', 0755);
        }

        return (bool) $needConfigure;
    }

    /**
     * Process extension globals.
     *
     * @param string $namespace
     *
     * @throws Exception
     *
     * @return array
     */
    public function processExtensionGlobals($namespace)
    {
        $globalCode = '';
        $globalStruct = '';
        $globalsDefault = [[], []];
        $initEntries = [];

        /**
         * Generate the extensions globals declaration.
         */
        $globals = $this->config->get('globals');
        if (\is_array($globals)) {
            $structures = [];
            $variables = [];
            foreach ($globals as $name => $global) {
                $parts = explode('.', $name);
                if (isset($parts[1])) {
                    $structures[$parts[0]][$parts[1]] = $global;
                } else {
                    $variables[$parts[0]] = $global;
                }
            }

            /*
             * Process compound structures
             */
            foreach ($structures as $structureName => $internalStructure) {
                if (preg_match('/^[0-9a-zA-Z\_]$/', $structureName)) {
                    throw new Exception("Struct name: '".$structureName."' contains invalid characters");
                }

                $structBuilder = new Code\Builder\Struct('_zephir_struct_'.$structureName, $structureName);
                foreach ($internalStructure as $field => $global) {
                    if (preg_match('/^[0-9a-zA-Z\_]$/', $field)) {
                        throw new Exception("Struct field name: '".$field."' contains invalid characters");
                    }

                    $structBuilder->addProperty($field, $global);

                    $isModuleGlobal = (int) !empty($global['module']);
                    $globalsDefault[$isModuleGlobal][] = $structBuilder->getCDefault($field, $global, $namespace).PHP_EOL;
                    $initEntries[] = $structBuilder->getInitEntry($field, $global, $namespace);
                }

                $globalStruct .= $structBuilder.PHP_EOL;
            }

            $globalCode = PHP_EOL;
            foreach ($structures as $structureName => $internalStructure) {
                $globalCode .= "\t".'zephir_struct_'.$structureName.' '.
                                $structureName.';'.PHP_EOL.PHP_EOL;
            }
            /*
             * Process single variables
             */
            foreach ($variables as $name => $global) {
                if (preg_match('/^[0-9a-zA-Z\_]$/', $name)) {
                    throw new Exception("Extension global variable name: '".$name."' contains invalid characters");
                }

                if (!isset($global['default'])) {
                    throw new Exception("Extension global variable name: '".$name."' contains invalid characters");
                }

                $isModuleGlobal = (int) !empty($global['module']);
                $type = $global['type'];
                // @todo Add support for 'string', 'hash'
                // @todo Zephir\Optimizers\FunctionCall\GlobalsSetOptimizer
                switch ($global['type']) {
                    case 'boolean':
                    case 'bool':
                        $type = 'zend_bool';
                        if (true === $global['default']) {
                            $globalsDefault[$isModuleGlobal][] = "\t".$namespace.'_globals->'.$name.' = 1;'.PHP_EOL;
                        } else {
                            $globalsDefault[$isModuleGlobal][] = "\t".$namespace.'_globals->'.$name.' = 0;'.PHP_EOL;
                        }
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'double':
                        $globalsDefault[$isModuleGlobal][]
                            = "\t".$namespace.'_globals->'.$name.' = '.
                            $global['default'].';'.PHP_EOL;
                        break;

                    case 'char':
                    case 'uchar':
                        $globalsDefault[$isModuleGlobal][]
                            = "\t".$namespace.'_globals->'.$name.' = \''.
                            $global['default'].'\';'.PHP_EOL;
                        break;
                    default:
                        throw new Exception(
                            "Unknown type '".$global['type']."' for extension global '".$name."'"
                        );
                }

                $globalCode .= "\t".$type.' '.$name.';'.PHP_EOL.PHP_EOL;
                $iniEntry = [];
                if (isset($global['ini-entry'])) {
                    $iniEntry = $global['ini-entry'];
                }

                if (!isset($iniEntry['name'])) {
                    $iniName = $namespace.'.'.$name;
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
                        'STD_PHP_INI_BOOLEAN("'.
                        $iniName.
                        '", "'.
                        (int) (true === $global['default']).
                        '", '.
                        $scope.
                        ', OnUpdateBool, '.
                        $name.
                        ', zend_'.
                        $namespace.
                        '_globals, '.
                        $namespace.'_globals)';
                        break;
                }
            }
        }
        $globalsDefault[0] = implode('', $globalsDefault[0]);
        $globalsDefault[1] = implode('', $globalsDefault[1]);

        return [$globalCode, $globalStruct, $globalsDefault, $initEntries];
    }

    /**
     * Generates phpinfo() sections showing information about the extension.
     *
     * @return string
     */
    public function processExtensionInfo()
    {
        $phpinfo = '';

        $info = $this->config->get('info');
        if (\is_array($info)) {
            foreach ($info as $table) {
                $phpinfo .= "\t".'php_info_print_table_start();'.PHP_EOL;
                if (isset($table['header'])) {
                    $headerArray = [];
                    foreach ($table['header'] as $header) {
                        $headerArray[] = '"'.htmlentities($header).'"';
                    }

                    $phpinfo .= "\t".'php_info_print_table_header('.\count($headerArray).', '.
                            implode(', ', $headerArray).');'.PHP_EOL;
                }
                if (isset($table['rows'])) {
                    foreach ($table['rows'] as $row) {
                        $rowArray = [];
                        foreach ($row as $field) {
                            $rowArray[] = '"'.htmlentities($field).'"';
                        }

                        $phpinfo .= "\t".'php_info_print_table_row('.\count($rowArray).', '.
                                implode(', ', $rowArray).');'.PHP_EOL;
                    }
                }
                $phpinfo .= "\t".'php_info_print_table_end();'.PHP_EOL;
            }
        }

        return $phpinfo;
    }

    /**
     * Process extension code injection.
     *
     * @param array  $entries
     * @param string $section
     *
     * @return array
     */
    public function processCodeInjection(array $entries, $section = 'request')
    {
        $codes = [];
        $includes = [];

        if (isset($entries[$section])) {
            foreach ($entries[$section] as $entry) {
                if (isset($entry['code']) && !empty($entry['code'])) {
                    $codes[] = $entry['code'].';';
                }
                if (isset($entry['include']) && !empty($entry['include'])) {
                    $includes[] = '#include "'.$entry['include'].'"';
                }
            }
        }

        return [implode(PHP_EOL, $includes), implode("\n\t", $codes)];
    }

    /**
     * Adds an external dependency to the compiler.
     *
     * @param string $namespace
     * @param string $location
     */
    public function addExternalDependency($namespace, $location)
    {
        $this->externalDependencies[$namespace] = $location;
    }

    /**
     * @param CompilerFile[] $files
     * @param null           $_dependency
     */
    public function calculateDependencies(array $files, $_dependency = null)
    {
        /*
         * Classes are ordered according to a dependency ranking
         * Classes with higher rank, need to be initialized first
         * We first build a dependency tree and then set the rank accordingly
         */
        if (null == $_dependency) {
            $dependencyTree = [];
            foreach ($files as $file) {
                if (!$file->isExternal()) {
                    $classDefinition = $file->getClassDefinition();
                    $dependencyTree[$classDefinition->getCompleteName()] = $classDefinition->getDependencies();
                }
            }

            // Make sure the dependencies are loaded first (recursively)
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
     * Create project.c and project.h according to the current extension.
     *
     * TODO: Move the part of the logic which depends on templates (backend-specific) to backend?
     *
     * @param string $project
     *
     * @throws Exception
     *
     * @return bool
     */
    public function createProjectFiles($project)
    {
        $needConfigure = $this->checkKernelFiles();

        /**
         * project.c.
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

        /*
         * Round 1. Calculate the dependency rank
         */
        $this->calculateDependencies($files);

        $classEntries = [];
        $classInits = [];

        $interfaceEntries = [];
        $interfaceInits = [];

        /*
         * Round 2. Generate the ZEPHIR_INIT calls according to the dependency rank
         */
        foreach ($files as $file) {
            /** @var \Zephir\Compiler\FileInterface $file */
            if (!$file->isExternal()) {
                $classDefinition = $file->getClassDefinition();
                if ($classDefinition) {
                    $dependencyRank = $classDefinition->getDependencyRank();
                    if ('class' == $classDefinition->getType()) {
                        if (!isset($classInits[$dependencyRank])) {
                            $classEntries[$dependencyRank] = [];
                            $classInits[$dependencyRank] = [];
                        }
                        $classEntries[$dependencyRank][] =
                            'zend_class_entry *'.$classDefinition->getClassEntry().';';
                        $classInits[$dependencyRank][] =
                            'ZEPHIR_INIT('.$classDefinition->getCNamespace().'_'.
                             $classDefinition->getName().');';
                    } else {
                        if (!isset($interfaceInits[$dependencyRank])) {
                            $interfaceEntries[$dependencyRank] = [];
                            $interfaceInits[$dependencyRank] = [];
                        }
                        $interfaceEntries[$dependencyRank][] =
                            'zend_class_entry *'.$classDefinition->getClassEntry().';';
                        $interfaceInits[$dependencyRank][] =
                            'ZEPHIR_INIT('.$classDefinition->getCNamespace().'_'.
                            $classDefinition->getName().');';
                    }
                }
            }
        }

        krsort($classInits);
        krsort($classEntries);
        krsort($interfaceInits);
        krsort($interfaceEntries);

        $completeInterfaceInits = [];
        foreach ($interfaceInits as $rankInterfaceInits) {
            asort($rankInterfaceInits, SORT_STRING);
            $completeInterfaceInits = array_merge($completeInterfaceInits, $rankInterfaceInits);
        }

        $completeInterfaceEntries = [];
        foreach ($interfaceEntries as $rankInterfaceEntries) {
            asort($rankInterfaceEntries, SORT_STRING);
            $completeInterfaceEntries = array_merge($completeInterfaceEntries, $rankInterfaceEntries);
        }

        $completeClassInits = [];
        foreach ($classInits as $rankClassInits) {
            asort($rankClassInits, SORT_STRING);
            $completeClassInits = array_merge($completeClassInits, $rankClassInits);
        }

        $completeClassEntries = [];
        foreach ($classEntries as $rankClassEntries) {
            asort($rankClassEntries, SORT_STRING);
            $completeClassEntries = array_merge($completeClassEntries, $rankClassEntries);
        }

        /*
         * Round 3. Process extension globals
         */
        list($globalCode, $globalStruct, $globalsDefault, $initEntries) = $this->processExtensionGlobals($project);
        if ('zend' == $project) {
            $safeProject = 'zend_';
        } else {
            $safeProject = $project;
        }

        /**
         * Round 4. Process extension info.
         */
        $phpInfo = $this->processExtensionInfo();

        /*
         * Round 5. Generate Function entries (FE)
         */
        list($feHeader, $feEntries) = $this->generateFunctionInformation();

        /**
         * Check if there are module/request/global destructors.
         */
        $destructors = $this->config->get('destructors');
        if (\is_array($destructors)) {
            $invokeRequestDestructors = $this->processCodeInjection($destructors, 'request');
            $includes .= PHP_EOL.$invokeRequestDestructors[0];
            $reqDestructors = $invokeRequestDestructors[1];

            $invokePostRequestDestructors = $this->processCodeInjection($destructors, 'post-request');
            $includes .= PHP_EOL.$invokePostRequestDestructors[0];
            $prqDestructors = $invokePostRequestDestructors[1];

            $invokeModuleDestructors = $this->processCodeInjection($destructors, 'module');
            $includes .= PHP_EOL.$invokeModuleDestructors[0];
            $modDestructors = $invokeModuleDestructors[1];

            $invokeGlobalsDestructors = $this->processCodeInjection($destructors, 'globals');
            $includes .= PHP_EOL.$invokeGlobalsDestructors[0];
            $glbDestructors = $invokeGlobalsDestructors[1];
        }

        /**
         * Check if there are module/request/global initializers.
         */
        $initializers = $this->config->get('initializers');
        if (\is_array($initializers)) {
            $invokeRequestInitializers = $this->processCodeInjection($initializers, 'request');
            $includes .= PHP_EOL.$invokeRequestInitializers[0];
            $reqInitializers = $invokeRequestInitializers[1];

            $invokeModuleInitializers = $this->processCodeInjection($initializers, 'module');
            $includes .= PHP_EOL.$invokeModuleInitializers[0];
            $modInitializers = $invokeModuleInitializers[1];

            $invokeGlobalsInitializers = $this->processCodeInjection($initializers, 'globals');
            $includes .= PHP_EOL.$invokeGlobalsInitializers[0];
            $glbInitializers = $invokeGlobalsInitializers[1];
        }

        /**
         * Append extra details.
         */
        $extraClasses = $this->config->get('extra-classes');
        if (\is_array($extraClasses)) {
            foreach ($extraClasses as $value) {
                if (isset($value['init'])) {
                    $completeClassInits[] = 'ZEPHIR_INIT('.$value['init'].')';
                }

                if (isset($value['entry'])) {
                    $completeClassEntries[] = 'zend_class_entry *'.$value['entry'].';';
                }
            }
        }

        $modRequires = array_map(function ($mod) {
            return sprintf('ZEND_MOD_REQUIRED("%s")', strtolower($mod));
        }, $this->config->get('extensions', 'requires') ?: []);

        $toReplace = [
            '%PROJECT_LOWER_SAFE%' => strtolower($safeProject),
            '%PROJECT_LOWER%' => strtolower($project),
            '%PROJECT_UPPER%' => strtoupper($project),
            '%PROJECT_CAMELIZE%' => ucfirst($project),
            '%CLASS_ENTRIES%' => implode(
                PHP_EOL,
                array_merge($completeInterfaceEntries, $completeClassEntries)
            ),
            '%CLASS_INITS%' => implode(
                PHP_EOL."\t",
                array_merge($completeInterfaceInits, $completeClassInits)
            ),
            '%INIT_GLOBALS%' => implode(
                PHP_EOL."\t",
                array_merge((array) $globalsDefault[0], [$glbInitializers])
            ),
            '%INIT_MODULE_GLOBALS%' => $globalsDefault[1],
            '%DESTROY_GLOBALS%' => $glbDestructors,
            '%EXTENSION_INFO%' => $phpInfo,
            '%EXTRA_INCLUDES%' => implode(
                PHP_EOL,
                array_unique(explode(PHP_EOL, $includes))
            ),
            '%MOD_INITIALIZERS%' => $modInitializers,
            '%MOD_DESTRUCTORS%' => $modDestructors,
            '%REQ_INITIALIZERS%' => implode(
                PHP_EOL."\t",
                array_merge($this->internalInitializers, [$reqInitializers])
            ),
            '%REQ_DESTRUCTORS%' => $reqDestructors,
            '%POSTREQ_DESTRUCTORS%' => empty($prqDestructors) ? '' : implode(
                PHP_EOL,
                [
                    '#define ZEPHIR_POST_REQUEST 1',
                    'static PHP_PRSHUTDOWN_FUNCTION('.strtolower($project).')',
                    '{',
                    "\t".implode(
                        PHP_EOL."\t",
                        explode(PHP_EOL, $prqDestructors)
                    ),
                    '}',
                ]
            ),
            '%FE_HEADER%' => $feHeader,
            '%FE_ENTRIES%' => $feEntries,
            '%PROJECT_INI_ENTRIES%' => implode(PHP_EOL."\t", $initEntries),
            '%PROJECT_DEPENDENCIES%' => implode(PHP_EOL."\t", $modRequires),
        ];
        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        /*
         * Round 5. Generate and place the entry point of the project
         */
        file_put_contents_ex($content, 'ext/'.$safeProject.'.c');
        unset($content);

        /**
         * Round 6. Generate the project main header.
         */
        $content = $this->backend->getTemplateFileContents('project.h');
        if (empty($content)) {
            throw new Exception("Template project.h doesn't exists");
        }

        $includeHeaders = [];
        foreach ($this->compiledFiles as $file) {
            if ($file) {
                $fileH = str_replace('.c', '.zep.h', $file);
                $include = '#include "'.$fileH.'"';
                $includeHeaders[] = $include;
            }
        }

        /**
         * Append extra headers.
         */
        $extraClasses = $this->config->get('extra-classes');
        if (\is_array($extraClasses)) {
            foreach ($extraClasses as $value) {
                if (isset($value['header'])) {
                    $include = '#include "'.$value['header'].'"';
                    $includeHeaders[] = $include;
                }
            }
        }

        $toReplace = [
            '%INCLUDE_HEADERS%' => implode(PHP_EOL, $includeHeaders),
        ];

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        file_put_contents_ex($content, 'ext/'.$safeProject.'.h');
        unset($content);

        /**
         * Round 7. Create php_project.h.
         */
        $content = $this->backend->getTemplateFileContents('php_project.h');
        if (empty($content)) {
            throw new Exception("Template php_project.h doesn't exist");
        }

        $toReplace = [
            '%PROJECT_LOWER_SAFE%' => strtolower($safeProject),
            '%PROJECT_LOWER%' => strtolower($project),
            '%PROJECT_UPPER%' => strtoupper($project),
            '%PROJECT_EXTNAME%' => strtolower($project),
            '%PROJECT_NAME%' => utf8_decode($this->config->get('name')),
            '%PROJECT_AUTHOR%' => utf8_decode($this->config->get('author')),
            '%PROJECT_VERSION%' => utf8_decode($this->config->get('version')),
            '%PROJECT_DESCRIPTION%' => utf8_decode($this->config->get('description')),
            '%PROJECT_ZEPVERSION%' => Zephir::VERSION,
            '%EXTENSION_GLOBALS%' => $globalCode,
            '%EXTENSION_STRUCT_GLOBALS%' => $globalStruct,
        ];

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        file_put_contents_ex($content, 'ext/php_'.$safeProject.'.h');
        unset($content);

        return $needConfigure;
    }

    public function generateFunctionInformation()
    {
        $headerPrinter = new CodePrinter();
        $entryPrinter = new CodePrinter();

        /*
         * Specifying Argument Information
         */
        foreach ($this->functionDefinitions as $func) {
            $argInfo = new ArgInfoDefinition(
                $func->getArgInfoName(),
                $func,
                $headerPrinter,
                $func->getCallGathererPass()->getCompilationContext()
            );

            $funcName = $func->getInternalName();
            $argInfoName = $func->getArgInfoName();

            $headerPrinter->output('PHP_FUNCTION('.$funcName.');');

            $argInfo->setBooleanDefinition($this->backend->isZE3() ? '_IS_BOOL' : 'IS_BOOL');
            $argInfo->setRichFormat($this->backend->isZE3());

            $argInfo->render();

            /** Generate FE's */
            $paramData = 'NULL';

            $richFormat = $this->backend->isZE3() &&
                $func->isReturnTypesHintDetermined() &&
                $func->areReturnTypesCompatible();

            if ($richFormat || $func->hasParameters()) {
                $paramData = $argInfoName;
            }

            if ($func->isGlobal()) {
                $entryPrinter->output(
                    'ZEND_NAMED_FE('.$func->getName().', ZEND_FN('.$funcName.'), '.$paramData.')'
                );
            } else {
                $entryPrinter->output(
                    'ZEND_NS_NAMED_FE("'.str_replace('\\', '\\\\', $func->getNamespace()).'", '.
                    $func->getName().
                    ', ZEND_FN('.$funcName.'), '.
                    $paramData.')'
                );
            }
        }
        $entryPrinter->output('ZEND_FE_END');

        return [$headerPrinter->getOutput(), $entryPrinter->getOutput()];
    }

    /**
     * Check if the project must be phpized again.
     *
     * @return bool
     */
    public function checkIfPhpized()
    {
        return !file_exists('ext/Makefile');
    }

    /**
     * Returns a short user path.
     *
     * @param string $path
     *
     * @return string
     */
    public static function getShortUserPath($path)
    {
        return str_replace('\\', '/', str_replace(getcwd().\DIRECTORY_SEPARATOR, '', $path));
    }

    /**
     * Generate package-dependencies config for m4.
     *
     * TODO: Move the template depending part to backend?
     *
     * @param string $contentM4
     *
     * @return string
     */
    public function generatePackageDependenciesM4($contentM4)
    {
        $packageDependencies = $this->config->get('package-dependencies');
        if (\is_array($packageDependencies)) {
            $pkgconfigM4 = $this->backend->getTemplateFileContents('pkg-config.m4');
            $pkgconfigCheckM4 = $this->backend->getTemplateFileContents('pkg-config-check.m4');
            $extraCFlags = '';

            foreach ($packageDependencies as $pkg => $version) {
                $pkgM4Buf = $pkgconfigCheckM4;

                $operator = '=';
                $operatorCmd = '--exact-version';
                $ar = explode('=', $version);
                if (1 == \count($ar)) {
                    if ('*' == $version) {
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

                $toReplace = [
                    '%PACKAGE_LOWER%' => strtolower($pkg),
                    '%PACKAGE_UPPER%' => strtoupper($pkg),
                    '%PACKAGE_REQUESTED_VERSION%' => $operator.' '.$version,
                    '%PACKAGE_PKG_CONFIG_COMPARE_VERSION%' => $operatorCmd.'='.$version,
                ];

                foreach ($toReplace as $mark => $replace) {
                    $pkgM4Buf = str_replace($mark, $replace, $pkgM4Buf);
                }

                $pkgconfigM4 .= $pkgM4Buf;
                $extraCFlags .= '$PHP_'.strtoupper($pkg).'_INCS ';
            }
            $contentM4 = str_replace('%PROJECT_EXTRA_CFLAGS%', '%PROJECT_EXTRA_CFLAGS% '.$extraCFlags, $contentM4);

            $contentM4 = str_replace('%PROJECT_PACKAGE_DEPENDENCIES%', $pkgconfigM4, $contentM4);

            return $contentM4;
        }

        $contentM4 = str_replace('%PROJECT_PACKAGE_DEPENDENCIES%', '', $contentM4);

        return $contentM4;
    }

    /**
     * Pre-compiles classes creating a CompilerFile definition.
     *
     * @param string $filePath
     *
     * @throws IllegalStateException
     */
    private function preCompile($filePath)
    {
        if (!$this->parserManager->isAvailable()) {
            throw new IllegalStateException($this->parserManager->requirements());
        }

        if (preg_match('#\.zep$#', $filePath)) {
            $className = str_replace(\DIRECTORY_SEPARATOR, '\\', $filePath);
            $className = preg_replace('#.zep$#', '', $className);

            $className = implode('\\', array_map('ucfirst', explode('\\', $className)));

            $compilerFile = $this->compilerFileFactory->create($className, $filePath);
            $compilerFile->preCompile($this);

            $this->files[$className] = $compilerFile;
            $this->definitions[$className] = $compilerFile->getClassDefinition();
        }
    }

    /**
     * Recursively pre-compiles all sources found in the given path.
     *
     * @param string $path
     *
     * @throws IllegalStateException
     * @throws InvalidArgumentException
     */
    private function recursivePreCompile($path)
    {
        if (!is_dir($path)) {
            throw new InvalidArgumentException(
                sprintf(
                    "An invalid path was passed to the compiler. Unable to obtain the '%s%s%s' directory.",
                    getcwd(),
                    \DIRECTORY_SEPARATOR,
                    $path
                )
            );
        }

        /*
         * Pre compile all files.
         */
        $iterator = new \RecursiveIteratorIterator(
            new \RecursiveDirectoryIterator($path),
            \RecursiveIteratorIterator::SELF_FIRST
        );

        $files = [];
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
     * Copies the base kernel to the extension destination.
     *
     * @todo
     *
     * @deprecated
     *
     * @param $src
     * @param $dest
     * @param string $pattern
     * @param mixed  $callback
     *
     * @return bool
     */
    private function recursiveProcess($src, $dest, $pattern = null, $callback = 'copy')
    {
        $success = true;
        $iterator = new \DirectoryIterator($src);
        foreach ($iterator as $item) {
            $pathName = $item->getPathname();
            if (!is_readable($pathName)) {
                $this->logger->warning('File is not readable :'.$pathName);
                continue;
            }

            $fileName = $item->getFileName();

            if ($item->isDir()) {
                if ('.' != $fileName && '..' != $fileName && '.libs' != $fileName) {
                    if (!is_dir($dest.\DIRECTORY_SEPARATOR.$fileName)) {
                        mkdir($dest.\DIRECTORY_SEPARATOR.$fileName, 0755, true);
                    }
                    $this->recursiveProcess($pathName, $dest.\DIRECTORY_SEPARATOR.$fileName, $pattern, $callback);
                }
            } elseif (!$pattern || ($pattern && 1 === preg_match($pattern, $fileName))) {
                $path = $dest.\DIRECTORY_SEPARATOR.$fileName;
                $success = $success && \call_user_func($callback, $pathName, $path);
            }
        }

        return $success;
    }

    /**
     * Recursively deletes files in a specified location.
     *
     * @deprecated
     *
     * @param string $path Directory to deletes files
     * @param string $mask Regular expression to deletes files
     */
    private function recursiveDeletePath($path, $mask)
    {
        if (!file_exists($path) || !is_dir($path) || !is_readable($path)) {
            $this->logger->warning("Directory '{$path}' is not readable. Skip...");

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
     * Registers C-constants as PHP constants from a C-file.
     *
     * @param array $constantsSources
     *
     * @throws Exception
     */
    private function loadConstantsSources($constantsSources)
    {
        foreach ($constantsSources as $constantsSource) {
            if (!file_exists($constantsSource)) {
                throw new Exception("File '".$constantsSource."' with constants definitions");
            }

            foreach (file($constantsSource) as $line) {
                if (preg_match('/^\#define[ \t]+([A-Z0-9\_]+)[ \t]+([0-9]+)/', $line, $matches)) {
                    $this->constants[$matches[1]] = ['int', $matches[2]];
                    continue;
                }
                if (preg_match('/^\#define[ \t]+([A-Z0-9\_]+)[ \t]+(\'(.){1}\')/', $line, $matches)) {
                    $this->constants[$matches[1]] = ['char', $matches[3]];
                }
            }
        }
    }

    /**
     * Process config.w32 sections.
     *
     * @param array  $sources
     * @param string $project
     *
     * @return array
     */
    private function processAddSources($sources, $project)
    {
        $groupSources = [];
        foreach ($sources as $source) {
            $dirName = str_replace(\DIRECTORY_SEPARATOR, '/', \dirname($source));
            if (!isset($groupSources[$dirName])) {
                $groupSources[$dirName] = [];
            }
            $groupSources[$dirName][] = basename($source);
        }
        $groups = [];
        foreach ($groupSources as $dirname => $files) {
            $groups[] = 'ADD_SOURCES(configure_module_dirname + "/'.$dirname.'", "'.
                        implode(' ', $files).'", "'.$project.'");';
        }

        return $groups;
    }

    /**
     * Ensure that required extensions is present.
     *
     * @throws RuntimeException
     */
    private function assertRequiredExtensionsIsPresent()
    {
        $extensionRequires = $this->config->get('extensions', 'requires');
        if (true === empty($extensionRequires)) {
            return;
        }

        $extensions = [];
        foreach ($extensionRequires as $key => $value) {
            // TODO: We'll use this as an object in the future.
            // Right not it should be a string.
            if (!\is_string($value)) {
                continue;
            }
            if (false === \extension_loaded($value)) {
                $extensions[] = $value;
            }
        }

        if (false === empty($extensions)) {
            throw new RuntimeException(
                sprintf(
                    'Could not load extension(s): %s. You must load extensions above before build this extension.',
                    implode(', ', $extensions)
                )
            );
        }
    }

    /**
     * Checks if a file must be copied.
     *
     * @param string $src
     * @param string $dst
     *
     * @return bool
     */
    private function checkKernelFile($src, $dst)
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
     * Checks which files in the base kernel must be copied.
     *
     * @throws Exception
     *
     * @return bool
     */
    private function checkKernelFiles()
    {
        $kernelPath = 'ext'.\DIRECTORY_SEPARATOR.'kernel';

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
            $this->logger->info('Cleaning old kernel files...');
            $this->recursiveDeletePath($kernelPath, '@^.*\.[lcho]$@');

            @mkdir($kernelPath);

            $this->logger->info('Copying new kernel files...');
            $this->recursiveProcess($sourceKernelPath, $kernelPath, '@^.*\.[ch]$@');
        }

        return !$configured;
    }

    /**
     * Checks if the current directory is a valid Zephir project.
     *
     * @throws Exception
     *
     * @return string
     */
    private function checkDirectory()
    {
        $namespace = $this->config->get('namespace');
        if (!$namespace) {
            // TODO: Add more user friendly message.
            // For example assume if the user call the command from the wrong dir
            throw new Exception('Extension namespace cannot be loaded');
        }

        if (!\is_string($namespace)) {
            throw new Exception('Extension namespace is invalid');
        }

        if (!$this->filesystem->isInitialized()) {
            $this->filesystem->initialize();
        }

        if (!$this->filesystem->exists('.')) {
            if (!$this->checkIfPhpized()) {
                $this->logger->info(
                    'Zephir version has changed, use "zephir fullclean" to perform a full clean of the project'
                );
            }

            $this->filesystem->makeDirectory('.');
        }

        return $namespace;
    }

    /**
     * Returns current GCC version.
     *
     * @return string
     */
    private function getGccVersion()
    {
        if (is_windows()) {
            return '0.0.0';
        }

        if ($this->filesystem->exists('gcc-version')) {
            return $this->filesystem->read('gcc-version');
        }

        $this->filesystem->system('gcc -dumpversion', 'stdout', 'gcc-version');
        $lines = $this->filesystem->file('gcc-version');
        $lines = array_filter($lines);

        $lastLine = $lines[\count($lines) - 1];
        if (preg_match('/\d+\.\d+\.\d+/', $lastLine, $matches)) {
            return $matches[0];
        }

        return '0.0.0';
    }
}
