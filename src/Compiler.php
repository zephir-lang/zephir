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
use Zephir\Cache\PropertyCacheSlots;
use Zephir\Backend\FcallManagerInterface;
use Zephir\Backend\StringsManager;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Definition\TraitMerger;
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
use function array_values;
use function asort;
use function basename;
use function call_user_func;
use function chmod;
use function class_exists;
use function count;
use function defined;
use function dirname;
use function escapeshellarg;
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
use function is_file;
use function is_readable;
use function is_string;
use function json_encode;
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
use function str_contains;
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
use const PHP_PREFIX;
use const PHP_VERSION;
use const ZEND_THREAD_SAFE;
use const SORT_STRING;
use const STDERR;

final class Compiler
{
    use LoggerAwareTrait;

    /**
     * Cache entry holding the fingerprint of the toolchain and flags the
     * current `ext/Makefile` was configured with.
     */
    public const BUILD_FINGERPRINT = 'build-fingerprint';
    /**
     * Parallel `make` jobs to fall back on when the processor count cannot be
     * read. Deliberately low: it is what every build used before the count was
     * detected at all.
     */
    public const FALLBACK_JOBS = 2;
    /**
     * Cache entry left behind by a `generate()` that changed something
     * `configure` depends on. It is a file rather than in-memory state because
     * `zephir generate` and `zephir compile` are separate processes.
     */
    public const NEEDS_CONFIGURE = 'needs-configure';
    /**
     * Cache entry holding the pre-compiled header prelude. Lives in the cache
     * directory (git-ignored) rather than in `ext/`, because the companion
     * `.gch` is well over a hundred megabytes and `ext/` is a tracked
     * directory in some projects.
     *
     * Deliberately outside the per-version container: `Zephir::VERSION` carries
     * an unexpanded `$Id$` keyword, and this path is handed to `make`, which
     * would take the `$I` in it for a variable reference.
     */
    public const PCH_HEADER = 'pch/zephir_pch.h';
    /**
     * Kernel headers baked into the pre-compiled header. Every generated
     * translation unit includes a subset of these; a header missing from this
     * list is still compiled normally, it just does not get pre-compiled.
     */
    private const PCH_KERNEL_HEADERS = [
        'main.h',
        'memory.h',
        'fcall.h',
        'operators.h',
        'object.h',
        'array.h',
        'string.h',
        'concat.h',
        'math.h',
        'file.h',
        'iterator.h',
        'time.h',
        'exception.h',
        'variables.h',
        'filter.h',
        'require.h',
        'debug.h',
        'backtrace.h',
        'exit.h',
    ];

    /**
     * @var FunctionDefinition[]
     */
    public array  $functionDefinitions = [];
    /**
     * Function names already reported by the `missing-optimizer` warning, so the
     * report stays one line per function instead of one per call site.
     *
     * @var array<string, true>
     */
    public array  $reportedMissingOptimizers = [];
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
    /**
     * Forward declarations for the static-property initializers, emitted at
     * file scope in project.c instead of the per-class header. Keeps the
     * declaration and definition on the same translation unit so a single-file
     * (concatenated) build cannot end up with a static definition and a
     * non-static header prototype. See #2601.
     */
    private array          $internalInitializerHeaders = [];
    private static bool    $loadedPrototypes     = false;
    /**
     * Outcome of the last preCompileHeaders() in this process: the prelude to
     * force-include, or null when headers are not pre-compiled. Lets install()
     * reuse what compile() just built instead of spending another couple of
     * seconds on an identical `.gch`.
     */
    private ?string        $precompiledHeader    = null;
    /**
     * Flags the current `.gch` was built with, or null when there is none.
     * Reusing it is only sound for a build with the very same flags, since
     * that is all GCC validates a PCH against.
     */
    private ?string        $precompiledHeaderBuiltFor = null;
    /**
     * Proper-case root namespace of the first generator method found, or null
     * when the project contains no `yield`. Enables the kernel generator
     * runtime (ZEPHIR_GENERATOR_ENABLED) and names the <Ns>\Generator class.
     */
    private ?string        $generatorNamespace   = null;
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

