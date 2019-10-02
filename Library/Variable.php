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

use Zephir\Exception\CompilerException;
use Zephir\Variable\Globals;

/**
 * Variable.
 *
 * This represents a variable in a symbol table
 */
class Variable implements TypeAwareInterface
{
    const BRANCH_MAGIC = '$$';
    /**
     * Variable's type.
     */
    protected $type;

    /**
     * Current dynamic type of the variable.
     *
     * @var array
     */
    protected $dynamicTypes = ['unknown' => true];

    /**
     * Variable's name.
     */
    protected $name;

    /**
     * Branch where the variable was declared.
     *
     * @var Branch|null
     */
    protected $branch;

    /**
     * Branch where the variable was initialized for the first time.
     */
    protected $initBranch = false;

    /**
     * Compiled variable's name.
     */
    protected $lowName;

    /**
     * Number of times the variable has been read.
     */
    protected $numberUses = 0;

    /**
     * Whether the variable is temporal or not.
     */
    protected $temporal = false;

    /**
     * Temporal variables are marked as idle.
     */
    protected $idle = false;

    /**
     * Reusable temporary variables?
     */
    protected $reusable = true;

    /**
     * Number of mutations to the variable.
     */
    protected $numberMutates = 0;

    /**
     * Whether the variable has received any assignment.
     */
    protected $initialized = false;

    protected $initBranches = [];

    protected $isExternal = false;

    protected $variantInits = 0;

    protected $mustInitNull = false;

    protected $readOnly = false;

    protected $localOnly = false;

    protected $memoryTracked = true;

    protected $doublePointer = false;

    protected $defaultInitValue;

    /**
     * Class types.
     */
    protected $classTypes = [];

    /**
     * Associated class.
     */
    protected $associatedClass;

    /**
     * Initialization skips.
     */
    protected $numberSkips = 0;

    /**
     * AST node where the variable was originally declared or created.
     */
    protected $node;

    /**
     * Possible constant value assigned to the variable.
     */
    protected $possibleValue;

    /**
     * Branch where the variable got its last possible value.
     */
    protected $possibleValueBranch;

    /**
     * Whether the variable was used or not.
     */
    protected $used = true;

    /**
     * Last AST node where the variable was used.
     */
    protected $usedNode;

    /**
     * @var Globals
     */
    protected $globalsManager;

    /**
     * Complex variable type, they may need special treatment.
     *
     * @var array
     */
    protected $complexTypes = [
        'variable' => 1,
        'string' => 1,
        'array' => 1,
        'resource' => 1,
        'callable' => 1,
        'object' => 1,
    ];

    /**
     * Variable constructor.
     *
     * @param string $type
     * @param string $name
     * @param Branch $branch
     */
    public function __construct($type, $name, Branch $branch = null)
    {
        $this->globalsManager = new Globals();

        if (\in_array($type, ['callable', 'object', 'resource'], true)) {
            $type = 'variable';
        }

        $this->type = $type;
        $this->name = $name;
        $this->branch = $branch;
    }

    /**
     * Get init branch.
     *
     * @return bool|int
     */
    public function getInitBranch()
    {
        return $this->initBranch;
    }

    /**
     * Get init marked branch.
     *
     * @return Branch[]
     */
    public function getInitBranches()
    {
        return $this->initBranches;
    }

    /**
     * Sets the type of variable.
     *
     * @param string $type
     */
    public function setType($type)
    {
        $this->type = $type;
    }

    /**
     * Returns the type of variable.
     *
     * @return string
     */
    public function getType()
    {
        return $this->type;
    }

    /**
     * Sets if the variable is local-only scoped.
     *
     * @param bool $localOnly
     */
    public function setLocalOnly($localOnly)
    {
        $this->localOnly = $localOnly;
    }

    /**
     * Checks if the variable is local-only scoped.
     *
     * @return bool
     */
    public function isLocalOnly()
    {
        return $this->localOnly;
    }

    /**
     * Marks the variable to be defined as a double pointer.
     *
     * @param bool $doublePointer
     */
    public function setIsDoublePointer($doublePointer)
    {
        $this->doublePointer = $doublePointer;
    }

    /**
     * Returns the variable.
     */
    public function isDoublePointer()
    {
        return $this->doublePointer;
    }

    /**
     * Returns variable's real name.
     *
     * @return string
     */
    public function getRealName()
    {
        return $this->name;
    }

