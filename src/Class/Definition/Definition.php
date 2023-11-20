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

namespace Zephir\Class\Definition;

use Zephir\AliasManager;
use Zephir\Class\Constant;
use Zephir\Class\Entry;
use Zephir\Class\Method\Method;
use Zephir\Class\Method\Parameters;
use Zephir\Class\Property;
use Zephir\Code\ArgInfoDefinition;
use Zephir\Code\Printer;
use Zephir\CompilationContext;
use Zephir\Compiler;
use Zephir\Documentation\Docblock;
use Zephir\Documentation\DocblockParser;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Exception\InvalidArgumentException;
use Zephir\HeadersManager;
use Zephir\StatementsBlock;

/**
 * Represents a class/interface and their properties and methods.
 */
final class Definition extends AbstractDefinition
{
    public const TYPE_CLASS = 'class';
    public const TYPE_INTERFACE = 'interface';

    /**
     * Class short name
     *
     * If didn't specify, then equals to $name
     *
     * @see Definition::name
     */
    protected string $shortName;

    /**
     * Definition type
     */
    protected string $type = self::TYPE_CLASS;

    /**
     * Name of inherited class
     */
    protected ?string $extendsClass = null;

    /**
     * List of implemented interfaces of current class
     */
    protected array $interfaces = [];

    /**
     * Contains "final" in the definition
     */
    protected bool $final = false;

    /**
     * Contains "abstract" in the definition
     */
    protected bool $abstract = false;

    /**
     * When class is from external dependency
     */
    protected bool $external = false;

    /**
     * Definition object of inherited class
     */
    protected ?AbstractDefinition $extendsClassDefinition = null;

    protected array $implementedInterfaceDefinitions = [];

    /**
     * @var Property[]
     */
    protected array $properties = [];

    /**
     * @var Constant[]
     */
    protected array $constants = [];

    /**
     * @var Method[]
     */
    protected array $methods = [];

    protected string $docBlock = '';

    protected ?Docblock $parsedDocblock = null;

    protected int $dependencyRank = 0;

    protected array $originalNode = [];

    protected bool $isBundled = false;

    protected ?AliasManager $aliasManager = null;

    protected Compiler $compiler;

    public function __construct(protected string $namespace, string $name, string $shortName = null)
    {
        $this->name = $name;
        $this->shortName = $shortName ?: $name;
    }

    /**
     * Sets if the class is internal or not.
     */
    public function setIsBundled(bool $isBundled): void
    {
        $this->isBundled = $isBundled;
    }

    /**
     * Returns whether the class is bundled or not.
     */
    public function isBundled(): bool
    {
        return $this->isBundled;
    }

    /**
     * Sets whether the class is external or not.
     */
    public function setIsExternal(bool $isExternal): void
    {
        $this->external = $isExternal;
    }

    /**
     * Returns whether the class is internal or not.
     */
    public function isExternal(): bool
    {
        return $this->external;
    }

    /**
     * Set the class' type (class/interface).
     */
    public function setType(string $type): void
    {
        $this->type = $type;
    }

    /**
     * Returns the class type.
     */
    public function getType(): string
    {
        return $this->type;
    }

    /**
     * Returns the class name without namespace.
     */
    public function getShortName(): string
    {
        return $this->shortName;
    }

    /**
     * Check if the class definition correspond to an interface.
     */
    public function isInterface(): bool
    {
        return 'interface' === $this->type;
    }

    /**
     * Sets if the class is final.
     */
    public function setIsFinal(bool $final): void
    {
        $this->final = $final;
    }

    /**
     * Sets if the class is final.
     */
    public function setIsAbstract(bool $abstract): void
    {
        $this->abstract = $abstract;
    }

    /**
     * Checks whether the class is abstract or not.
     */
    public function isAbstract(): bool
    {
        return $this->abstract;
    }

    /**
     * Checks whether the class is abstract or not.
     */
    public function isFinal(): bool
    {
        return $this->final;
    }

    /**
     * Returns the class name including its namespace.
     */
    public function getCompleteName(): string
    {
        return $this->namespace.'\\'.$this->shortName;
    }

    /**
     * Return the class namespace.
     */
    public function getNamespace(): string
    {
        return $this->namespace;
    }

    /**
     * Set the original node where the class was declared.
     */
    public function setOriginalNode(array $originalNode): void
    {
        $this->originalNode = $originalNode;
    }

