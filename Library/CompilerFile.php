<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

/**
 * CompilerFile
 *
 * This class represents every file compiled in a project
 * Every file may contain a class or an interface
 */
class CompilerFile
{

    /**
     * Namespace of the
     */
    protected $_namespace;

    protected $_className;

    protected $_filePath;

    protected $_external = false;

    /**
     * Original internal representation (IR) of the file
     */
    protected $_ir;

    protected $_originalNode;

    protected $_compiledFile;

    /**
     * @var ClassDefinition
     */
    protected $_classDefinition;

    /**
     * @var FunctionDefinition[]
     */
    protected $_functionDefinitions = array();

    /**
     * @var array
     */
    protected $_headerCBlocks;

    /**
     * @var Config
     */
    protected $_config = null;

    /**
     * @var Logger
     */
    protected $_logger = null;

    /**
     * CompilerFile constructor
     *
     * @param string $className
     * @param string $filePath
     * @param Config $config
     * @param Logger $logger
     */
    public function __construct($className, $filePath, Config $config, Logger $logger)
    {
        $this->_className = $className;
        $this->_filePath = $filePath;
        $this->_headerCBlocks = array();
        $this->_config = $config;
        $this->_logger = $logger;
    }

    /**
     * Returns the class definition related to the compiled file
     *
     * @return ClassDefinition
     */
    public function getClassDefinition()
    {
        $this->_classDefinition->setAliasManager($this->_aliasManager);
        return $this->_classDefinition;
    }

    public function getFunctionDefinitions()
    {
        return $this->_functionDefinitions;
    }

    /**
     * Sets if the class belongs to an external dependency or not
     *
     * @param boolean $external
     */
    public function setIsExternal($external)
    {
        $this->_external = (bool) $external;
    }

    /**
     * Checks if the class file belongs to an external dependency or not
     *
     * @return bool
     */
    public function isExternal()
    {
        return $this->_external;
    }

    /**
     * Adds a function to the function definitions
     *
     * @param FunctionDefinition $func
     * @param array $statement
     */
    public function addFunction(Compiler $compiler, FunctionDefinition $func, $statement = null)
    {
        $compiler->addFunction($func, $statement);
        $funcName = strtolower($func->getInternalName());
        if (isset($this->_functionDefinitions[$funcName])) {
            throw new CompilerException("Function '" . $func->getName() . "' was defined more than one time (in the same file)", $statement);
        }
        $this->_functionDefinitions[$funcName] = $func;
    }

    /**
     * Compiles the file generating a JSON intermediate representation
     *
     * @param Compiler $compiler
     * @return array
     */
    public function genIR(Compiler $compiler)
    {
        $normalizedPath = str_replace(array(DIRECTORY_SEPARATOR, ":", '/'), '_', realpath($this->_filePath));
        $compilePath = DIRECTORY_SEPARATOR . Compiler::VERSION . DIRECTORY_SEPARATOR . $normalizedPath . ".js";
        $zepRealPath = realpath($this->_filePath);

        if (PHP_OS == "WINNT") {
            $zephirParserBinary = ZEPHIRPATH . 'bin\zephir-parser.exe';
        } else {
            $zephirParserBinary = ZEPHIRPATH . 'bin/zephir-parser';
        }

        if (!file_exists($zephirParserBinary)) {
            throw new Exception($zephirParserBinary . ' was not found');
        }

        $changed = false;
        $fileSystem = $compiler->getFileSystem();
        if ($fileSystem->exists($compilePath)) {
            $modificationTime = $fileSystem->modificationTime($compilePath);
            if ($modificationTime < filemtime($zepRealPath) || $modificationTime < filemtime($zephirParserBinary)) {
                $fileSystem->system($zephirParserBinary . ' ' . $zepRealPath, 'stdout', $compilePath);
                $changed = true;
            }
        } else {
            $fileSystem->system($zephirParserBinary . ' ' . $zepRealPath, 'stdout', $compilePath);
            $changed = true;
        }

        if ($changed || !$fileSystem->exists($compilePath . '.php')) {
            $json = json_decode($fileSystem->read($compilePath), true);
            $data = '<?php return ' . var_export($json, true) . ';';
            $fileSystem->write($compilePath . '.php', $data);
        }

        return $fileSystem->requireFile($compilePath . '.php');
    }

