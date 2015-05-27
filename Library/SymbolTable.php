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

use Zephir\Passes\LocalContextPass;
use Zephir\Passes\StaticTypeInference;

/**
 * SymbolTable
 *
 * A symbol table stores all the variables defined in a method, their data types and default values
 */
class SymbolTable
{
    protected $mustGrownStack = false;

    protected $variables = array();

    protected $tempVariable = 0;

    protected $tempVariables = array();

    /**
     * @var LocalContextPass
     */
    protected $localContext;

    /**
     * SymbolTable
     *
     * @param CompilationContext $compilationContext
     */
    public function __construct(CompilationContext $compilationContext)
    {
        /* this_ptr */
        $thisVar = new Variable('variable', 'this', $compilationContext->currentBranch);
        $thisVar->setIsInitialized(true, $compilationContext, array());
        $thisVar->increaseUses();
        $thisVar->setReadOnly(true);
        $thisVar->setLowName('this_ptr');
        $thisVar->setDynamicTypes('object');
        $this->variables['this'] = $thisVar;

        $returnValue = new Variable('variable', 'return_value', $compilationContext->currentBranch);
        $returnValue->setIsInitialized(true, $compilationContext, array());
        $returnValue->increaseUses();
        $this->variables['return_value'] = $returnValue;

        $returnValue = new Variable('variable', 'return_value_ptr', $compilationContext->currentBranch);
        $returnValue->setIsInitialized(true, $compilationContext, array());
        $returnValue->increaseUses();
        $this->variables['return_value_ptr'] = $returnValue;
    }

    /**
     * Sets the local context information
     *
     * @param LocalContextPass $localContext
     */
    public function setLocalContext(LocalContextPass $localContext)
    {
        $this->localContext = $localContext;
    }

    /**
     * Check if a variable is declared in the current symbol table
     *
     * @param string $name
     * @return boolean
     */
    public function hasVariable($name)
    {
        return isset($this->variables[$name]);
    }

    /**
     * Adds a variable to the symbol table
     *
     * @param int $type
     * @param string $name
     * @param CompilationContext $compilationContext
     * @param mixed $defaultValue
     * @return Variable
     */
    public function addVariable($type, $name, CompilationContext $compilationContext, $defaultValue = null)
    {
        $variable = new Variable($type, $name, $compilationContext->currentBranch, $defaultValue);
        if ($type == 'variable') {
            if ($this->localContext) {
                /**
                 * Checks whether a variable can be optimized to be static or not
                 */
                if ($this->localContext->shouldBeLocal($name)) {
                    $variable->setLocalOnly(true);
                }
            }
        }
        $this->variables[$name] = $variable;
        return $variable;
    }

    /**
     * Adds a raw variable to the symbol table
     *
     * @param Variable $variable
     * @return Variable
     */
    public function addRawVariable(Variable $variable)
    {
        $this->variables[$variable->getName()] = $variable;
        return $variable;
    }

    /**
     * Returns a variable in the symbol table
     *
     * @param $name
     * @return bool
     */
    public function getVariable($name)
    {
        if (isset($this->variables[$name])) {
            return $this->variables[$name];
        }

        return false;
    }

    /**
     * Returns all the variables defined in the symbol table
     *
     * @return array
     */
    public function getVariables()
    {
        return $this->variables;
    }

    /**
     * Checks if a variable is a superglobal
     *
     * @param string $name
     * @return boolean
     */
    public function isSuperGlobal($name)
    {
        return $name == '_GET' || $name == '_POST' || $name == '_COOKIE' || $name == '_SERVER' || $name == '_SESSION' || $name == '_REQUEST' || $name == '_FILES';
    }