    /**
     * Sets the extended class.
     */
    public function setExtendsClass(string $extendsClass): void
    {
        $this->extendsClass = $extendsClass;
    }

    /**
     * Sets the implemented interfaces.
     */
    public function setImplementsInterfaces(array $implementedInterfaces): void
    {
        $this->interfaces = [];
        foreach ($implementedInterfaces as $implementedInterface) {
            $this->interfaces[] = $implementedInterface['value'];
        }
    }

    /**
     * Returns the extended class.
     */
    public function getExtendsClass(): ?string
    {
        return $this->extendsClass;
    }

    /**
     * Returns the implemented interfaces.
     */
    public function getImplementedInterfaces(): array
    {
        return $this->interfaces;
    }

    /**
     * Sets the class definition for the extended class.
     */
    public function setExtendsClassDefinition(AbstractDefinition $classDefinition): void
    {
        $this->extendsClassDefinition = $classDefinition;
    }

    /**
     * Returns the class definition related to the extended class.
     */
    public function getExtendsClassDefinition(): ?AbstractDefinition
    {
        if (!$this->extendsClassDefinition && $this->extendsClass && $this->compiler) {
            $this->setExtendsClassDefinition($this->compiler->getClassDefinition($this->extendsClass));
        }

        return $this->extendsClassDefinition;
    }

    /**
     * Sets the class definition for the implemented interfaces.
     */
    public function setImplementedInterfaceDefinitions(array $implementedInterfaceDefinitions): void
    {
        $this->implementedInterfaceDefinitions = $implementedInterfaceDefinitions;
    }

    /**
     * Returns the class definition for the implemented interfaces.
     */
    public function getImplementedInterfaceDefinitions(): array
    {
        return $this->implementedInterfaceDefinitions;
    }

    /**
     * Calculate the dependency rank of the class based on its dependencies.
     */
    public function getDependencies(): array
    {
        $dependencies = [];
        if ($this->extendsClassDefinition instanceof self) {
            $dependencies[] = $this->extendsClassDefinition;
        }

        foreach ($this->implementedInterfaceDefinitions as $interfaceDefinition) {
            if ($interfaceDefinition instanceof self) {
                $dependencies[] = $interfaceDefinition;
            }
        }

        return $dependencies;
    }

    /**
     * A class definition calls this method to mark this class as a dependency of another.
     *
     * @param int $rank
     */
    public function increaseDependencyRank(int $rank): void
    {
        $this->dependencyRank += $rank + 1;
    }

    /**
     * Returns the dependency rank for this class.
     */
    public function getDependencyRank(): int
    {
        return $this->dependencyRank;
    }

    /**
     * Sets the class/interface docBlock.
     */
    public function setDocBlock(string $docBlock): void
    {
        $this->docBlock = $docBlock;
    }

    /**
     * Returns the class/interface docBlock.
     */
    public function getDocBlock(): string
    {
        return $this->docBlock;
    }

    /**
     * Returns the parsed docBlock.
     */
    public function getParsedDocBlock(): ?DocBlock
    {
        if ($this->parsedDocblock instanceof Docblock) {
            return $this->parsedDocblock;
        }

        if ($this->docBlock === '') {
            return null;
        }

        $this->parsedDocblock = (new DocblockParser('/'.$this->docBlock.'/'))->parse();

        return $this->parsedDocblock;
    }

    /**
     * Adds a property to the definition.
     *
     * @throws CompilerException
     */
    public function addProperty(Property $property): void
    {
        if (isset($this->properties[$property->getName()])) {
            throw new CompilerException("Property '".$property->getName()."' was defined more than one time", $property->getOriginal());
        }

        $this->properties[$property->getName()] = $property;
    }

    /**
     * Adds a constant to the definition.
     *
     * @throws CompilerException
     */
    public function addConstant(Constant $constant): void
    {
        if (isset($this->constants[$constant->getName()])) {
            throw new CompilerException("Constant '".$constant->getName()."' was defined more than one time");
        }

        $this->constants[$constant->getName()] = $constant;
    }

    /**
     * Checks if a class definition has a property.
     */
    public function hasProperty(string $name): bool
    {
        if (isset($this->properties[$name])) {
            return true;
        }

        $extendsClassDefinition = $this->getExtendsClassDefinition();

        return $extendsClassDefinition instanceof self && $extendsClassDefinition->hasProperty($name);
    }