    /**
     * Compiles the class/interface contained in the file
     *
     * @param CompilationContext $compilationContext
     * @param string $namespace
     * @param array $classStatement
     */
    public function compileClass(CompilationContext $compilationContext, $namespace, $classStatement)
    {
        $classDefinition = $this->_classDefinition;

        /**
         * Do the compilation
         */
        $classDefinition->compile($compilationContext);
    }

    /**
     * Compiles a function
     *
     * @param CompilationContext $compilationContext
     * @param FunctionDefinition $functionDefinition
     */
    public function compileFunction(CompilationContext $compilationContext, FunctionDefinition $functionDefinition)
    {
        /** Make sure we do not produce calls like ZEPHIR_CALL_SELF */
        $bakClassDefinition = $compilationContext->classDefinition;
        $compilationContext->classDefinition = null;
        $compilationContext->currentMethod = $functionDefinition;

        $codePrinter = $compilationContext->codePrinter;
        $codePrinter->output('PHP_FUNCTION(' . $functionDefinition->getInternalName() . ') {');
        $functionDefinition->compile($compilationContext);
        $codePrinter->output('}');
        $codePrinter->outputBlankLine();

        /** Restore */
        $compilationContext->classDefinition = $bakClassDefinition;
        $compilationContext->currentMethod = null;
    }

    /**
     * Compiles a comment as a top-level statement
     *
     * @param CompilationContext $compilationContext
     * @param array $topStatement
     */
    public function compileComment(CompilationContext $compilationContext, $topStatement)
    {
        $compilationContext->codePrinter->output('/' . $topStatement['value'] . '/');
    }

    /**
     * Creates a definition for an interface
     *
     * @param string $namespace
     * @param array $topStatement
     * @param array $docblock
     */
    public function preCompileInterface($namespace, $topStatement, $docblock)
    {
        $classDefinition = new ClassDefinition($namespace, $topStatement['name']);
        $classDefinition->setIsExternal($this->_external);

        if (isset($topStatement['extends'])) {
            foreach ($topStatement['extends'] as &$extend) {
                $extend['value'] = $this->getFullName($extend['value']);
            }
            $classDefinition->setImplementsInterfaces($topStatement['extends']);
        }

        $classDefinition->setType('interface');

        if (is_array($docblock)) {
            $classDefinition->setDocBlock($docblock["value"]);
        }

        if (isset($topStatement['definition'])) {
            $definition = $topStatement['definition'];

            /**
             * Register constants
             */
            if (isset($definition['constants'])) {
                foreach ($definition['constants'] as $constant) {
                    $classConstant = new ClassConstant(
                        $constant['name'],
                        isset($constant['default']) ? $constant['default'] : null,
                        isset($constant['docblock']) ? $constant['docblock'] : null
                    );
                    $classDefinition->addConstant($classConstant);
                }
            }

            /**
             * Register methods
             */
            if (isset($definition['methods'])) {
                foreach ($definition['methods'] as $method) {
                    $classMethod = new ClassMethod(
                        $classDefinition,
                        $method['visibility'],
                        $method['name'],
                        isset($method['parameters']) ? new ClassMethodParameters($method['parameters']) : null,
                        null,
                        isset($method['docblock']) ? $method['docblock'] : null,
                        isset($method['return-type']) ? $method['return-type'] : null,
                        $method
                    );
                    $classDefinition->addMethod($classMethod, $method);
                }
            }
        }

        $this->_classDefinition = $classDefinition;
    }

