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

use Psr\Log\LoggerAwareTrait;
use Psr\Log\NullLogger;
use ReflectionException;
use Zephir\Class\Constant;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Definition\DefinitionRuntime;
use Zephir\Class\Method\Method;
use Zephir\Class\Method\Parameters;
use Zephir\Class\Property;
use Zephir\Code\Printer;
use Zephir\Compiler\FileInterface;
use Zephir\Documentation\DocblockParser;
use Zephir\Exception\CompilerException;
use Zephir\Exception\IllegalStateException;
use Zephir\Exception\ParseException;
use Zephir\FileSystem\FileSystemInterface;
use Zephir\Traits\CompilerTrait;

use function array_map;
use function count;
use function explode;
use function file_exists;
use function file_put_contents;
use function filemtime;
use function is_array;
use function json_decode;
use function json_encode;
use function md5;
use function preg_match;
use function realpath;
use function sprintf;
use function str_replace;
use function strtolower;
use function substr;
use function trim;

use const DIRECTORY_SEPARATOR;
use const JSON_PRETTY_PRINT;
use const PHP_EOL;

/**
 * This class represents every file compiled in a project.
 * Every file may contain a class or an interface.
 */
final class CompilerFile implements FileInterface
{
    use CompilerTrait;
    use LoggerAwareTrait;

    /**
     * @var AliasManager
     */
    private AliasManager $aliasManager;
    /**
     * @var Definition|null
     */
    private ?Definition $classDefinition = null;
    private ?string     $className       = null;
    /**
     * @var string|null
     */
    private ?string $compiledFile = null;
    /**
     * @var Config
     */
    private Config $config;
    /**
     * @var bool
     */
    private bool $external = false;
    /**
     * @var string|null
     */
    private ?string $filePath = null;
    /**
     * @var FileSystemInterface
     */
    private FileSystemInterface $filesystem;
    /**
     * @var FunctionDefinition[]
     */
    private array $functionDefinitions = [];

    /**
     * @var array
     */
    private array $headerCBlocks = [];
    /**
     * Original internal representation (IR) of the file.
     *
     * @var array|null
     */
    private ?array  $ir        = null;
    private ?string $namespace = null;
    /**
     * @var mixed
     */
    private $originalNode;

    /**
     * CompilerFile constructor.
     *
     * @param Config              $config
     * @param AliasManager        $aliasManager
     * @param FileSystemInterface $filesystem
     */
    public function __construct(
        Config $config,
        AliasManager $aliasManager,
        FileSystemInterface $filesystem
    ) {
        $this->config       = $config;
        $this->logger       = new NullLogger();
        $this->aliasManager = $aliasManager;
        $this->filesystem   = $filesystem;
    }

    /**
     * Adds a function to the function definitions.
     *
     * @param Compiler           $compiler
     * @param FunctionDefinition $func
     * @param array              $statement
     *
     * @throws CompilerException
     */
    public function addFunction(Compiler $compiler, FunctionDefinition $func, array $statement = []): void
    {
        $compiler->addFunction($func, $statement);
        $funcName = strtolower($func->getInternalName());
        if (isset($this->functionDefinitions[$funcName])) {
            throw new CompilerException(
                sprintf("Function '%s' was defined more than one time (in the same file)", $func->getName()),
                $statement
            );
        }

        $this->functionDefinitions[$funcName] = $func;
    }

    public function applyClassHeaders(CompilationContext $compilationContext): void
    {
        $classDefinition = $this->classDefinition;

        $code = $this->generateCodeHeadersPre($classDefinition);

        if ('class' == $classDefinition->getType()) {
            $code .= '#include <Zend/zend_operators.h>' . PHP_EOL;
            $code .= '#include <Zend/zend_exceptions.h>' . PHP_EOL;
            $code .= '#include <Zend/zend_interfaces.h>' . PHP_EOL;
        } else {
            $code .= '#include <Zend/zend_exceptions.h>' . PHP_EOL;
        }

        $this->generateClassHeadersPost($code, $classDefinition, $compilationContext);
    }