    /**
     * Returns a method definition by its name.
     *
     * @param string $propertyName
     *
     * @return Property|null
     */
    public function getProperty(string $propertyName): ?Property
    {
        if (isset($this->properties[$propertyName])) {
            return $this->properties[$propertyName];
        }

        $extendsClassDefinition = $this->getExtendsClassDefinition();
        if ($extendsClassDefinition instanceof self) {
            return $extendsClassDefinition->getProperty($propertyName);
        }

        return null;
    }

    /**
     * Checks if class definition has a property.
     */
    public function hasConstant(string $name): bool
    {
        if (isset($this->constants[$name])) {
            return true;
        }

        $extendsClassDefinition = $this->getExtendsClassDefinition();
        if ($extendsClassDefinition instanceof self && $extendsClassDefinition->hasConstant($name)) {
            return true;
        }

        /**
         * Check if constant is defined in interfaces
         */
        return $this->hasConstantFromInterfaces($name);
    }

    /**
     * Returns a constant definition by its name.
     *
     * @throws InvalidArgumentException
     */
    public function getConstant(string $constantName): ?Constant
    {
        if (isset($this->constants[$constantName])) {
            return $this->constants[$constantName];
        }

        $extendsClassDefinition = $this->getExtendsClassDefinition();
        if ($extendsClassDefinition instanceof self && $extendsClassDefinition->hasConstant($constantName)) {
            return $extendsClassDefinition->getConstant($constantName);
        }

        /**
         * Gets constant from interfaces
         */
        return $this->getConstantFromInterfaces($constantName);
    }

    /**
     * Adds a method to the class definition.
     *
     * @throws CompilerException
     */
    public function addMethod(Method $method, array $statement = null): void
    {
        $methodName = strtolower($method->getName());
        if (isset($this->methods[$methodName])) {
            throw new CompilerException("Method '".$method->getName()."' was defined more than one time", $statement);
        }

        $this->methods[$methodName] = $method;
    }

    /**
     * Updates an existing method definition.
     *
     * @throws CompilerException
     */
    public function updateMethod(Method $method, array $statement = null): void
    {
        $methodName = strtolower($method->getName());
        if (!isset($this->methods[$methodName])) {
            throw new CompilerException("Method '".$method->getName()."' does not exist", $statement);
        }

        $this->methods[$methodName] = $method;
    }

    /**
     * Returns all properties defined in the class.
     *
     * @return Property[]
     */
    public function getProperties(): array
    {
        return $this->properties;
    }

    /**
     * Returns all constants defined in the class.
     *
     * @return Constant[]
     */
    public function getConstants(): array
    {
        return $this->constants;
    }

    /**
     * Returns all methods defined in the class.
     *
     * @return Method[]
     */
    public function getMethods(): array
    {
        return $this->methods;
    }

    /**
     * Checks if the class implements a specific name.
     *
     * @param string $methodName
     *
     * @return bool
     */
    public function hasMethod(string $methodName): bool
    {
        $methodNameLower = strtolower($methodName);
        foreach ($this->methods as $name => $method) {
            if ($methodNameLower === $name) {
                return true;
            }
        }

        $extendsClassDefinition = $this->getExtendsClassDefinition();
        if ($extendsClassDefinition instanceof DefinitionRuntime) {
            try {
                $extendsClassDefinition = $this->compiler->getInternalClassDefinition(
                    $extendsClassDefinition->getName()
                );
            } catch (\ReflectionException $e) {
                // Do nothing
                return false;
            }
        }

        while ($extendsClassDefinition instanceof self) {
            if ($extendsClassDefinition->hasMethod($methodName)) {
                return true;
            }

            $extendsClassDefinition = $extendsClassDefinition->getExtendsClassDefinition();
        }

        return false;
    }

    /**
     * Returns a method by its name.
     */
    public function getMethod(string $methodName, bool $checkExtends = true): ?Method
    {
        $methodNameLower = strtolower($methodName);
        foreach ($this->methods as $name => $method) {
            if ($methodNameLower === $name) {
                return $method;
            }
        }

        if (!$checkExtends) {
            return null;
        }

        $extendsClassDefinition = $this->getExtendsClassDefinition();
        if ($extendsClassDefinition instanceof self) {
            if ($extendsClassDefinition->hasMethod($methodName)) {
                return $extendsClassDefinition->getMethod($methodName);
            }
        }

        return null;
    }