    /**
     * Creates the property shortcuts
     *
     * @param array $property
     * @param ClassDefinition $classDefinition
     * @throws CompilerException
     */
    protected function _processShorcuts(array $property, ClassDefinition $classDefinition)
    {
        foreach ($property['shortcuts'] as $shortcut) {
            if (substr($property['name'], 0, 1) == '_') {
                $name = substr($property['name'], 1);
            } else {
                $name = $property['name'];
            }

            switch ($shortcut['name']) {

                case 'get':
                    $classDefinition->addMethod(new ClassMethod(
                        $classDefinition,
                        array('public'),
                        'get' . ucfirst($name),
                        null,
                        new StatementsBlock(array(
                            array(
                                'type' => 'return',
                                'expr' => array(
                                    'type' => 'property-access',
                                    'left' => array(
                                        'type' => 'variable',
                                        'value' => 'this'
                                    ),
                                    'right' => array(
                                        'type' => 'variable',
                                        'value' => $property['name']
                                    )
                                )
                            )
                        )),
                        isset($shortcut['docblock']) ? $shortcut['docblock'] : isset($property['docblock']) ? $property['docblock'] : null,
                        null,
                        $shortcut
                    ), $shortcut);
                    break;

                case 'set':
                    $classDefinition->addMethod(new ClassMethod(
                        $classDefinition,
                        array('public'),
                        'set' . ucfirst($name),
                        new ClassMethodParameters(array(
                            array(
                                'type' => 'parameter',
                                'name' => $name,
                                'const' => 0,
                                'data-type' => 'variable',
                                'mandatory' => 0
                            )
                        )),
                        new StatementsBlock(array(
                            array(
                                'type' => 'let',
                                'assignments' => array(
                                    array(
                                        'assign-type' => 'object-property',
                                        'operator' => 'assign',
                                        'variable' => 'this',
                                        'property' => $property['name'],
                                        'expr' => array(
                                            'type' => 'variable',
                                            'value' => $name,
                                            'file'  => $property['file'],
                                            'line'  => $property['line'],
                                            'char'  => $property['char']
                                        ),
                                        'file'  => $property['file'],
                                        'line'  => $property['line'],
                                        'char'  => $property['char']
                                    )
                                )
                            )
                        )),
                        isset($shortcut['docblock']) ? $shortcut['docblock'] : isset($property['docblock']) ? $property['docblock'] : null,
                        null,
                        $shortcut
                    ), $shortcut);
                    break;

                case 'toString':
                case '__toString':
                    $classDefinition->addMethod(new ClassMethod(
                        $classDefinition,
                        array('public'),
                        '__toString',
                        null,
                        new StatementsBlock(array(
                            array(
                                'type' => 'return',
                                'expr' => array(
                                    'type' => 'property-access',
                                    'left' => array(
                                        'type' => 'variable',
                                        'value' => 'this'
                                    ),
                                    'right' => array(
                                        'type' => 'variable',
                                        'value' => $property['name']
                                    )
                                )
                            )
                        )),
                        isset($shortcut['docblock']) ? $shortcut['docblock'] : isset($property['docblock']) ? $property['docblock'] : null,
                        null,
                        $shortcut
                    ), $shortcut);
                    break;

                default:
                    throw new CompilerException("Unknown shortcut '" . $shortcut['name'] . "'", $shortcut);
            }
        }
    }

    /**
     * Creates a definition for a class
     *
     * @param CompilationContext $compilationContext
     * @param string $namespace
     * @param array $topStatement
     * @param array $docblock
     */
    public function preCompileClass(CompilationContext $compilationContext, $namespace, $topStatement, $docblock)
    {
        $classDefinition = new ClassDefinition($namespace, $topStatement['name']);
        $classDefinition->setIsExternal($this->_external);

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
            $classDefinition->setIsAbstract($topStatement['abstract']);
        }

        if (isset($topStatement['final'])) {
            $classDefinition->setIsFinal($topStatement['final']);
        }

        if (is_array($docblock)) {
            $classDefinition->setDocBlock($docblock["value"]);
        }