    /**
     * Check dependencies.
     *
     * @param Compiler $compiler
     *
     * @throws ReflectionException
     */
    public function checkDependencies(Compiler $compiler): void
    {
        $classDefinition = $this->classDefinition;

        $extendedClass = $classDefinition->getExtendsClass();
        if ($extendedClass) {
            if ('class' == $classDefinition->getType()) {
                if ($compiler->isClass($extendedClass)) {
                    $extendedDefinition = $compiler->getClassDefinition($extendedClass);
                    $classDefinition->setExtendsClassDefinition($extendedDefinition);
                } else {
                    if ($compiler->isBundledClass($extendedClass)) {
                        $extendedDefinition = $compiler->getInternalClassDefinition($extendedClass);
                        $classDefinition->setExtendsClassDefinition($extendedDefinition);
                    } else {
                        $extendedDefinition = new DefinitionRuntime($extendedClass);
                        $classDefinition->setExtendsClassDefinition($extendedDefinition);

                        $this->logger->warning(
                            sprintf(
                                'Cannot locate class "%s" when extending class "%s"',
                                $extendedClass,
                                $classDefinition->getCompleteName()
                            ),
                            ['nonexistent-class', $this->originalNode]
                        );
                    }
                }
            } else {
                // Whether the $extendedClass is part of the compiled extension
                if ($compiler->isInterface($extendedClass)) {
                    $extendedDefinition = $compiler->getClassDefinition($extendedClass);
                    $classDefinition->setExtendsClassDefinition($extendedDefinition);
                } else {
                    if ($compiler->isBundledInterface($extendedClass)) {
                        $extendedDefinition = $compiler->getInternalClassDefinition($extendedClass);
                        $classDefinition->setExtendsClassDefinition($extendedDefinition);
                    } else {
                        $extendedDefinition = new DefinitionRuntime($extendedClass);
                        $classDefinition->setExtendsClassDefinition($extendedDefinition);

                        $this->logger->warning(
                            sprintf(
                                'Cannot locate class "%s" when extending interface "%s"',
                                $extendedClass,
                                $classDefinition->getCompleteName()
                            ),
                            ['nonexistent-class', $this->originalNode]
                        );
                    }
                }
            }
        }

        $interfaceDefinitions = [];
        foreach ($classDefinition->getImplementedInterfaces() as $interface) {
            if ($compiler->isInterface($interface)) {
                $interfaceDefinitions[$interface] = $compiler->getClassDefinition($interface);
            } else {
                if ($compiler->isBundledInterface($interface)) {
                    $interfaceDefinitions[$interface] = $compiler->getInternalClassDefinition($interface);
                } else {
                    if ($extendedClass !== null) {
                        $classDefinition->setExtendsClassDefinition(new DefinitionRuntime($extendedClass));
                    }

                    $this->logger->warning(
                        sprintf(
                            'Cannot locate class "%s" when extending interface "%s"',
                            $interface,
                            $classDefinition->getCompleteName()
                        ),
                        ['nonexistent-class', $this->originalNode]
                    );
                }
            }
        }

        if (count($interfaceDefinitions) > 0) {
            $classDefinition->setImplementedInterfaceDefinitions($interfaceDefinitions);
        }
    }