    /**
     * Set a method and its body.
     */
    public function setMethod(string $methodName, Method $method): void
    {
        $this->methods[$methodName] = $method;
    }

    /**
     * Tries to find the most similar name.
     */
    public function getPossibleMethodName(string $methodName): ?string
    {
        $methodNameLower = strtolower($methodName);

        foreach ($this->methods as $name => $method) {
            if (metaphone($methodNameLower) === metaphone($name)) {
                return $method->getName();
            }
        }

        if ($this->extendsClassDefinition instanceof self) {
            return $this->extendsClassDefinition->getPossibleMethodName($methodName);
        }

        return null;
    }

    /**
     * Returns the name of the zend_class_entry according to the class name.
     *
     * @throws Exception
     */
    public function getClassEntry(CompilationContext $compilationContext = null): string
    {
        if ($this->external) {
            if ($compilationContext === null) {
                throw new Exception('A compilation context is required');
            }

            $this->compiler = $compilationContext->compiler;

            /**
             * Automatically add the external header
             */
            $compilationContext->headersManager->add($this->getExternalHeader(), HeadersManager::POSITION_LAST);
        }

        return strtolower(str_replace('\\', '_', $this->namespace).'_'.$this->name).'_ce';
    }

    /**
     * Returns a valid namespace to be used in C-sources.
     */
    public function getCNamespace(): string
    {
        return str_replace('\\', '_', $this->namespace);
    }

    /**
     * Returns a valid namespace to be used in C-sources.
     */
    public function getNCNamespace(): string
    {
        return str_replace('\\', '\\\\', $this->namespace);
    }

    /**
     * Class name without namespace prefix for class registration.
     */
    public function getSCName(string $namespace): string
    {
        return str_replace($namespace.'_', '', strtolower(str_replace('\\', '_', $this->namespace).'_'.$this->name));
    }

    /**
     * Returns an absolute location to the class header.
     */
    public function getExternalHeader(): string
    {
        $parts = explode('\\', $this->namespace);

        return 'ext/'.strtolower($parts[0].\DIRECTORY_SEPARATOR.str_replace('\\', \DIRECTORY_SEPARATOR, $this->namespace).\DIRECTORY_SEPARATOR.$this->name).'.zep';
    }

    /**
     * Checks if a class implements an interface.
     *
     * @throws CompilerException
     */
    public function checkInterfaceImplements(self $classDefinition, self $interfaceDefinition): void
    {
        foreach ($interfaceDefinition->getMethods() as $method) {
            if (!$classDefinition->hasMethod($method->getName())) {
                throw new CompilerException(
                    sprintf(
                        'Class %s must implement a method called: "%s" as requirement of interface: "%s"',
                        $classDefinition->getCompleteName(),
                        $method->getName(),
                        $interfaceDefinition->getCompleteName()
                    )
                );
            }

            if (!$method->hasParameters()) {
                continue;
            }

            $implementedMethod = $classDefinition->getMethod($method->getName());
            if ($implementedMethod->getNumberOfRequiredParameters() > $method->getNumberOfRequiredParameters()
                || $implementedMethod->getNumberOfParameters() < $method->getNumberOfParameters()
            ) {
                throw new CompilerException(
                    sprintf(
                        'Method %s::%s() does not have the same number of required parameters in interface: "%s"',
                        $classDefinition->getCompleteName(),
                        $method->getName(),
                        $interfaceDefinition->getCompleteName()
                    )
                );
            }
        }
    }

    /**
     * Pre-compiles a class/interface gathering method information required by other methods.
     *
     * @throws CompilerException
     */
    public function preCompile(CompilationContext $compilationContext): void
    {
        $this->compiler = $compilationContext->compiler;

        /**
         * Pre-Compile methods
         */
        foreach ($this->methods as $method) {
            if (self::TYPE_CLASS === $this->getType() && !$method->isAbstract()) {
                $method->preCompile($compilationContext);
            }
        }
    }

    /**
     * Returns the initialization method if any does exist.
     */
    public function getInitMethod(): ?Method
    {
        $initClassName = $this->getCNamespace().'_'.$this->getName();

        return $this->getMethod('zephir_init_properties_'.$initClassName);
    }

