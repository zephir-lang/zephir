<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Psr\Log\LoggerInterface;
use Zephir\Exception\CompilerException;

/**
 * CompilationContext.
 *
 * This class encapsulates important entities required during compilation
 */
class CompilationContext
{
    /**
     * @var EventsManager
     */
    public $eventsManager;

    /**
     * Compiler.
     *
     * @var Compiler
     */
    public $compiler;

    /**
     * Current code printer.
     *
     * @var CodePrinter
     */
    public $codePrinter;

    /**
     * Whether the current method is static or not.
     *
     * @var bool
     */
    public $staticContext;

    /**
     * Code printer for the header.
     *
     * @var CodePrinter
     */
    public $headerPrinter;

    /**
     * Current symbol table.
     *
     * @var SymbolTable
     */
    public $symbolTable;

    /**
     * Type inference data.
     *
     * @var \Zephir\Passes\StaticTypeInference
     */
    public $typeInference;

    /**
     * Represents the class currently being compiled.
     *
     * @var ClassDefinition
     */
    public $classDefinition;

    /**
     * Current method or function that being compiled.
     *
     * @var ClassMethod|FunctionDefinition
     */
    public $currentMethod;

    /**
     * Methods warm-up.
     *
     * @var MethodCallWarmUp
     */
    public $methodWarmUp;

    /**
     * Represents the c-headers added to the file.
     *
     * @var HeadersManager
     */
    public $headersManager;

    /**
     * Represents interned strings and concatenations made in the project.
     *
     * @var StringsManager
     */
    public $stringsManager;

    /**
     * Tells if the the compilation is being made inside a cycle/loop.
     *
     * @var int
     */
    public $insideCycle = 0;

    /**
     * Tells if the the compilation is being made inside a try/catch block.
     *
     * @var int
     */
    public $insideTryCatch = 0;

    /**
     * Tells if the the compilation is being made inside a switch.
     *
     * @var int
     */
    public $insideSwitch = 0;

    /**
     * Current cycle/loop block.
     *
     * @var array
     */
    public $cycleBlocks = [];

    /**
     * The current branch, variables declared in conditional branches
     * must be market if they're used out of those branches.
     */
    public $currentBranch = 0;

    /**
     * Global consecutive for try/catch blocks.
     *
     * @var int
     */
    public $currentTryCatch = 0;

    /**
     * Helps to create graphs of conditional/jump branches in a specific method.
     *
     * @var BranchManager
     */
    public $branchManager;

    /**
     * Manages both function and method call caches.
     *
     * @var CacheManager
     */
    public $cacheManager;

    /**
     * Manages class renamings using keyword 'use'.
     *
     * @var AliasManager
     */
    public $aliasManager;

    /**
     * Function Cache.
     *
     * @var Cache\FunctionCache
     */
    public $functionCache;

    /**
     * Global config.
     *
     * @var Config
     */
    public $config;

    /**
     * Global logger.
     *
     * @var LoggerInterface
     */
    public $logger;

    /**
     * The current backend.
     *
     * @var BaseBackend
     */
    public $backend;

    /**
     * Transform class/interface name to FQN format.
     *
     * @todo WHY WHY :'(
     *
     * @param string $className
     *
     * @return string
     */
    public function getFullName(string $className): string
    {
        $isFunction = $this->currentMethod && $this->currentMethod instanceof FunctionDefinition;
        $namespace = $isFunction ? $this->currentMethod->getNamespace() : $this->classDefinition->getNamespace();

        return fqcn($className, $namespace, $this->aliasManager);
    }

    /**
     * Lookup a class from a given class name.
     *
     * @param string     $className
     * @param array|null $statement
     *
     * @return ClassDefinition
     */
    public function classLookup(string $className, array $statement = null): ClassDefinition
    {
        if (!\in_array($className, ['self', 'static', 'parent'])) {
            $className = $this->getFullName($className);
            if ($this->compiler->isClass($className)) {
                return $this->compiler->getClassDefinition($className);
            }

            throw new CompilerException("Cannot locate class '{$className}'", $statement);
        }

        if (\in_array($className, ['self', 'static'])) {
            return $this->classDefinition;
        }

        $parent = $this->classDefinition->getExtendsClass();
        if (!$parent instanceof ClassDefinition) {
            throw new CompilerException(
                sprintf(
                    'Cannot access parent:: because class %s does not extend any class',
                    $this->classDefinition->getCompleteName()
                ),
                $statement
            );
        }

        return $this->classDefinition->getExtendsClassDefinition();
    }
}