    /**
     * Compiles the file.
     *
     * @param Compiler       $compiler
     * @param StringsManager $stringsManager
     *
     * @throws CompilerException
     */
    public function compile(Compiler $compiler, StringsManager $stringsManager): void
    {
        if (!$this->ir) {
            throw new CompilerException('Unable to locate the intermediate representation of the compiled file');
        }

        /**
         * External classes should not be compiled as part of the extension
         */
        if ($this->external) {
            return;
        }

        /**
         * Compilation context stores common objects required by compilation entities.
         */
        $compilationContext = new CompilationContext();

        /**
         * Set global compiler in the compilation context
         */
        $compilationContext->compiler = $compiler;

        /**
         * Set global config in the compilation context
         */
        $compilationContext->config = $this->config;

        /**
         * Set global logger in the compilation context
         */
        $compilationContext->logger = $this->logger;

        /**
         * Set global strings manager
         */
        $compilationContext->stringsManager = $stringsManager;
        $compilationContext->backend        = $compiler->backend;

        /**
         * Headers manager.
         */
        $headersManager                     = new HeadersManager();
        $compilationContext->headersManager = $headersManager;

        /**
         * Main code-printer for the file.
         */
        $codePrinter                     = new Printer();
        $compilationContext->codePrinter = $codePrinter;

        /**
         * Alias manager
         */
        $compilationContext->aliasManager = $this->aliasManager;

        $codePrinter->outputBlankLine();

        $class     = false;
        $interface = false;

        foreach ($this->ir as $topStatement) {
            switch ($topStatement['type']) {
                case 'class':
                case 'interface':
                    if ($interface || $class) {
                        throw new CompilerException('More than one class defined in the same file', $topStatement);
                    }
                    $class = true;
                    $this->compileClass($compilationContext);
                    break;

                case 'comment':
                    $this->compileComment($compilationContext, $topStatement);
                    break;

                default:
                    break;
            }
        }

        /* ensure functions are handled last */
        foreach ($this->functionDefinitions as $funcDef) {
            $this->compileFunction($compilationContext, $funcDef);
        }

        /* apply headers */
        $this->applyClassHeaders($compilationContext);

        $classDefinition = $this->classDefinition;
        if (!$classDefinition) {
            $this->ir = null;

            return;
        }

        $classDefinition->setOriginalNode($this->originalNode);

        $completeName = $classDefinition->getCompleteName();

        [$path, $filePath, $filePathHeader] = $this->calculatePaths($completeName);

        /**
         * If the file does not exist we create it for the first time
         */
        if (!file_exists($filePath)) {
            file_put_contents($filePath, $codePrinter->getOutput());
            if ($compilationContext->headerPrinter) {
                file_put_contents($filePathHeader, $compilationContext->headerPrinter->getOutput());
            }
        } else {
            /**
             * Use md5 hash to avoid rewrite the file again and again when it hasn't changed
             * thus avoiding unnecessary recompilations.
             */
            $output = $codePrinter->getOutput();
            $hash   = $this->filesystem->getHashFile('md5', $filePath, true);
            if (md5($output) != $hash) {
                file_put_contents($filePath, $output);
            }

            if ($compilationContext->headerPrinter) {
                $output = $compilationContext->headerPrinter->getOutput();
                $hash   = $this->filesystem->getHashFile('md5', $filePathHeader, true);
                if (md5($output) != $hash) {
                    file_put_contents($filePathHeader, $output);
                }
            }
        }

        /**
         * Add to file compiled
         */
        $this->compiledFile = $path . '.c';
        $this->ir           = null;
    }

    /**
     * Compiles the class/interface contained in the file.
     *
     * @param CompilationContext $compilationContext
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compileClass(CompilationContext $compilationContext): void
    {
        $this->classDefinition->compile($compilationContext);
    }

    /**
     * Compiles a comment as a top-level statement.
     *
     * @param CompilationContext $compilationContext
     * @param array              $topStatement
     */
    public function compileComment(CompilationContext $compilationContext, $topStatement): void
    {
        $compilationContext->codePrinter->output('/' . $topStatement['value'] . '/');
    }

    /**
     * Compiles a function.
     *
     * @param CompilationContext $compilationContext
     * @param FunctionDefinition $functionDefinition
     *
     * @throws Exception
     */
    public function compileFunction(
        CompilationContext $compilationContext,
        FunctionDefinition $functionDefinition
    ): void {
        /** Make sure we do not produce calls like ZEPHIR_CALL_SELF */
        $bakClassDefinition                  = $compilationContext->classDefinition;
        $compilationContext->classDefinition = null;
        $compilationContext->currentMethod   = $functionDefinition;

        $codePrinter = $compilationContext->codePrinter;
        $codePrinter->output('PHP_FUNCTION(' . $functionDefinition->getInternalName() . ') {');
        $functionDefinition->compile($compilationContext);
        $codePrinter->output('}');
        $codePrinter->outputBlankLine();

        /* Restore */
        $compilationContext->classDefinition = $bakClassDefinition;
        $compilationContext->currentMethod   = null;
    }

