<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir;

use DirectoryIterator;
use Psr\Log\LoggerAwareTrait;
use Psr\Log\NullLogger;
use RecursiveDirectoryIterator;
use RecursiveIteratorIterator;
use ReflectionClass;
use ReflectionException;
use Zephir\Backend\Backend;
use Zephir\Backend\FcallManagerInterface;
use Zephir\Backend\StringsManager;
use Zephir\Class\Definition\Definition;
use Zephir\Code\ArgInfoDefinition;
use Zephir\Code\Builder\Struct;
use Zephir\Code\Printer;
use Zephir\Compiler\CompilerFileFactory;
use Zephir\Compiler\FileInterface;
use Zephir\Exception\CompilerException;
use Zephir\Exception\IllegalStateException;
use Zephir\Exception\InvalidArgumentException;
use Zephir\Exception\NotImplementedException;
use Zephir\Exception\ParseException;
use Zephir\Exception\RuntimeException;
use Zephir\FileSystem\FileSystemInterface;
use Zephir\FileSystem\HardDisk;
use Zephir\Parser\Manager;

use function array_filter;
use function array_map;
use function array_merge;
use function array_unique;
use function asort;
use function basename;
use function call_user_func;
use function chmod;
use function class_exists;
use function count;
use function defined;
use function dirname;
use function exec;
use function explode;
use function extension_loaded;
use function file;
use function file_exists;
use function file_get_contents;
use function file_put_contents;
use function filemtime;
use function fwrite;
use function getcwd;
use function getenv;
use function htmlentities;
use function implode;
use function in_array;
use function interface_exists;
use function is_array;
use function is_dir;
use function is_readable;
use function is_string;
use function krsort;
use function md5;
use function md5_file;
use function mkdir;
use function ob_get_clean;
use function ob_start;
use function phpinfo;
use function preg_match;
use function preg_replace;
use function realpath;
use function sprintf;
use function str_replace;
use function strcasecmp;
use function strip_tags;
use function strlen;
use function strpos;
use function strtolower;
use function strtoupper;
use function substr;
use function trim;
use function ucfirst;
use function unlink;
use function version_compare;

use const DIRECTORY_SEPARATOR;
use const INFO_GENERAL;
use const PHP_EOL;
use const PHP_INT_SIZE;
use const ZEND_THREAD_SAFE;
use const SORT_STRING;
use const STDERR;

final class Compiler
{
    use LoggerAwareTrait;

    /**
     * @var FunctionDefinition[]
     */
    public array  $functionDefinitions = [];
    private array $anonymousFiles      = [];
    private array $compiledFiles       = [];
    private array $constants           = [];
    /**
     * @var Definition[]
     */
    private array                 $definitions          = [];
    private array                 $externalDependencies = [];
    private array                 $extraFiles           = [];
    private FcallManagerInterface $fcallManager;
    /**
     * @var CompilerFile[]
     */
    private array $files   = [];
    private array $globals = [];
    /**
     * @var Definition[]
     */
    private static array $internalDefinitions = [];
    /**
     * Additional initializer code.
     * Used for static property initialization.
     */
    private array          $internalInitializers = [];
    private static bool    $loadedPrototypes     = false;
    private ?string        $optimizersPath;
    private ?string        $prototypesPath;
    private StringsManager $stringManager;
    private ?string        $templatesPath;

    public function __construct(
        private Config $config,
        public Backend $backend,
        private Manager $parserManager,
        private FileSystemInterface $filesystem,
        private CompilerFileFactory $compilerFileFactory,
    ) {
        $this->logger        = new NullLogger();
        $this->stringManager = new StringsManager();
        $this->fcallManager  = $this->backend->getFcallManager();

        try {
            $this->assertRequiredExtensionsIsPresent();
        } catch (RuntimeException $e) {
            fwrite(STDERR, trim($e->getMessage()) . PHP_EOL);
            exit(1);
        }
    }

    /**
     * Inserts an anonymous class definition in the compiler.
     */
    public function addClassDefinition(CompilerFileAnonymous $file, Definition $classDefinition): void
    {
        $this->definitions[$classDefinition->getCompleteName()]    = $classDefinition;
        $this->anonymousFiles[$classDefinition->getCompleteName()] = $file;
    }

    /**
     * Adds an external dependency to the compiler.
     */
    public function addExternalDependency(string $namespace, string $location): void
    {
        $this->externalDependencies[$namespace] = $location;
    }

    /**
     * Adds a function to the function definitions.
     */
    public function addFunction(FunctionDefinition $func, array $statement = []): void
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
     * Generate a HTML API.
     *
     * @throws ConfigException
     * @throws Exception
     * @throws ReflectionException
     */
    public function api(array $options = [], bool $fromGenerate = false): void
    {
        if (!$fromGenerate) {
            $this->generate();
        }

        $templatesPath = $this->templatesPath ?: dirname(__DIR__) . '/templates';

        $documentator = new Documentation($this->files, $this->config, $templatesPath, $options);
        $documentator->setLogger($this->logger);

        $this->logger->info('Generating API into ' . $documentator->getOutputDirectory());
        $documentator->build();
    }