    /**
     * Returns variable's name.
     *
     * @return string
     */
    public function getName()
    {
        if ($this->lowName) {
            return $this->lowName;
        }

        return $this->name;
    }

    /**
     * Sets the compiled variable's name.
     *
     * @param string $lowName
     */
    public function setLowName($lowName)
    {
        $this->lowName = $lowName;
    }

    /**
     * Sets if the variable is read only.
     *
     * @param bool $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->readOnly = $readOnly;
    }

    /**
     * Returns if the variable is read only.
     *
     * @return bool
     */
    public function isReadOnly()
    {
        return $this->readOnly;
    }

    /**
     * Sets whether the variable is temporal or not.
     *
     * @param bool $temporal
     */
    public function setTemporal($temporal)
    {
        $this->temporal = $temporal;
    }

    /**
     * Returns whether the variable is temporal or not.
     *
     * @return bool
     */
    public function isTemporal()
    {
        return $this->temporal;
    }

    /**
     * Once a temporal variable is unused in a specific branch it is marked as idle.
     *
     * @param bool $idle
     */
    public function setIdle($idle)
    {
        $this->idle = false;

        if ($this->reusable) {
            $this->classTypes = [];
            $this->dynamicTypes = ['unknown' => true];
            $this->idle = $idle;
        }
    }

    /**
     * Checks if the variable is idle.
     *
     * @return bool
     */
    public function isIdle()
    {
        return $this->idle;
    }

    /**
     * Some temporary variables can't be reused.
     *
     * @param bool $reusable
     */
    public function setReusable($reusable)
    {
        $this->reusable = $reusable;
    }

    /**
     * Checks if the temporary variable is reusable.
     *
     * @return bool
     */
    public function isReusable()
    {
        return $this->reusable;
    }

    /**
     * Sets the latest node where a variable was used.
     *
     * @param bool  $used
     * @param array $node
     */
    public function setUsed($used, $node)
    {
        $this->used = $used;
        $this->usedNode = $node;
    }

    /**
     * Checks whether the last value assigned was used.
     *
     * @return bool
     */
    public function isUsed()
    {
        return $this->used;
    }

    /**
     * Returns the last node where the variable was assigned or used.
     *
     * @return array
     */
    public function getLastUsedNode()
    {
        return $this->usedNode;
    }

    /**
     * Sets if the variable is not tracked by the memory manager.
     *
     * @param bool $memoryTracked
     */
    public function setMemoryTracked($memoryTracked)
    {
        $this->memoryTracked = $memoryTracked;
    }

    /**
     * Checks if the variable is tracked by the memory manager.
     *
     * @return bool
     */
    public function isMemoryTracked()
    {
        return $this->memoryTracked;
    }

    /**
     * Get the branch where the variable was declared.
     *
     * @return Branch|null
     */
    public function getBranch()
    {
        return $this->branch;
    }

    /**
     * Set the original AST node where the variable was declared.
     *
     * @param array $node
     */
    public function setOriginal(array $node)
    {
        $this->node = $node;
    }

    /**
     * Returns the original AST node where the variable was declared.
     *
     * @return array
     */
    public function getOriginal()
    {
        $node = $this->node;

        if (!$node) {
            $node = ['file' => 'unknown', 'line' => 0, 'char' => 0];
        }

        return $node;
    }

    /**
     * Sets the PHP class related to variable.
     *
     * @param array|string $classTypes
     */
    public function setClassTypes($classTypes)
    {
        if ($classTypes) {
            if (\is_string($classTypes)) {
                if (!\in_array($classTypes, $this->classTypes)) {
                    $this->classTypes[] = $classTypes;
                }
            } else {
                foreach ($classTypes as $classType) {
                    if (!\in_array($classType, $this->classTypes)) {
                        $this->classTypes[] = $classType;
                    }
                }
            }
        }
    }

    /**
     * Returns the PHP classes associated to the variable.
     *
     * @return array
     */
    public function getClassTypes()
    {
        return $this->classTypes;
    }

    /**
     * Sets the PHP class related to variable.
     *
     * @param ClassDefinition $associatedClass
     */
    public function setAssociatedClass($associatedClass)
    {
        $this->associatedClass = $associatedClass;
    }

    /**
     * Returns the class related to the variable.
     *
     * @return ClassDefinition
     */
    public function getAssociatedClass()
    {
        return $this->associatedClass;
    }

