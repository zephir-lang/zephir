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
 * Variable
 *
 * This represents a variable in a symbol table
 */
class Variable
{
    /**
     * Variable's type
     */
    protected $type;

    /**
     * Current dynamic type of the variable
     * @var array
     */
    protected $dynamicTypes = array('unknown' => true);

    /**
     * Variable's name
     */
    protected $name;

    /**
     * Branch where the variable was declared
     */
    protected $branch;

    /**
     * Branch where the variable was initialized for the first time
     */
    protected $initBranch = false;

    /**
     * Compiled variable's name
     */
    protected $lowName;

    /**
     * Number of times the variable has been read
     */
    protected $numberUses = 0;

    /**
     * Whether the variable is temporal or not
     */
    protected $temporal = false;

    /**
     * Temporal variables are marked as idle
     */
    protected $idle = false;

    /**
     * Reusable temporary variables?
     */
    protected $reusable = true;

    /**
     * Number of mutations to the variable
     */
    protected $numberMutates = 0;

    /**
     * Whether the variable has received any assignment
     */
    protected $initialized = false;

    protected $initBranches = array();

    protected $isExternal = false;

    protected $variantInits = 0;

    protected $mustInitNull = false;

    protected $readOnly = false;

    protected $localOnly = false;

    protected $memoryTracked = true;

    protected $doublePointer = false;

    protected $defaultInitValue;

    /**
     * Class types
     */
    protected $classTypes = array();

    /**
     * Associated class
     */
    protected $associatedClass;

    /**
     * Initialization skips
     */
    protected $numberSkips = 0;

    /**
     * AST node where the variable was originally declared or created
     */
    protected $node;

    /**
     * Possible constant value assigned to the variable
     */
    protected $possibleValue;

    /**
     * Branch where the variable got its last possible value
     */
    protected $possibleValueBranch;

    /**
     * Whether the variable was used or not
     */
    protected $used = true;

    /**
     * Last AST node where the variable was used
     */
    protected $usedNode;

    /**
     * Variable constructor
     *
     * @param string $type
     * @param string $name
     * @param Branch $branch
     * @param mixed $defaultInitValue
     */
    public function __construct($type, $name, $branch, $defaultInitValue = null)
    {
        switch ($type) {
            case 'callable':
            case 'object':
            case 'resource':
                $type = 'variable';
                break;
        }

        $this->type = $type;
        $this->name = $name;
        $this->branch = $branch;
    }

    /**
     * Get init branch
     *
     * @return bool|int
     */
    public function getInitBranch()
    {
        return $this->initBranch;
    }

    /**
     * Get init marked branch
     *
     * @return bool|int
     */
    public function getInitBranches()
    {
        return $this->initBranches;
    }

    /**
     * Returns the type of variable
     *
     * @return string
     */
    public function getType()
    {
        return $this->type;
    }

    /**
     * Sets if the variable is local-only scoped
     *
     * @param boolean $localOnly
     */
    public function setLocalOnly($localOnly)
    {
        $this->localOnly = $localOnly;
    }

    /**
     * Checks if the variable is local-only scoped
     *
     * @return boolean
     */
    public function isLocalOnly()
    {
        return $this->localOnly;
    }

    /**
     * Marks the variable to be defined as a double pointer
     *
     * @param boolean $doublePointer
     */
    public function setIsDoublePointer($doublePointer)
    {
        $this->doublePointer = $doublePointer;
    }

    /**
     * Returns the variable
     */
    public function isDoublePointer()
    {
        return $this->doublePointer;
    }

    /**
     * Returns variable's real name
     *
     * @return string
     */
    public function getRealName()
    {
        return $this->name;
    }

    /**
     * Returns variable's name
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
     * Sets the compiled variable's name
     *
     * @param string $lowName
     */
    public function setLowName($lowName)
    {
        $this->lowName = $lowName;
    }