    /**
     * Compiles the file generating a JSON intermediate representation.
     *
     * @param Compiler $compiler
     *
     * @return array
     *
     * @throws ParseException
     * @throws IllegalStateException if the intermediate representation is not of type 'array'
     */
    public function genIR(Compiler $compiler): array
    {
        $normalizedPath = $this->filesystem->normalizePath($this->filePath);

        $compilePath = "{$normalizedPath}.json";
        $zepRealPath = realpath($this->filePath);

        if ($this->filesystem->exists($compilePath)) {
            if ($this->filesystem->modificationTime($compilePath) < filemtime($zepRealPath)) {
                $this->filesystem->delete($compilePath);
            }
        }

        if (!$this->filesystem->exists($compilePath)) {
            $parser = $compiler->getParserManager()->getParser();
            $ir     = $parser->parse($zepRealPath);
            $this->filesystem->write($compilePath, json_encode($ir, JSON_PRETTY_PRINT));
        } else {
            $ir = json_decode($this->filesystem->read($compilePath), true);
        }

        return $ir;
    }

    /**
     * {@inheritdoc}
     *
     * @return Definition
     */
    public function getClassDefinition()
    {
        $this->classDefinition->setAliasManager($this->aliasManager);

        return $this->classDefinition;
    }

    /**
     * Returns the path to the compiled file.
     *
     * @return string
     */
    public function getCompiledFile(): string
    {
        return $this->compiledFile;
    }

    /**
     * Returns the path to the source file.
     *
     * @return string
     */
    public function getFilePath(): string
    {
        return $this->filePath;
    }

    /**
     * @return bool
     */
    public function isExternal(): bool
    {
        return $this->external;
    }

    /**
     * Pre-compiles a Zephir file.
     *
     * Generates the IR and perform basic validations.
     *
     * @param Compiler $compiler
     *
     * @throws CompilerException
     * @throws IllegalStateException
     * @throws ParseException
     */
    public function preCompile(Compiler $compiler): void
    {
        $ir = $this->genIR($compiler);

        if (isset($ir['type']) && 'error' == $ir['type']) {
            throw new ParseException($ir['message'], $ir);
        }

        /**
         * Compilation context stores common objects required by compilation entities.
         */
        $compilationContext               = new CompilationContext();
        $compilationContext->compiler     = $compiler;
        $compilationContext->config       = $this->config;
        $compilationContext->logger       = $this->logger;
        $compilationContext->aliasManager = $this->aliasManager;
        $compilationContext->backend      = $compiler->backend;

        /**
         * Traverse the top level statements looking for the namespace.
         */
        $namespace = '';

        foreach ($ir as $topStatement) {
            switch ($topStatement['type']) {
                case 'namespace':
                    if ($namespace !== '') {
                        throw new CompilerException('The namespace must be defined just one time', $topStatement);
                    }

                    $namespace       = $topStatement['name'];
                    $this->namespace = $namespace;

                    if (!preg_match('/^[A-Z]/', $namespace)) {
                        throw new CompilerException(
                            "Namespace '{$namespace}' must be in camelized-form",
                            $topStatement
                        );
                    }

                    break;

                case 'cblock':
                    $this->headerCBlocks[] = $topStatement['value'];
                    break;

                case 'function':
                    $statements = null;
                    if (isset($topStatement['statements'])) {
                        $statements = new StatementsBlock($topStatement['statements']);
                    }

                    $parameters = null;
                    if (isset($topStatement['parameters'])) {
                        $parameters = new Parameters($topStatement['parameters']);
                    }

                    $returnType = null;
                    if (isset($topStatement['return-type'])) {
                        $returnType = $topStatement['return-type'];
                    }

                    // Just do the pre-compilation of the function
                    $functionDefinition = new FunctionDefinition(
                        $namespace,
                        $topStatement['name'],
                        $parameters,
                        $statements,
                        $returnType,
                        $topStatement
                    );
                    $functionDefinition->preCompile($compilationContext);
                    $this->addFunction($compiler, $functionDefinition, $topStatement);
                    break;
            }
        }

        if (!$namespace) {
            throw new CompilerException('A namespace is required', $topStatement ?? null);
        }

        // Set namespace and flag as global, if before namespace declaration
        foreach ($this->functionDefinitions as $funcDef) {
            if (null == $funcDef->getNamespace()) {
                $funcDef->setGlobal(true);
                $funcDef->setNamespace($this->config->get('namespace'));
            }
        }

        $name        = null;
        $class       = false;
        $interface   = false;
        $lastComment = null;

        foreach ($ir as $topStatement) {
            switch ($topStatement['type']) {
                case 'class':
                    if ($class || $interface) {
                        throw new CompilerException(
                            'More than one class/interface defined in the same file',
                            $topStatement
                        );
                    }
                    $class = true;
                    $name  = $topStatement['name'];
                    $this->preCompileClass($compilationContext, $namespace, $topStatement, $lastComment);
                    $this->originalNode = $topStatement;
                    $lastComment        = null;
                    break;

                case 'interface':
                    if ($class || $interface) {
                        throw new CompilerException(
                            'More than one class/interface defined in the same file',
                            $topStatement
                        );
                    }
                    $interface = true;
                    $name      = $topStatement['name'];
                    $this->preCompileInterface($namespace, $topStatement, $lastComment);
                    $this->originalNode = $topStatement;
                    $lastComment        = null;
                    break;

                case 'use':
                    if ($interface || $class) {
                        throw new CompilerException(
                            'Aliasing must be done before declaring any class or interface',
                            $topStatement
                        );
                    }
                    $this->aliasManager->add($topStatement);
                    break;

                case 'comment':
                    $lastComment = $topStatement;
                    break;
            }
        }

        if (!$class && !$interface) {
            throw new CompilerException(
                'Every file must contain at least a class or an interface',
                $topStatement ?? null
            );
        }

        if (!$this->external) {
            $expectedPath = strtolower(
                str_replace('\\', DIRECTORY_SEPARATOR, $namespace) . DIRECTORY_SEPARATOR . $name . '.zep'
            );

            if (strtolower($this->filePath) != $expectedPath) {
                $className = $namespace . '\\' . $name;

                throw new CompilerException(
                    sprintf(
                        "Unexpected class name '%s' in file: '%s', expected: '%s'",
                        $className,
                        $this->filePath,
                        $expectedPath
                    )
                );
            }
        }

        if ($compilationContext->classDefinition) {
            if ($extendsClass = $compilationContext->classDefinition->getExtendsClass()) {
                $compiler->isClass($extendsClass);
            }
            if ($interfaces = $compilationContext->classDefinition->getImplementedInterfaces()) {
                foreach ($interfaces as $interface) {
                    $compiler->isInterface($interface);
                }
            }
        }

        $this->ir = $ir;
    }