    /**
     * Return a variable in the symbol table, it will be used for a read operation
     *
     * @param string $name
     * @param CompilationContext $compilationContext
     * @param array $statement
     * @return Variable
     */
    public function getVariableForRead($name, CompilationContext $compilationContext = null, array $statement = null)
    {
        /**
         * Validate that 'this' cannot be used in a static function
         */
        if ($name == 'this' || $name == 'this_ptr') {
            if ($compilationContext->staticContext) {
                throw new CompilerException("Cannot use variable 'this' in static context", $statement);
            }
        }

        /**
         * Create superglobals just in time
         */
        if ($this->isSuperGlobal($name)) {
            if (!$this->hasVariable($name)) {
                /**
                 * @TODO, injecting globals, initialize to null and check first?
                 */
                $variable = new Variable('variable', $name, $compilationContext->currentBranch);
                $variable->setIsInitialized(true, $compilationContext, $statement);
                $variable->setDynamicTypes('array');
                $variable->setIsExternal(true);
                $this->variables[$name] = $variable;
            } else {
                $variable = $this->getVariable($name);
            }
            $variable->increaseUses();
            return $variable;
        }

        if (!$this->hasVariable($name)) {
            throw new CompilerException("Cannot read variable '" . $name . "' because it wasn't declared", $statement);
        }

        $variable = $this->getVariable($name);
        if (!$variable->isInitialized()) {
            /* DON'T REMOVE THIS!! */
            throw new CompilerException("Variable '" . $name . "' cannot be read because it's not initialized", $statement);
        }

        $variable->increaseUses();

        /**
         * Analize branches to detect possible initialization of variables in conditional branches
         */
        if (!$variable->isTemporal() && !$variable->getSkipVariant()) {
            if ($name != 'return_value' && $name != 'this') {
                if (is_object($compilationContext) && is_object($compilationContext->branchManager)) {
                    if ($compilationContext->config->get('check-invalid-reads', 'optimizations')) {
                        switch ($variable->getType()) {
                            case 'variable':
                            case 'string':
                            case 'array':
                                if (!$variable->isLocalOnly()) {
                                    $variable->setMustInitNull(true);
                                    $compilationContext->codePrinter->output('ZEPHIR_CHECK_POINTER(' . $variable->getName() . ');');
                                }
                                break;
                        }
                    }

                    $initBranches = $variable->getInitBranches();

                    $currentBranch = $compilationContext->branchManager->getCurrentBranch();
                    $branches = array_reverse($initBranches);

                    if (count($branches) == 1) {
                        if (Branch::TYPE_CONDITIONAL_TRUE == $branches[0]->getType()) {
                            if ($branches[0]->isUnreachable() === true) {
                                throw new CompilerException('Initialization of variable "' . $name . '" depends on unreacheable branch, consider initialize it at its declaration', $statement);
                            }
                        } else {
                            if (Branch::TYPE_CONDITIONAL_FALSE == $branches[0]->getType()) {
                                if ($branches[0]->isUnreachable() === false) {
                                    throw new CompilerException('Initialization of variable "' . $name . '" depends on unreacheable branch, consider initialize at its declaration', $statement);
                                }
                            }
                        }

                        $tempBranch = $branches[0]->getParentBranch();
                        while ($tempBranch) {
                            if ($tempBranch->getType() == Branch::TYPE_CONDITIONAL_TRUE) {
                                if ($tempBranch->isUnreachable() === true) {
                                    throw new CompilerException('Initialization of variable "' . $name . '" depends on unreacheable branch, consider initialize it at its declaration', $statement);
                                }
                            }

                            $tempBranch = $tempBranch->getParentBranch();
                        }
                    }

                    $found = false;
                    foreach ($branches as $branch) {
                        /*+
                         * Variable was initialized in the same current branch
                         */
                        if ($branch == $currentBranch) {
                            $found = true;
                            break;
                        }

                        /**
                         * 'root' and 'external' branches are safe branches
                         */
                        if ($branch->getType() == Branch::TYPE_ROOT || $branch->getType() == Branch::TYPE_EXTERNAL) {
                            $found = true;
                            break;
                        }
                    }

                    if (!$found) {
                        /**
                         * Check if last assignment
                         * Variable was initialized in a sub-branch and it's being used in a parent branch
                         */
                        $possibleBadAssignment = false;

                        if ($currentBranch->getLevel() < $branches[0]->getLevel()) {
                            $possibleBadAssignment = true;
                        }

                        if ($possibleBadAssignment) {
                            if (count($branches) > 1) {
                                $graph = new BranchGraph();
                                foreach ($branches as $branch) {
                                    $graph->addLeaf($branch);
                                }
                                //echo $graph->getRoot()->show();
                            } else {
                                /**
                                 * Variable is assigned just once and it's assigned in a conditional branch
                                 */
                                if ($branches[0]->getType() == Branch::TYPE_CONDITIONAL_TRUE) {
                                    $evalExpression = $branches[0]->getRelatedStatement()->getEvalExpression();

                                    if ($evalExpression->isUnreachable() === true) {
                                        throw new CompilerException("Variable '" . $name . "' was assigned for the first time in conditional branch, consider initialize it at its declaration", $statement);
                                    } else {
                                        $variable->enableDefaultAutoInitValue();
                                        $compilationContext->logger->warning("Variable '" . $name . "' was assigned for the first time in conditional branch, consider initialize it at its declaration", 'conditional-initialization', $statement);
                                    }
                                } else {
                                    if ($branches[0]->getType() == Branch::TYPE_CONDITIONAL_FALSE) {
                                        $evalExpression = $branches[0]->getRelatedStatement()->getEvalExpression();

                                        if ($evalExpression->isUnreachableElse() === true) {
                                            throw new CompilerException("Variable '" . $name . "' was assigned for the first time in conditional branch, consider initialize it at its declaration", $statement);
                                        } else {
                                            $variable->enableDefaultAutoInitValue();
                                            $compilationContext->logger->warning("Variable '" . $name . "' was assigned for the first time in conditional branch, consider initialize it at its declaration", 'conditional-initialization', $statement);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /**
         * Saves the lastest place where the variable was used
         */
        $variable->setUsed(true, $statement);

        return $variable;
    }

    /**
     * Return a variable in the symbol table, it will be used for a write operation
     * Some variables aren't writable themselves but their members do
     *
     * @param string $name
     * @param CompilationContext $compilationContext
     * @param array $statement
     * @return Variable
     */
    public function getVariableForWrite($name, CompilationContext $compilationContext, array $statement = null)
    {
        /**
         * Create superglobals just in time
         */
        if ($this->isSuperGlobal($name)) {
            if (!$this->hasVariable($name)) {
                /**
                 * @TODO, injecting globals, initialize to null and check first?
                 */
                $superVar = new Variable('variable', $name, $compilationContext->currentBranch);
                $superVar->setIsInitialized(true, $compilationContext, $statement);
                $superVar->setDynamicTypes('array');
                $superVar->increaseMutates();
                $superVar->increaseUses();
                $superVar->setUsed(true, $statement);
                $this->variables[$name] = $superVar;
                return $superVar;
            }
        }

        if (!$this->hasVariable($name)) {
            throw new CompilerException("Cannot mutate variable '" . $name . "' because it wasn't defined", $statement);
        }

        $variable = $this->getVariable($name);
        $variable->increaseUses();
        $variable->increaseMutates();

        /**
         * Saves the last place where the variable was mutated
         * We discard mutations inside loops because iterations could use the value
         * and Zephir only provides top-down compilation
         */
        if (!$compilationContext->insideCycle) {
            $variable->setUsed(false, $statement);
        } else {
            $variable->setUsed(true, $statement);
        }

        return $variable;
    }

    /**
     * Return a variable in the symbol table, it will be used for a mutating operation
     * This method implies mutation of one of the members of the variable but no the variables it self
     *
     * @param string $name
     * @param CompilationContext $compilationContext
     * @param array $statement
     * @return Variable
     */
    public function getVariableForUpdate($name, CompilationContext $compilationContext, array $statement = null)
    {
        /**
         * Create superglobals just in time
         */
        if ($this->isSuperGlobal($name)) {
            if (!$this->hasVariable($name)) {
                /**
                 * @TODO, injecting globals, initialize to null and check first?
                 */
                $superVar = new Variable('variable', $name, $compilationContext->currentBranch);
                $superVar->setIsInitialized(true, $compilationContext, $statement);
                $superVar->setDynamicTypes('array');
                $superVar->increaseMutates();
                $superVar->increaseUses();
                $superVar->setIsExternal(true);
                $superVar->setUsed(true, $statement);
                $this->variables[$name] = $superVar;
                return $superVar;
            }
        }

        if (!$this->hasVariable($name)) {
            throw new CompilerException("Cannot mutate variable '" . $name . "' because it wasn't defined", $statement);
        }

        $variable = $this->getVariable($name);
        $variable->increaseUses();
        $variable->increaseMutates();

        /**
         * Saves the last place where the variable was mutated
         * We discard mutations inside loops because iterations could use the value
         * and Zephir only provides top-down compilation
         */
        $variable->setUsed(true, $statement);

        return $variable;
    }

    /**
     * Return a variable in the symbol table, it will be used for a write operation
     *
     * @param boolean $mustGrownStack
     */
    public function mustGrownStack($mustGrownStack)
    {
        $this->mustGrownStack = $mustGrownStack;
    }

    /**
     * Returns if the current symbol label must add a memory frame
     *
     * @return boolean
     */
    public function getMustGrownStack()
    {
        return $this->mustGrownStack;
    }

    /**
     * Register a variable as temporal
     *
     * @param string $type
     * @param string $location
     * @param Variable $variable
     */
    protected function registerTempVariable($type, $location, Variable $variable)
    {
        if (!isset($this->tempVariables[$location][$type])) {
            $this->tempVariables[$location][$type] = array();
        }
        $this->tempVariables[$location][$type][] = $variable;
    }

    /**
     * Reuse variables marked as idle after leave a branch
     *
     * @param string $type
     * @param string $location
     * @return Variable
     */
    protected function reuseTempVariable($type, $location)
    {
        if (isset($this->tempVariables[$location][$type])) {
            foreach ($this->tempVariables[$location][$type] as $variable) {
                if (!$variable->isDoublePointer()) {
                    if ($variable->isIdle()) {
                        $variable->setIdle(false);
                        return $variable;
                    }
                }
            }
        }
        return null;
    }

    /**
     * Returns a temporal variable
     *
     * @param string $type
     * @param CompilationContext $compilationContext
     * @return Variable
     */
    public function getTempVariable($type, CompilationContext $compilationContext)
    {
        $tempVar = $this->tempVariable++;
        $variable = $this->addVariable($type, '_' . $tempVar, $compilationContext);
        $variable->setTemporal(true);
        return $variable;
    }

    /**
     * Creates a temporary variable to be used in a write operation
     *
     * @param string $type
     * @param CompilationContext $context
     * @return Variable
     */
    public function getTempVariableForWrite($type, CompilationContext $context)
    {
        $variable = $this->reuseTempVariable($type, 'heap');
        if (is_object($variable)) {
            $variable->increaseUses();
            $variable->increaseMutates();
            if ($type == 'variable' || $type == 'string' || $type == 'array') {
                $variable->initVariant($context);
            }
            return $variable;
        }

        $tempVar = $this->tempVariable++;
        $variable = $this->addVariable($type, '_' . $tempVar, $context);
        $variable->setIsInitialized(true, $context, array());
        $variable->setTemporal(true);
        $variable->increaseUses();
        $variable->increaseMutates();
        if ($type == 'variable' || $type == 'string' || $type == 'array') {
            $variable->initVariant($context);
        }

        $this->registerTempVariable($type, 'heap', $variable);
        return $variable;
    }

    /**
     * Creates a temporary variable to be used to point to a heap variable
     * These kind of variables MUST not be tracked by the Zephir memory manager
     *
     * @param $type
     * @param CompilationContext $context
     * @param bool $initNonReferenced
     * @return Variable
     */
    public function getTempNonTrackedVariable($type, CompilationContext $context, $initNonReferenced = false)
    {
        $variable = $this->reuseTempVariable($type, 'non-tracked');
        if (is_object($variable)) {
            $variable->increaseUses();
            $variable->increaseMutates();
            if ($initNonReferenced) {
                $variable->initNonReferenced($context);
            }
            return $variable;
        }

        $tempVar = $this->tempVariable++;
        $variable = $this->addVariable($type, '_' . $tempVar, $context);
        $variable->setIsInitialized(true, $context, array());
        $variable->setTemporal(true);
        $variable->setMemoryTracked(false);
        $variable->increaseUses();
        $variable->increaseMutates();

        $this->registerTempVariable($type, 'non-tracked', $variable);

        if ($initNonReferenced) {
            $variable->initNonReferenced($context);
        }

        return $variable;
    }

    /**
     * Creates a temporary variable to be used in a read-only operation within native-array-access and property-access
     * These kind of variables MUST not be tracked by the Zephir memory manager
     *
     * @param string $type
     * @param CompilationContext $context
     * @return Variable
     */
    public function getTempNonTrackedUninitializedVariable($type, CompilationContext $context)
    {
        $variable = $this->reuseTempVariable($type, 'non-tracked-uninitialized');
        if (is_object($variable)) {
            $variable->increaseUses();
            $variable->increaseMutates();
            return $variable;
        }

        $tempVar = $this->tempVariable++;
        $variable = $this->addVariable($type, '_' . $tempVar, $context);
        $variable->setIsInitialized(true, $context, array());
        $variable->setTemporal(true);
        $variable->setMemoryTracked(false);
        $variable->increaseUses();
        $variable->increaseMutates();

        $this->registerTempVariable($type, 'non-tracked-uninitialized', $variable);

        return $variable;
    }

    /**
     * Creates a temporary variable to be used in a write operation
     * the body of the variable is freed between iterations instead of
     * request a new full zval variable
     *
     * @param string $type
     * @param CompilationContext $context
     * @return Variable
     */
    public function getTempComplexLiteralVariableForWrite($type, CompilationContext $context)
    {
        $variable = $this->reuseTempVariable($type, 'heap-literal');
        if (is_object($variable)) {
            $variable->increaseUses();
            $variable->increaseMutates();
            if ($type == 'variable' || $type == 'string' || $type == 'array') {
                $variable->initComplexLiteralVariant($context);
            }
            return $variable;
        }

        $tempVar = $this->tempVariable++;
        $variable = $this->addVariable($type, '_' . $tempVar, $context);
        $variable->setIsInitialized(true, $context, array());
        $variable->increaseUses();
        $variable->increaseMutates();
        $variable->setTemporal(true);
        if ($type == 'variable' || $type == 'string' || $type == 'array') {
            $variable->initComplexLiteralVariant($context);
        }

        $this->registerTempVariable($type, 'heap-literal', $variable);
        return $variable;
    }

    /**
     * Creates a temporary variable to be used in a write operation
     *
     * @param string $type
     * @param CompilationContext $context
     * @return Variable
     */
    public function getTempLocalVariableForWrite($type, CompilationContext $context)
    {
        $variable = $this->reuseTempVariable($type, 'stack');
        if (is_object($variable)) {
            $variable->increaseUses();
            $variable->increaseMutates();
            $variable->setLocalOnly(true);
            if ($type == 'variable' || $type == 'string' || $type == 'array') {
                $variable->initVariant($context);
            }
            return $variable;
        }

        $tempVar = $this->tempVariable++;
        $variable = $this->addVariable($type, '_' . $tempVar, $context);
        $variable->setIsInitialized(true, $context, array());
        $variable->increaseUses();
        $variable->increaseMutates();
        $variable->setLocalOnly(true);
        $variable->setTemporal(true);
        if ($type == 'variable' || $type == 'string' || $type == 'array') {
            $variable->initVariant($context);
        }

        $this->registerTempVariable($type, 'stack', $variable);
        return $variable;
    }

    /**
     * Creates a temporary variable
     *
     * @param string $type
     * @param CompilationContext $context
     * @return Variable
     */
    public function addTemp($type, CompilationContext $context)
    {
        /*$variable = $this->reuseTempVariable($type, 'heap');
        if (is_object($variable)) {
            $variable->increaseUses();
            $variable->increaseMutates();
            return $variable;
        }*/

        $tempVar = $this->tempVariable++;
        $variable = $this->addVariable($type, '_' . $tempVar, $context);
        $variable->setIsInitialized(true, $context, array());
        $variable->setTemporal(true);
        $variable->increaseUses();
        $variable->increaseMutates();

        //$this->registerTempVariable($type, 'heap', $variable);
        return $variable;
    }

    /**
     * Creates a temporary variable to be used as intermediate variable of a read operation
     * Variables are automatically tracked by the memory manager
     *
     * @param string $type
     * @param CompilationContext $context
     * @return Variable
     */
    public function getTempVariableForObserve($type, CompilationContext $context)
    {
        $variable = $this->reuseTempVariable($type, 'observe');
        if (is_object($variable)) {
            $variable->increaseUses();
            $variable->increaseMutates();
            $variable->observeVariant($context);
            return $variable;
        }

        $tempVar = $this->tempVariable++;
        $variable = $this->addVariable($type, '_' . $tempVar, $context);
        $variable->setIsInitialized(true, $context, array());
        $variable->setTemporal(true);
        $variable->increaseUses();
        $variable->increaseMutates();
        $variable->observeVariant($context);

        $this->registerTempVariable($type, 'observe', $variable);
        return $variable;
    }

    /**
     * Creates a temporary variable to be used as intermediate variable in a call operation
     * Variables are automatically tracked by the memory manager
     *
     * @param string $type
     * @param CompilationContext $context
     * @return Variable
     */
    public function getTempVariableForObserveOrNullify($type, CompilationContext $context)
    {
        $variable = $this->reuseTempVariable($type, 'observe-nullify');
        if (is_object($variable)) {
            $variable->increaseUses();
            $variable->increaseMutates();
            $variable->observeOrNullifyVariant($context);
            return $variable;
        }

        $tempVar = $this->tempVariable++;
        $variable = $this->addVariable($type, '_' . $tempVar, $context);
        $variable->setIsInitialized(true, $context, array());
        $variable->setTemporal(true);
        $variable->increaseUses();
        $variable->increaseMutates();
        $variable->observeOrNullifyVariant($context);

        $this->registerTempVariable($type, 'observe-nullify', $variable);
        return $variable;
    }

    /**
     * Returns the temporal variables declared in a given context
     *
     * @return array
     */
    public function getTemporalVariables()
    {
        return $this->tempVariables;
    }

    /**
     * Traverses temporal variables created in a specific branch
     * marking them as idle
     *
     * @param CompilationContext $compilationContext
     */
    public function markTemporalVariablesIdle(CompilationContext $compilationContext)
    {
        $branch = $compilationContext->currentBranch;
        foreach ($this->tempVariables as $location => $typeVariables) {
            foreach ($typeVariables as $type => $variables) {
                foreach ($variables as $variable) {
                    if ($branch == $variable->getBranch()) {
                        $variable->setIdle(true);
                    }
                }
            }
        }
    }

    /**
     * Returns the number of expected mutations for a variable
     *
     * @param string $variable
     * @return int
     */
    public function getExpectedMutations($variable)
    {
        if ($this->localContext) {
            return $this->localContext->getNumberOfMutations($variable);
        }
        return 0;
    }

    /**
     * Returns the last line where any kind of call was performed within the method
     * This is not necessary related to the symbol table but this information is gathered
     * by the LocalContextPass
     *
     * @return int
     */
    public function getLastCallLine()
    {
        if ($this->localContext) {
            return $this->localContext->getLastCallLine();
        }
        return 0;
    }

    /**
     * Returns the last line where an 'unset' operation was made within the current method
     * This is not necessary related to the symbol table but this information is gathered
     * by the LocalContextPass
     *
     * @return int
     */
    public function getLastUnsetLine()
    {
        if ($this->localContext) {
            return $this->localContext->getLastUnsetLine();
        }
        return 0;
    }
}
