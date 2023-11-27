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

namespace Zephir\Variable;

use ReflectionClass;
use Zephir\Branch;
use Zephir\BranchManager;
use Zephir\Class\Definition\Definition;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\TypeAwareInterface;

use function in_array;
use function is_string;

/**
 * This represents a variable in a symbol table
 */
class Variable implements TypeAwareInterface
{
    public const BRANCH_MAGIC     = '$$';
    public const VAR_RETURN_VALUE = 'return_value';
    public const VAR_THIS_POINTER = 'this_ptr';
    protected Definition|ReflectionClass|null $associatedClass  = null;
    protected array                           $classTypes       = [];
    protected mixed                           $defaultInitValue = null;
    protected bool                            $doublePointer    = false;
    /**
     * Current dynamic type of the variable.
     *
     * @var array
     */
    protected array $dynamicTypes = ['unknown' => true];
    /**
     * @var Globals
     */
    protected Globals $globalsManager;
    /**
     * Temporal variables are marked as idle.
     */
    protected bool $idle = false;
    /**
     * Branch where the variable was initialized for the first time.
     */
    protected bool  $initBranch   = false;
    protected array $initBranches = [];
    /**
     * Whether the variable has received any assignment.
     */
    protected bool $initialized = false;
    protected bool $isExternal  = false;
    protected bool $localOnly   = false;
    /**
     * Compiled variable's name.
     */
    protected string $lowName       = '';
    protected bool   $memoryTracked = true;
    protected bool   $mustInitNull  = false;
    /**
     * AST node where the variable was originally declared or created.
     */
    protected ?array $node = null;
    /**
     * Number of mutations to the variable.
     */
    protected int $numberMutates = 0;
    /**
     * Initialization skips.
     */
    protected int $numberSkips = 0;
    /**
     * Number of times the variable has been read.
     */
    protected int $numberUses = 0;
    /**
     * Possible constant value assigned to the variable.
     */
    protected mixed $possibleValue = null;
    /**
     * Branch where the variable got its last possible value.
     */
    protected mixed $possibleValueBranch = null;
    protected bool  $readOnly            = false;
    /**
     * Reusable temporary variables?
     */
    protected bool $reusable = true;
    /**
     * Whether the variable is temporal or not.
     */
    protected bool $temporal = false;
    /**
     * Whether the variable was used or not.
     */
    protected bool $used = true;

    /**
     * Last AST node where the variable was used.
     */
    protected ?array $usedNode     = null;
    protected int    $variantInits = 0;

    public function __construct(protected string $type, protected string $name, protected ?Branch $branch = null)
    {
        $this->globalsManager = new Globals();
        $this->type           = in_array($type, ['callable', 'object', 'resource'], true) ? 'variable' : $type;
    }

    /**
     * Sets an automatic safe default init value according to its type.
     */
    public function enableDefaultAutoInitValue(): void
    {
        switch ($this->type) {
            case 'char':
            case 'boolean':
            case 'bool':
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'double':
            case 'zephir_ce_guard':
                $this->defaultInitValue = 0;
                break;

            case 'variable':
            case 'string':
            case 'array':
                $this->defaultInitValue = null;
                $this->setDynamicTypes('null');
                $this->setMustInitNull(true);
                $this->setLocalOnly(false);
                break;

            default:
                throw new CompilerException(
                    'Cannot create an automatic safe default value for variable type: ' . $this->type
                );
        }
    }

    /**
     * Returns the class related to the variable.
     */
    public function getAssociatedClass(): Definition|ReflectionClass|null
    {
        return $this->associatedClass;
    }

    /**
     * Get the branch where the variable was declared.
     *
     * @return Branch|null
     */
    public function getBranch(): ?Branch
    {
        return $this->branch;
    }

    /**
     * Returns the PHP classes associated to the variable.
     *
     * @return array
     */
    public function getClassTypes(): array
    {
        return $this->classTypes;
    }