    /**
     * Creates a definition for a class.
     *
     * @param CompilationContext $compilationContext
     * @param string             $namespace
     * @param array              $topStatement
     * @param array              $docblock
     */
    public function preCompileClass(CompilationContext $compilationContext, $namespace, $topStatement, $docblock): void
    {
        $classDefinition = new Definition($namespace, $topStatement['name']);
        $classDefinition->setIsExternal($this->external);

        if (isset($topStatement['extends'])) {
            $classDefinition->setExtendsClass($this->getFullName($topStatement['extends']));
        }

        if (isset($topStatement['implements'])) {
            foreach ($topStatement['implements'] as &$implement) {
                $implement['value'] = $this->getFullName($implement['value']);
            }
            $classDefinition->setImplementsInterfaces($topStatement['implements']);
        }

        if (isset($topStatement['abstract'])) {
            $classDefinition->setIsAbstract((bool)$topStatement['abstract']);
        }

        if (isset($topStatement['final'])) {
            $classDefinition->setIsFinal((bool)$topStatement['final']);
        }

        if (is_array($docblock)) {
            $classDefinition->setDocBlock($docblock['value']);
        }

        if (isset($topStatement['definition'])) {
            $definition = $topStatement['definition'];

            if (isset($definition['properties'])) {
                foreach ($definition['properties'] as $property) {
                    /**
                     * Add property to the definition
                     */
                    $classDefinition->addProperty(
                        new Property(
                            $classDefinition,
                            $property['visibility'],
                            $property['name'],
                            $property['default'] ?? null,
                            $property['docblock'] ?? null,
                            $property
                        )
                    );

                    /**
                     * Check and process shortcuts
                     */
                    if (isset($property['shortcuts'])) {
                        $this->processShortcuts($property, $classDefinition);
                    }
                }
            }

            /**
             * Register constants
             */
            if (isset($definition['constants'])) {
                foreach ($definition['constants'] as $constant) {
                    $classDefinition->addConstant(
                        new Constant(
                            $constant['name'],
                            $constant['default'] ?? null,
                            $constant['docblock'] ?? null
                        )
                    );
                }
            }

            /**
             * Register methods
             */
            if (isset($definition['methods'])) {
                foreach ($definition['methods'] as $method) {
                    $classDefinition->addMethod(
                        new Method(
                            $classDefinition,
                            $method['visibility'],
                            $method['name'],
                            isset($method['parameters']) ? new Parameters($method['parameters']) : null,
                            isset($method['statements']) ? new StatementsBlock($method['statements']) : null,
                            $method['docblock'] ?? null,
                            $method['return-type'] ?? null,
                            $method
                        ),
                        $method
                    );
                }
            }
        }

        $this->classDefinition = $classDefinition;

        /**
         * Assign current class definition to the compilation context
         */
        $compilationContext->classDefinition = $classDefinition;

        /**
         * Run pre-compilation passes
         */
        $classDefinition->preCompile($compilationContext);
    }