    /**
     * Sets if the variable is read only
     *
     * @param boolean $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->readOnly = $readOnly;
    }

    /**
     * Returns if the variable is read only
     *
     * @return boolean
     */
    public function isReadOnly()
    {
        return $this->readOnly;
    }

    /**
     * Sets whether the variable is temporal or not
     *
     * @param boolean $temporal
     */
    public function setTemporal($temporal)
    {
        $this->temporal = $temporal;
    }

    /**
     * Returns whether the variable is temporal or not
     *
     * @return boolean
     */
    public function isTemporal()
    {
        return $this->temporal;
    }

    /**
     * Once a temporal variable is unused in a specific branch it is marked as idle
     *
     * @param boolean $idle
     */
    public function setIdle($idle)
    {
        if ($this->reusable) {
            $this->classTypes = array();
            $this->dynamicTypes = array('unknown' => true);
            $this->idle = $idle;
        } else {
            $this->idle = false;
        }
    }

    /**
     * Checks if the variable is idle
     *
     * @return boolean
     */
    public function isIdle()
    {
        return $this->idle;
    }

    /**
     * Some temporary variables can't be reused
     *
     * @param boolean $reusable
     */
    public function setReusable($reusable)
    {
        $this->reusable = $reusable;
    }

    /**
     * Checks if the temporary variable is reusable
     *
     * @return boolean
     */
    public function isReusable()
    {
        return $this->reusable;
    }

    /**
     * Sets the latest node where a variable was used
     *
     * @param boolean $used
     * @param array $node
     */
    public function setUsed($used, $node)
    {
        $this->used = $used;
        $this->usedNode = $node;
    }

    /**
     * Checks whether the last value assigned was used
     *
     * @return boolean
     */
    public function isUsed()
    {
        return $this->used;
    }

    /**
     * Returns the last node where the variable was assigned or used
     *
     * @return array
     */
    public function getLastUsedNode()
    {
        return $this->usedNode;
    }

    /**
     * Sets if the variable is not tracked by the memory manager
     *
     * @param boolean $memoryTracked
     */
    public function setMemoryTracked($memoryTracked)
    {
        $this->memoryTracked = $memoryTracked;
    }

    /**
     * Checks if the variable is tracked by the memory manager
     *
     * @return boolean
     */
    public function isMemoryTracked()
    {
        return $this->memoryTracked;
    }

    /**
     * Get the branch where the variable was declared
     *
     * @return int
     */
    public function getBranch()
    {
        return $this->branch;
    }

    /**
     * Set the original AST node where the variable was declared
     *
     * @param array $node
     */
    public function setOriginal($node)
    {
        $this->node = $node;
    }

    /**
     * Returns the original AST node where the variable was declared
     *
     * @return array
     */
    public function getOriginal()
    {
        $node = $this->node;
        if ($node) {
            return $node;
        }
        return array('file' => 'unknown', 'line' => 0, 'char' => 0);
    }

    /**
     * Sets the PHP class related to variable
     *
     * @param string|array $classTypes
     */
    public function setClassTypes($classTypes)
    {
        if ($classTypes) {
            if (is_string($classTypes)) {
                if (!in_array($classTypes, $this->classTypes)) {
                    $this->classTypes[] = $classTypes;
                }
            } else {
                foreach ($classTypes as $classType) {
                    if (!in_array($classType, $this->classTypes)) {
                        $this->classTypes[] = $classType;
                    }
                }
            }
        }
    }

    /**
     * Returns the PHP classes associated to the variable
     *
     * @return array
     */
    public function getClassTypes()
    {
        return $this->classTypes;
    }

    /**
     * Sets the PHP class related to variable
     *
     * @param ClassDefinition $associatedClass
     */
    public function setAssociatedClass($associatedClass)
    {
        $this->associatedClass = $associatedClass;
    }

    /**
     * Returns the class related to the variable
     *
     * @return ClassDefinition
     */
    public function getAssociatedClass()
    {
        return $this->associatedClass;
    }