    /**
     * Returns the default init value.
     *
     * @return mixed
     */
    public function getDefaultInitValue(): mixed
    {
        return $this->defaultInitValue;
    }

    /**
     * Returns the current dynamic type in a polymorphic variable.
     *
     * @return array
     */
    public function getDynamicTypes(): array
    {
        return $this->dynamicTypes;
    }

    /**
     * Get init branch.
     *
     * @return bool
     */
    public function getInitBranch(): bool
    {
        return $this->initBranch;
    }

    /**
     * Get init marked branch.
     *
     * @return Branch[]
     */
    public function getInitBranches(): array
    {
        return $this->initBranches;
    }

    /**
     * Returns the last node where the variable was assigned or used.
     */
    public function getLastUsedNode(): ?array
    {
        return $this->usedNode;
    }

    /**
     * Returns variable's name.
     *
     * @return string
     */
    public function getName(): string
    {
        return $this->lowName ?: $this->name;
    }

    /**
     * Returns the number of mutations performed over the variable.
     *
     * @return int
     */
    public function getNumberMutations(): int
    {
        return $this->numberMutates;
    }

    /**
     * Return the number of uses.
     *
     * @return int
     */
    public function getNumberUses(): int
    {
        return $this->numberUses;
    }

    /**
     * Returns the original AST node where the variable was declared.
     *
     * @return array
     */
    public function getOriginal(): array
    {
        if ($this->node) {
            return $this->node;
        }

        return ['file' => 'unknown', 'line' => 0, 'char' => 0];
    }

    /**
     * Returns the latest CompiledExpression assigned to a variable.
     *
     * @return mixed
     */
    public function getPossibleValue(): mixed
    {
        return $this->possibleValue;
    }

    /**
     * Returns the branch where the variable was assigned for the last time.
     */
    public function getPossibleValueBranch(): ?Branch
    {
        return $this->possibleValueBranch;
    }

    /**
     * Returns variable's real name.
     *
     * @return string
     */
    public function getRealName(): string
    {
        return $this->name;
    }

    /**
     * Get the number of initializations remaining to skip.
     *
     * @return int
     */
    public function getSkipVariant(): int
    {
        return $this->numberSkips;
    }

    /**
     * Returns the type of variable.
     *
     * @return string
     */
    public function getType(): string
    {
        return $this->type;
    }

    /**
     * Get the number of times the variable has been initialized.
     *
     * @return int
     */
    public function getVariantInits(): int
    {
        return $this->variantInits;
    }

    /**
     * Checks if the variable has any of the passed dynamic.
     *
     * @param array|string $types
     *
     * @return bool
     */
    public function hasAnyDynamicType(array|string $types): bool
    {
        if (is_string($types)) {
            $types = [$types];
        }

        foreach ($types as $type) {
            if (isset($this->dynamicTypes[$type])) {
                return true;
            }
        }

        return false;
    }

    /**
     * Check if the variable has at least one dynamic type to the ones passed in the list.
     *
     * @param array $types
     *
     * @return bool
     */
    public function hasDifferentDynamicType(array $types): bool
    {
        $number = 0;
        foreach ($types as $type) {
            if (isset($this->dynamicTypes[$type])) {
                ++$number;
            }
        }

        return 0 === $number;
    }

    /**
     * Increase the number of mutations a variable may have.
     */
    public function increaseMutates(): void
    {
        ++$this->numberMutates;
    }

    /**
     * Increase the number of uses a variable may have.
     */
    public function increaseUses(): void
    {
        ++$this->numberUses;
    }

    /**
     * Increase the number of times the variable has been initialized.
     */
    public function increaseVariantIfNull(): void
    {
        ++$this->variantInits;
    }