        if (isset($topStatement['definition'])) {
            $definition = $topStatement['definition'];

            if (isset($definition['properties'])) {
                foreach ($definition['properties'] as $property) {
                    /**
                     * Add property to the definition
                     */
                    $classDefinition->addProperty(new ClassProperty(
                        $classDefinition,
                        $property['visibility'],
                        $property['name'],
                        isset($property['default']) ? $property['default'] : null,
                        isset($property['docblock']) ? $property['docblock'] : null,
                        $property
                    ));

                    /**
                     * Check and process shortcuts
                     */
                    if (isset($property['shortcuts'])) {
                        $this->_processShorcuts($property, $classDefinition);
                    }
                }
            }

            /**
             * Register constants
             */
            if (isset($definition['constants'])) {
                foreach ($definition['constants'] as $constant) {
                    $classDefinition->addConstant(new ClassConstant(
                        $constant['name'],
                        isset($constant['default']) ? $constant['default'] : null,
                        isset($constant['docblock']) ? $constant['docblock'] : null
                    ));
                }
            }

            /**
             * Register methods
             */
            if (isset($definition['methods'])) {
                foreach ($definition['methods'] as $method) {
                    $classDefinition->addMethod(new ClassMethod(
                        $classDefinition,
                        $method['visibility'],
                        $method['name'],
                        isset($method['parameters']) ? new ClassMethodParameters($method['parameters']) : null,
                        isset($method['statements']) ? new StatementsBlock($method['statements']) : null,
                        isset($method['docblock']) ? $method['docblock'] : null,
                        isset($method['return-type']) ? $method['return-type'] : null,
                        $method
                    ), $method);
                }
            }
        }

        $this->_classDefinition = $classDefinition;

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
     * Pre-compiles a Zephir file. Generates the IR and perform basic validations
     *
     * @param Compiler $compiler
     * @throws ParseException
     * @throws CompilerException
     * @throws Exception
     */
    public function preCompile(Compiler $compiler)
    {
        $ir = $this->genIR($compiler);

        if (!is_array($ir)) {
            throw new Exception("Cannot parse file: " . realpath($this->_filePath));
        }

        if (isset($ir['type']) && $ir['type'] == 'error') {
            throw new ParseException($ir['message'], $ir);
        }

        /**
         * Alias Manager
         */
        $this->_aliasManager = new AliasManager();

        /**
         * Compilation context stores common objects required by compilation entities
         */
        $compilationContext = new CompilationContext;

        /**
         * Set global compiler in the compilation context
         */
        $compilationContext->compiler = $compiler;

        /**
         * Set global config in the compilation context
         */
        $compilationContext->config = $this->_config;

        /**
         * Set global logger in the compilation context
         */
        $compilationContext->logger = $this->_logger;

        /**
         * Alias manager
         */
        $compilationContext->aliasManager = $this->_aliasManager;

        /**
         * Traverse the top level statements looking for the namespace
         */
        $namespace = null;
        foreach ($ir as $topStatement) {
            switch ($topStatement['type']) {

                case 'namespace':
                    if ($namespace !== null) {
                        throw new CompilerException("The namespace must be defined just one time", $topStatement);
                    }
                    $namespace = $topStatement['name'];
                    $this->_namespace = $namespace;
                    if (!preg_match('/^[A-Z]/', $namespace)) {
                        throw new CompilerException("Namespace '" . $namespace . "' must be in camelized-form", $topStatement);
                    }
                    break;

                case 'cblock':
                    $this->_headerCBlocks[] = $topStatement['value'];
                    break;

                case 'function':
                    /* Just do the precompilation of the function */
                    $functionDefinition = new FunctionDefinition(
                        $namespace,
                        $topStatement['name'],
                        isset($topStatement['parameters']) ? new ClassMethodParameters($topStatement['parameters']) : null,
                        isset($topStatement['statements']) ? new StatementsBlock($topStatement['statements']) : null,
                        isset($topStatement['return-type']) ? $topStatement['return-type'] : null,
                        $topStatement
                    );
                    $functionDefinition->preCompile($compilationContext);
                    $this->addFunction($compiler, $functionDefinition, $topStatement);
                    break;
            }
        }

        if (!$namespace) {
            throw new CompilerException("A namespace is required", $topStatement);
        }

        /* Set namespace and flag as global, if before namespace declaration */
        foreach ($this->_functionDefinitions as $funcDef) {
            if ($funcDef->getNamespace() == null) {
                $funcDef->setGlobal(true);
                $funcDef->setNamespace($compiler->getConfig()->get('namespace'));
            }
        }

        $class = false;
        $interface = false;
        $lastComment = null;

        foreach ($ir as $topStatement) {
            switch ($topStatement['type']) {

                case 'class':
                    if ($class || $interface) {
                        throw new CompilerException("More than one class/interface defined in the same file", $topStatement);
                    }
                    $class = true;
                    $name = $topStatement['name'];
                    $this->preCompileClass($compilationContext, $namespace, $topStatement, $lastComment);
                    $this->_originalNode = $topStatement;
                    $lastComment = null;
                    break;

                case 'interface':
                    if ($class || $interface) {
                        throw new CompilerException("More than one class/interface defined in the same file", $topStatement);
                    }
                    $interface = true;
                    $name = $topStatement['name'];
                    $this->preCompileInterface($namespace, $topStatement, $lastComment);
                    $this->_originalNode = $topStatement;
                    $lastComment = null;
                    break;

                case 'use':
                    if ($interface || $class) {
                        throw new CompilerException("Aliasing must be done before declaring any class or interface", $topStatement);
                    }
                    $this->_aliasManager->add($topStatement);
                    break;

                case 'comment':
                    $lastComment = $topStatement;
                    break;
            }
        }

        if (!$class && !$interface) {
            throw new CompilerException("Every file must contain at least a class or an interface", $topStatement);
        }

        if (!$this->_external) {
            $expectedPath = strtolower(str_replace('\\', DIRECTORY_SEPARATOR, $namespace) . DIRECTORY_SEPARATOR . $name) . '.zep';
            if (strtolower($this->_filePath) != $expectedPath) {
                $className = str_replace('\\', '/', $namespace) . '\\' . $name;
                $message = 'Unexpected class name ' . $className . ' in file: ' . $this->_filePath . ', expected: ' . $expectedPath;
                throw new CompilerException($message);
            }
        }

        $this->_ir = $ir;
    }