    /**
     * Creates a definition for an interface.
     *
     * @param string $namespace
     * @param array  $topStatement
     * @param array  $docblock
     */
    public function preCompileInterface($namespace, $topStatement, $docblock): void
    {
        $classDefinition = new Definition($namespace, $topStatement['name']);
        $classDefinition->setIsExternal($this->external);

        if (isset($topStatement['extends'])) {
            foreach ($topStatement['extends'] as &$extend) {
                $extend['value'] = $this->getFullName($extend['value']);
            }
            $classDefinition->setImplementsInterfaces($topStatement['extends']);
        }

        $classDefinition->setType('interface');

        if (is_array($docblock)) {
            $classDefinition->setDocBlock($docblock['value']);
        }

        if (isset($topStatement['definition'])) {
            $definition = $topStatement['definition'];

            /**
             * Register constants
             */
            if (isset($definition['constants'])) {
                foreach ($definition['constants'] as $constant) {
                    $classConstant = new Constant(
                        $constant['name'],
                        $constant['default'] ?? null,
                        $constant['docblock'] ?? null
                    );
                    $classDefinition->addConstant($classConstant);
                }
            }

            /**
             * Register methods
             */
            if (isset($definition['methods'])) {
                foreach ($definition['methods'] as $method) {
                    $classMethod = new Method(
                        $classDefinition,
                        $method['visibility'],
                        $method['name'],
                        isset($method['parameters']) ? new Parameters($method['parameters']) : null,
                        null,
                        $method['docblock'] ?? null,
                        $method['return-type'] ?? null,
                        $method
                    );
                    $classDefinition->addMethod($classMethod, $method);
                }
            }
        }

        $this->classDefinition = $classDefinition;
    }

    /**
     * @param string $className
     */
    public function setClassName(string $className): void
    {
        $this->className = $className;
    }

    /**
     * @param string $filePath
     */
    public function setFilePath(string $filePath): void
    {
        $this->filePath = $filePath;
    }

    /**
     * Sets if the class belongs to an external dependency or not.
     *
     * @param bool $external
     */
    public function setIsExternal($external): void
    {
        $this->external = (bool)$external;
    }

    /**
     * Create returns type list.
     */
    protected function createReturnsType(array $types, bool $annotated = false): ?array
    {
        if (!$types) {
            return null;
        }

        $list = [];

        foreach ($types as $type) {
            $list[] = [
                'type'      => $annotated ? 'return-type-annotation' : 'return-type-paramater',
                'data-type' => 'mixed' === $type ? 'variable' : $type,
                'mandatory' => false,
            ];
        }

        return [
            'type' => 'return-type',
            'list' => $list,
            'void' => empty($list),
        ];
    }

    /**
     * Transform class/interface name to FQN format.
     */
    protected function getFullName(string $name): string
    {
        return Name::fetchFQN($name, $this->namespace, $this->aliasManager);
    }