    /**
     * Initializes a variant variable that is intended to have the special
     * behavior of only freed its body value instead of the full variable.
     *
     * @param CompilationContext $compilationContext
     */
    public function initComplexLiteralVariant(CompilationContext $compilationContext): void
    {
        if ($this->numberSkips) {
            --$this->numberSkips;

            return;
        }

        if (self::VAR_THIS_POINTER != $this->getName() && self::VAR_RETURN_VALUE != $this->getName()) {
            if (!$this->initBranch) {
                $this->initBranch = $compilationContext->currentBranch === 0;
            }

            $compilationContext->headersManager->add('kernel/memory');
            $compilationContext->symbolTable->mustGrownStack(true);
            if (!$this->isLocalOnly()) {
                if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(&' . $this->getName() . ');');
                } else {
                    $compilationContext->backend->initVar($this, $compilationContext);
                }
            } else {
                if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(&' . $this->getName() . ');');
                }
            }

            ++$this->variantInits;
        }
    }

    /**
     * Allocate memory for variable and init it null val
     *
     * @param CompilationContext $compilationContext
     */
    public function initNonReferenced(CompilationContext $compilationContext): void
    {
        $compilationContext->codePrinter->output('ZVAL_UNDEF(&' . $this->getName() . ');');
    }

    /**
     * Initializes a variant variable.
     *
     * @param CompilationContext $compilationContext
     */
    public function initVariant(CompilationContext $compilationContext): void
    {
        if ($this->numberSkips) {
            --$this->numberSkips;

            return;
        }

        /**
         * Variables are allocated for the first time using ZVAL_NULL
         * the second, third, etc. times are allocated using ZEPHIR_INIT_NVAR
         * Variables initialized for the first time in a cycle are always initialized using ZEPHIR_INIT_NVAR
         */
        if (self::VAR_THIS_POINTER !== $this->getName() && self::VAR_RETURN_VALUE !== $this->getName()) {
            if (!$this->initBranch) {
                $this->initBranch = $compilationContext->currentBranch === 0;
            }

            $compilationContext->headersManager->add('kernel/memory');
            $compilationContext->symbolTable->mustGrownStack(true);

            if (!$this->isLocalOnly()) {
                if ($compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->backend->initVar($this, $compilationContext, true, true);
                } else {
                    if ($this->variantInits > 0) {
                        if ($this->initBranch) {
                            $compilationContext->codePrinter->output('ZEPHIR_INIT_BNVAR(' . $this->getName() . ');');
                        } else {
                            $this->mustInitNull = true;
                            $compilationContext->backend->initVar($this, $compilationContext, true, true);
                        }
                    } else {
                        $compilationContext->backend->initVar($this, $compilationContext);
                    }
                }
            } else {
                if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(&' . $this->getName() . ');');
                }
            }

            ++$this->variantInits;
            $this->associatedClass = null;
        }
    }

    /**
     * Shortcut is type double?
     *
     * @return bool
     */
    public function isArray(): bool
    {
        return 'array' === $this->type;
    }

    /**
     * Shortcut is type bool?
     *
     * @return bool
     */
    public function isBoolean(): bool
    {
        return 'bool' === $this->type;
    }

    /**
     * Shortcut is type double?
     *
     * @return bool
     */
    public function isDouble(): bool
    {
        return 'double' === $this->type;
    }

    /**
     * Returns the variable.
     */
    public function isDoublePointer(): bool
    {
        return $this->doublePointer;
    }

    /**
     * Check if the variable is a parameter.
     *
     * @return bool
     */
    public function isExternal(): bool
    {
        return $this->isExternal;
    }

    /**
     * Checks if the variable is idle.
     *
     * @return bool
     */
    public function isIdle(): bool
    {
        return $this->idle;
    }

    /**
     * Check if the variable is initialized or not.
     *
     * @return bool
     */
    public function isInitialized(): bool
    {
        return $this->initialized;
    }

    /**
     * Shortcut is type int?
     *
     * @return bool
     */
    public function isInt(): bool
    {
        return 'int' === $this->type;
    }

    /**
     * Checks if the variable is local-only scoped.
     *
     * @return bool
     */
    public function isLocalOnly(): bool
    {
        return $this->localOnly;
    }

    /**
     * Checks if a variable is a local static.
     *
     * @return bool
     */
    public function isLocalStatic(): bool
    {
        return $this->isExternal && $this->localOnly;
    }

    /**
     * Checks if the variable is tracked by the memory manager.
     *
     * @return bool
     */
    public function isMemoryTracked(): bool
    {
        return $this->memoryTracked;
    }

    /**
     * Shortcut is type mixed?
     *
     * @return bool
     */
    public function isMixed(): bool
    {
        return 'mixed' === $this->type;
    }

    /**
     * Shortcut is type variable or string?
     *
     * @return bool
     */
    public function isNotVariable(): bool
    {
        return !$this->isVariable();
    }

    /**
     * Shortcut is type variable or array?
     *
     * @return bool
     */
    public function isNotVariableAndArray(): bool
    {
        return !$this->isVariable() && !$this->isArray();
    }

    /**
     * Shortcut is type variable or mixed or string?
     *
     * @return bool
     */
    public function isNotVariableAndMixedAndString(): bool
    {
        return !$this->isVariable() && !$this->isMixed() && !$this->isString();
    }

    /**
     * Shortcut is type variable or string?
     *
     * @return bool
     */
    public function isNotVariableAndString(): bool
    {
        return !$this->isVariable() && !$this->isString();
    }

    /**
     * Returns if the variable is read only.
     *
     * @return bool
     */
    public function isReadOnly(): bool
    {
        return $this->readOnly;
    }

    /**
     * Checks if the temporary variable is reusable.
     *
     * @return bool
     */
    public function isReusable(): bool
    {
        return $this->reusable;
    }

    /**
     * Shortcut is type string?
     *
     * @return bool
     */
    public function isString(): bool
    {
        return 'string' === $this->type;
    }

    /**
     * Checks if a variable is a super global.
     *
     * @return bool
     */
    public function isSuperGlobal(): bool
    {
        return $this->isExternal && $this->globalsManager->isSuperGlobal($this->name);
    }

    /**
     * Returns whether the variable is temporal or not.
     *
     * @return bool
     */
    public function isTemporal(): bool
    {
        return $this->temporal;
    }

    /**
     * Checks whether the last value assigned was used.
     *
     * @return bool
     */
    public function isUsed(): bool
    {
        return $this->used;
    }

    /**
     * Shortcut is type variable?
     *
     * @return bool
     */
    public function isVariable(): bool
    {
        return 'variable' === $this->type;
    }

    /**
     * Get if the variable must be initialized to null.
     *
     * @return bool
     */
    public function mustInitNull(): bool
    {
        return $this->mustInitNull;
    }

    /**
     * Observes a variable in the memory frame without initialization or nullify
     * an existing allocated variable.
     *
     * @param CompilationContext $compilationContext
     */
    public function observeOrNullifyVariant(CompilationContext $compilationContext): void
    {
        if ($this->numberSkips) {
            --$this->numberSkips;

            return;
        }

        if (in_array($this->getName(), [self::VAR_THIS_POINTER, self::VAR_RETURN_VALUE], true)) {
            return;
        }

        if (!$this->initBranch) {
            $this->initBranch = $compilationContext->currentBranch === 0;
        }

        $compilationContext->headersManager->add('kernel/memory');
        $compilationContext->symbolTable->mustGrownStack(true);
        if ($this->variantInits > 0 || $compilationContext->insideCycle) {
            $this->mustInitNull = true;
        }

        ++$this->variantInits;
        $this->setMustInitNull(true);
    }

    /**
     * Observes a variable in the memory frame without initialization.
     *
     * @param CompilationContext $compilationContext
     */
    public function observeVariant(CompilationContext $compilationContext): void
    {
        if ($this->numberSkips) {
            --$this->numberSkips;

            return;
        }

        $name = $this->getName();
        if (self::VAR_THIS_POINTER != $name && self::VAR_RETURN_VALUE != $name) {
            if (!$this->initBranch) {
                $this->initBranch = $compilationContext->currentBranch === 0;
            }

            $compilationContext->headersManager->add('kernel/memory');
            $compilationContext->symbolTable->mustGrownStack(true);
            $symbol = $compilationContext->backend->getVariableCode($this);

            if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                $this->mustInitNull = true;
                $compilationContext->codePrinter->output('ZEPHIR_OBS_NVAR(' . $symbol . ');');
            }

            ++$this->variantInits;
        }
    }

    /**
     * Separates variables before being updated.
     *
     * @param CompilationContext $compilationContext
     */
    public function separate(CompilationContext $compilationContext): void
    {
        if (!in_array($this->getName(), [self::VAR_THIS_POINTER, self::VAR_RETURN_VALUE], true)) {
            $compilationContext->codePrinter->output(
                'SEPARATE_ZVAL(' . $compilationContext->backend->getVariableCode($this) . ');'
            );
        }
    }

    /**
     * Sets the PHP class related to variable.
     *
     * @param ReflectionClass|Definition $associatedClass
     */
    public function setAssociatedClass(ReflectionClass|Definition $associatedClass): void
    {
        $this->associatedClass = $associatedClass;
    }

    /**
     * Sets the PHP class related to variable.
     *
     * @param array|string $classTypes
     */
    public function setClassTypes(array|string $classTypes): void
    {
        if (is_string($classTypes)) {
            if (!in_array($classTypes, $this->classTypes)) {
                $this->classTypes[] = $classTypes;
            }

            return;
        }

        foreach ($classTypes as $classType) {
            if (!in_array($classType, $this->classTypes)) {
                $this->classTypes[] = $classType;
            }
        }
    }

    /**
     * Sets the default init value.
     *
     * @param mixed $value
     */
    public function setDefaultInitValue(mixed $value): void
    {
        $this->defaultInitValue = $value;
    }

    /**
     * Sets the current dynamic type in a polymorphic variable.
     *
     * @param array|string $types
     */
    public function setDynamicTypes(array|string $types): void
    {
        unset($this->dynamicTypes['unknown']);

        if (is_string($types)) {
            $types = [$types];
        }

        foreach ($types as $type) {
            if (!isset($this->dynamicTypes[$type])) {
                $this->dynamicTypes[$type] = true;
            }
        }
    }

    /**
     * Once a temporal variable is unused in a specific branch it is marked as idle.
     *
     * @param bool $idle
     */
    public function setIdle(bool $idle): void
    {
        $this->idle = false;

        if ($this->reusable) {
            $this->classTypes   = [];
            $this->dynamicTypes = ['unknown' => true];
            $this->idle         = $idle;
        }
    }

    /**
     * Marks the variable to be defined as a double pointer.
     *
     * @param bool $doublePointer
     */
    public function setIsDoublePointer(bool $doublePointer): void
    {
        $this->doublePointer = $doublePointer;
    }

    /**
     * Set if the symbol is a parameter of the method or not.
     *
     * @param bool $isExternal
     */
    public function setIsExternal(bool $isExternal): void
    {
        $this->isExternal   = $isExternal;
        $this->variantInits = 1;
    }

    /**
     * Sets if the variable is initialized
     * This allow to throw an exception if the variable is being read without prior initialization.
     *
     * @param bool               $initialized
     * @param CompilationContext $compilationContext
     */
    public function setIsInitialized(bool $initialized, CompilationContext $compilationContext): void
    {
        $this->initialized = $initialized;

        if (!$initialized || !$compilationContext->branchManager instanceof BranchManager) {
            return;
        }

        $currentBranch = $compilationContext->branchManager->getCurrentBranch();

        if ($currentBranch instanceof Branch) {
            $this->initBranches[] = $currentBranch;
        }
    }

    /**
     * Sets if the variable is local-only scoped.
     *
     * @param bool $localOnly
     */
    public function setLocalOnly(bool $localOnly): void
    {
        $this->localOnly = $localOnly;
    }

    /**
     * Sets the compiled variable's name.
     *
     * @param string $lowName
     */
    public function setLowName(string $lowName): void
    {
        $this->lowName = $lowName;
    }

    /**
     * Sets if the variable is not tracked by the memory manager.
     *
     * @param bool $memoryTracked
     */
    public function setMemoryTracked(bool $memoryTracked): void
    {
        $this->memoryTracked = $memoryTracked;
    }

    /**
     * Set if the variable must be initialized to null.
     *
     * @param bool $mustInitNull
     */
    public function setMustInitNull(bool $mustInitNull): void
    {
        $this->mustInitNull = $mustInitNull;
    }

    /**
     * Set the original AST node where the variable was declared.
     *
     * @param array $node
     */
    public function setOriginal(array $node): void
    {
        $this->node = $node;
    }

    /**
     * Sets the latest CompiledExpression assigned to a variable.
     *
     * @param CompiledExpression $possibleValue
     * @param CompilationContext $compilationContext
     */
    public function setPossibleValue(CompiledExpression $possibleValue, CompilationContext $compilationContext): void
    {
        $this->possibleValue       = $possibleValue;
        $this->possibleValueBranch = $compilationContext->branchManager->getCurrentBranch();
    }

    /**
     * Sets if the variable is read only.
     *
     * @param bool $readOnly
     */
    public function setReadOnly(bool $readOnly): void
    {
        $this->readOnly = $readOnly;
    }

    /**
     * Some temporary variables can't be reused.
     *
     * @param bool $reusable
     */
    public function setReusable(bool $reusable): void
    {
        $this->reusable = $reusable;
    }

    /**
     * Sets whether the variable is temporal or not.
     *
     * @param bool $temporal
     */
    public function setTemporal(bool $temporal): void
    {
        $this->temporal = $temporal;
    }

    /**
     * Sets the type of variable.
     *
     * @param string $type
     */
    public function setType(string $type): void
    {
        $this->type = $type;
    }

    /**
     * Sets the latest node where a variable was used.
     *
     * @param bool       $used
     * @param array|null $node
     */
    public function setUsed(bool $used, array $node = null): void
    {
        $this->used     = $used;
        $this->usedNode = $node;
    }

    /**
     * Skips variable initialization.
     *
     * @param int $numberSkips
     */
    public function skipInitVariant(int $numberSkips): void
    {
        $this->numberSkips += $numberSkips;
    }

    /**
     * Tells the compiler a generated code will track the variable.
     *
     * @param CompilationContext $compilationContext
     */
    public function trackVariant(CompilationContext $compilationContext): void
    {
        if ($this->numberSkips) {
            --$this->numberSkips;

            return;
        }

        /**
         * Variables are allocated for the first time using ZVAL_NULL
         * the second, third, etc. times are allocated using ZEPHIR_INIT_NVAR
         * Variables initialized for the first time in a cycle are always initialized using ZEPHIR_INIT_NVAR
         */
        if (self::VAR_THIS_POINTER !== $this->getName() && self::VAR_RETURN_VALUE !== $this->getName()) {
            if (!$this->initBranch) {
                $this->initBranch = $compilationContext->currentBranch === 0;
            }

            if (!$this->isLocalOnly()) {
                $compilationContext->symbolTable->mustGrownStack(true);
                if ($compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                } else {
                    if ($this->variantInits > 0) {
                        if (!$this->initBranch) {
                            $this->mustInitNull = true;
                        }
                    }
                }
            } else {
                if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                }
            }

            ++$this->variantInits;
        }
    }
}