    /**
     * Returns the path to the compiled file
     *
     * @return string
     */
    public function getCompiledFile()
    {
        return $this->_compiledFile;
    }

    /**
     * Check dependencies
     *
     * @param Compiler $compiler
     */
    public function checkDependencies(Compiler $compiler)
    {
        $classDefinition = $this->_classDefinition;

        $extendedClass = $classDefinition->getExtendsClass();
        if ($extendedClass) {
            if ($classDefinition->getType() == 'class') {
                if ($compiler->isClass($extendedClass)) {
                    $extendedDefinition = $compiler->getClassDefinition($extendedClass);
                    $classDefinition->setExtendsClassDefinition($extendedDefinition);
                } else {
                    if ($compiler->isBundledClass($extendedClass)) {
                        $extendedDefinition = $compiler->getInternalClassDefinition($extendedClass);
                        $classDefinition->setExtendsClassDefinition($extendedDefinition);
                    } else {
                        $extendedDefinition = new ClassDefinitionRuntime($extendedClass);
                        $classDefinition->setExtendsClassDefinition($extendedDefinition);
                        $this->_logger->warning('Cannot locate class "' . $extendedClass . '" when extending class "' . $classDefinition->getCompleteName() . '"', 'nonexistent-class', $this->_originalNode);
                    }
                }
            } else {
                if ($compiler->isInterface($extendedClass)) {
                    $extendedDefinition = $compiler->getClassDefinition($extendedClass);
                    $classDefinition->setExtendsClassDefinition($extendedDefinition);
                } else {
                    if ($compiler->isBundledInterface($extendedClass)) {
                        $extendedDefinition = $compiler->getInternalClassDefinition($extendedClass);
                        $classDefinition->setExtendsClassDefinition($extendedDefinition);
                    } else {
                        $extendedDefinition = new ClassDefinitionRuntime($extendedClass);
                        $classDefinition->setExtendsClassDefinition($extendedDefinition);
                        $this->_logger->warning('Cannot locate class "' . $extendedClass . '" when extending interface "' . $classDefinition->getCompleteName() . '"', 'nonexistent-class', $this->_originalNode);
                    }
                }
            }
        }

        $implementedInterfaces = $classDefinition->getImplementedInterfaces();
        if ($implementedInterfaces) {
            $interfaceDefinitions = array();

            foreach ($implementedInterfaces as $interface) {
                if ($compiler->isInterface($interface)) {
                    $interfaceDefinitions[$interface] = $compiler->getClassDefinition($interface);
                } else {
                    if ($compiler->isBundledInterface($interface)) {
                        $interfaceDefinitions[$interface] = $compiler->getInternalClassDefinition($interface);
                    } else {
                        $extendedDefinition = new ClassDefinitionRuntime($extendedClass);
                        $classDefinition->setExtendsClassDefinition($extendedDefinition);
                        $this->_logger->warning('Cannot locate class "' . $interface . '" when extending interface "' . $classDefinition->getCompleteName() . '"', 'nonexistent-class', $this->_originalNode);
                    }
                }
            }

            if ($interfaceDefinitions) {
                $classDefinition->setImplementedInterfaceDefinitions($interfaceDefinitions);
            }
        }
    }