    /**
     * Sets the current dynamic type in a polymorphic variable.
     *
     * @param array|string $types
     */
    public function setDynamicTypes($types)
    {
        if ($types) {
            unset($this->dynamicTypes['unknown']);

            if (\is_string($types)) {
                if (!isset($this->dynamicTypes[$types])) {
                    $this->dynamicTypes[$types] = true;
                }
            } else {
                foreach ($types as $type => $one) {
                    if (!isset($this->dynamicTypes[$one])) {
                        $this->dynamicTypes[$one] = true;
                    }
                }
            }
        }
    }

    /**
     * Returns the current dynamic type in a polymorphic variable.
     *
     * @return array
     */
    public function getDynamicTypes()
    {
        return $this->dynamicTypes;
    }

    /**
     * Checks if the variable has any of the passed dynamic.
     *
     * @param mixed $types
     *
     * @return bool
     */
    public function hasAnyDynamicType($types)
    {
        if (\is_string($types)) {
            return isset($this->dynamicTypes[$types]);
        } else {
            foreach ($types as $type) {
                if (isset($this->dynamicTypes[$type])) {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * Check if the variable has at least one dynamic type to the ones passed in the list.
     *
     * @param array|string $types
     *
     * @return bool
     */
    public function hasDifferentDynamicType($types)
    {
        $number = 0;
        foreach ($types as $type) {
            if (isset($this->dynamicTypes[$type])) {
                ++$number;
            }
        }

        return 0 == $number;
    }

    /**
     * Increase the number of uses a variable may have.
     */
    public function increaseUses()
    {
        ++$this->numberUses;
    }

    /**
     * Increase the number of mutations a variable may have.
     */
    public function increaseMutates()
    {
        ++$this->numberMutates;
    }

    /**
     * Return the number of uses.
     *
     * @return int
     */
    public function getNumberUses()
    {
        return $this->numberUses;
    }

    /**
     * Returns the number of mutations performed over the variable.
     *
     * @return int
     */
    public function getNumberMutations()
    {
        return $this->numberMutates;
    }

    /**
     * Sets if the variable is initialized
     * This allow to throw an exception if the variable is being read without prior initialization.
     *
     * @param bool               $initialized
     * @param CompilationContext $compilationContext
     */
    public function setIsInitialized($initialized, CompilationContext $compilationContext)
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
     * Check if the variable is initialized or not.
     *
     * @return bool
     */
    public function isInitialized()
    {
        return $this->initialized;
    }

    /**
     * Set if the symbol is a parameter of the method or not.
     *
     * @param bool $isExternal
     */
    public function setIsExternal($isExternal)
    {
        $this->isExternal = $isExternal;
        $this->variantInits = 1;
    }

    /**
     * Check if the variable is a parameter.
     *
     * @return bool
     */
    public function isExternal()
    {
        return $this->isExternal;
    }

    /**
     * Get if the variable must be initialized to null.
     *
     * @return bool
     */
    public function mustInitNull()
    {
        return $this->mustInitNull;
    }

    /**
     * Set if the variable must be initialized to null.
     *
     * @param mixed $mustInitNull
     */
    public function setMustInitNull($mustInitNull)
    {
        $this->mustInitNull = (bool) $mustInitNull;
    }

    /**
     * Sets the default init value.
     *
     * @param mixed $value
     */
    public function setDefaultInitValue($value)
    {
        $this->defaultInitValue = $value;
    }

    /**
     * Sets an automatic safe default init value according to its type.
     */
    public function enableDefaultAutoInitValue()
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
                throw new CompilerException('Cannot create an automatic safe default value for variable type: '.$this->type);
        }
    }

    /**
     * Returns the default init value.
     *
     * @return mixed
     */
    public function getDefaultInitValue()
    {
        return $this->defaultInitValue;
    }

    /**
     * Separates variables before being updated.
     *
     * @param CompilationContext $compilationContext
     */
    public function separate(CompilationContext $compilationContext)
    {
        if ('this_ptr' != $this->getName() && 'return_value' != $this->getName()) {
            $compilationContext->codePrinter->output('ZEPHIR_SEPARATE('.$compilationContext->backend->getVariableCode($this).');');
        }
    }

    /**
     * Skips variable initialization.
     *
     * @param int $numberSkips
     */
    public function skipInitVariant($numberSkips)
    {
        $this->numberSkips += $numberSkips;
    }

    /**
     * Get the number of initializations remaining to skip.
     *
     * @return int
     */
    public function getSkipVariant()
    {
        return $this->numberSkips;
    }

    /*
     * Allocate memory for variable and init it null val
     *
     * @param CompilationContext $compilationContext
     */
    public function initNonReferenced(CompilationContext $compilationContext)
    {
        $compilationContext->headersManager->add('kernel/memory');
        $compilationContext->codePrinter->output('ZEPHIR_INIT_ZVAL_NREF('.$this->getName().');');
    }

    /**
     * Get the number of times the variable has been initialized.
     *
     * @return int
     */
    public function geVariantInits()
    {
        return $this->variantInits;
    }

    /**
     * Increase the number of times the variable has been initialized.
     */
    public function increaseVariantIfNull()
    {
        ++$this->variantInits;
    }

    /**
     * Initializes a variant variable.
     *
     * @param CompilationContext $compilationContext
     */
    public function initVariant(CompilationContext $compilationContext)
    {
        if ($this->numberSkips) {
            --$this->numberSkips;

            return;
        }

        /*
         * Variables are allocated for the first time using ZEPHIR_INIT_VAR
         * the second, third, etc times are allocated using ZEPHIR_INIT_NVAR
         * Variables initialized for the first time in a cycle are always initialized using ZEPHIR_INIT_NVAR
         */
        if ('this_ptr' != $this->getName() && 'return_value' != $this->getName()) {
            if (false === $this->initBranch) {
                $this->initBranch = $compilationContext->currentBranch;
            }

            $compilationContext->headersManager->add('kernel/memory');
            $compilationContext->symbolTable->mustGrownStack(true);

            if (!$this->isLocalOnly()) {
                if ($compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->backend->initVar($this, $compilationContext, true, true);
                } else {
                    if ($this->variantInits > 0) {
                        if (0 === $this->initBranch) {
                            $compilationContext->codePrinter->output('ZEPHIR_INIT_BNVAR('.$this->getName().');');
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
                    $compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(&'.$this->getName().');');
                } else {
                    $compilationContext->codePrinter->output('ZEPHIR_SINIT_VAR('.$this->getName().');');
                }
            }

            ++$this->variantInits;
            $this->associatedClass = null;
        }
    }

    /**
     * Tells the compiler a generated code will track the variable.
     *
     * @param CompilationContext $compilationContext
     */
    public function trackVariant(CompilationContext $compilationContext)
    {
        if ($this->numberSkips) {
            --$this->numberSkips;

            return;
        }

        /*
         * Variables are allocated for the first time using ZEPHIR_INIT_VAR
         * the second, third, etc times are allocated using ZEPHIR_INIT_NVAR
         * Variables initialized for the first time in a cycle are always initialized using ZEPHIR_INIT_NVAR
         */
        if ('this_ptr' != $this->getName() && 'return_value' != $this->getName()) {
            if (false === $this->initBranch) {
                $this->initBranch = $compilationContext->currentBranch;
            }

            if (!$this->isLocalOnly()) {
                $compilationContext->symbolTable->mustGrownStack(true);
                if ($compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                } else {
                    if ($this->variantInits > 0) {
                        if (1 !== $this->initBranch) {
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

    /**
     * Initializes a variant variable that is intended to have the special
     * behavior of only freed its body value instead of the full variable.
     *
     * @param CompilationContext $compilationContext
     */
    public function initComplexLiteralVariant(CompilationContext $compilationContext)
    {
        if ($this->numberSkips) {
            --$this->numberSkips;

            return;
        }

        if ('this_ptr' != $this->getName() && 'return_value' != $this->getName()) {
            if (false === $this->initBranch) {
                $this->initBranch = $compilationContext->currentBranch;
            }

            $compilationContext->headersManager->add('kernel/memory');
            $compilationContext->symbolTable->mustGrownStack(true);
            if (!$this->isLocalOnly()) {
                if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(&'.$this->getName().');');
                } else {
                    $compilationContext->backend->initVar($this, $compilationContext);
                }
            } else {
                if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(&'.$this->getName().');');
                } else {
                    $compilationContext->codePrinter->output('ZEPHIR_SINIT_VAR('.$this->getName().');');
                }
            }
            ++$this->variantInits;
        }
    }

    /**
     * Observes a variable in the memory frame without initialization.
     *
     * @param CompilationContext $compilationContext
     */
    public function observeVariant(CompilationContext $compilationContext)
    {
        if ($this->numberSkips) {
            --$this->numberSkips;

            return;
        }

        $name = $this->getName();
        if ('this_ptr' != $name && 'return_value' != $name) {
            if (false === $this->initBranch) {
                $this->initBranch = $compilationContext->currentBranch;
            }

            $compilationContext->headersManager->add('kernel/memory');
            $compilationContext->symbolTable->mustGrownStack(true);
            $symbol = $compilationContext->backend->getVariableCode($this);

            if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                $this->mustInitNull = true;
                $compilationContext->codePrinter->output('ZEPHIR_OBS_NVAR('.$symbol.');');
            } else {
                $compilationContext->codePrinter->output('ZEPHIR_OBS_VAR('.$symbol.');');
            }

            ++$this->variantInits;
        }
    }

    /**
     * Observes a variable in the memory frame without initialization or nullify
     * an existing allocated variable.
     *
     * @param CompilationContext $compilationContext
     */
    public function observeOrNullifyVariant(CompilationContext $compilationContext)
    {
        if ($this->numberSkips) {
            --$this->numberSkips;

            return;
        }

        $name = $this->getName();
        if ('this_ptr' != $name && 'return_value' != $name) {
            if (false === $this->initBranch) {
                $this->initBranch = $compilationContext->currentBranch;
            }

            $compilationContext->headersManager->add('kernel/memory');
            $compilationContext->symbolTable->mustGrownStack(true);
            /**
             * TODO: Do we need this?
             * $compilationContext->codePrinter->output('ZEPHIR_OBS_NVAR(' . $this->getName() . ');');.
             *
             * TODO: What about else?
             * $compilationContext->codePrinter->output('ZEPHIR_OBS_VAR(' . $this->getName() . ');');
             */
            if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                $this->mustInitNull = true;
            }

            ++$this->variantInits;

            $this->setMustInitNull(true);
        }
    }

    /**
     * Whether the variable is of a complex type.
     *
     * @return bool
     */
    public function isComplex()
    {
        return isset($this->complexTypes[$this->type]);
    }

    /**
     * Checks if a variable is a superglobal.
     *
     * @return bool
     */
    public function isSuperGlobal()
    {
        return $this->isExternal && $this->globalsManager->isSuperGlobal($this->name);
    }

    /**
     * Checks if a variable is a local static.
     *
     * @return bool
     */
    public function isLocalSatic()
    {
        return $this->isExternal && $this->localOnly;
    }

    /**
     * Shortcut is type variable?
     *
     * @return bool
     */
    public function isVariable()
    {
        return 'variable' == $this->type;
    }

    /**
     * Shortcut is type bool?
     *
     * @return bool
     */
    public function isBoolean()
    {
        return 'bool' == $this->type;
    }

    /**
     * Shortcut is type string?
     *
     * @return bool
     */
    public function isString()
    {
        return 'string' == $this->type;
    }

    /**
     * Shortcut is type int?
     *
     * @return bool
     */
    public function isInt()
    {
        return 'int' == $this->type;
    }

    /**
     * Shortcut is type double?
     *
     * @return bool
     */
    public function isDouble()
    {
        return 'double' == $this->type;
    }

    /**
     * Shortcut is type double?
     *
     * @return bool
     */
    public function isArray()
    {
        return 'array' == $this->type;
    }

    /**
     * Shortcut is type variable or string?
     *
     * @return bool
     */
    public function isNotVariable()
    {
        return !$this->isVariable();
    }

    /**
     * Shortcut is type variable or string?
     *
     * @return bool
     */
    public function isNotVariableAndString()
    {
        return !$this->isVariable() && !$this->isString();
    }

    /**
     * Shortcut is type variable or array?
     *
     * @return bool
     */
    public function isNotVariableAndArray()
    {
        return !$this->isVariable() && !$this->isArray();
    }

    /**
     * Sets the latest CompiledExpression assigned to a variable.
     *
     * @param CompiledExpression $possibleValue
     * @param CompilationContext $compilationContext
     */
    public function setPossibleValue(CompiledExpression $possibleValue, CompilationContext $compilationContext)
    {
        $this->possibleValue = $possibleValue;
        $this->possibleValueBranch = $compilationContext->branchManager->getCurrentBranch();
    }

    /**
     * Returns the latest CompiledExpression assigned to a variable.
     *
     * @return mixed
     */
    public function getPossibleValue()
    {
        return $this->possibleValue;
    }

    /**
     * Returns the branch where the variable was assigned for the last time.
     *
     * @return Branch
     */
    public function getPossibleValueBranch()
    {
        return $this->possibleValueBranch;
    }
}
