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

use Psr\Log\LoggerInterface;
use Zephir\Backend\Backend;
use Zephir\Cache\FunctionCache;
use Zephir\Exception\CompilerException;
use Zephir\Passes\StaticTypeInference;

use function in_array;

/**
 * This class encapsulates important entities required during compilation
 */
class CompilationContext
{
    public ?Compiler $compiler = null;

    /**
     * Current code printer.
     */
    public ?CodePrinter $codePrinter = null;

    /**
     * Whether the current method is static or not.
     */
    public bool $staticContext = false;

    /**
     * Code printer for the header.
     */
    public ?CodePrinter $headerPrinter = null;

    /**
     * Current symbol table.
     */
    public ?SymbolTable $symbolTable = null;

    /**
     * Type inference data.
     */
    public ?StaticTypeInference $typeInference = null;

    /**
     * Represents the class currently being compiled.
     */
    public ?ClassDefinition $classDefinition = null;

    /**
     * Current method or function that being compiled.
     */
    public ?ClassMethod $currentMethod = null;

    /**
     * Represents the c-headers added to the file.
     */
    public ?HeadersManager $headersManager = null;

    /**
     * Represents interned strings and concatenations made in the project.
     */
    public ?StringsManager $stringsManager = null;

    /**
     * Tells if the compilation is being made inside a cycle/loop.
     */
    public int $insideCycle = 0;

    /**
     * Tells if the compilation is being made inside a try/catch block.
     */
    public int $insideTryCatch = 0;

    /**
     * Tells if the compilation is being made inside a switch.
     */
    public int $insideSwitch = 0;

    /**
     * Current cycle/loop block.
     */
    public array $cycleBlocks = [];

    /**
     * The current branch, variables declared in conditional branches
     * must be market if they're used out of those branches.
     */
    public int $currentBranch = 0;

    /**
     * Global consecutive for try/catch blocks.
     */
    public int $currentTryCatch = 0;

    /**
     * Helps to create graphs of conditional/jump branches in a specific method.
     */
    public ?BranchManager $branchManager = null;

    /**
     * Manages both function and method call caches.
     */
    public ?CacheManager $cacheManager = null;

    /**
     * Manages class renaming using keyword 'use'.
     */
    public ?AliasManager $aliasManager = null;

    /**
     * Function Cache.
     */
    public ?FunctionCache $functionCache = null;

    /**
     * Global config.
     */
    public ?Config $config = null;

    /**
     * Global logger.
     */
    public ?LoggerInterface $logger = null;

    /**
     * The current backend.
     */
    public ?Backend $backend = null;

    /**
     * Transform class/interface name to FQN format.
     *
     * @param string $className
     *
     * @return string
     */
    public function getFullName(string $className): string
    {
        $isFunction = $this->currentMethod instanceof FunctionDefinition;
        $namespace = $isFunction ? $this->currentMethod->getNamespace() : $this->classDefinition->getNamespace();

        return fqcn($className, $namespace, $this->aliasManager);
    }

    /**
     * Lookup a class from a given class name.
     */
    public function classLookup(string $className, array $statement = null): AbstractClassDefinition
    {
        if (!in_array($className, ['self', 'static', 'parent'])) {
            $className = $this->getFullName($className);
            if ($this->compiler->isClass($className)) {
                return $this->compiler->getClassDefinition($className);
            }

            throw new CompilerException("Cannot locate class '$className'", $statement);
        }

        if (in_array($className, ['self', 'static'])) {
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