    /**
     * Returns the initialization method if any does exist.
     */
    public function getStaticInitMethod(): ?Method
    {
        $initClassName = $this->getCNamespace().'_'.$this->getName();

        return $this->getMethod('zephir_init_static_properties_'.$initClassName);
    }

    /**
     * Returns the initialization method if any does exist.
     */
    public function getLocalOrParentInitMethod(): ?Method
    {
        $method = $this->getInitMethod();
        if ($method === null) {
            return null;
        }

        $parentClassDefinition = $this->getExtendsClassDefinition();
        if ($parentClassDefinition instanceof self) {
            $method = $parentClassDefinition->getInitMethod();
            if ($method instanceof Method) {
                $this->addInitMethod($method->getStatementsBlock());
            }
        }

        return $method;
    }

    /**
     * Creates the initialization method.
     */
    public function addInitMethod(StatementsBlock $statementsBlock): void
    {
        if ($statementsBlock->isEmpty()) {
            return;
        }

        $initClassName = $this->getCNamespace().'_'.$this->getName();
        $classMethod = new Method(
            $this,
            ['internal'],
            'zephir_init_properties_'.$initClassName,
            null,
            $statementsBlock
        );

        $classMethod->setIsInitializer(true);
        $this->addMethod($classMethod);
    }

    /**
     * Creates the static initialization method.
     */
    public function addStaticInitMethod(StatementsBlock $statementsBlock): void
    {
        $initClassName = $this->getCNamespace().'_'.$this->getName();

        $classMethod = new Method(
            $this,
            ['internal'],
            'zephir_init_static_properties_'.$initClassName,
            null,
            $statementsBlock
        );

        $classMethod->setIsInitializer(true);
        $classMethod->setIsStatic(true);
        $this->addMethod($classMethod);
    }

    /**
     * Compiles a class/interface.
     *
     * @throws Exception
     * @throws \ReflectionException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $this->compiler = $compilationContext->compiler;

        /**
         * Sets the current object as global class definition
         */
        $compilationContext->classDefinition = $this;

        /**
         * Get the global codePrinter.
         */
        $codePrinter = $compilationContext->codePrinter;

        /**
         * The ZEPHIR_INIT_CLASS defines properties and constants exported by the class.
         */
        $initClassName = $this->getCNamespace().'_'.$this->getName();
        $codePrinter->output('ZEPHIR_INIT_CLASS('.$initClassName.')');
        $codePrinter->output('{');
        $codePrinter->increaseLevel();

        /**
         * Method entry.
         */
        $methods = &$this->methods;
        $initMethod = $this->getLocalOrParentInitMethod();

        if (\count($methods) > 0 || $initMethod) {
            $methodEntry = strtolower($this->getCNamespace()).'_'.strtolower($this->getName()).'_method_entry';
        } else {
            $methodEntry = 'NULL';
        }

        foreach ($methods as $method) {
            $method->setupOptimized($compilationContext);
        }

        $namespace = str_replace('\\', '_', $compilationContext->config->get('namespace'));

        $flags = '0';
        if ($this->isAbstract()) {
            $flags = 'ZEND_ACC_EXPLICIT_ABSTRACT_CLASS';
        }

        if ($this->isFinal()) {
            if ('0' === $flags) {
                $flags = 'ZEND_ACC_FINAL_CLASS';
            } else {
                $flags .= '|ZEND_ACC_FINAL_CLASS';
            }
        }

        /**
         * Register the class with extends + interfaces.
         */
        $classExtendsDefinition = null;
        if ($this->extendsClass) {
            $classExtendsDefinition = $this->extendsClassDefinition;
            if ($classExtendsDefinition instanceof self && !$classExtendsDefinition->isBundled()) {
                $classEntry = $classExtendsDefinition->getClassEntry($compilationContext);
            } else {
                $className = method_exists($classExtendsDefinition, 'getCompleteName') ? $classExtendsDefinition->getCompleteName() : $classExtendsDefinition->getName();
                $classEntry = (new Entry('\\'.ltrim($className, '\\'), $compilationContext))->get();
            }

            if (self::TYPE_CLASS === $this->getType()) {
                $codePrinter->output('ZEPHIR_REGISTER_CLASS_EX('.$this->getNCNamespace().', '.$this->getName().', '.$namespace.', '.strtolower($this->getSCName($namespace)).', '.$classEntry.', '.$methodEntry.', '.$flags.');');
            } else {
                $codePrinter->output('ZEPHIR_REGISTER_INTERFACE_EX('.$this->getNCNamespace().', '.$this->getName().', '.$namespace.', '.strtolower($this->getSCName($namespace)).', '.$classEntry.', '.$methodEntry.');');
            }
        } else {
            if (self::TYPE_CLASS === $this->getType()) {
                $codePrinter->output('ZEPHIR_REGISTER_CLASS('.$this->getNCNamespace().', '.$this->getName().', '.$namespace.', '.strtolower($this->getSCName($namespace)).', '.$methodEntry.', '.$flags.');');
            } else {
                $codePrinter->output('ZEPHIR_REGISTER_INTERFACE('.$this->getNCNamespace().', '.$this->getName().', '.$namespace.', '.strtolower($this->getSCName($namespace)).', '.$methodEntry.');');
            }
        }

