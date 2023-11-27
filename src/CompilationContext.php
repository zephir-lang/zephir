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
use Zephir\Cache\Manager;
use Zephir\Class\Definition\AbstractDefinition;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Code\Printer;
use Zephir\Exception\CompilerException;
use Zephir\Passes\StaticTypeInference;

use function in_array;

/**
 * This class encapsulates important entities required during compilation
 */
class CompilationContext
{
    /**
     * Manages class renaming using keyword 'use'.
     */
    public ?AliasManager $aliasManager = null;
    /**
     * The current backend.
     */
    public ?Backend $backend = null;
    /**
     * Helps to create graphs of conditional/jump branches in a specific method.
     */
    public ?BranchManager $branchManager = null;
    /**
     * Manages both function and method call caches.
     */
    public ?Manager $cacheManager = null;
    /**
     * Represents the class currently being compiled.
     */
    public ?Definition $classDefinition = null;
    /**
     * Current code printer.
     */
    public ?Printer  $codePrinter = null;
    public ?Compiler $compiler    = null;
    /**
     * Global config.
     */
    public ?Config $config = null;
    /**
     * The current branch, variables declared in conditional branches
     * must be market if they're used out of those branches.
     */
    public int $currentBranch = 0;
    /**
     * Current method or function that being compiled.
     */
    public ?Method $currentMethod = null;
    /**
     * Global consecutive for try/catch blocks.
     */
    public int $currentTryCatch = 0;
    /**
     * Current cycle/loop block.
     */
    public array $cycleBlocks = [];
    /**
     * Function Cache.
     */
    public ?FunctionCache $functionCache = null;
    /**
     * Code printer for the header.
     */
    public ?Printer $headerPrinter = null;
    /**
     * Represents the c-headers added to the file.
     */
    public ?HeadersManager $headersManager = null;
    /**
     * Tells if the compilation is being made inside a cycle/loop.
     */
    public int $insideCycle = 0;
    /**
     * Tells if the compilation is being made inside a switch.
     */
    public int $insideSwitch = 0;
    /**
     * Tells if the compilation is being made inside a try/catch block.
     */
    public int $insideTryCatch = 0;
    /**
     * Global logger.
     */
    public ?LoggerInterface $logger = null;
    /**
     * Whether the current method is static or not.
     */
    public bool $staticContext = false;
    /**
     * Represents interned strings and concatenations made in the project.
     */
    public ?StringsManager $stringsManager = null;
    /**
     * Current symbol table.
     */
    public ?SymbolTable $symbolTable = null;
    /**
     * Type inference data.
     */
    public ?StaticTypeInference $typeInference = null;

    /**
     * Lookup a class from a given class name.
     */
    public function classLookup(string $className, array $statement = null): AbstractDefinition
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
        if (!$parent instanceof Definition) {
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

    /**
     * Transform class/interface name to FQN format.
     */
    public function getFullName(string $className): string
    {
        $isFunction = $this->currentMethod instanceof FunctionDefinition;
        $namespace  = $isFunction ? $this->currentMethod->getNamespace() : $this->classDefinition->getNamespace();

        return Name::fetchFQN($className, $namespace, $this->aliasManager);
    }
}