        // Fresh inline-property-cache slot numbering per compilation run
        // (keeps slot indices stable/deterministic per build).
        PropertyCacheSlots::reset();

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
     * Records that the project contains at least one generator method, so the
     * kernel <Ns>\Generator runtime must be compiled in and registered.
     *
     * @param string $properCaseNamespace root namespace in source case, e.g. "Stub"
     */
    public function markGeneratorsInUse(string $properCaseNamespace): void
    {
        if (null === $this->generatorNamespace) {
            $this->generatorNamespace = $properCaseNamespace;
        }
    }

    public function getGeneratorNamespace(): ?string
    {
        return $this->generatorNamespace;
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
     */
    public function checkIfPhpized(): bool
    {
        return !file_exists('ext/Makefile');
    }

    /**
     * Compiles the extension without installing it.
     *
     * @param int|null          $jobs      Parallel `make` jobs. Defaults to the
     *                                     number of processors available.
     * @param PhpToolchain|null $toolchain PHP build tools to compile against.
     *                                     Defaults to the ones in the `PATH`.
     *
     * @throws Exception
     */
    public function compile(bool $development = false, ?int $jobs = null, ?PhpToolchain $toolchain = null): void
    {
        $jobs      = $this->resolveJobs($jobs);
        $toolchain = $toolchain ?? PhpToolchain::default();

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

        $reconfigure = Os::isWindows() || $this->needsConfigure($development, $toolchain);

        /**
         * Only dropped when everything is rebuilt anyway. On an incremental
         * build `make` would not put it back: the module is linked from
         * `modules/<ns>.la`, which is still up to date, so removing just the
         * `.so` leaves the project without one.
         */
        if ($reconfigure && file_exists("$currentDir/ext/modules/{$namespace}.so")) {
            unlink("$currentDir/ext/modules/{$namespace}.so");
        }

        if (Os::isWindows()) {
            // Prefer the dev-pack's phpize when %PHP_DEVPACK% is exported,
            // otherwise fall back to the `phpize` already on PATH (the PHP SDK
            // setup puts it there). Without this fallback a cold compile in an
            // environment that lacks %PHP_DEVPACK% runs `\phpize`, which Windows
            // reports as "The system cannot find the path specified." and never
            // produces the configure.js read further below.
            $phpize = getenv('PHP_DEVPACK') ? '%PHP_DEVPACK%\\phpize' : 'phpize';

            exec('cd ext && ' . $phpize . ' --clean', $output, $exit);

            $releaseFolder = $this->getWindowsReleaseDir();
            if (file_exists($releaseFolder)) {
                exec('rd /s /q ' . $releaseFolder, $output, $exit);
            }

            $this->logger->info('Preparing for PHP compilation...');
            exec('cd ext && ' . $phpize, $output, $exit);

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
        } elseif ($reconfigure) {
            $phpize = $toolchain->phpizeCommand();

            exec('cd ext && make clean && ' . $phpize . ' --clean', $output, $exit);
            $this->logger->info('Preparing for PHP compilation...');
            exec('cd ext && ' . $phpize, $output, $exit);
            $this->logger->info('Preparing configuration file...');

            exec(
                'cd ext && export CC="gcc" && export CFLAGS="' .
                $this->getGccFlags($development) .
                '" && ./configure --enable-' .
                $extensionName .
                $toolchain->configureOption()
            );

            $this->filesystem->write(
                self::BUILD_FINGERPRINT,
                $this->buildFingerprint($development, $toolchain)
            );

            if ($this->filesystem->exists(self::NEEDS_CONFIGURE)) {
                $this->filesystem->delete(self::NEEDS_CONFIGURE);
            }
        } else {
            /**
             * `generate()` rewrites a `.zep.c` only when its contents changed,
             * so `make` recompiles exactly the changed translation units.
             */
            $this->logger->info('Reusing the existing build configuration, compiling changed files only...');
        }

        $currentDir = getcwd();
        if (Os::isWindows()) {
            $this->logger->info('Compiling...');
            exec(
                'cd ext && nmake 2>' . $currentDir . '\compile-errors.log 1>' .
                $currentDir . '\compile.log',
                $output,
                $exit
            );
        } else {
            $this->logger->info(sprintf('Compiling with %d parallel job(s)...', $jobs));
            $extraCFlags = $this->makeVariableForPrelude(
                $this->preCompileHeaders($development, $toolchain),
                $currentDir . DIRECTORY_SEPARATOR . 'ext'
            );

            exec(
                'cd ext && (make -s -j' . $jobs . ' ' . $extraCFlags . ' 2>' . $currentDir .
                '/compile-errors.log 1>' .
                $currentDir .
                '/compile.log)',
                $output,
                $exit
            );
        }

        if ($exit !== 0) {
            throw new CompilerException("Compilation failed. Check compile-errors.log");
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
                /* Interfaces AND traits: both must register before any class init */
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
            '%REQ_INITIALIZER_HEADERS%' => $this->internalInitializerHeaders === []
                ? ''
                : implode(PHP_EOL, $this->internalInitializerHeaders) . PHP_EOL,
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
            '%GENERATOR_DEFINES%'        => $this->generatorDefines(),
        ];

        foreach ($toReplace as $mark => $replace) {
            $content = str_replace($mark, $replace, $content);
        }

        HardDisk::persistByHash($content, 'ext/php_' . $safeProject . '.h');
        unset($content);

        return $needConfigure;
    }

    /**
     * Emits the defines enabling the kernel generator runtime when the
     * project contains generator methods; empty otherwise (the runtime is
     * then compiled out entirely).
     */
    private function generatorDefines(): string
    {
        if (null === $this->generatorNamespace) {
            return '';
        }

        $generatorClass = strtolower($this->generatorNamespace . '\\Generator');
        foreach ($this->definitions as $completeName => $definition) {
            if (strtolower((string)$completeName) === $generatorClass) {
                throw new CompilerException(
                    'Class "' . $completeName . '" collides with the compiler-provided generator '
                    . 'runtime class registered for `yield` support. Rename the class.'
                );
            }
        }

        return '#define ZEPHIR_GENERATOR_ENABLED 1' . PHP_EOL
            . '#define ZEPHIR_GENERATOR_NAMESPACE "' . addslashes($this->generatorNamespace) . '"';
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
         * Load function optimizers.
         *
         * This must run before Round 2: the prototypes declare the classes of
         * extensions that are not loaded in the running PHP. Round 2 validates
         * `use` statements with `class_exists(..., false)`, so a prototype that
         * is loaded later makes an optional extension (redis, memcached, ...)
         * warn as nonexistent. See phalcon/cphalcon#17517.
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
         * Round 2. Check 'extends' and 'implements' dependencies
         */
        foreach ($this->files as $compileFile) {
            $compileFile->checkDependencies($this);
        }

        /**
         * Round 2.5. Copy trait members into every definition that uses them.
         * All definitions exist and trait names are resolved, but nothing has
         * consumed member tables yet — the only safe point to merge (#504).
         */
        $traitMergeContext           = new CompilationContext();
        $traitMergeContext->compiler = $this;
        $traitMergeContext->config   = $this->config;
        $traitMergeContext->logger   = $this->logger;
        $traitMergeContext->backend  = $this->backend;

        $traitMerger = new TraitMerger($this->files, $traitMergeContext);
        foreach ($this->files as $compileFile) {
            $classDefinition = $compileFile->getClassDefinition();
            if ($classDefinition !== null) {
                $traitMerger->merge($classDefinition);
            }
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
                        // File-scope forward declaration (mirror of Backend::getInternalSignature
                        // for the static initializer). Emitted in project.c, not the class header.
                        $this->internalInitializerHeaders[] = 'void ' . $method->getName() . '();';
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

        /**
         * Recorded for compile(), which reuses the existing `ext/Makefile`
         * when nothing that `configure` depends on has changed. Written from
         * here because `zephir generate` and `zephir compile` are separate
         * processes; cleared once configure has run.
         */
        if ($needConfigure) {
            $this->filesystem->write(self::NEEDS_CONFIGURE, Zephir::VERSION);
        }

        /**
         * Class headers were just rewritten, so whatever `.gch` exists no
         * longer describes them.
         */
        $this->precompiledHeaderBuiltFor = null;
        $this->precompiledHeader         = null;

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
     * Returns the php include directories reported by php-config.
     *
     * Falls back to the layout of the running PHP when php-config cannot be
     * reached, so that callers always get something usable.
     */
    public function getPhpIncludeDirs(?PhpToolchain $toolchain = null): string
    {
        $toolchain ??= PhpToolchain::default();

        $this->filesystem->system($toolchain->phpConfigCommand() . ' --includes', 'stdout', 'php-includes');
        $includes = trim($this->filesystem->read('php-includes'));

        if ('' !== $includes) {
            return $includes;
        }

        $root = PHP_PREFIX . '/include/php';

        return implode(' ', [
            '-I' . $root,
            '-I' . $root . '/main',
            '-I' . $root . '/TSRM',
            '-I' . $root . '/Zend',
            '-I' . $root . '/ext',
            '-I' . $root . '/ext/date/lib',
        ]);
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
     * The `make` here is a no-op relink after compile(), but `zephir install`
     * on its own reaches it with nothing built, in which case it is a full
     * build and the job count matters as much as it does in compile().
     *
     * @param int|null $jobs Parallel `make` jobs. Defaults to the number of
     *                       processors available.
     *
     * @throws Exception
     * @throws NotImplementedException
     * @throws CompilerException
     */
    public function install(bool $development = false, ?int $jobs = null): void
    {
        // Get global namespace
        $namespace  = str_replace('\\', '_', $this->checkDirectory());
        $currentDir = getcwd();

        if (Os::isWindows()) {
            throw new NotImplementedException('Installation is not implemented for Windows yet. Aborting.');
        }

        $jobs = $this->resolveJobs($jobs);

        $this->logger->info(sprintf('Installing with %d parallel job(s)...', $jobs));
        $gccFlags = $this->getGccFlags($development);

        /**
         * Same prelude compile() uses. Free when it ran in this process, and
         * worth the couple of seconds when `zephir install` is on its own and
         * this `make` turns out to be a real build.
         */
        $command = strtr(
        // TODO: Sort out with sudo
            'cd ext && export CC="gcc" && export CFLAGS=":cflags" && ' .
            'make -j:jobs :extra 2>> ":stderr" 1>> ":stdout" && ' .
            'sudo make install 2>> ":stderr" 1>> ":stdout"',
            [
                ':cflags' => $gccFlags,
                ':jobs'   => (string)$jobs,
                ':extra'  => $this->makeVariableForPrelude(
                    $this->preCompileHeaders($development),
                    $currentDir . DIRECTORY_SEPARATOR . 'ext'
                ),
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
     * Allows checking if a trait is part of the compiled extension.
     */
    public function isTrait(string $className): bool
    {
        foreach ($this->definitions as $key => $value) {
            if (!strcasecmp($key, $className) && Definition::TYPE_TRAIT === $value->getType()) {
                return true;
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
        /**
         * Canonical registry key. Class names are matched case-insensitively
         * everywhere else (see getClassDefinition()/isClass()), so the key only
         * needs to be stable, not an exact match of the on-disk casing.
         */
        $registryKey = implode(
            '\\',
            array_map('ucfirst', explode('\\', $className))
        );

        if (isset($this->files[$registryKey])) {
            return true;
        }

        $filePath = $this->locateExternalClassFile($className, $location);
        if ($filePath === null) {
            return false;
        }

        /** @var CompilerFile|CompilerFileAnonymous $compilerFile */
        $compilerFile = $this->compilerFileFactory->create($registryKey, $filePath);
        $compilerFile->setIsExternal(true);
        $compilerFile->preCompile($this);

        $this->files[$registryKey]       = $compilerFile;
        $this->definitions[$registryKey] = $compilerFile->getClassDefinition();

        return true;
    }

    /**
     * Resolves the `.zep` file for an external class, trying path casings in
     * priority order:
     *
     *   1. the namespace exactly as written — PSR-4 layouts require the
     *      directory/file casing to match the namespace, and on a
     *      case-sensitive filesystem this is the only form that resolves;
     *   2. a fully lower-cased path — backward compatibility with the historic
     *      all-lowercase behavior.
     *
     * Returns the first candidate that exists, or null when none match.
     *
     * @see https://github.com/zephir-lang/zephir/pull/2499
     */
    private function locateExternalClassFile(string $className, string $location): ?string
    {
        $relativePath = str_replace('\\', DIRECTORY_SEPARATOR, $className);

        $candidates = [$relativePath];
        $lowercased = strtolower($relativePath);
        if ($lowercased !== $relativePath) {
            $candidates[] = $lowercased;
        }

        foreach ($candidates as $candidate) {
            $filePath = $location . DIRECTORY_SEPARATOR . $candidate . '.zep';
            if (file_exists($filePath)) {
                return $filePath;
            }
        }

        return null;
    }

    /**
     * Pre-compiles the header prelude that every generated file includes.
     *
     * Each generated `.zep.c` opens with php.h plus `ext.h`, and `ext.h` pulls
     * in the header of *every* class in the project. A project with N classes
     * therefore parses N headers N times: for Phalcon that is 1672 headers and
     * 180k pre-processed lines per translation unit, ~0.9s of the ~0.95s it
     * takes to compile a median 1.8KB generated file.
     *
     * An earlier attempt gave up on GCC pre-compiled headers because a PCH is
     * only used when it is the first token of the translation unit, which a
     * generated file's own `#include <php.h>` prevents. A `-include` on the
     * command line *is* that first token, so the prelude can be pre-compiled
     * once and force-fed to all of them without touching the sources at all.
     * The header the sources include a second time is then skipped by its own
     * include guard.
     *
     * Objects come out identical (verified byte-for-byte); everything here is
     * fail-soft, because a rejected PCH only costs the speed-up. Set
     * `ZEPHIR_NO_PCH=1` to skip it, e.g. to reclaim the disk the `.gch` takes.
     *
     * @return string|null Prelude to force-include, or null to compile without one.
     */
    public function preCompileHeaders(bool $development = false, ?PhpToolchain $toolchain = null): ?string
    {
        if (Os::isWindows()) {
            // MSVC pre-compiles headers with /Yc + /Yu instead.
            return null;
        }

        if (getenv('ZEPHIR_NO_PCH')) {
            return null;
        }

        $extPath = getcwd() . DIRECTORY_SEPARATOR . 'ext';
        $toolchain ??= PhpToolchain::default();

        $prelude = $this->filesystem->path(self::PCH_HEADER, false);
        $gch     = $prelude . '.gch';

        /**
         * make expands the recipe and hands it to a shell, unquoted, so a path
         * a shell would chew on has to be left alone. Losing the speed-up
         * beats mangling every compile command.
         */
        if (!self::isShellSafePath($prelude)) {
            $this->logger->info('Headers are not pre-compiled: the build path needs quoting');

            return null;
        }

        if (!is_dir(dirname($prelude))) {
            mkdir(dirname($prelude), 0755, true);
        }

        $flags = $this->precompiledHeaderFlags($development, $extPath, $toolchain);

        /**
         * Already built in this very process, for these very flags — which is
         * what install() hits right after compile(). Nothing in between
         * rewrites a header: generate() invalidates this.
         */
        if ($flags === $this->precompiledHeaderBuiltFor && (null === $this->precompiledHeader || is_file($gch))) {
            return $this->precompiledHeader;
        }

        file_put_contents($prelude, $this->precompiledHeaderSource($this->precompiledKernelHeaders($extPath)));

        /**
         * A stale .gch would be trusted blindly by GCC, so it is always
         * rebuilt from the current headers. That costs ~2s against the ~330s
         * it saves on a project the size of Phalcon.
         */
        if (is_file($gch)) {
            unlink($gch);
        }

        $this->precompiledHeaderBuiltFor = $flags;
        $this->precompiledHeader         = null;

        $this->logger->info('Pre-compiling headers...');
        exec(
            sprintf('gcc %s -x c-header %s -o %s 2>&1', $flags, escapeshellarg($prelude), escapeshellarg($gch)),
            $output,
            $exit
        );

        if (0 !== $exit || !is_file($gch)) {
            $this->logger->info('Headers could not be pre-compiled, compiling without them');

            return null;
        }

        if (!$this->precompiledHeaderIsUsable($prelude, $flags)) {
            $this->logger->info('Pre-compiled headers were rejected by the compiler, compiling without them');
            unlink($gch);

            return null;
        }

        $this->precompiledHeader = $prelude;

        return $prelude;
    }

    /**
     * The `make` argument that force-includes the pre-compiled prelude, or an
     * empty string when there is none to include.
     *
     * `EXTRA_CFLAGS` is on every compile line of a phpize Makefile, and the
     * `-I` keeps the prelude's own `#include "php_ext.h"` resolvable from
     * outside ext/.
     */
    private function makeVariableForPrelude(?string $prelude, string $extPath): string
    {
        if (null === $prelude) {
            return '';
        }

        return 'EXTRA_CFLAGS=' . escapeshellarg('-include ' . $prelude . ' -I' . $extPath);
    }

    /**
     * Whether a path survives being pasted into a Makefile recipe, which make
     * expands and then hands to `/bin/sh` without quoting anything.
     */
    public static function isShellSafePath(string $path): bool
    {
        return 1 === preg_match('#^[A-Za-z0-9/._+@:=-]+$#', $path);
    }

    /**
     * The prelude source: the header block every generated file opens with,
     * plus the kernel headers they pick from.
     *
     * @param string[] $kernelHeaders Kernel header file names, e.g. `main.h`.
     *
     * @see \Zephir\Traits\CompilerTrait::generateCodeHeadersPre() Emitter this mirrors.
     */
    public function precompiledHeaderSource(array $kernelHeaders): string
    {
        $code = '/* Generated by Zephir to pre-compile the header prelude. Do not edit. */' . PHP_EOL;
        $code .= '#ifdef HAVE_CONFIG_H' . PHP_EOL;
        $code .= '#include "ext_config.h"' . PHP_EOL;
        $code .= '#endif' . PHP_EOL . PHP_EOL;
        $code .= '#include <php.h>' . PHP_EOL;
        $code .= '#include "php_ext.h"' . PHP_EOL;
        $code .= '#include "ext.h"' . PHP_EOL . PHP_EOL;
        $code .= '#include <Zend/zend_operators.h>' . PHP_EOL;
        $code .= '#include <Zend/zend_exceptions.h>' . PHP_EOL;
        $code .= '#include <Zend/zend_interfaces.h>' . PHP_EOL . PHP_EOL;

        foreach ($kernelHeaders as $header) {
            $code .= '#include "kernel/' . $header . '"' . PHP_EOL;
        }

        return $code;
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
     * Fingerprint of everything the generated `ext/Makefile` was configured
     * with. A mismatch means the objects on disk were built for another PHP,
     * another php-config or other flags, and must not be reused.
     *
     * This is what makes skipping `configure` safe when several PHP versions
     * build the very same project directory in turn.
     */
    private function buildFingerprint(bool $development, PhpToolchain $toolchain): string
    {
        $phpConfig = $toolchain->phpConfigCommand();

        $this->filesystem->system($phpConfig . ' --version', 'stdout', 'php-config-version');
        $this->filesystem->system($phpConfig . ' --extension-dir', 'stdout', 'php-extension-dir');

        return self::fingerprintOf([
            'zephir'        => Zephir::VERSION,
            'development'   => $development,
            'gcc-flags'     => $this->getGccFlags($development),
            'phpize'        => $toolchain->phpizeCommand(),
            'configure'     => $toolchain->configureOption(),
            'php-version'   => trim($this->filesystem->read('php-config-version')) ?: PHP_VERSION,
            'extension-dir' => trim($this->filesystem->read('php-extension-dir')),
            'zts'           => ZEND_THREAD_SAFE,
            'int-size'      => PHP_INT_SIZE,
        ]);
    }

    /**
     * Hashes the parts a build fingerprint is made of.
     */
    public static function fingerprintOf(array $parts): string
    {
        return md5(json_encode($parts));
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
                $previousVersion = $this->filesystem->getPreviousVersion();
                if (null !== $previousVersion) {
                    $this->logger->info(sprintf(
                        'Zephir version changed (%s -> %s), use "zephir fullclean" to perform a full clean of the project',
                        $previousVersion,
                        $this->filesystem->getVersion()
                    ));
                } else {
                    $this->logger->info(
                        'Zephir version has changed, use "zephir fullclean" to perform a full clean of the project'
                    );
                }
            }

            $this->filesystem->makeDirectory('.');
        }

        return $namespace;
    }

    /**
     * Checks if a file must be copied.
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
     *
     * The cached and the freshly queried value go through the very same
     * parsing. They used not to: the cache held the raw `gcc -dumpversion`
     * output, so a first build parsed `14` into `0.0.0` while every build after
     * it compared the raw `14`, and the two disagreed about which flags the
     * project is built with.
     */
    private function getGccVersion(): string
    {
        if (Os::isWindows()) {
            return '0.0.0';
        }

        if (!$this->filesystem->exists('gcc-version')) {
            $this->filesystem->system('gcc -dumpversion', 'stdout', 'gcc-version');
        }

        return self::parseGccVersion($this->filesystem->file('gcc-version'));
    }

    /**
     * The version out of `gcc -dumpversion` output, or `0.0.0` when it cannot
     * be read as one.
     *
     * Note that since GCC 7 `-dumpversion` prints the major version alone
     * (`14`), which is not a version this recognizes; `-dumpfullversion` is the
     * option that still prints `14.2.0`. Switching to it would turn on the
     * `-flto` branch of getGccFlags() for every modern toolchain, so it is
     * left as a deliberate, separate decision.
     *
     * @param string[] $lines
     */
    public static function parseGccVersion(array $lines): string
    {
        foreach (array_filter($lines) as $line) {
            if (preg_match('/\d+\.\d+\.\d+/', $line, $matches)) {
                return $matches[0];
            }
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
     * Whether `phpize` and `configure` have to run, wiping every object file
     * with them.
     *
     * They only have to when something they produce is out of date: a changed
     * `config.m4` or project file, a class added or removed, a missing
     * Makefile, or a different toolchain than the objects were built with.
     * Otherwise the existing Makefile is reused and `make` recompiles just the
     * translation units whose `.zep.c` actually changed.
     */
    private function needsConfigure(bool $development, PhpToolchain $toolchain): bool
    {
        if ($this->filesystem->exists(self::NEEDS_CONFIGURE) || $this->checkIfPhpized()) {
            return true;
        }

        // A Makefile without config.h means configure never got to the end.
        if (!is_file('ext' . DIRECTORY_SEPARATOR . 'config.h')) {
            return true;
        }

        if (!$this->filesystem->exists(self::BUILD_FINGERPRINT)) {
            return true;
        }

        return $this->filesystem->read(self::BUILD_FINGERPRINT)
            !== $this->buildFingerprint($development, $toolchain);
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
     * Compiler flags the pre-compiled header is built with.
     *
     * A PCH is only accepted for a translation unit compiled with the same
     * flags, so these are taken from the generated Makefile wherever possible
     * rather than derived a second time: `CFLAGS` and `INCLUDES` are what its
     * recipes pass, already fully expanded. The rest of a recipe's compile
     * line is added here: the extension's own include dirs (`-I.` relative to
     * ext/), `$(DEFS)`, the `-D_GNU_SOURCE` of `CFLAGS_CLEAN`,
     * ZEND_COMPILE_DL_EXT, and libtool's -fPIC -DPIC.
     *
     * The fallbacks matter only before the project has ever been configured.
     */
    private function precompiledHeaderFlags(bool $development, string $extPath, PhpToolchain $toolchain): string
    {
        $includes = $this->makefileVariable($extPath, 'INCLUDES') ?? $this->getPhpIncludeDirs($toolchain);
        $cflags   = $this->makefileVariable($extPath, 'CFLAGS') ?? $this->getGccFlags($development);

        return implode(' ', [
            '-I' . escapeshellarg($extPath),
            '-I' . escapeshellarg($extPath . DIRECTORY_SEPARATOR . 'include'),
            '-I' . escapeshellarg($extPath . DIRECTORY_SEPARATOR . 'main'),
            $includes,
            '-DHAVE_CONFIG_H',
            '-D_GNU_SOURCE',
            '-DZEND_COMPILE_DL_EXT=1',
            $cflags,
            '-fPIC',
            '-DPIC',
        ]);
    }

    /**
     * A variable of the generated `ext/Makefile`, or null when there is no
     * Makefile yet or it does not define one.
     */
    private function makefileVariable(string $extPath, string $name): ?string
    {
        $makefile = $extPath . DIRECTORY_SEPARATOR . 'Makefile';

        if (!is_file($makefile)) {
            return null;
        }

        return self::parseMakefileVariable(file_get_contents($makefile), $name);
    }

    /**
     * Reads one variable off a Makefile's contents. Only the plain
     * `NAME = value` form the phpize Makefile uses is understood, and a value
     * still holding a `$(reference)` is refused rather than passed on
     * unexpanded.
     */
    public static function parseMakefileVariable(string $contents, string $name): ?string
    {
        if (!preg_match('/^' . preg_quote($name, '/') . '[ \t]*=[ \t]*(.*)$/m', $contents, $matches)) {
            return null;
        }

        $value = trim($matches[1]);

        if ('' === $value || str_contains($value, '$(')) {
            return null;
        }

        return $value;
    }

    /**
     * Whether the compiler actually uses the pre-compiled header just built.
     *
     * GCC falls back to parsing the headers when it rejects a `.gch`, silently
     * unless asked with -Winvalid-pch. The probe is a minimal translation unit
     * shaped like a generated one: same header prelude, force-included PCH.
     */
    private function precompiledHeaderIsUsable(string $prelude, string $flags): bool
    {
        $probe = $this->filesystem->path('pch/zephir_pch_probe.c', false);

        file_put_contents(
            $probe,
            $this->precompiledHeaderSource([]) . PHP_EOL . 'int zephir_pch_probe(void) { return 0; }' . PHP_EOL
        );

        exec(
            sprintf(
                'gcc %s -Winvalid-pch -include %s -c %s -o %s 2>&1',
                $flags,
                escapeshellarg($prelude),
                escapeshellarg($probe),
                escapeshellarg($probe . '.o')
            ),
            $output,
            $exit
        );

        unlink($probe);
        if (is_file($probe . '.o')) {
            unlink($probe . '.o');
        }

        // Any diagnostic naming the .gch means it was found but not used.
        return 0 === $exit && !str_contains(implode(PHP_EOL, $output), '.gch');
    }

    /**
     * The kernel headers present in the project, in the order they are
     * pre-compiled. Headers this Zephir does not know about are left out; they
     * are still compiled normally by the translation units needing them.
     *
     * @return string[]
     */
    private function precompiledKernelHeaders(string $extPath): array
    {
        $kernelPath = $extPath . DIRECTORY_SEPARATOR . 'kernel' . DIRECTORY_SEPARATOR;

        return array_values(array_filter(
            self::PCH_KERNEL_HEADERS,
            static fn(string $header): bool => is_file($kernelPath . $header)
        ));
    }

    /**
     * How many translation units to compile at once.
     *
     * The C compilation of an extension is entirely CPU bound — measured at 96%
     * parallel efficiency on a four-core machine — so an explicit `--jobs`
     * aside, one job per processor is the right default. Builds used to be
     * pinned to two jobs no matter how many processors were idle.
     */
    private function resolveJobs(?int $jobs): int
    {
        if (null !== $jobs && $jobs > 0) {
            return $jobs;
        }

        return $this->detectProcessorCount();
    }

    /**
     * The number of processors available to this build, or the fallback when no
     * source can tell.
     */
    private function detectProcessorCount(): int
    {
        if (Os::isWindows()) {
            return self::parseProcessorCount((string)getenv('NUMBER_OF_PROCESSORS')) ?: self::FALLBACK_JOBS;
        }

        // nproc honors the CPU affinity mask, so a build confined to a subset
        // of the processors does not oversubscribe them.
        foreach (['nproc', 'getconf _NPROCESSORS_ONLN', 'sysctl -n hw.ncpu'] as $command) {
            exec($command . ' 2>/dev/null', $output, $exit);
            $count = 0 === $exit && isset($output[0]) ? self::parseProcessorCount($output[0]) : 0;
            $output = [];

            if ($count > 0) {
                return $count;
            }
        }

        return self::FALLBACK_JOBS;
    }

    /**
     * Reads a processor count off a command's output, or 0 when it does not
     * hold one.
     */
    public static function parseProcessorCount(string $raw): int
    {
        if (!preg_match('/^\s*(\d+)/', $raw, $matches)) {
            return 0;
        }

        return (int)$matches[1];
    }

    /**
     * Process config.w32 sections.
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