        $codePrinter->outputBlankLine();

        /**
         * Compile properties.
         */
        foreach ($this->getProperties() as $property) {
            $docBlock = $property->getDocBlock();
            if ($docBlock) {
                $codePrinter->outputDocBlock($docBlock);
            }

            $property->compile($compilationContext);
            $codePrinter->outputBlankLine();
        }

        $initMethod = $this->getInitMethod();
        if ($initMethod) {
            $codePrinter->output($namespace.'_'.strtolower($this->getSCName($namespace)).'_ce->create_object = '.$initMethod->getName().';');
        }

        /**
         * Compile constants.
         */
        foreach ($this->getConstants() as $constant) {
            $docBlock = $constant->getDocBlock();
            if ($docBlock) {
                $codePrinter->outputDocBlock($docBlock);
            }

            $constant->compile($compilationContext);
            $codePrinter->outputBlankLine();
        }

        /**
         * Implemented interfaces.
         */
        $interfaces = $this->interfaces;
        $compiler = $compilationContext->compiler;

        if (\is_array($interfaces)) {
            $codePrinter->outputBlankLine(true);

            foreach ($interfaces as $interface) {
                /**
                 * Try to find the interface.
                 */
                $classEntry = null;

                if ($compiler->isInterface($interface)) {
                    $classInterfaceDefinition = $compiler->getClassDefinition($interface);
                    $classEntry = $classInterfaceDefinition->getClassEntry($compilationContext);
                } elseif ($compiler->isBundledInterface($interface)) {
                    $classInterfaceDefinition = $compiler->getInternalClassDefinition($interface);
                    $classEntry = (new Entry('\\'.$classInterfaceDefinition->getName(), $compilationContext))->get();
                }

                if (!$classEntry) {
                    if ($compiler->isClass($interface)) {
                        throw new CompilerException(
                            sprintf(
                                'Cannot locate interface %s when implementing interfaces on %s. '.
                                '%s is currently a class',
                                $interface,
                                $this->getCompleteName(),
                                $interface
                            ),
                            $this->originalNode
                        );
                    } else {
                        throw new CompilerException(
                            sprintf(
                                'Cannot locate interface %s when implementing interfaces on %s',
                                $interface,
                                $this->getCompleteName()
                            ),
                            $this->originalNode
                        );
                    }
                }

                /**
                 * We don't check if abstract classes implement the methods in their interfaces
                 */
                if (!$this->isAbstract() && !$this->isInterface()) {
                    $this->checkInterfaceImplements($this, $classInterfaceDefinition);
                }

                $codePrinter->output(sprintf(
                    'zend_class_implements(%s, 1, %s);',
                    $this->getClassEntry(),
                    $classEntry
                ));
            }
        }

        if (!$this->isAbstract() && !$this->isInterface()) {
            /**
             * Interfaces in extended classes may have
             */
            if ($classExtendsDefinition instanceof self && !$classExtendsDefinition->isBundled()) {
                $interfaces = $classExtendsDefinition->getImplementedInterfaces();
                foreach ($interfaces as $interface) {
                    $classInterfaceDefinition = null;
                    if ($compiler->isInterface($interface)) {
                        $classInterfaceDefinition = $compiler->getClassDefinition($interface);
                    } elseif ($compiler->isBundledInterface($interface)) {
                        $classInterfaceDefinition = $compiler->getInternalClassDefinition($interface);
                    }

                    if ($classInterfaceDefinition !== null) {
                        $this->checkInterfaceImplements($this, $classInterfaceDefinition);
                    }
                }
            }
        }