    public function calculateDependencies(array $files, $_dependency = null): void
    {
        /**
         * Classes are ordered according to a dependency ranking
         * Classes with higher rank, need to be initialized first
         * We first build a dependency tree and then set the rank accordingly
         */
        if (null === $_dependency) {
            $dependencyTree = [];
            foreach ($files as $file) {
                if (!$file->isExternal()) {
                    $classDefinition                                     = $file->getClassDefinition();
                    $dependencyTree[$classDefinition->getCompleteName()] = $classDefinition->getDependencies();
                }
            }

            // Make sure the dependencies are loaded first (recursively)
            foreach ($dependencyTree as $dependencies) {
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
     * Check if the project must be phpized again.
     *
     * @return bool
     */
    public function checkIfPhpized(): bool
    {
        return !file_exists('ext/Makefile');
    }

    /**
     * Compiles the extension without installing it.
     *
     * @param bool     $development
     * @param int|null $jobs
     *
     * @throws Exception
     */
    public function compile(bool $development = false, ?int $jobs = null): void
    {
        $jobs = $jobs ?: 2;

        /**
         * Get global namespace.
         */
        $namespace     = str_replace('\\', '_', $this->checkDirectory());
        $extensionName = $this->config->get('extension-name');
        if (empty($extensionName) || !is_string($extensionName)) {
            $extensionName = $namespace;
        }

        $currentDir = getcwd();
        if (file_exists("$currentDir/compile.log")) {
            unlink("$currentDir/compile.log");
        }

        if (file_exists("$currentDir/compile-errors.log")) {
            unlink("$currentDir/compile-errors.log");
        }

        if (file_exists("$currentDir/ext/modules/{$namespace}.so")) {
            unlink("$currentDir/ext/modules/{$namespace}.so");
        }

        if (Os::isWindows()) {
            // TODO(klay): Make this better. Looks like it is non standard Env. Var
            exec('cd ext && %PHP_DEVPACK%\\phpize --clean', $output, $exit);

            $releaseFolder = $this->getWindowsReleaseDir();
            if (file_exists($releaseFolder)) {
                exec('rd /s /q ' . $releaseFolder, $output, $exit);
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
            $configureFix  = ["var PHP_ANALYZER = 'disabled';", "var PHP_PGO = 'no';", "var PHP_PGI = 'no';"];
            $hasChanged    = false;

            if (!str_contains($configureFile, $fixMarker)) {
                $configureFile = $fixMarker . PHP_EOL . implode(PHP_EOL, $configureFix) . PHP_EOL . $configureFile;
                $hasChanged    = true;
            }

            /* fix php's broken phpize patching ... */
            $marker = 'var build_dir = (dirname ? dirname : "").replace(new RegExp("^..\\\\\\\\"), "");';
            $pos    = strpos($configureFile, $marker);
            if (false !== $pos) {
                $spMarker = 'if (MODE_PHPIZE) {';
                $sp       = strpos($configureFile, $spMarker, $pos - 200);
                if (false === $sp) {
                    throw new CompilerException('outofdate... phpize seems broken again');
                }
                $configureFile = substr($configureFile, 0, $sp) .
                    'if (false) {' . substr($configureFile, $sp + strlen($spMarker));
                $hasChanged    = true;
            }

            if ($hasChanged) {
                file_put_contents('ext\\configure.js', $configureFile);
            }

            $this->logger->info('Preparing configuration file...');
            exec('cd ext && configure --enable-' . $extensionName);
        } else {
            exec('cd ext && make clean && phpize --clean', $output, $exit);
            $this->logger->info('Preparing for PHP compilation...');
            exec('cd ext && phpize', $output, $exit);
            $this->logger->info('Preparing configuration file...');

            exec(
                'cd ext && export CC="gcc" && export CFLAGS="' .
                $this->getGccFlags($development) .
                '" && ./configure --enable-' .
                $extensionName
            );
        }

        $currentDir = getcwd();
        $this->logger->info('Compiling...');
        if (Os::isWindows()) {
            exec(
                'cd ext && nmake 2>' . $currentDir . '\compile-errors.log 1>' .
                $currentDir . '\compile.log',
                $output,
                $exit
            );
        } else {
            $this->preCompileHeaders();
            exec(
                'cd ext && (make -s -j' . $jobs . ' 2>' . $currentDir . '/compile-errors.log 1>' .
                $currentDir .
                '/compile.log)',
                $output,
                $exit
            );
        }
    }

    /**
     * Create config.m4 and config.w32 for the extension.
     *
     * TODO: move this to backend?
     *
     * @throws Exception
     */
    public function createConfigFiles(string $project): bool
    {
        $contentM4 = $this->backend->getTemplateFileContents('config.m4');
        if (empty($contentM4)) {
            throw new Exception("Template config.m4 doesn't exist");
        }

        $contentW32 = $this->backend->getTemplateFileContents('config.w32');
        if (empty($contentW32)) {
            throw new Exception("Template config.w32 doesn't exist");
        }

        $safeProject = 'zend' === $project ? 'zend_' : $project;

        $compiledFiles = array_map(fn($file) => str_replace('.c', '.zep.c', $file), $this->compiledFiles);

        /**
         * If export-classes is enabled all headers are copied to include/php/ext.
         */
        $exportClasses = $this->config->get('export-classes', 'extra');
        if ($exportClasses) {
            $compiledHeaders = array_map(fn($file) => str_replace('.c', '.zep.h', $file), $this->compiledFiles);
        } else {
            $compiledHeaders = ['php_' . strtoupper($project) . '.h'];
        }

        /**
         * Check extra-libs, extra-cflags, package-dependencies exists
         */
        $extraLibs   = (string)$this->config->get('extra-libs');
        $extraCflags = (string)$this->config->get('extra-cflags');
        $contentM4   = $this->generatePackageDependenciesM4($contentM4);

        $buildDirs = [];

        foreach ($compiledFiles as $file) {
            $dir = dirname($file);

            if (!in_array($dir, $buildDirs)) {
                $buildDirs[] = $dir;
            }
        }

        asort($buildDirs);

        /**
         * Generate config.m4.
         */
        $toReplace = [
            '%PROJECT_LOWER_SAFE%'   => strtolower($safeProject),
            '%PROJECT_LOWER%'        => strtolower($project),
            '%PROJECT_UPPER%'        => strtoupper($project),
            '%PROJECT_CAMELIZE%'     => ucfirst($project),
            '%FILES_COMPILED%'       => implode("\n\t", $this->toUnixPaths($compiledFiles)),
            '%HEADERS_COMPILED%'     => implode(' ', $this->toUnixPaths($compiledHeaders)),
            '%EXTRA_FILES_COMPILED%' => implode("\n\t", $this->toUnixPaths($this->extraFiles)),
            '%PROJECT_EXTRA_LIBS%'   => $extraLibs,
            '%PROJECT_EXTRA_CFLAGS%' => $extraCflags,
            '%PROJECT_BUILD_DIRS%'   => implode(' ', $buildDirs),
        ];

        foreach ($toReplace as $mark => $replace) {
            $contentM4 = str_replace($mark, $replace, $contentM4);
        }

        HardDisk::persistByHash($contentM4, 'ext/config.m4');

        /**
         * Generate config.w32.
         */
        $toReplace = [
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
        ];

        foreach ($toReplace as $mark => $replace) {
            $contentW32 = str_replace($mark, $replace, $contentW32);
        }

        $needConfigure = HardDisk::persistByHash($contentW32, 'ext/config.w32');

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

        HardDisk::persistByHash($content, 'ext/php_ext.h');

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

        HardDisk::persistByHash($content, 'ext/ext.h');

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

        HardDisk::persistByHash($content, 'ext/ext_config.h');

        /**
         * ext_clean.
         */
        $content = $this->backend->getTemplateFileContents('clean');
        if (empty($content)) {
            throw new Exception("Clean file doesn't exist");
        }

        if (HardDisk::persistByHash($content, 'ext/clean')) {
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

        if (HardDisk::persistByHash($content, 'ext/install')) {
            chmod('ext/install', 0755);
        }

        return (bool)$needConfigure;
    }

    /**
     * Create project.c and project.h according to the current extension.
     *
     * TODO: Move the part of the logic which depends on templates (backend-specific) to backend?
     *
     * @throws Exception
     */
    public function createProjectFiles(string $project): bool
    {
        $needConfigure = $this->checkKernelFiles();

        /**
         * project.c.
         */
        $content = $this->backend->getTemplateFileContents('project.c');
        if (empty($content)) {
            throw new Exception("Template project.c doesn't exist");
        }

        $includes        = '';
        $reqInitializers = '';
        $reqDestructors  = '';
        $prqDestructors  = '';
        $modInitializers = '';
        $modDestructors  = '';
        $glbInitializers = '';
        $glbDestructors  = '';
        $files           = array_merge($this->files, $this->anonymousFiles);

        /**
         * Round 1. Calculate the dependency rank
         */
        $this->calculateDependencies($files);

        $classEntries = [];
        $classInits   = [];

        $interfaceEntries = [];
        $interfaceInits   = [];

        /**
         * Round 2. Generate the ZEPHIR_INIT calls according to the dependency rank
         */
        /** @var FileInterface $file */
        foreach ($files as $file) {
            if ($file->isExternal()) {
                continue;
            }

            $classDefinition = $file->getClassDefinition();
            if ($classDefinition === null) {
                continue;
            }

            $dependencyRank = $classDefinition->getDependencyRank();
            if ('class' === $classDefinition->getType()) {
                $classEntries[$dependencyRank][] = 'zend_class_entry *' . $classDefinition->getClassEntry() . ';';
                $classInits[$dependencyRank][]   = 'ZEPHIR_INIT('
                    . $classDefinition->getCNamespace()
                    . '_'
                    . $classDefinition->getName()
                    . ');';
            } else {
                $interfaceEntries[$dependencyRank][] = 'zend_class_entry *' . $classDefinition->getClassEntry() . ';';
                $interfaceInits[$dependencyRank][]   = 'ZEPHIR_INIT('
                    . $classDefinition->getCNamespace()
                    . '_'
                    . $classDefinition->getName()
                    . ');';
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

        /**
         * Round 3. Process extension globals
         */
        [$globalCode, $globalStruct, $globalsDefault, $initEntries] = $this->processExtensionGlobals($project);
        if ('zend' == $project) {
            $safeProject = 'zend_';
        } else {
            $safeProject = $project;
        }

        /**
         * Round 4. Process extension info.
         */
        $phpInfo = $this->processExtensionInfo();

        /**
         * Round 5. Generate Function entries (FE)
         */
        [$feHeader, $feEntries] = $this->generateFunctionInformation();

        /**
         * Check if there are module/request/global destructors.
         */
        $destructors = $this->config->get('destructors');
        if (is_array($destructors)) {
            $invokeRequestDestructors = $this->processCodeInjection($destructors, 'request');
            $includes                 .= PHP_EOL . $invokeRequestDestructors[0];
            $reqDestructors           = $invokeRequestDestructors[1];

            $invokePostRequestDestructors = $this->processCodeInjection($destructors, 'post-request');
            $includes                     .= PHP_EOL . $invokePostRequestDestructors[0];
            $prqDestructors               = $invokePostRequestDestructors[1];

            $invokeModuleDestructors = $this->processCodeInjection($destructors, 'module');
            $includes                .= PHP_EOL . $invokeModuleDestructors[0];
            $modDestructors          = $invokeModuleDestructors[1];

            $invokeGlobalsDestructors = $this->processCodeInjection($destructors, 'globals');
            $includes                 .= PHP_EOL . $invokeGlobalsDestructors[0];
            $glbDestructors           = $invokeGlobalsDestructors[1];
        }

        /**
         * Check if there are module/request/global initializers.
         */
        $initializers = $this->config->get('initializers');
        if (is_array($initializers)) {
            $invokeRequestInitializers = $this->processCodeInjection($initializers, 'request');
            $includes                  .= PHP_EOL . $invokeRequestInitializers[0];
            $reqInitializers           = $invokeRequestInitializers[1];

            $invokeModuleInitializers = $this->processCodeInjection($initializers, 'module');
            $includes                 .= PHP_EOL . $invokeModuleInitializers[0];
            $modInitializers          = $invokeModuleInitializers[1];

            $invokeGlobalsInitializers = $this->processCodeInjection($initializers, 'globals');
            $includes                  .= PHP_EOL . $invokeGlobalsInitializers[0];
            $glbInitializers           = $invokeGlobalsInitializers[1];
        }

        /**
         * Append extra details.
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

        $modRequires = array_map(
            fn($mod) => sprintf('ZEND_MOD_REQUIRED("%s")', strtolower($mod)),
            $this->config->get('extensions', 'requires') ?: []
        );

        $toReplace = [
            '%PROJECT_LOWER_SAFE%'   => strtolower($safeProject),
            '%PROJECT_LOWER%'        => strtolower($project),
            '%PROJECT_UPPER%'        => strtoupper($project),
            '%PROJECT_CAMELIZE%'     => ucfirst($project),
            '%CLASS_ENTRIES%'        => implode(
                PHP_EOL,
                array_merge($completeInterfaceEntries, $completeClassEntries)
            ),
            '%CLASS_INITS%'          => implode(
                PHP_EOL . "\t",
                array_merge($completeInterfaceInits, $completeClassInits)
            ),
            '%INIT_GLOBALS%'         => implode(
                PHP_EOL . "\t",
                array_merge((array)$globalsDefault[0], [$glbInitializers])
            ),
            '%INIT_MODULE_GLOBALS%'  => $globalsDefault[1],
            '%DESTROY_GLOBALS%'      => $glbDestructors,
            '%EXTENSION_INFO%'       => $phpInfo,
            '%EXTRA_INCLUDES%'       => implode(
                PHP_EOL,
                array_unique(explode(PHP_EOL, $includes))
            ),
            '%MOD_INITIALIZERS%'     => $modInitializers,
            '%MOD_DESTRUCTORS%'      => $modDestructors,
            '%REQ_INITIALIZERS%'     => implode(
                PHP_EOL . "\t",
                array_merge($this->internalInitializers, [$reqInitializers])
            ),
            '%REQ_DESTRUCTORS%'      => $reqDestructors,
            '%POSTREQ_DESTRUCTORS%'  => empty($prqDestructors) ? '' : implode(
                PHP_EOL,
                [
                    '#define ZEPHIR_POST_REQUEST 1',
                    'static PHP_PRSHUTDOWN_FUNCTION(' . strtolower($project) . ')',
                    '{',
                    "\t" . implode(
                        PHP_EOL . "\t",
                        explode(PHP_EOL, $prqDestructors)
                    ),
                    '}',
                ]
            ),
            '%FE_HEADER%'            => $feHeader,
            '%FE_ENTRIES%'           => $feEntries,
            '%PROJECT_INI_ENTRIES%'  => implode(PHP_EOL . "\t", $initEntries),
            '%PROJECT_DEPENDENCIES%' => implode(PHP_EOL . "\t", $modRequires),
        ];
        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        /**
         * Round 5. Generate and place the entry point of the project
         */
        HardDisk::persistByHash($content, 'ext/' . $safeProject . '.c');
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
                $fileH            = str_replace('.c', '.zep.h', $file);
                $include          = '#include "' . $fileH . '"';
                $includeHeaders[] = $include;
            }
        }

        /**
         * Append extra headers.
         */
        $extraClasses = $this->config->get('extra-classes');
        if (is_array($extraClasses)) {
            foreach ($extraClasses as $value) {
                if (isset($value['header'])) {
                    $include          = '#include "' . $value['header'] . '"';
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

        HardDisk::persistByHash($content, 'ext/' . $safeProject . '.h');
        unset($content);

        /**
         * Round 7. Create php_project.h.
         */
        $content = $this->backend->getTemplateFileContents('php_project.h');
        if (empty($content)) {
            throw new Exception("Template php_project.h doesn't exist");
        }

        $toReplace = [
            '%PROJECT_LOWER_SAFE%'       => strtolower($safeProject),
            '%PROJECT_LOWER%'            => strtolower($project),
            '%PROJECT_UPPER%'            => strtoupper($project),
            '%PROJECT_EXTNAME%'          => strtolower($project),
            '%PROJECT_NAME%'             => mb_convert_encoding($this->config->get('name'), 'ISO-8859-1', 'UTF-8'),
            '%PROJECT_AUTHOR%'           => mb_convert_encoding($this->config->get('author'), 'ISO-8859-1', 'UTF-8'),
            '%PROJECT_VERSION%'          => mb_convert_encoding($this->config->get('version'), 'ISO-8859-1', 'UTF-8'),
            '%PROJECT_DESCRIPTION%'      => mb_convert_encoding(
                $this->config->get('description'),
                'ISO-8859-1',
                'UTF-8'
            ),
            '%PROJECT_ZEPVERSION%'       => Zephir::VERSION,
            '%EXTENSION_GLOBALS%'        => $globalCode,
            '%EXTENSION_STRUCT_GLOBALS%' => $globalStruct,
        ];

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        HardDisk::persistByHash($content, 'ext/php_' . $safeProject . '.h');
        unset($content);

        return $needConfigure;
    }

    /**
     * Generates the C sources from Zephir without compiling them.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function generate(bool $fromGenerate = false): bool
    {
        /**
         * Get global namespace.
         */
        $namespace = $this->checkDirectory();

        /**
         * Check whether there are external dependencies.
         */
        $externalDependencies = $this->config->get('external-dependencies');
        if (is_array($externalDependencies)) {
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
         * Sort the files by dependency ranking.
         */
        $files       = [];
        $rankedFiles = [];
        $this->calculateDependencies($this->files);

        foreach ($this->files as $rankFile) {
            $rank                 = $rankFile->getClassDefinition()->getDependencyRank();
            $rankedFiles[$rank][] = $rankFile;
        }

        krsort($rankedFiles);
        foreach ($rankedFiles as $rankFiles) {
            $files = array_merge($files, $rankFiles);
        }
        $this->files = $files;

        /**
         * Convert C-constants into PHP constants.
         */
        $constantsSources = $this->config->get('constants-sources');
        if (is_array($constantsSources)) {
            $this->loadConstantsSources($constantsSources);
        }

        /**
         * Set extension globals.
         */
        $globals = $this->config->get('globals');
        if (is_array($globals)) {
            $this->setExtensionGlobals($globals);
        }

        /**
         * Load function optimizers
         */
        if (false === self::$loadedPrototypes) {
            $optimizersPath = $this->resolveOptimizersPath();
            FunctionCall::addOptimizerDir("{$optimizersPath}/FunctionCall");

            $customOptimizersPaths = $this->config->get('optimizer-dirs');
            if (is_array($customOptimizersPaths)) {
                foreach ($customOptimizersPaths as $directory) {
                    FunctionCall::addOptimizerDir(realpath($directory));
                }
            }

            /**
             * Load additional extension prototypes.
             */
            $prototypesPath = $this->resolvePrototypesPath();
            foreach (new DirectoryIterator($prototypesPath) as $file) {
                if ($file->isDir() || $file->isDot()) {
                    continue;
                }

                // Do not use $file->getRealPath() because it does not work inside phar
                $realPath  = "{$file->getPath()}/{$file->getFilename()}";
                $extension = $file->getBasename(".{$file->getExtension()}");

                if (!extension_loaded($extension)) {
                    require_once $realPath;
                }
            }

            /**
             * Load customer additional extension prototypes.
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
                            foreach (new RecursiveDirectoryIterator($prototypeRealpath) as $file) {
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
            /**
             * Only compile classes in the local extension, ignore external classes
             */
            if (!$compileFile->isExternal()) {
                $compileFile->compile($this, $this->stringManager);
                $compiledFile = $compileFile->getCompiledFile();

                $methods         = [];
                $classDefinition = $compileFile->getClassDefinition();
                foreach ($classDefinition->getMethods() as $method) {
                    $methods[] = '[' . $method->getName() . ':' . implode('-', $method->getVisibility()) . ']';
                    if ($method->isInitializer() && $method->isStatic()) {
                        $this->internalInitializers[] = "\t" . $method->getName() . '();';
                    }
                }

                $files[] = $compiledFile;

                $hash .= '|'
                    . $compiledFile
                    . ':'
                    . $classDefinition->getClassEntry()
                    . '['
                    . implode('|', $methods)
                    . ']';
            }
        }

        /**
         * Round 3.2. Compile anonymous classes
         */
        foreach ($this->anonymousFiles as $compileFile) {
            $compileFile->compile($this, $this->stringManager);
            $compiledFile = $compileFile->getCompiledFile();

            $methods         = [];
            $classDefinition = $compileFile->getClassDefinition();
            foreach ($classDefinition->getMethods() as $method) {
                $methods[] = '['
                    . $method->getName()
                    . ':'
                    . implode('-', $method->getVisibility())
                    . ']';
            }

            $files[] = $compiledFile;

            $hash .= '|'
                . $compiledFile
                . ':'
                . $classDefinition->getClassEntry()
                . '['
                . implode('|', $methods)
                . ']';
        }

        $hash                = md5($hash);
        $this->compiledFiles = $files;

        /**
         * Round 3.3. Load extra C-sources.
         */
        $extraSources = $this->config->get('extra-sources');
        if (is_array($extraSources)) {
            $this->extraFiles = $extraSources;
        } else {
            $this->extraFiles = [];
        }

        /**
         * Round 3.4. Load extra classes sources.
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
         * Round 4. Create config.m4 and config.w32 files / Create project.c and project.h files.
         */
        $namespace     = str_replace('\\', '_', $namespace);
        $extensionName = $this->config->get('extension-name');
        if (empty($extensionName) || !is_string($extensionName)) {
            $extensionName = $namespace;
        }

        $needConfigure = $this->createConfigFiles($extensionName);
        $needConfigure |= $this->createProjectFiles($extensionName);
        $needConfigure |= $this->checkIfPhpized();
        // Bitwise returns `int` instead of `bool`.
        $needConfigure = (bool)$needConfigure;

        /**
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

        /**
         * Round 5. Generate concatenation functions
         */
        $this->stringManager->genConcatCode();
        $this->fcallManager->genFcallCode();

        if ($this->config->get('stubs-run-after-generate', 'stubs')) {
            $this->stubs($fromGenerate);
        }

        return $needConfigure;
    }

    public function generateFunctionInformation(): array
    {
        $headerPrinter = new Printer();
        $entryPrinter  = new Printer();

        /**
         * Specifying Argument Information
         */
        foreach ($this->functionDefinitions as $func) {
            $argInfo = new ArgInfoDefinition(
                $func->getArgInfoName(),
                $func,
                $headerPrinter,
                $func->getCallGathererPass()->getCompilationContext()
            );

            $funcName    = $func->getInternalName();
            $argInfoName = $func->getArgInfoName();

            $headerPrinter->output('PHP_FUNCTION(' . $funcName . ');');

            $argInfo->setBooleanDefinition('_IS_BOOL');
            $argInfo->setRichFormat(true);

            $argInfo->render();

            /** Generate FE's */
            $paramData = 'NULL';

            $richFormat = $func->isReturnTypesHintDetermined() && $func->areReturnTypesCompatible();

            if ($richFormat || $func->hasParameters()) {
                $paramData = $argInfoName;
            }

            if ($func->isGlobal()) {
                $entryPrinter->output(
                    'ZEND_NAMED_FE(' . $func->getName() . ', ZEND_FN(' . $funcName . '), ' . $paramData . ')'
                );
            } else {
                $entryPrinter->output(
                    'ZEND_NS_NAMED_FE("' . str_replace('\\', '\\\\', $func->getNamespace()) . '", ' .
                    $func->getName() .
                    ', ZEND_FN(' . $funcName . '), ' .
                    $paramData . ')'
                );
            }
        }

        $entryPrinter->output('ZEND_FE_END');

        return [$headerPrinter->getOutput(), $entryPrinter->getOutput()];
    }

    /**
     * Generate package-dependencies config for m4.
     *
     * TODO: Move the template depending part to backend?
     */
    public function generatePackageDependenciesM4(string $contentM4): string
    {
        $packageDependencies = $this->config->get('package-dependencies');
        if (!is_array($packageDependencies)) {
            return str_replace('%PROJECT_PACKAGE_DEPENDENCIES%', '', $contentM4);
        }

        $pkgconfigM4      = $this->backend->getTemplateFileContents('pkg-config.m4');
        $pkgconfigCheckM4 = $this->backend->getTemplateFileContents('pkg-config-check.m4');
        $extraCFlags      = '';

        foreach ($packageDependencies as $pkg => $version) {
            $pkgM4Buf = $pkgconfigCheckM4;

            $operator    = '=';
            $operatorCmd = '--exact-version';
            $ar          = explode('=', $version);

            if (1 === count($ar)) {
                if ('*' === $version) {
                    $version     = '0.0.0';
                    $operator    = '>=';
                    $operatorCmd = '--atleast-version';
                }
            } else {
                switch ($ar[0]) {
                    case '<':
                        $operator    = '<=';
                        $operatorCmd = '--max-version';
                        break;
                    case '>':
                        $operator    = '>=';
                        $operatorCmd = '--atleast-version';
                        break;
                }

                $version = trim($ar[1]);
            }

            $toReplace = [
                '%PACKAGE_LOWER%'                      => strtolower($pkg),
                '%PACKAGE_UPPER%'                      => strtoupper($pkg),
                '%PACKAGE_REQUESTED_VERSION%'          => $operator . ' ' . $version,
                '%PACKAGE_PKG_CONFIG_COMPARE_VERSION%' => $operatorCmd . '=' . $version,
            ];

            foreach ($toReplace as $mark => $replace) {
                $pkgM4Buf = str_replace($mark, $replace, $pkgM4Buf);
            }

            $pkgconfigM4 .= $pkgM4Buf;
            $extraCFlags .= '$PHP_' . strtoupper($pkg) . '_INCS ';
        }

        $contentM4 = str_replace('%PROJECT_EXTRA_CFLAGS%', '%PROJECT_EXTRA_CFLAGS% ' . $extraCFlags, $contentM4);

        return str_replace('%PROJECT_PACKAGE_DEPENDENCIES%', $pkgconfigM4, $contentM4);
    }

    /**
     * Returns class the class definition from a given class name.
     */
    public function getClassDefinition(string $className): Definition | bool
    {
        foreach ($this->definitions as $key => $value) {
            if (!strcasecmp($key, $className)) {
                return $value;
            }
        }

        return false;
    }

    /**
     * Returns a Zephir Constant by its name.
     */
    public function getConstant(string $name): mixed
    {
        return $this->constants[$name];
    }

    /**
     * Returns an extension global by its name.
     */
    public function getExtensionGlobal(string $name): array
    {
        return $this->globals[$name];
    }

    /**
     * Returns GCC flags for current compilation.
     */
    public function getGccFlags(bool $development = false): string
    {
        if (Os::isWindows()) {
            // TODO
            return '';
        }

        $gccFlags = getenv('CFLAGS');

        if (!is_string($gccFlags)) {
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
     * Returns class the class definition from a given class name.
     *
     * @throws ReflectionException
     */
    public function getInternalClassDefinition(string $className): Definition
    {
        if (!isset(self::$internalDefinitions[$className])) {
            $reflection                            = new ReflectionClass($className);
            self::$internalDefinitions[$className] = Definition::buildFromReflection($reflection);
        }

        return self::$internalDefinitions[$className];
    }

    /**
     * Gets the Zephir Parser Manager.
     *
     * @deprecated
     */
    public function getParserManager(): Parser\Manager
    {
        return $this->parserManager;
    }

    /**
     * Returns the php include directories returned by php-config.
     */
    public function getPhpIncludeDirs(): string
    {
        $this->filesystem->system('php-config --includes', 'stdout', 'php-includes');

        return trim($this->filesystem->read('php-includes'));
    }

    /**
     * Returns a short user path.
     */
    public static function getShortUserPath(string $path): string
    {
        return str_replace('\\', '/', str_replace(getcwd() . DIRECTORY_SEPARATOR, '', $path));
    }

    /**
     * Compiles and installs the extension.
     *
     * @throws Exception
     * @throws NotImplementedException
     * @throws CompilerException
     */
    public function install(bool $development = false): void
    {
        // Get global namespace
        $namespace  = str_replace('\\', '_', $this->checkDirectory());
        $currentDir = getcwd();

        if (Os::isWindows()) {
            throw new NotImplementedException('Installation is not implemented for Windows yet. Aborting.');
        }

        $this->logger->info('Installing...');
        $gccFlags = $this->getGccFlags($development);

        $command = strtr(
        // TODO: Sort out with sudo
            'cd ext && export CC="gcc" && export CFLAGS=":cflags" && ' .
            'make 2>> ":stderr" 1>> ":stdout" && ' .
            'sudo make install 2>> ":stderr" 1>> ":stdout"',
            [
                ':cflags' => $gccFlags,
                ':stderr' => "{$currentDir}/compile-errors.log",
                ':stdout' => "{$currentDir}/compile.log",
            ]
        );

        array_map(function ($entry): void {
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
     * Allows checking if a class is part of PHP.
     */
    public function isBundledClass(string $className): bool
    {
        return class_exists($className, false);
    }

    /**
     * Allows checking if an interface is part of PHP.
     */
    public function isBundledInterface(string $className): bool
    {
        return interface_exists($className, false);
    }

    /**
     * Allows to check if a class is part of the compiled extension.
     */
    public function isClass(string $className): bool
    {
        foreach ($this->definitions as $key => $value) {
            if (!strcasecmp($key, $className) && 'class' === $value->getType()) {
                return true;
            }
        }

        /**
         * Try to autoload the class from an external dependency
         */
        foreach ($this->externalDependencies as $namespace => $location) {
            if (preg_match('#^' . $namespace . '\\\\#i', $className)) {
                return $this->loadExternalClass($className, $location);
            }
        }

        return false;
    }

    /**
     * Checks if $name is a Zephir constant.
     */
    public function isConstant(string $name): bool
    {
        return isset($this->constants[$name]);
    }

    /**
     * Checks if a specific extension global is defined.
     */
    public function isExtensionGlobal(string $name): bool
    {
        return isset($this->globals[$name]);
    }

    /**
     * Allows checking if an interface is part of the compiled extension.
     *
     * @throws CompilerException
     * @throws IllegalStateException
     * @throws ParseException
     */
    public function isInterface(string $className): bool
    {
        foreach ($this->definitions as $key => $value) {
            if (!strcasecmp($key, $className) && Definition::TYPE_INTERFACE === $value->getType()) {
                return true;
            }
        }

        /**
         * Try to autoload the class from an external dependency
         */
        foreach ($this->externalDependencies as $namespace => $location) {
            if (preg_match('#^' . $namespace . '\\\\#i', $className)) {
                return $this->loadExternalClass($className, $location);
            }
        }

        return false;
    }

    /**
     * Loads a class definition in an external dependency.
     *
     * @throws CompilerException
     * @throws IllegalStateException
     * @throws ParseException
     */
    public function loadExternalClass(string $className, string $location): bool
    {
        $filePath = $location
            . DIRECTORY_SEPARATOR
            . strtolower(
                str_replace('\\', DIRECTORY_SEPARATOR, $className)
            )
            . '.zep';

        /**
         * Fix the class name.
         */
        $className = implode(
            '\\',
            array_map(
                'ucfirst',
                explode('\\', $className)
            )
        );

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

        $this->files[$className]       = $compilerFile;
        $this->definitions[$className] = $compilerFile->getClassDefinition();

        return true;
    }

    /**
     * Pre-compile headers to speed up compilation.
     */
    public function preCompileHeaders(): void
    {
        if (Os::isWindows()) {
            // TODO: Add Windows support
            return;
        }

        $phpIncludes = $this->getPhpIncludeDirs();

        /** @var DirectoryIterator $file */
        foreach (new DirectoryIterator('ext/kernel') as $file) {
            if ($file->isDir() || $file->getExtension() !== 'h') {
                continue;
            }

            $command = sprintf(
                'cd ext && gcc -c kernel/%s  -I. %s  -o kernel/%s.gch',
                $file->getBaseName(),
                $phpIncludes,
                $file->getBaseName()
            );

            $path = $file->getRealPath();
            if (!file_exists($path . '.gch') || filemtime($path) > filemtime($path . '.gch')) {
                $this->filesystem->system($command, 'stdout', 'compile-header');
            }
        }
    }

    /**
     * Process extension code injection.
     */
    public function processCodeInjection(array $entries, string $section = 'request'): array
    {
        $codes    = [];
        $includes = [];

        if (isset($entries[$section])) {
            foreach ($entries[$section] as $entry) {
                if (!empty($entry['code'])) {
                    $codes[] = $entry['code'] . ';';
                }

                if (!empty($entry['include'])) {
                    $includes[] = '#include "' . $entry['include'] . '"';
                }
            }
        }

        return [implode(PHP_EOL, $includes), implode("\n\t", $codes)];
    }

    /**
     * Process extension globals.
     *
     * @throws Exception
     */
    public function processExtensionGlobals(string $namespace): array
    {
        $globalCode     = '';
        $globalStruct   = '';
        $globalsDefault = [[], []];
        $initEntries    = [];

        /**
         * Generate the extensions globals declaration.
         */
        $globals = $this->config->get('globals');
        if (is_array($globals)) {
            $structures = [];
            $variables  = [];
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
                if (preg_match('/^[0-9a-zA-Z_]$/', $structureName)) {
                    throw new Exception("Struct name: '" . $structureName . "' contains invalid characters");
                }

                $structBuilder = new Struct('_zephir_struct_' . $structureName, $structureName);
                foreach ($internalStructure as $field => $global) {
                    if (preg_match('/^[0-9a-zA-Z_]$/', $field)) {
                        throw new Exception("Struct field name: '" . $field . "' contains invalid characters");
                    }

                    $structBuilder->addProperty($field, $global['type']);

                    $isModuleGlobal                    = (int)!empty($global['module']);
                    $globalsDefault[$isModuleGlobal][] = $structBuilder->getCDefault($field, $global, $namespace);
                    $initEntries[]                     = $structBuilder->getInitEntry($field, $global, $namespace);
                }

                $globalStruct .= $structBuilder . PHP_EOL;
            }

            $globalCode = PHP_EOL;
            foreach ($structures as $structureName => $internalStructure) {
                $globalCode .= "\t" . 'zephir_struct_' . $structureName . ' ' . $structureName . ';' . PHP_EOL;
            }

            /**
             * Process single variables
             */
            foreach ($variables as $name => $global) {
                if (preg_match('/^[0-9a-zA-Z_]$/', $name)) {
                    throw new Exception("Extension global variable name: '" . $name . "' contains invalid characters");
                }

                if (!isset($global['default'])) {
                    throw new Exception("Extension global variable name: '" . $name . "' contains invalid characters");
                }

                $isModuleGlobal = (int)!empty($global['module']);
                $type           = $global['type'];
                // TODO: Add support for 'hash'
                // TODO: Zephir\Optimizers\FunctionCall\GlobalsSetOptimizer
                switch ($global['type']) {
                    case 'boolean':
                    case 'bool':
                        $type = 'zend_bool';
                        if (true === $global['default']) {
                            $globalsDefault[$isModuleGlobal][] = "\t" . $namespace . '_globals->' . $name . ' = 1;';
                        } else {
                            $globalsDefault[$isModuleGlobal][] = "\t" . $namespace . '_globals->' . $name . ' = 0;';
                        }
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'double':
                        $globalsDefault[$isModuleGlobal][] = "\t" . $namespace . '_globals->' . $name . ' = ' . $global['default'] . ';';
                        break;

                    case 'char':
                    case 'uchar':
                        $globalsDefault[$isModuleGlobal][] = "\t" . $namespace . '_globals->' . $name . ' = \'' . $global['default'] . '\';';
                        break;
                    case 'string':
                        $type                              = 'char *';
                        $globalsDefault[$isModuleGlobal][] = "\t" . $namespace . '_globals->' . $name . ' = ZSTR_VAL(zend_string_init(ZEND_STRL("' . $global['default'] . '"), 0));';
                        break;
                    default:
                        throw new Exception(
                            "Unknown type '" . $global['type'] . "' for extension global '" . $name . "'"
                        );
                }

                $globalCode .= "\t" . $type . ' ' . $name . ';' . PHP_EOL;

                $iniEntry = $global['ini-entry'] ?? [];
                $iniName  = $iniEntry['name'] ?? $namespace . '.' . $name;
                $scope    = $iniEntry['scope'] ?? 'PHP_INI_ALL';

                switch ($global['type']) {
                    case 'boolean':
                    case 'bool':
                        $initEntries[] =
                            'STD_PHP_INI_BOOLEAN("' .
                            $iniName .
                            '", "' .
                            (int)(true === $global['default']) .
                            '", ' .
                            $scope .
                            ', OnUpdateBool, ' .
                            $name .
                            ', zend_' .
                            $namespace .
                            '_globals, ' .
                            $namespace . '_globals)';
                        break;

                    case 'string':
                        $initEntries[] = sprintf(
                            'STD_PHP_INI_ENTRY(%s, %s, %s, NULL, %s, %s, %s)',
                            '"' . $iniName . '"',
                            '"' . $global['default'] . '"',
                            $scope,
                            $name,
                            'zend_' . $namespace . '_globals',
                            $namespace . '_globals',
                        );
                        break;
                }
            }
        }

        $globalsDefault[0] = implode(PHP_EOL, $globalsDefault[0]);
        $globalsDefault[1] = implode(PHP_EOL, $globalsDefault[1]);

        return [$globalCode, $globalStruct, $globalsDefault, $initEntries];
    }

    /**
     * Generates phpinfo() sections showing information about the extension.
     */
    public function processExtensionInfo(): string
    {
        $phpinfo = '';

        $info = $this->config->get('info');
        if (!is_array($info)) {
            return $phpinfo;
        }

        foreach ($info as $table) {
            $phpinfo .= "\t" . 'php_info_print_table_start();' . PHP_EOL;
            if (isset($table['header'])) {
                $headerArray = [];
                foreach ($table['header'] as $header) {
                    $headerArray[] = '"' . htmlentities($header) . '"';
                }

                $phpinfo .= "\t" . 'php_info_print_table_header(' . count($headerArray) . ', ' .
                    implode(', ', $headerArray) . ');' . PHP_EOL;
            }

            if (isset($table['rows'])) {
                foreach ($table['rows'] as $row) {
                    $rowArray = [];
                    foreach ($row as $field) {
                        $rowArray[] = '"' . htmlentities($field) . '"';
                    }

                    $phpinfo .= "\t" . 'php_info_print_table_row(' . count($rowArray) . ', ' .
                        implode(', ', $rowArray) . ');' . PHP_EOL;
                }
            }

            $phpinfo .= "\t" . 'php_info_print_table_end();' . PHP_EOL;
        }

        return $phpinfo;
    }

    /**
     * Sets extensions globals.
     */
    public function setExtensionGlobals(array $globals): void
    {
        foreach ($globals as $key => $value) {
            $this->globals[$key] = $value;
        }
    }

    public function setOptimizersPath(string $optimizersPath): void
    {
        $this->optimizersPath = $optimizersPath;
    }

    public function setPrototypesPath(string $prototypesPath): void
    {
        $this->prototypesPath = $prototypesPath;
    }

    public function setTemplatesPath(string $templatesPath): void
    {
        $this->templatesPath = $templatesPath;
    }

    /**
     * Generate IDE stubs.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function stubs(bool $fromGenerate = false): void
    {
        if (!$fromGenerate) {
            $this->generate();
        }

        $this->logger->info('Generating stubs...');

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

        (new Stubs\Generator($this->files))->generate(
            $this->config->get('namespace'),
            $path,
            $this->config->get('indent', 'extra'),
            $this->config->get('banner', 'stubs') ?? ''
        );
    }

    /**
     * Ensure that required extensions is present.
     *
     * @throws RuntimeException
     */
    private function assertRequiredExtensionsIsPresent(): void
    {
        $extensionRequires = $this->config->get('extensions', 'requires');
        if (empty($extensionRequires)) {
            return;
        }

        $extensions = [];
        foreach ($extensionRequires as $value) {
            // TODO: We'll use this as an object in the future.
            if (!is_string($value)) {
                continue;
            }

            if (!extension_loaded($value)) {
                $extensions[] = $value;
            }
        }

        if (!empty($extensions)) {
            throw new RuntimeException(
                sprintf(
                    'Could not load extension(s): %s. You must load extensions above before build this extension.',
                    implode(', ', $extensions)
                )
            );
        }
    }

    /**
     * Checks if the current directory is a valid Zephir project.
     *
     * @throws Exception
     */
    private function checkDirectory(): string
    {
        $namespace = $this->config->get('namespace');
        if (!$namespace) {
            // TODO: Add more user friendly message.
            // For example assume if the user call the command from the wrong dir
            throw new Exception('Extension namespace cannot be loaded');
        }

        if (!is_string($namespace)) {
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
     * Checks if a file must be copied.
     *
     * @param string $src
     * @param string $dst
     *
     * @return bool
     */
    private function checkKernelFile(string $src, string $dst): bool
    {
        if (preg_match('#kernels/ZendEngine[2-9]/concat\.#', $src)) {
            return true;
        }

        if (!file_exists($dst)) {
            return false;
        }

        return md5_file($src) === md5_file($dst);
    }

    /**
     * Checks which files in the base kernel must be copied.
     *
     * @throws Exception
     */
    private function checkKernelFiles(): bool
    {
        $kernelPath = 'ext' . DIRECTORY_SEPARATOR . 'kernel';

        if (!file_exists($kernelPath)) {
            if (!mkdir($kernelPath, 0775, true)) {
                throw new Exception("Cannot create kernel directory: {$kernelPath}");
            }
        }

        $kernelPath       = realpath($kernelPath);
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
     * Returns current GCC version.
     */
    private function getGccVersion(): string
    {
        if (Os::isWindows()) {
            return '0.0.0';
        }

        if ($this->filesystem->exists('gcc-version')) {
            return $this->filesystem->read('gcc-version');
        }

        $this->filesystem->system('gcc -dumpversion', 'stdout', 'gcc-version');
        $lines = $this->filesystem->file('gcc-version');
        $lines = array_filter($lines);

        $lastLine = $lines[count($lines) - 1];
        if (preg_match('/\d+\.\d+\.\d+/', $lastLine, $matches)) {
            return $matches[0];
        }

        return '0.0.0';
    }

    private function getWindowsReleaseDir(): string
    {
        if ($this->isZts()) {
            if (PHP_INT_SIZE === 4) {
                // 32-bit version of PHP
                return 'ext\\Release_TS';
            }

            if (PHP_INT_SIZE === 8) {
                // 64-bit version of PHP
                return 'ext\\x64\\Release_TS';
            }

            // fallback
            return 'ext\\Release_TS';
        }

        if (PHP_INT_SIZE === 4) {
            // 32-bit version of PHP
            return 'ext\\Release';
        }

        if (PHP_INT_SIZE === 8) {
            // 64-bit version of PHP
            return 'ext\\x64\\Release';
        }

        // fallback
        return 'ext\\Release';
    }

    private function isZts(): bool
    {
        if (defined('ZEND_THREAD_SAFE') && ZEND_THREAD_SAFE === true) {
            return true;
        }

        ob_start();
        phpinfo(INFO_GENERAL);

        return (bool)preg_match('/Thread\s*Safety\s*enabled/i', strip_tags(ob_get_clean()));
    }

    /**
     * Registers C-constants as PHP constants from a C-file.
     *
     * @param array $constantsSources
     *
     * @throws Exception
     */
    private function loadConstantsSources(array $constantsSources): void
    {
        foreach ($constantsSources as $constantsSource) {
            if (!file_exists($constantsSource)) {
                throw new Exception("File '" . $constantsSource . "' with constants definitions");
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
     * Pre-compiles classes creating a CompilerFile definition.
     *
     * @throws IllegalStateException
     */
    private function preCompile(string $filePath): void
    {
        if (!$this->parserManager->isAvailable()) {
            throw new IllegalStateException($this->parserManager->requirements());
        }

        if (preg_match('#\.zep$#', $filePath)) {
            $className = str_replace(DIRECTORY_SEPARATOR, '\\', $filePath);
            $className = preg_replace('#.zep$#', '', $className);

            $className = implode('\\', array_map('ucfirst', explode('\\', $className)));

            $compilerFile = $this->compilerFileFactory->create($className, $filePath);
            $compilerFile->preCompile($this);

            $this->files[$className]       = $compilerFile;
            $this->definitions[$className] = $compilerFile->getClassDefinition();
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
    private function processAddSources(array $sources, string $project): array
    {
        $groupSources = [];
        foreach ($sources as $source) {
            $dirName = str_replace(DIRECTORY_SEPARATOR, '/', dirname($source));
            if (!isset($groupSources[$dirName])) {
                $groupSources[$dirName] = [];
            }

            $groupSources[$dirName][] = basename($source);
        }

        $groups = [];
        foreach ($groupSources as $dirname => $files) {
            $groups[] = 'ADD_SOURCES(configure_module_dirname + "/'
                . $dirname
                . '", "'
                . implode(' ', $files)
                . '", "'
                . $project
                . '");';
        }

        return $groups;
    }

    /**
     * Recursively deletes files in a specified location.
     *
     * @param string $path Directory to deletes files
     * @param string $mask Regular expression to deletes files
     *
     * @deprecated
     *
     */
    private function recursiveDeletePath($path, $mask): void
    {
        if (!file_exists($path) || !is_dir($path) || !is_readable($path)) {
            $this->logger->warning("Directory '{$path}' is not readable. Skip...");

            return;
        }

        $objects = new RecursiveIteratorIterator(
            new RecursiveDirectoryIterator($path),
            RecursiveIteratorIterator::SELF_FIRST
        );

        foreach ($objects as $name => $object) {
            if (preg_match($mask, $name)) {
                @unlink($name);
            }
        }
    }

    /**
     * Recursively pre-compiles all sources found in the given path.
     *
     * @throws IllegalStateException
     * @throws InvalidArgumentException
     */
    private function recursivePreCompile(string $path): void
    {
        if (!is_dir($path)) {
            throw new InvalidArgumentException(
                sprintf(
                    "An invalid path was passed to the compiler. Unable to obtain the '%s%s%s' directory.",
                    getcwd(),
                    DIRECTORY_SEPARATOR,
                    $path
                )
            );
        }

        /**
         * Pre compile all files.
         */
        $iterator = new RecursiveIteratorIterator(
            new RecursiveDirectoryIterator($path),
            RecursiveIteratorIterator::SELF_FIRST
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
     * TODO:
     *
     * @param        $src
     * @param        $dest
     * @param string $pattern
     * @param mixed  $callback
     *
     * @return bool
     * @deprecated
     *
     */
    private function recursiveProcess($src, $dest, $pattern = null, $callback = 'copy')
    {
        $success  = true;
        $iterator = new DirectoryIterator($src);
        foreach ($iterator as $item) {
            $pathName = $item->getPathname();
            if (!is_readable($pathName)) {
                $this->logger->warning('File is not readable :' . $pathName);
                continue;
            }

            $fileName = $item->getFileName();

            if ($item->isDir()) {
                if ('.' != $fileName && '..' != $fileName && '.libs' != $fileName) {
                    if (!is_dir($dest . DIRECTORY_SEPARATOR . $fileName)) {
                        mkdir($dest . DIRECTORY_SEPARATOR . $fileName, 0755, true);
                    }
                    $this->recursiveProcess($pathName, $dest . DIRECTORY_SEPARATOR . $fileName, $pattern, $callback);
                }
            } elseif (!$pattern || ($pattern && 1 === preg_match($pattern, $fileName))) {
                $path    = $dest . DIRECTORY_SEPARATOR . $fileName;
                $success = $success && call_user_func($callback, $pathName, $path);
            }
        }

        return $success;
    }

    /**
     * Resolves path to the internal optimizers.
     *
     * @throws IllegalStateException in case of absence internal optimizers directory
     */
    private function resolveOptimizersPath(): ?string
    {
        $optimizersPath = $this->optimizersPath;

        // fallback
        if (empty($optimizersPath)) {
            $optimizersPath = __DIR__ . '/Optimizers';
        }

        if (!is_dir($optimizersPath) || !is_readable($optimizersPath)) {
            throw new IllegalStateException('Unable to resolve internal optimizers directory.');
        }

        return $optimizersPath;
    }

    /**
     * Resolves path to the internal prototypes.
     */
    private function resolvePrototypesPath(): ?string
    {
        $prototypesPath = $this->prototypesPath;

        // fallback
        if (empty($prototypesPath)) {
            $prototypesPath = dirname(__DIR__) . '/prototypes';
        }

        if (!is_dir($prototypesPath) || !is_readable($prototypesPath)) {
            throw new IllegalStateException('Unable to resolve internal prototypes directory.');
        }

        return $prototypesPath;
    }

    private function toUnixPaths(array $paths): array
    {
        return array_map(
            static fn(string $path): string => str_replace(DIRECTORY_SEPARATOR, '/', $path),
            $paths
        );
    }
}
