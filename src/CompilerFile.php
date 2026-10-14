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
use function in_array;
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
     * `@var` words that look like a class name but are not one.
     */
    private const DOCBLOCK_PSEUDO_TYPES = [
        'array', 'bool', 'boolean', 'callable', 'char', 'double', 'false', 'float', 'int', 'integer',
        'iterable', 'list', 'long', 'mixed', 'never', 'null', 'numeric', 'object', 'parent', 'resource',
        'scalar', 'self', 'static', 'string', 'this', 'true', 'uchar', 'uint', 'ulong', 'var', 'variable',
        'void',
    ];

    /**
     * `@var` words a shortcut accessor can be typed with, after the
     * synonyms below are applied. Any other non-class word leaves the
     * accessors untyped.
     */
    private const DOCBLOCK_ACCESSOR_TYPES = [
        'array', 'bool', 'callable', 'char', 'double', 'false', 'int', 'long', 'mixed', 'null', 'object',
        'resource', 'string', 'uchar', 'uint', 'ulong', 'var', 'variable',
    ];

    /**
     * PHPDoc spellings of a Zephir type.
     */
    private const DOCBLOCK_TYPE_SYNONYMS = [
        'boolean' => 'bool',
        'float'   => 'double',
        'integer' => 'int',
    ];

    private AliasManager $aliasManager;

    private ?Definition $classDefinition = null;
    private ?string     $className       = null;

    private ?string $compiledFile = null;

    private Config $config;

    private bool $external = false;

    private ?string $filePath = null;

    private FileSystemInterface $filesystem;

    private array $functionDefinitions = [];

    private array $headerCBlocks = [];

    /**
     * Original internal representation (IR) of the file.
     */
    private ?array  $ir        = null;
    private ?string $namespace = null;

    /**
     * `use` statements as parsed from the IR — kept around so
     * checkDependencies() can validate their FQCNs after every file in the
     * extension has been preCompiled (preCompile order is alphabetical, so
     * an in-extension class referenced by a use in an earlier-sorted file
     * isn't yet registered when that file is being preCompiled).
     *
     * @var array<int, array>
     */
    private array $useStatements = [];

    /**
     * @var mixed
     */
    private $originalNode;

    public function __construct(
        Config $config,
        AliasManager $aliasManager,
        FileSystemInterface $filesystem,
    ) {
        $this->config       = $config;
        $this->logger       = new NullLogger();
        $this->aliasManager = $aliasManager;
        $this->filesystem   = $filesystem;
    }

    /**
     * Adds a function to the function definitions.
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
        $code = $this->generateCodeHeadersPre($this->classDefinition);

        if (!$this->classDefinition->isInterface()) {
            $code .= '#include <Zend/zend_operators.h>' . PHP_EOL;
            $code .= '#include <Zend/zend_exceptions.h>' . PHP_EOL;
            $code .= '#include <Zend/zend_interfaces.h>' . PHP_EOL;
        } else {
            $code .= '#include <Zend/zend_exceptions.h>' . PHP_EOL;
        }

        $this->generateClassHeadersPost($code, $compilationContext);
    }

    /**
     * Check dependencies.
     *
     * @throws ReflectionException
     */
    public function checkDependencies(Compiler $compiler): void
    {
        $classDefinition = $this->classDefinition;

        /**
         * Validate `use Foo\Bar` FQCNs now that every file in the extension
         * has been preCompiled — intra-extension classes, interfaces and
         * traits are guaranteed to be in
         * $compiler->isClass()/isInterface()/isTrait() at this point.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2435.
         */
        foreach ($this->useStatements as $useStatement) {
            foreach ($useStatement['aliases'] as $useAlias) {
                $fqcn = $useAlias['name'];
                if (
                    $compiler->isClass($fqcn)
                    || $compiler->isInterface($fqcn)
                    || $compiler->isTrait($fqcn)
                    || $compiler->isBundledClass($fqcn)
                    || $compiler->isBundledInterface($fqcn)
                    || class_exists($fqcn, false)
                    || interface_exists($fqcn, false)
                    || trait_exists($fqcn, false)
                ) {
                    continue;
                }

                $this->logger->warning(
                    sprintf(
                        'Class or interface "%s" used in `use` statement does not exist at compile time',
                        $fqcn
                    ),
                    ['nonexistent-class', $useAlias]
                );
            }
        }

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

        /**
         * Resolve used trait names to their Definitions. Unlike interfaces,
         * unresolved traits are hard errors: the merge needs the trait's AST,
         * so a missing trait cannot degrade to a runtime lookup.
         */
        $traitDefinitions = [];
        foreach ($classDefinition->getUsedTraits() as $traitName) {
            if ($compiler->isTrait($traitName)) {
                $traitDefinitions[strtolower($traitName)] = $compiler->getClassDefinition($traitName);
                continue;
            }

            if ($compiler->isClass($traitName) || $compiler->isInterface($traitName)) {
                throw new CompilerException(
                    sprintf(
                        '"%s" cannot be used as a trait on "%s": it is %s',
                        $traitName,
                        $classDefinition->getCompleteName(),
                        $compiler->isClass($traitName) ? 'a class' : 'an interface'
                    ),
                    $this->originalNode
                );
            }

            if (trait_exists($traitName, false)) {
                throw new CompilerException(
                    sprintf(
                        'PHP trait "%s" cannot be used by "%s": traits must be declared within the extension',
                        $traitName,
                        $classDefinition->getCompleteName()
                    ),
                    $this->originalNode
                );
            }

            throw new CompilerException(
                sprintf(
                    'Cannot locate trait "%s" when processing `use` on "%s"',
                    $traitName,
                    $classDefinition->getCompleteName()
                ),
                $this->originalNode
            );
        }

        if (count($traitDefinitions) > 0) {
            $classDefinition->setUsedTraitDefinitions($traitDefinitions);
        }
    }

    /**
     * Compiles the file.
     *
     * @throws Exception
     * @throws ReflectionException
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
         * Validate PHPStan/Psalm type expressions in docblocks.
         * Emits warnings only; never blocks compilation.
         */
        $this->validatePhpStanAnnotations();

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
        $compilationContext->headersManager = new HeadersManager();

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
                case Definition::TYPE_INTERFACE:
                case Definition::TYPE_TRAIT:
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
     * @throws Exception
     * @throws ReflectionException
     */
    public function compileClass(CompilationContext $compilationContext): void
    {
        $this->classDefinition->compile($compilationContext);
    }

    /**
     * Compiles a comment as a top-level statement.
     */
    public function compileComment(CompilationContext $compilationContext, array $topStatement): void
    {
        $compilationContext->codePrinter->output('/' . $topStatement['value'] . '/');
    }

    /**
     * Compiles a function.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compileFunction(
        CompilationContext $compilationContext,
        FunctionDefinition $functionDefinition,
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

    public function getClassDefinition(): ?Definition
    {
        $this->classDefinition->setAliasManager($this->aliasManager);

        return $this->classDefinition;
    }

    /**
     * Validates the value portion of type-bearing PHPStan/Psalm docblock
     * tags on the class definition (top-of-class, properties, constants,
     * methods). Emits one logger warning per malformed tag occurrence.
     *
     * Never blocks compilation. Stub generation is unaffected.
     */
    public function validatePhpStanAnnotations(): void
    {
        if ($this->classDefinition === null) {
            return;
        }

        $validator = new \Zephir\Stubs\PhpStanTagValidator();

        $this->validateDocblockForPhpStan(
            $this->classDefinition->getDocBlock(),
            $validator,
            'class ' . $this->classDefinition->getCompleteName(),
            null,
        );

        foreach ($this->classDefinition->getProperties() as $property) {
            $original = $property->getOriginal();
            $line     = is_array($original) && isset($original['line']) ? (int) $original['line'] : null;

            $this->validateDocblockForPhpStan(
                $property->getDocBlock(),
                $validator,
                'property $' . $property->getName(),
                $line,
            );
        }

        foreach ($this->classDefinition->getConstants() as $constant) {
            $this->validateDocblockForPhpStan(
                $constant->getDocBlock(),
                $validator,
                'constant ' . $constant->getName(),
                null,
            );
        }

        foreach ($this->classDefinition->getMethods() as $method) {
            $line = $method->getLine();
            $this->validateDocblockForPhpStan(
                $method->getDocBlock(),
                $validator,
                'method ' . $method->getName() . '()',
                is_int($line) ? $line : null,
            );
        }
    }

    /**
     * Emits a logger warning for an invalid PHPStan annotation, including
     * the originating .zep file path and (when available) the line number
     * of the offending member so the developer can jump straight to it.
     */
    private function validateDocblockForPhpStan(
        ?string $docBlock,
        \Zephir\Stubs\PhpStanTagValidator $validator,
        string $location,
        ?int $line,
    ): void {
        if ($docBlock === null || trim($docBlock) === '') {
            return;
        }

        $parsed = (new DocblockParser('/' . $docBlock . '/'))->parse();

        foreach ($parsed->getAnnotations() as $annotation) {
            $name = $annotation->getName();

            if (!\Zephir\Stubs\PhpStanTagValidator::isTypeBearingTag($name)) {
                continue;
            }

            $error = $validator->validateTagValue($name, $annotation->getString());
            if ($error === null) {
                continue;
            }

            $where = $this->filePath;
            if ($line !== null) {
                $where .= ':' . $line;
            }

            $this->logger->warning(sprintf(
                "%s in %s: PHPStan annotation '@%s' has invalid type expression: %s",
                $where,
                $location,
                $name,
                $error->message,
            ));
        }
    }

    /**
     * Returns the path to the compiled file.
     */
    public function getCompiledFile(): string
    {
        return $this->compiledFile;
    }

    /**
     * Returns the path to the source file.
     */
    public function getFilePath(): string
    {
        return $this->filePath;
    }

    public function isExternal(): bool
    {
        return $this->external;
    }

    /**
     * Pre-compiles a Zephir file.
     *
     * Generates the IR and perform basic validations.
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
        $trait       = false;
        $lastComment = null;

        foreach ($ir as $topStatement) {
            switch ($topStatement['type']) {
                case 'class':
                    if ($class || $interface || $trait) {
                        throw new CompilerException(
                            'More than one class/interface/trait defined in the same file',
                            $topStatement
                        );
                    }
                    $class = true;
                    $name  = $topStatement['name'];
                    $this->preCompileClass($compilationContext, $namespace, $topStatement, $lastComment);
                    $this->originalNode = $topStatement;
                    $lastComment        = null;
                    break;

                case Definition::TYPE_INTERFACE:
                    if ($class || $interface || $trait) {
                        throw new CompilerException(
                            'More than one class/interface/trait defined in the same file',
                            $topStatement
                        );
                    }
                    $interface = true;
                    $name      = $topStatement['name'];
                    $this->preCompileInterface($namespace, $topStatement, $lastComment);
                    $this->originalNode = $topStatement;
                    $lastComment        = null;
                    break;

                case Definition::TYPE_TRAIT:
                    if ($class || $interface || $trait) {
                        throw new CompilerException(
                            'More than one class/interface/trait defined in the same file',
                            $topStatement
                        );
                    }
                    $trait = true;
                    $name  = $topStatement['name'];
                    $this->preCompileTrait($compilationContext, $namespace, $topStatement, $lastComment);
                    $this->originalNode = $topStatement;
                    $lastComment        = null;
                    break;

                case 'use':
                    if ($interface || $class || $trait) {
                        throw new CompilerException(
                            'Aliasing must be done before declaring any class, interface or trait',
                            $topStatement
                        );
                    }
                    $this->aliasManager->add($topStatement);

                    /**
                     * Defer FQCN existence validation to checkDependencies() —
                     * preCompile() is per-file, so an in-extension class
                     * referenced via `use` from a file that sorts earlier
                     * than the class's own file is not yet registered with
                     * $compiler when we'd check here. checkDependencies()
                     * runs after every file has been preCompiled, with the
                     * full intra-extension class table populated.
                     *
                     * @see https://github.com/zephir-lang/zephir/issues/2435.
                     */
                    $this->useStatements[] = $topStatement;
                    break;

                case 'comment':
                    $lastComment = $topStatement;
                    break;
            }
        }

        if (!$class && !$interface && !$trait) {
            throw new CompilerException(
                'Every file must contain at least a class, an interface or a trait',
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
     */
    public function preCompileClass(
        CompilationContext $compilationContext,
        string $namespace,
        array $topStatement,
        ?array $docblock = null,
    ): void {
        $classDefinition = new Definition($namespace, $topStatement['name']);
        $classDefinition->setIsExternal($this->external);

        /**
         * Needed here, not only at the end of compile(): Definition::compile()
         * reads the class node for its `#[...]` attributes, and it runs first.
         * Idempotent with the assignment there, which stores the same node.
         */
        $classDefinition->setOriginalNode($topStatement);

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
            $this->registerDefinitionMembers($classDefinition, $topStatement['definition']);
        }

        $classDefinition->setUsedTraits($this->extractUsedTraits($topStatement));

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
     * Creates a definition for a trait. Traits take no modifiers and cannot
     * extend or implement (enforced by the parser grammar). The raw AST node
     * is stored right away — trait members are re-instantiated from it into
     * each using class before code generation.
     */
    public function preCompileTrait(
        CompilationContext $compilationContext,
        string $namespace,
        array $topStatement,
        ?array $docblock = null,
    ): void {
        $classDefinition = new Definition($namespace, $topStatement['name']);
        $classDefinition->setIsExternal($this->external);
        $classDefinition->setType(Definition::TYPE_TRAIT);
        $classDefinition->setOriginalNode($topStatement);

        if (is_array($docblock)) {
            $classDefinition->setDocBlock($docblock['value']);
        }

        if (isset($topStatement['definition'])) {
            /**
             * An *instance* array/empty-array property default is materialized as a
             * persistent immutable array on the trait ce (see Property::compile), so
             * PHP's native trait binding carries it into every using class. A *static*
             * one cannot be made PHP-identical here: Zephir resolves `self::` inside a
             * trait method to the trait ce itself, so each using class would not get
             * its own copy — so static array defaults on traits stay rejected [#2607].
             */
            foreach ($topStatement['definition']['properties'] ?? [] as $property) {
                $defaultType = $property['default']['type'] ?? null;
                $isArray     = 'array' === $defaultType || 'empty-array' === $defaultType;
                if ($isArray && in_array('static', $property['visibility'] ?? [], true)) {
                    throw new CompilerException(
                        sprintf(
                            'Static property "%s" of trait "%s" cannot have an array default value',
                            $property['name'],
                            $classDefinition->getCompleteName()
                        ),
                        $property
                    );
                }
            }

            $this->registerDefinitionMembers($classDefinition, $topStatement['definition']);
        }

        $classDefinition->setUsedTraits($this->extractUsedTraits($topStatement));

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
     * Builds Property/Constant/Method objects from a parsed `definition` node
     * and registers them on the given class/trait definition. Public so the
     * TraitMerger can re-instantiate trait members into a using class with
     * identical construction semantics.
     */
    public function registerDefinitionMembers(Definition $classDefinition, array $definition): void
    {
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
                        $property,
                        $property['data-type'] ?? null,
                        $property['cast'] ?? null,
                        !empty($property['nullable']),
                        $property['data-types'] ?? null,
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
                        $constant['docblock'] ?? null,
                        $constant
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

    /**
     * Collects fully-qualified trait names from a class/trait node's in-body
     * `use` statements.
     *
     * @return string[]
     */
    private function extractUsedTraits(array $topStatement): array
    {
        $names = [];
        foreach ($topStatement['definition']['uses'] ?? [] as $useStatement) {
            /**
             * A use-trait statement is an ordinary class member, so the grammar
             * accepts a `#[...]` prefix on it. PHP has no such attribute target,
             * and silently dropping the attribute would be worse than saying so.
             */
            if (isset($useStatement['attributes'])) {
                throw new CompilerException(
                    'Attributes are not supported on a `use` statement; PHP allows them on a class, '
                    . 'interface, trait, property, class constant, method or parameter',
                    $useStatement['attributes'][0]
                );
            }

            foreach ($useStatement['traits'] as $traitReference) {
                $names[] = $this->getFullName($traitReference['value']);
            }
        }

        return $names;
    }

    /**
     * Creates a definition for an interface.
     */
    public function preCompileInterface(string $namespace, array $topStatement, ?array $docblock = null): void
    {
        $classDefinition = new Definition($namespace, $topStatement['name']);
        $classDefinition->setIsExternal($this->external);
        $classDefinition->setOriginalNode($topStatement);

        if (isset($topStatement['extends'])) {
            foreach ($topStatement['extends'] as &$extend) {
                $extend['value'] = $this->getFullName($extend['value']);
            }
            $classDefinition->setImplementsInterfaces($topStatement['extends']);
        }

        $classDefinition->setType(Definition::TYPE_INTERFACE);

        if ($docblock !== null) {
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
                        $constant['docblock'] ?? null,
                        $constant
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

    public function setClassName(string $className): void
    {
        $this->className = $className;
    }

    public function setFilePath(string $filePath): void
    {
        $this->filePath = $filePath;
    }

    /**
     * Sets if the class belongs to an external dependency or not.
     */
    public function setIsExternal($external): void
    {
        $this->external = (bool)$external;
    }

    /**
     * Create returns type list from shortcut type members.
     *
     * A member is `['data-type' => <type>]` or `['cast' => <class node>]`, the
     * shape the parsers emit for a union type; a cast member becomes the same
     * node a hand-written `-> <Class>` produces.
     */
    protected function createReturnsType(array $members, bool $annotated = false): ?array
    {
        if (!$members) {
            return null;
        }

        $list = [];

        foreach ($members as $member) {
            if (isset($member['cast'])) {
                $list[] = [
                    'type'       => 'return-type-parameter',
                    'cast'       => $member['cast'],
                    'collection' => 0,
                    'mandatory'  => false,
                ];
                continue;
            }

            $list[] = [
                'type'      => $annotated ? 'return-type-annotation' : 'return-type-parameter',
                'data-type' => 'mixed' === $member['data-type'] ? 'variable' : $member['data-type'],
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

            $typeMembers = $this->declaredShortcutTypes($property);
            $declared    = [] !== $typeMembers;

            if ($docBlock) {
                $docBlockParser = new DocblockParser('/' . $docBlock . '/');
                $docBlockParsed = $docBlockParser->parse();

                $annotations = $docBlockParsed->getAnnotationsByType('var');
                if (!$declared && $annotations) {
                    $typeMembers = $this->docblockShortcutTypes($annotations[0]->getString(), $property);
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
                                    'expr' => $this->shortcutPropertyRead($property),
                                ],
                            ]),
                            $docBlock,
                            $this->createReturnsType($typeMembers, !$declared),
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
                                $this->shortcutSetterParameter($name, $typeMembers, $declared),
                            ]),
                            new StatementsBlock([
                                [
                                    'type'        => 'let',
                                    'assignments' => [
                                        [
                                            'assign-type' => $this->isStaticProperty($property)
                                                ? 'static-property'
                                                : 'object-property',
                                            'operator'    => 'assign',
                                            'variable'    => $this->isStaticProperty($property) ? 'self' : 'this',
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
                                    'expr' => $this->shortcutPropertyRead($property),
                                ],
                            ]),
                            $docBlock,
                            $this->createReturnsType([['data-type' => 'string']]),
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

    /**
     * The getter's read of the property: `this->prop`, or `self::prop` for a
     * static property, which `this->prop` would never reach.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2391
     */
    private function shortcutPropertyRead(array $property): array
    {
        $position = [
            'file' => $property['file'] ?? null,
            'line' => $property['line'] ?? null,
            'char' => $property['char'] ?? null,
        ];

        return [
            'type'  => $this->isStaticProperty($property) ? 'static-property-access' : 'property-access',
            'left'  => [
                'type'  => 'variable',
                'value' => $this->isStaticProperty($property) ? 'self' : 'this',
            ] + $position,
            'right' => [
                'type'  => 'variable',
                'value' => $property['name'],
            ] + $position,
        ] + $position;
    }

    private function isStaticProperty(array $property): bool
    {
        return in_array('static', $property['visibility'], true);
    }

    /**
     * Type members of a property's own declaration (`string name`,
     * `?<Class> name`, `int|string name`), or none when it is untyped.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2761
     */
    private function declaredShortcutTypes(array $property): array
    {
        if (isset($property['data-types'])) {
            return $property['data-types'];
        }

        if (isset($property['cast'])) {
            $members = [['cast' => $property['cast']]];
        } elseif (isset($property['data-type']) && 'variable' !== $property['data-type']) {
            $members = [['data-type' => $property['data-type']]];
        } else {
            return [];
        }

        if (!empty($property['nullable'])) {
            $members[] = ['data-type' => 'null'];
        }

        return $members;
    }

    /**
     * Type members of a `@var` annotation. A class name becomes a cast member
     * so it produces a class type, and PHPDoc spellings map onto Zephir types
     * (`float`, `?int`, `string[]`, `array<int, string>`). A type no accessor
     * can carry (`numeric`, `self`, `iterable`, a lone `null`) leaves the
     * accessors untyped rather than failing the build.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2391
     */
    private function docblockShortcutTypes(string $annotation, array $property): array
    {
        $members = [];

        foreach ($this->docblockUnionMembers($this->docblockTypeExpression($annotation)) as $type) {
            if (str_starts_with($type, '?')) {
                $type      = substr($type, 1);
                $members[] = ['data-type' => 'null'];
            }

            if (str_ends_with($type, '[]') || preg_match('/^(array|list)</i', $type)) {
                $type = 'array';
            }

            $word = strtolower($type);
            $word = self::DOCBLOCK_TYPE_SYNONYMS[$word] ?? $word;

            if (in_array($word, self::DOCBLOCK_ACCESSOR_TYPES, true)) {
                $members[] = ['data-type' => 'mixed' === $word ? 'variable' : $word];
                continue;
            }

            if (!$this->isDocblockClassName($type)) {
                return [];
            }

            $members[] = [
                'cast' => [
                    'type'  => 'variable',
                    'value' => $type,
                    'file'  => $property['file'] ?? null,
                    'line'  => $property['line'] ?? null,
                    'char'  => $property['char'] ?? null,
                ],
            ];
        }

        // `null` and `false` only exist as part of a wider type.
        foreach ($members as $member) {
            if (isset($member['cast']) || !in_array($member['data-type'], ['null', 'false'], true)) {
                return $members;
            }
        }

        return [];
    }

    /**
     * The type part of a `@var` annotation (`Type $name some text`), which
     * ends at the first space outside `<...>`, `{...}` or `(...)`.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2543
     */
    private function docblockTypeExpression(string $annotation): string
    {
        $annotation = trim($annotation);
        $depth      = 0;

        for ($i = 0, $length = strlen($annotation); $i < $length; $i++) {
            $char = $annotation[$i];
            if (str_contains('<{(', $char)) {
                $depth++;
            } elseif (str_contains('>})', $char)) {
                $depth--;
            } elseif (0 === $depth && ctype_space($char)) {
                return substr($annotation, 0, $i);
            }
        }

        return $annotation;
    }

    /**
     * Splits a type expression on the `|` that are not inside `<...>`,
     * `{...}` or `(...)`.
     *
     * @return string[]
     */
    private function docblockUnionMembers(string $expression): array
    {
        $members = [];
        $current = '';
        $depth   = 0;

        for ($i = 0, $length = strlen($expression); $i < $length; $i++) {
            $char = $expression[$i];
            if (str_contains('<{(', $char)) {
                $depth++;
            } elseif (str_contains('>})', $char)) {
                $depth--;
            } elseif (0 === $depth && '|' === $char) {
                $members[] = trim($current);
                $current   = '';
                continue;
            }
            $current .= $char;
        }
        $members[] = trim($current);

        return array_values(array_filter($members, static fn (string $member): bool => '' !== $member));
    }

    private function isDocblockClassName(string $type): bool
    {
        if (!preg_match('/^\\\\?[A-Za-z_]\w*(\\\\[A-Za-z_]\w*)*$/', $type)) {
            return false;
        }

        return !in_array(strtolower($type), self::DOCBLOCK_PSEUDO_TYPES, true);
    }

    /**
     * The setter's parameter. A declared type is used in full; a `@var` type
     * only when it names a single type, as before.
     */
    private function shortcutSetterParameter(string $name, array $typeMembers, bool $declared): array
    {
        $parameter = [
            'type'      => 'parameter',
            'name'      => $name,
            'const'     => 0,
            'data-type' => 'variable',
            'mandatory' => 0,
        ];

        if (1 === count($typeMembers)) {
            if (isset($typeMembers[0]['cast'])) {
                $parameter['cast'] = $typeMembers[0]['cast'];
            } else {
                $parameter['data-type'] = $typeMembers[0]['data-type'];
            }
        } elseif ($declared && count($typeMembers) > 1) {
            $parameter['data-types'] = $typeMembers;
        }

        return $parameter;
    }
}