        $codePrinter->output('return SUCCESS;');
        $codePrinter->decreaseLevel();

        $codePrinter->output('}');
        $codePrinter->outputBlankLine();

        /**
         * Compile methods
         */
        foreach ($methods as $method) {
            $docBlock = $method->getDocBlock();
            if ($docBlock) {
                $codePrinter->outputDocBlock($docBlock);
            }

            if (self::TYPE_CLASS === $this->getType()) {
                if (!$method->isInternal()) {
                    $codePrinter->output('PHP_METHOD('.$this->getCNamespace().'_'.$this->getName().', '.$method->getName().')');
                } else {
                    $codePrinter->output($compilationContext->backend->getInternalSignature($method, $compilationContext));
                }
                $codePrinter->output('{');

                if (!$method->isAbstract()) {
                    $method->compile($compilationContext);
                }

                $codePrinter->output('}');
                $codePrinter->outputBlankLine();
            } else {
                $codePrinter->output('ZEPHIR_DOC_METHOD('.$this->getCNamespace().'_'.$this->getName().', '.$method->getName().');');
            }
        }

        /**
         * Check whether classes must be exported.
         */
        $exportClasses = $compilationContext->config->get('export-classes', 'extra');
        $exportAPI = $exportClasses ? 'extern ZEPHIR_API' : 'extern';

        /**
         * Create a code printer for the header file.
         */
        $codePrinter = new Printer();

        $codePrinter->outputBlankLine();
        $codePrinter->output($exportAPI.' zend_class_entry *'.$this->getClassEntry().';');
        $codePrinter->outputBlankLine();

        $codePrinter->output('ZEPHIR_INIT_CLASS('.$this->getCNamespace().'_'.$this->getName().');');
        $codePrinter->outputBlankLine();

        if (self::TYPE_CLASS === $this->getType() && \count($methods) > 0) {
            foreach ($methods as $method) {
                if (!$method->isInternal()) {
                    $codePrinter->output('PHP_METHOD('.$this->getCNamespace().'_'.$this->getName().', '.$method->getName().');');
                } else {
                    $internalSignature = $compilationContext->backend->getInternalSignature($method, $compilationContext);
                    $codePrinter->output($internalSignature.';');
                }
            }
            $codePrinter->outputBlankLine();
        }

        /**
         * Specifying Argument Information
         */
        foreach ($methods as $method) {
            $argInfo = new ArgInfoDefinition(
                $method->getArgInfoName($this),
                $method,
                $codePrinter,
                $compilationContext
            );

            $argInfo->setBooleanDefinition('_IS_BOOL');
            $argInfo->setRichFormat(true);

            $argInfo->render();
        }

        if (\count($methods) > 0) {
            $codePrinter->output(
                sprintf(
                    'ZEPHIR_INIT_FUNCS(%s_%s_method_entry) {',
                    strtolower($this->getCNamespace()),
                    strtolower($this->getName())
                )
            );

            foreach ($methods as $method) {
                if (self::TYPE_CLASS === $this->getType()) {
                    if (!$method->isInternal()) {
                        $richFormat = $method->isReturnTypesHintDetermined() && $method->areReturnTypesCompatible();

                        if ($richFormat || $method->hasParameters()) {
                            $codePrinter->output(
                                sprintf(
                                    // TODO: Rename to ZEND_ME
                                    "\tPHP_ME(%s_%s, %s, %s, %s)",
                                    $this->getCNamespace(),
                                    $this->getName(),
                                    $method->getName(),
                                    $method->getArgInfoName($this),
                                    $method->getModifiers()
                                )
                            );
                        } else {
                            $codePrinter->output(
                                sprintf(
                                    // TODO: Rename to ZEND_ME
                                    "PHP_ME(%s_%s, %s, %s, %s)",
                                    $this->getCNamespace(),
                                    $this->getName(),
                                    $method->getName(),
                                    $method->getArgInfoName($this),
                                    $method->getModifiers()
                                )
                            );
                        }
                    }
                } else {
                    $richFormat = $method->isReturnTypesHintDetermined() && $method->areReturnTypesCompatible();

                    if ($method->isStatic()) {
                        if ($richFormat || $method->hasParameters()) {
                            $codePrinter->output(
                                sprintf(
                                    "\tZEND_FENTRY(%s, NULL, %s, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)",
                                    $method->getName(),
                                    $method->getArgInfoName($this)
                                )
                            );
                        } else {
                            $codePrinter->output(
                                sprintf(
                                    "ZEND_FENTRY(%s, NULL, %s, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)",
                                    $method->getName(),
                                    $method->getArgInfoName($this)
                                )
                            );
                        }
                    } else {
                        $isInterface = $method->getClassDefinition()->isInterface();
                        $codePrinter->output(
                            sprintf(
                                "\tPHP_ABSTRACT_ME(%s_%s, %s, %s)",
                                $this->getCNamespace(),
                                $this->getName(),
                                $method->getName(),
                                $isInterface ? $method->getArgInfoName($this) : 'NULL'
                            )
                        );
                    }
                }
            }

            $codePrinter->output("\t".'PHP_FE_END');
            $codePrinter->output('};'); // ZEPHIR_INIT_FUNCS
        }