    /**
     * Compiles the file
     *
     * @param Compiler $compiler
     * @param StringsManager $stringsManager
     */
    public function compile(Compiler $compiler, StringsManager $stringsManager)
    {
        if (!$this->_ir) {
            throw new CompilerException('IR related to compiled file is missing');
        }

        /**
         * External classes should not be compiled as part of the extension
         */
        if ($this->_external) {
            return;
        }

        /**
         * Compilation context stores common objects required by compilation entities
         */
        $compilationContext = new CompilationContext;

        /**
         * Set global compiler in the compilation context
         */
        $compilationContext->compiler = $compiler;

        /**
         * Set global config in the compilation context
         */
        $compilationContext->config = $this->_config;

        /**
         * Set global logger in the compilation context
         */
        $compilationContext->logger = $this->_logger;

        /**
         * Set global strings manager
         */
        $compilationContext->stringsManager = $stringsManager;

        /**
         * Headers manager
         */
        $headersManager = new HeadersManager();
        $compilationContext->headersManager = $headersManager;

        /**
         * Main code-printer for the file
         */
        $codePrinter = new CodePrinter();
        $compilationContext->codePrinter = $codePrinter;

        /**
         * Alias manager
         */
        $compilationContext->aliasManager = $this->_aliasManager;

        $codePrinter->outputBlankLine();

        $class = false;
        $interface = false;

        foreach ($this->_ir as $topStatement) {
            switch ($topStatement['type']) {

                case 'class':
                    if ($interface || $class) {
                        throw new CompilerException("More than one class defined in the same file", $topStatement);
                    }
                    $class = true;
                    $this->compileClass($compilationContext, $this->_namespace, $topStatement);
                    break;

                case 'interface':
                    if ($interface || $class) {
                        throw new CompilerException("More than one class defined in the same file", $topStatement);
                    }
                    $class = true;
                    $this->compileClass($compilationContext, $this->_namespace, $topStatement);
                    break;

                case 'comment':
                    $this->compileComment($compilationContext, $topStatement);
                    break;
            }
        }

        /* ensure functions are handled last */
        foreach ($this->_functionDefinitions as $funcDef) {
            $this->compileFunction($compilationContext, $funcDef);
        }

        /* apply headers */
        $this->applyClassHeaders($compilationContext);

        $classDefinition = $this->_classDefinition;
        if (!$classDefinition) {
            $this->_ir = null;
            return;
        }

        $classDefinition->setOriginalNode($this->_originalNode);

        $completeName = $classDefinition->getCompleteName();

        $path = str_replace('\\', DIRECTORY_SEPARATOR, strtolower($completeName));

        $filePath = 'ext/' . $path . '.zep.c';
        $filePathHeader = 'ext/' . $path . '.zep.h';

        if (strpos($path, DIRECTORY_SEPARATOR)) {
            $dirname = dirname($filePath);
            if (!is_dir($dirname)) {
                mkdir($dirname, 0755, true);
            }
        }

        if ($codePrinter) {
            /**
             * If the file does not exists we create it for the first time
             */
            if (!file_exists($filePath)) {
                file_put_contents($filePath, $codePrinter->getOutput());
                if ($compilationContext->headerPrinter) {
                    file_put_contents($filePathHeader, $compilationContext->headerPrinter->getOutput());
                }
            } else {
                $fileSystem = $compiler->getFileSystem();

                /**
                 * Use md5 hash to avoid rewrite the file again and again when it hasn't changed
                 * thus avoiding unnecessary recompilations
                 */
                $output = $codePrinter->getOutput();
                $hash = $fileSystem->getHashFile('md5', $filePath, true);
                if (md5($output) != $hash) {
                    file_put_contents($filePath, $output);
                }

                if ($compilationContext->headerPrinter) {
                    $output = $compilationContext->headerPrinter->getOutput();
                    $hash = $fileSystem->getHashFile('md5', $filePathHeader, true);
                    if (md5($output) != $hash) {
                        file_put_contents($filePathHeader, $output);
                    }
                }
            }
        }

        /**
         * Add to file compiled
         */
        $this->_compiledFile = $path . '.c';
        $this->_ir = null;
    }