    /**
     * Sets the current dynamic type in a polymorphic variable
     *
     * @param string|array $types
     */
    public function setDynamicTypes($types)
    {
        if ($types) {
            unset($this->dynamicTypes['unknown']);

            if (is_string($types)) {
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
     * Returns the current dynamic type in a polymorphic variable
     *
     * @return array
     */
    public function getDynamicTypes()
    {
        return $this->dynamicTypes;
    }

    /**
     * Checks if the variable has any of the passed dynamic
     *
     * @param mixed $types
     */
    public function hasAnyDynamicType($types)
    {
        if (is_string($types)) {
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
     * Check if the variable has at least one dynamic type to the ones passed in the list
     *
     * @param array|string $types
     */
    public function hasDifferentDynamicType($types)
    {
        $number = 0;
        foreach ($types as $type) {
            if (isset($this->dynamicTypes[$type])) {
                $number++;
            }
        }
        return $number == 0;
    }

    /**
     * Increase the number of uses a variable may have
     */
    public function increaseUses()
    {
        $this->numberUses++;
    }

    /**
     * Increase the number of mutations a variable may have
     */
    public function increaseMutates()
    {
        $this->numberMutates++;
    }

    /**
     * Return the number of uses
     *
     * @return long
     */
    public function getNumberUses()
    {
        return $this->numberUses;
    }

    /**
     * Returns the number of mutations performed over the variable
     *
     * @return long
     */
    public function getNumberMutations()
    {
        return $this->numberMutates;
    }

    /**
     * Sets if the variable is initialized
     * This allow to throw an exception if the variable is being read without prior initialization
     *
     * @param boolean $initialized
     * @param CompilationContext $compilationContext
     */
    public function setIsInitialized($initialized, CompilationContext $compilationContext)
    {
        if ($initialized) {
            if ($compilationContext->branchManager) {
                $currentBranch = $compilationContext->branchManager->getCurrentBranch();
                if ($currentBranch) {
                    $this->initBranches[] = $currentBranch;
                }
            }
        }
        $this->initialized = $initialized;
    }

    /**
     * Check if the variable is initialized or not
     *
     * @return boolean
     */
    public function isInitialized()
    {
        return $this->initialized;
    }

    /**
     * Set if the symbol is a parameter of the method or not
     *
     * @param boolean $isExternal
     */
    public function setIsExternal($isExternal)
    {
        $this->isExternal = $isExternal;
        $this->variantInits = 1;
    }

    /**
     * Check if the variable is a parameter
     *
     * @return boolean
     */
    public function isExternal()
    {
        return $this->isExternal;
    }

    /**
     * Set if the variable must be initialized to null
     *
     * @return boolean
     */
    public function mustInitNull()
    {
        return $this->mustInitNull;
    }

    /**
     * Set if the variable must be initialized to null
     *
     * @param boolean $mustInitNull
     * @return boolean
     */
    public function setMustInitNull($mustInitNull)
    {
        $this->mustInitNull = (boolean) $mustInitNull;
    }

    /**
     * Sets the default init value
     *
     * @param mixed $value
     */
    public function setDefaultInitValue($value)
    {
        $this->defaultInitValue = $value;
    }

    /**
     * Sets an automatic safe default init value according to its type
     *
     */
    public function enableDefaultAutoInitValue()
    {
        switch ($this->type) {

            case 'boolean':
            case 'bool':
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
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
                throw new CompilerException('Cannot create an automatic safe default value for variable type: ' . $this->type);
        }
    }

    /**
     * Returns the default init value
     *
     * @return mixed
     */
    public function getDefaultInitValue()
    {
        return $this->defaultInitValue;
    }

    /**
     * Separates variables before being updated
     *
     * @param CompilationContext $compilationContext
     */
    public function separate(CompilationContext $compilationContext)
    {
        if ($this->getName() != 'this_ptr' && $this->getName() != 'return_value') {
            $compilationContext->codePrinter->output('ZEPHIR_SEPARATE(' . $this->getName() . ');');
        }
    }

    /**
     * Skips variable initialization
     *
     * @param int $numberSkips
     */
    public function skipInitVariant($numberSkips)
    {
        $this->numberSkips += $numberSkips;
    }

    /**
     * Get the number of initializations remaining to skip
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
        $compilationContext->codePrinter->output('ZEPHIR_INIT_ZVAL_NREF(' . $this->name . ');');
    }

    /**
     * Increase the number of times the varible has been initialized
     */
    public function increaseVariantIfNull()
    {
        $this->variantInits++;
    }

    /**
     * Initializes a variant variable
     *
     * @param CompilationContext $compilationContext
     */
    public function initVariant(CompilationContext $compilationContext)
    {
        if ($this->numberSkips) {
            $this->numberSkips--;
            return;
        }

        /**
         * Variables are allocated for the first time using ZEPHIR_INIT_VAR
         * the second, third, etc times are allocated using ZEPHIR_INIT_NVAR
         * Variables initialized for the first time in a cycle are always initialized using ZEPHIR_INIT_NVAR
         */
        if ($this->getName() != 'this_ptr' && $this->getName() != 'return_value') {
            if ($this->initBranch === false) {
                $this->initBranch = $compilationContext->currentBranch;
            }

            $compilationContext->headersManager->add('kernel/memory');

            if (!$this->isLocalOnly()) {
                $compilationContext->symbolTable->mustGrownStack(true);
                if ($compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(' . $this->getName() . ');');
                } else {
                    if ($this->variantInits > 0) {
                        if ($this->initBranch === 0) {
                            $compilationContext->codePrinter->output('ZEPHIR_INIT_BNVAR(' . $this->getName() . ');');
                        } else {
                            $this->mustInitNull = true;
                            $compilationContext->codePrinter->output('ZEPHIR_INIT_NVAR(' . $this->getName() . ');');
                        }
                    } else {
                        $compilationContext->codePrinter->output('ZEPHIR_INIT_VAR(' . $this->getName() . ');');
                    }
                }
            } else {
                if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->codePrinter->output('ZEPHIR_SINIT_NVAR(' . $this->getName() . ');');
                } else {
                    $compilationContext->codePrinter->output('ZEPHIR_SINIT_VAR(' . $this->getName() . ');');
                }
            }

            $this->variantInits++;
            $this->associatedClass = null;
        }
    }

    /**
     * Tells the compiler a generated code will track the variable
     *
     * @param CompilationContext $compilationContext
     */
    public function trackVariant(CompilationContext $compilationContext)
    {
        if ($this->numberSkips) {
            $this->numberSkips--;
            return;
        }

        /**
         * Variables are allocated for the first time using ZEPHIR_INIT_VAR
         * the second, third, etc times are allocated using ZEPHIR_INIT_NVAR
         * Variables initialized for the first time in a cycle are always initialized using ZEPHIR_INIT_NVAR
         */
        if ($this->getName() != 'this_ptr' && $this->getName() != 'return_value') {
            if ($this->initBranch === false) {
                $this->initBranch = $compilationContext->currentBranch;
            }

            if (!$this->isLocalOnly()) {
                $compilationContext->symbolTable->mustGrownStack(true);
                if ($compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                } else {
                    if ($this->variantInits > 0) {
                        if ($this->initBranch !== 1) {
                            $this->mustInitNull = true;
                        }
                    }
                }
            } else {
                if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                }
            }

            $this->variantInits++;
        }
    }

    /**
     * Initializes a variant variable that is intended to have the special
     * behavior of only freed its body value instead of the full variable
     *
     * @param CompilationContext $compilationContext
     */
    public function initComplexLiteralVariant(CompilationContext $compilationContext)
    {
        if ($this->numberSkips) {
            $this->numberSkips--;
            return;
        }

        if ($this->getName() != 'this_ptr' && $this->getName() != 'return_value') {
            if ($this->initBranch === false) {
                $this->initBranch = $compilationContext->currentBranch;
            }

            $compilationContext->headersManager->add('kernel/memory');
            if (!$this->isLocalOnly()) {
                $compilationContext->symbolTable->mustGrownStack(true);
                if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->codePrinter->output('ZEPHIR_INIT_LNVAR(' . $this->getName() . ');');
                } else {
                    $compilationContext->codePrinter->output('ZEPHIR_INIT_VAR(' . $this->getName() . ');');
                }
            } else {
                if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                    $this->mustInitNull = true;
                    $compilationContext->codePrinter->output('ZEPHIR_SINIT_LNVAR(' . $this->getName() . ');');
                } else {
                    $compilationContext->codePrinter->output('ZEPHIR_SINIT_VAR(' . $this->getName() . ');');
                }
            }
            $this->variantInits++;
        }
    }

    /**
     * Observes a variable in the memory frame without initialization
     *
     * @param CompilationContext $compilationContext
     */
    public function observeVariant(CompilationContext $compilationContext)
    {
        if ($this->numberSkips) {
            $this->numberSkips--;
            return;
        }

        $name = $this->getName();
        if ($name != 'this_ptr' && $name != 'return_value') {
            if ($this->initBranch === false) {
                $this->initBranch = $compilationContext->currentBranch;
            }

            $compilationContext->headersManager->add('kernel/memory');
            $compilationContext->symbolTable->mustGrownStack(true);
            if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                $this->mustInitNull = true;
                $compilationContext->codePrinter->output('ZEPHIR_OBS_NVAR(' . $this->getName() . ');');
            } else {
                $compilationContext->codePrinter->output('ZEPHIR_OBS_VAR(' . $this->getName() . ');');
            }
            $this->variantInits++;
        }
    }