    /**
     * Creates the property shortcuts.
     *
     * @param array      $property
     * @param Definition $classDefinition
     *
     * @throws CompilerException
     */
    protected function processShortcuts(array $property, Definition $classDefinition): void
    {
        foreach ($property['shortcuts'] as $shortcut) {
            if (str_starts_with($property['name'], '_')) {
                $name = substr($property['name'], 1);
            } else {
                $name = $property['name'];
            }

            $docBlock = null;
            if (isset($shortcut['docblock'])) {
                $docBlock = $shortcut['docblock'];
            } elseif (isset($property['docblock'])) {
                $docBlock = $property['docblock'];
            }

            $returnsType = [];

            if ($docBlock) {
                $docBlockParser = new DocblockParser('/' . $docBlock . '/');
                $docBlockParsed = $docBlockParser->parse();

                if ($annotations = $docBlockParsed->getAnnotationsByType('var')) {
                    $returnsType = array_map(
                        fn($type) => 'mixed' == ($type = trim($type)) ? 'variable' : $type,
                        explode('|', $annotations[0]->getString())
                    );
                }

                // Clear annotations
                $docBlockParsed->setAnnotations([]);
                $docBlock = $docBlockParsed->generate();
            }

            switch ($shortcut['name']) {
                case 'get':
                    $classDefinition->addMethod(
                        new Method(
                            $classDefinition,
                            ['public'],
                            'get' . Name::camelize($name),
                            null,
                            new StatementsBlock([
                                [
                                    'type' => 'return',
                                    'expr' => [
                                        'type'  => 'property-access',
                                        'left'  => [
                                            'type'  => 'variable',
                                            'value' => 'this',
                                        ],
                                        'right' => [
                                            'type'  => 'variable',
                                            'value' => $property['name'],
                                        ],
                                    ],
                                ],
                            ]),
                            $docBlock,
                            $this->createReturnsType($returnsType, true),
                            $shortcut
                        ),
                        $shortcut
                    );
                    break;

                case 'set':
                    $classDefinition->addMethod(
                        new Method(
                            $classDefinition,
                            ['public'],
                            'set' . Name::camelize($name),
                            new Parameters([
                                [
                                    'type'      => 'parameter',
                                    'name'      => $name,
                                    'const'     => 0,
                                    'data-type' => 1 == count($returnsType) ? $returnsType[0] : 'variable',
                                    'mandatory' => 0,
                                ],
                            ]),
                            new StatementsBlock([
                                [
                                    'type'        => 'let',
                                    'assignments' => [
                                        [
                                            'assign-type' => 'object-property',
                                            'operator'    => 'assign',
                                            'variable'    => 'this',
                                            'property'    => $property['name'],
                                            'expr'        => [
                                                'type'  => 'variable',
                                                'value' => $name,
                                                'file'  => $property['file'],
                                                'line'  => $property['line'],
                                                'char'  => $property['char'],
                                            ],
                                            'file'        => $property['file'],
                                            'line'        => $property['line'],
                                            'char'        => $property['char'],
                                        ],
                                    ],
                                ],
                                [
                                    'type' => 'return',
                                    'expr' => [
                                        'type'  => 'variable',
                                        'value' => 'this',
                                        'file'  => $property['file'],
                                        'line'  => $property['line'],
                                        'char'  => $property['char'],
                                    ],
                                ],
                            ]),
                            $docBlock,
                            null,
                            $shortcut
                        ),
                        $shortcut
                    );
                    break;

                case 'toString':
                case '__toString':
                    $classDefinition->addMethod(
                        new Method(
                            $classDefinition,
                            ['public'],
                            '__toString',
                            null,
                            new StatementsBlock([
                                [
                                    'type' => 'return',
                                    'expr' => [
                                        'type'  => 'property-access',
                                        'left'  => [
                                            'type'  => 'variable',
                                            'value' => 'this',
                                        ],
                                        'right' => [
                                            'type'  => 'variable',
                                            'value' => $property['name'],
                                        ],
                                    ],
                                ],
                            ]),
                            $docBlock,
                            $this->createReturnsType(['string']),
                            $shortcut
                        ),
                        $shortcut
                    );
                    break;

                default:
                    throw new CompilerException("Unknown shortcut '" . $shortcut['name'] . "'", $shortcut);
            }
        }
    }
}