    public function applyClassHeaders(CompilationContext $compilationContext)
    {
        $classDefinition = $this->_classDefinition;

        $separators = str_repeat('../', count(explode('\\', $classDefinition->getCompleteName())) - 1);

        $code  = '' . PHP_EOL;
        $code .= '#ifdef HAVE_CONFIG_H' . PHP_EOL;
        $code .= '#include "' . $separators . 'ext_config.h"' . PHP_EOL;
        $code .= '#endif' . PHP_EOL;
        $code .= '' . PHP_EOL;

        $code .= '#include <php.h>' . PHP_EOL;
        $code .= '#include "' . $separators . 'php_ext.h"' . PHP_EOL;
        $code .= '#include "' . $separators . 'ext.h"' . PHP_EOL;
        $code .= '' . PHP_EOL;

        if ($classDefinition->getType() == 'class') {
            $code .= '#include <Zend/zend_operators.h>' . PHP_EOL;
            $code .= '#include <Zend/zend_exceptions.h>' . PHP_EOL;
            $code .= '#include <Zend/zend_interfaces.h>' . PHP_EOL;
        } else {
            $code .= '#include <Zend/zend_exceptions.h>' . PHP_EOL;
        }
        $code .= '' . PHP_EOL;

        $code .= '#include "kernel/main.h"' . PHP_EOL;

        if ($classDefinition->getType() == 'class') {
            foreach ($compilationContext->headersManager->get() as $header => $one) {
                $code .= '#include "' . $header . '.h"' . PHP_EOL;
            }
        }

        if (count($this->_headerCBlocks) > 0) {
            $code .= implode($this->_headerCBlocks, PHP_EOL) . PHP_EOL;
        }

        /**
         * Prepend the required files to the header
         */
        $compilationContext->codePrinter->preOutput($code);
    }

    /**
     * Transform class/interface name to FQN format
     *
     * @param string $name
     * @return string
     */
    protected function getFullName($name)
    {
        return Utils::getFullName($name, $this->_namespace, $this->_aliasManager);
    }

    /**
     *
     * Returns the path to the source file
     *
     * @return type
     */
    public function getFilePath()
    {
        return $this->_filePath;
    }
}