        $compilationContext->headerPrinter = $codePrinter;
    }

    public function getAliasManager(): ?AliasManager
    {
        return $this->aliasManager;
    }

    public function setAliasManager(AliasManager $aliasManager): void
    {
        $this->aliasManager = $aliasManager;
    }

    /**
     * Builds a class definition from reflection.
     */
    public static function buildFromReflection(\ReflectionClass $class): self
    {
        $classDefinition = new self($class->getNamespaceName(), $class->getName(), $class->getShortName());

        foreach ($class->getMethods() as $method) {
            $parameters = [];

            foreach ($method->getParameters() as $row) {
                $params = [
                    'type' => 'parameter',
                    'name' => $row->getName(),
                    'const' => 0,
                    'data-type' => 'variable',
                    'mandatory' => !$row->isOptional(),
                ];

                if (!$params['mandatory']) {
                    try {
                        $params['default'] = $row->getDefaultValue();
                    } catch (\ReflectionException $e) {
                        // TODO: dummy default value
                        $params['default'] = true;
                    }
                }

                $parameters[] = $params;
            }

            $classMethod = new Method(
                $classDefinition,
                [],
                $method->getName(),
                new Parameters($parameters)
            );
            $classMethod->setIsStatic($method->isStatic());
            $classMethod->setIsBundled(true);
            $classDefinition->addMethod($classMethod);
        }

        foreach ($class->getConstants() as $constantName => $constantValue) {
            $type = self::convertPhpConstantType(\gettype($constantValue));
            $classConstant = new Constant($constantName, ['value' => $constantValue, 'type' => $type], null);
            $classDefinition->addConstant($classConstant);
        }

        foreach ($class->getProperties() as $property) {
            $visibility = [];

            if ($property->isPublic()) {
                $visibility[] = 'public';
            }

            if ($property->isPrivate()) {
                $visibility[] = 'private';
            }

            if ($property->isProtected()) {
                $visibility[] = 'protected';
            }

            if ($property->isStatic()) {
                $visibility[] = 'static';
            }

            $classProperty = new Property(
                $classDefinition,
                $visibility,
                $property->getName(),
                null,
                null,
                null
            );
            $classDefinition->addProperty($classProperty);
        }

        $classDefinition->setIsBundled(true);

        return $classDefinition;
    }

    protected function hasConstantFromInterfaces(string $name): bool
    {
        if ($interfaces = $this->getImplementedInterfaceDefinitions()) {
            foreach ($interfaces as $interface) {
                if ($interface instanceof self && $interface->hasConstant($name)) {
                    return true;
                }
            }
        }

        return false;
    }

    protected function getConstantFromInterfaces(string $name): ?Constant
    {
        if ($interfaces = $this->getImplementedInterfaceDefinitions()) {
            foreach ($interfaces as $interface) {
                if ($interface instanceof self && $interface->hasConstant($name)) {
                    return $interface->getConstant($name);
                }
            }
        }

        return null;
    }

    private static function convertPhpConstantType(string $phpType): string
    {
        $map = [
            'boolean' => 'bool',
            'integer' => 'int',
            'double' => 'double',
            'string' => 'string',
            'NULL' => 'null',
        ];

        if (!isset($map[$phpType])) {
            throw new CompilerException("Cannot parse constant type '$phpType'");
        }

        return $map[$phpType];
    }
}