    /**
     * Observes a variable in the memory frame without initialization or nullify
     * an existing allocated variable
     *
     * @param CompilationContext $compilationContext
     */
    public function observeOrNullifyVariant(CompilationContext $compilationContext)
    {
        if ($this->numberSkips) {
            $this->numberSkips--;
            return;
        }

        $name = $this->getName();
        if ($name != 'this_ptr' && $name != 'return_value') {
            if ($this->initBranch === false) {
                $this->initBranch = $compilationContext->currentBranch;
            }

            $compilationContext->headersManager->add('kernel/memory');
            $compilationContext->symbolTable->mustGrownStack(true);
            if ($this->variantInits > 0 || $compilationContext->insideCycle) {
                $this->mustInitNull = true;
                //$compilationContext->codePrinter->output('ZEPHIR_OBS_NVAR(' . $this->getName() . ');');
            } else {
                //$compilationContext->codePrinter->output('ZEPHIR_OBS_VAR(' . $this->getName() . ');');
            }
            $this->variantInits++;

            $this->setMustInitNull(true);
        }
    }

    /**
     * Shortcut is type variable?
     *
     * @return bool
     */
    public function isVariable()
    {
        return $this->type == 'variable';
    }

    /**
     * Shortcut is type bool?
     *
     * @return bool
     */
    public function isBoolean()
    {
        return $this->type == 'bool';
    }

    /**
     * Shortcut is type string?
     *
     * @return bool
     */
    public function isString()
    {
        return $this->type == 'string';
    }

    /**
     * Shortcut is type int?
     *
     * @return bool
     */
    public function isInt()
    {
        return $this->type == 'int';
    }

    /**
     * Shortcut is type double?
     *
     * @return bool
     */
    public function isDouble()
    {
        return $this->type == 'double';
    }

    /**
     * Shortcut is type double?
     *
     * @return bool
     */
    public function isArray()
    {
        return $this->type == 'array';
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
     * Sets the lastest CompiledExpression assigned to a variable
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
     * Returns the lastest CompiledExpression assigned to a variable
     *
     * @return mixed
     */
    public function getPossibleValue()
    {
        return $this->possibleValue;
    }

    /**
     * Returns the branch where the variable was assigned for the last time
     *
     * @return Branch
     */
    public function getPossibleValueBranch()
    {
        return $this->possibleValueBranch;
    }
}
