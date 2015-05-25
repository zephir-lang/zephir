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

use Zephir\Utils;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * FunctionCall
 *
 * Call functions. By default functions are called in the PHP userland if an optimizer
 * was not found or there is not a user-handler for it
 */
class FunctionCall extends Call
{
    /**
     * Function is called using a normal method name
     */
    const CALL_NORMAL = 1;

    /**
     * Function is called using a dynamic variable as method name
     */
    const CALL_DYNAMIC = 2;

    /**
     * Function is called using a dynamic string as method name
     */
    const CALL_DYNAMIC_STRING = 3;

    protected static $_optimizers = array();

    protected static $_functionReflection = array();

    protected static $_optimizerDirectories = array();

    private static $_functionCache = null;

    /**
     * Process the ReflectionFunction for the specified function name
     *
     * @param string $funcName
     * @return \ReflectionFunction
     */
    public function getReflector($funcName)
    {
        /**
         * Check if the optimizer is already cached
         */
        if (!isset(self::$_functionReflection[$funcName])) {
            try {
                $reflectionFunction = new \ReflectionFunction($funcName);
            } catch (\Exception $e) {
                $reflectionFunction = null;
            }
            self::$_functionReflection[$funcName] = $reflectionFunction;
            $this->_reflection = $reflectionFunction;
            return $reflectionFunction;
        }
        $reflectionFunction = self::$_functionReflection[$funcName];
        $this->_reflection = $reflectionFunction;
        return $reflectionFunction;
    }

    /**
     * This method gets the reflection of a function
     * to check if any of their parameters are passed by reference
     * Built-in functions rarely change the parameters if they aren't passed by reference
     *
     * @param string $funcName
     * @param array $expression
     * @return boolean
     */
    protected function isReadOnly($funcName, array $expression)
    {
        if ($this->isBuiltInFunction($funcName)) {
            return false;
        }

        /**
         * These functions are supposed to be read-only but they change parameters ref-count
         */
        switch ($funcName) {
            case 'min':
            case 'max':
            case 'array_fill':
            case 'array_pad':
            case 'call_user_func':
            case 'call_user_func_array':
                return false;
        }

        $reflector = $this->getReflector($funcName);
        if ($reflector) {
            if (isset($expression['parameters'])) {
                /**
                 * Check if the number of parameters
                 */
                $numberParameters = count($expression['parameters']);
                if ($numberParameters < $reflector->getNumberOfRequiredParameters()) {
                    throw new CompilerException("The number of parameters passed is lesser than the number of required parameters by '" . $funcName . "'", $expression);
                }
            } else {
                $numberParameters = 0;
                if ($reflector->getNumberOfRequiredParameters() > 0) {
                    throw new CompilerException("The number of parameters passed is lesser than the number of required parameters by '" . $funcName . "'", $expression);
                }
            }

            if ($reflector->getNumberOfParameters() > 0) {
                foreach ($reflector->getParameters() as $parameter) {
                    if ($parameter->isPassedByReference()) {
                        return false;
                    }
                }
            }
            return true;
        }

        return false;
    }

    /**
     * Once the function processes the parameters we should mark
     * specific parameters to be passed by reference
     *
     * @param string $funcName
     * @param array $parameters
     * @param CompilationContext $compilationContext
     * @param array $references
     * @param array $expression
     * @return boolean
     */
    protected function markReferences($funcName, $parameters, CompilationContext $compilationContext, &$references, $expression)
    {
        if ($this->isBuiltInFunction($funcName)) {
            return false;
        }

        $reflector = $this->getReflector($funcName);
        if ($reflector) {
            $numberParameters = count($parameters);
            if ($numberParameters > 0) {
                $n = 1;
                $funcParameters = $reflector->getParameters();
                foreach ($funcParameters as $parameter) {
                    if ($numberParameters >= $n) {
                        if ($parameter->isPassedByReference()) {
                            if (!preg_match('/^[a-zA-Z0-9\_]+$/', $parameters[$n - 1])) {
                                $compilationContext->logger->warning("Cannot mark complex expression as reference", "invalid-reference", $expression);
                                continue;
                            }

                            $variable = $compilationContext->symbolTable->getVariable($parameters[$n - 1]);
                            if ($variable) {
                                $variable->setDynamicTypes('undefined');
                                $compilationContext->codePrinter->output('Z_SET_ISREF_P(' . $parameters[$n - 1] . ');');
                                $references[] = $parameters[$n - 1] ;
                                return false;
                            }
                        }
                    }
                    $n++;
                }
            }
        }
    }

    /**
     * Tries to find specific an specialized optimizer for function calls
     *
     * @param string $funcName
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $compilationContext
     */
    protected function optimize($funcName, array $expression, Call $call, CompilationContext $compilationContext)
    {
        $optimizer = false;

        /**
         * Check if the optimizer is already cached
         */
        if (!isset(self::$_optimizers[$funcName])) {
            $camelizeFunctionName = Utils::camelize($funcName);

            /**
             * Check every optimizer directory for an optimizer
             */
            foreach (self::$_optimizerDirectories as $directory) {
                $path =  $directory . DIRECTORY_SEPARATOR . $camelizeFunctionName . 'Optimizer.php';
                if (file_exists($path)) {
                    require_once $path;

                    $className = 'Zephir\Optimizers\FunctionCall\\' . $camelizeFunctionName . 'Optimizer';
                    if (!class_exists($className, false)) {
                        throw new \Exception('Class ' . $className . ' cannot be loaded');
                    }

                    $optimizer = new $className();

                    if (!($optimizer instanceof OptimizerAbstract)) {
                        throw new \Exception('Class ' . $className . ' must be instance of OptimizerAbstract');
                    }

                    break;
                }
            }

            self::$_optimizers[$funcName] = $optimizer;
        } else {
            $optimizer = self::$_optimizers[$funcName];
        }

        if ($optimizer) {
            return $optimizer->optimize($expression, $call, $compilationContext);
        }

        return false;
    }

    /**
     * Checks if the function is a built-in provided by Zephir
     *
     * @param string $functionName
     */
    public function isBuiltInFunction($functionName)
    {
        switch ($functionName) {
            case 'memstr':
            case 'get_class_ns':
            case 'get_ns_class':
            case 'camelize':
            case 'uncamelize':
            case 'starts_with':
            case 'ends_with':
            case 'prepare_virtual_path':
            case 'create_instance':
            case 'create_instance_params':
            case 'create_symbol_table':
            case 'globals_get':
            case 'globals_set':
            case 'merge_append':
            case 'get_class_lower':
                return true;
        }
        return false;
    }

    /**
     * Checks if a function exists or is a built-in Zephir function
     *
     * @param string $functionName
     * @return boolean
     */
    public function functionExists($functionName, CompilationContext $context)
    {
        if (function_exists($functionName)) {
            return true;
        }
        if ($this->isBuiltInFunction($functionName)) {
            return true;
        }
        
        $internalName[] = 'f__'.$functionName;
        if (isset($context->classDefinition)) {
            $internalName[] = 'f_'.str_replace('\\', '_', strtolower($context->classDefinition->getNamespace())).'_'.$functionName;
        }
        foreach ($internalName as $name) {
            if (isset($context->compiler->functionDefinitions[$name])) {
                return true;
            }
        }
        return false;
    }

    /**
     * @param array $expression
     * @param CompilationContext $compilationContext
     */
    protected function _callNormal(array $expression, CompilationContext $compilationContext)
    {
        $funcName = strtolower($expression['name']);

        if ($funcName == 'array') {
            throw new CompilerException("Cannot use 'array' as a function call", $expression);
        }

        /**
         * Try to optimize function calls using existing optimizers
         */
        $compiledExpr = $this->optimize($funcName, $expression, $this, $compilationContext);
        if (is_object($compiledExpr)) {
            return $compiledExpr;
        }

        $exists = true;
        if (!$this->functionExists($funcName, $compilationContext)) {
            $compilationContext->logger->warning("Function \"$funcName\" does not exist at compile time", "nonexistent-function", $expression);
            $exists = false;
        }

        /**
         * Static variables can be passed using local variables saving memory if the function is read only
         */
        if ($exists) {
            $readOnly = $this->isReadOnly($funcName, $expression);
        } else {
            $readOnly = false;
        }

        /**
         * Resolve parameters
         */
        if (isset($expression['parameters'])) {
            if ($readOnly) {
                $params = $this->getReadOnlyResolvedParams($expression['parameters'], $compilationContext, $expression);
            } else {
                $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
            }
        } else {
            $params = array();
        }

        /**
         * Some functions receive parameters as references
         * We mark those parameters temporary as references to properly pass them
         */
        $this->markReferences($funcName, $params, $compilationContext, $references, $expression);
        $codePrinter = $compilationContext->codePrinter;

        /**
         * Process the expected symbol to be returned
         */
        $this->processExpectedObservedReturn($compilationContext);

        /**
         * At this point the function will be called in the PHP userland.
         * PHP functions only return zvals so we need to validate the target variable is also a zval
         */
        $symbolVariable = $this->getSymbolVariable();
        if ($symbolVariable) {
            if (!$symbolVariable->isVariable()) {
                throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
            }

            /**
             * We don't know the exact dynamic type returned by the method call
             */
            $symbolVariable->setDynamicTypes('undefined');
        }

        /**
         * Include fcall header
         */
        $compilationContext->headersManager->add('kernel/fcall');

        /**
         * Call functions must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        /**
         * Check if the function can have an inline cache
         */
        $functionCache = $compilationContext->cacheManager->getFunctionCache();
        $cachePointer = $functionCache->get($funcName, $compilationContext, $this, $exists);

        /**
         * Add the last call status to the current symbol table
         */
        $this->addCallStatusFlag($compilationContext);

        if (!count($params)) {
            if ($this->isExpectingReturn()) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_FUNCTION("' . $funcName . '", ' . $cachePointer . ');');
                } else {
                    if ($this->mustInitSymbolVariable()) {
                        $symbolVariable->setMustInitNull(true);
                        $symbolVariable->trackVariant($compilationContext);
                    }
                    $codePrinter->output('ZEPHIR_CALL_FUNCTION(&' . $symbolVariable->getName() . ', "' . $funcName . '", ' . $cachePointer . ');');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_FUNCTION(NULL, "' . $funcName . '", ' . $cachePointer . ');');
            }
        } else {
            if ($this->isExpectingReturn()) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_FUNCTION("' . $funcName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                } else {
                    if ($this->mustInitSymbolVariable()) {
                        $symbolVariable->setMustInitNull(true);
                        $symbolVariable->trackVariant($compilationContext);
                    }
                    $codePrinter->output('ZEPHIR_CALL_FUNCTION(&' . $symbolVariable->getName() . ', "' . $funcName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_FUNCTION(NULL, "' . $funcName . '", ' . $cachePointer . ', ' . join(', ', $params) . ');');
            }
        }

        /**
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }

        if (is_array($references)) {
            foreach ($references as $reference) {
                $compilationContext->codePrinter->output('Z_UNSET_ISREF_P(' . $reference . ');');
            }
        }

        $this->addCallStatusOrJump($compilationContext);

        /**
         * We can mark temporary variables generated as idle
         */
        foreach ($this->getTemporalVariables() as $tempVariable) {
            $tempVariable->setIdle(true);
        }

        if ($this->isExpectingReturn()) {
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        return new CompiledExpression('null', null, $expression);
    }

    /**
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     */
    protected function _callDynamic(array $expression, CompilationContext $compilationContext)
    {
        $variable = $compilationContext->symbolTable->getVariableForRead($expression['name'], $compilationContext, $expression);
        switch ($variable->getType()) {

            case 'variable':
            case 'string':
                break;

            default:
                throw new CompilerException("Variable type: " . $variable->getType() . " cannot be used as dynamic caller", $expression['left']);
        }

        /**
         * Resolve parameters
         */
        if (isset($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        } else {
            $params = array();
        }

        $codePrinter = $compilationContext->codePrinter;

        /**
         * Process the expected symbol to be returned
         */
        $this->processExpectedObservedReturn($compilationContext);

        /**
         * At this point the function will be called in the PHP userland.
         * PHP functions only return zvals so we need to validate the target variable is also a zval
         */
        $symbolVariable = $this->getSymbolVariable();
        if ($symbolVariable) {
            if (!$symbolVariable->isVariable()) {
                throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
            }

            /**
             * We don't know the exact dynamic type returned by the method call
             */
            $symbolVariable->setDynamicTypes('undefined');
        }

        /**
         * Include fcall header
         */
        $compilationContext->headersManager->add('kernel/fcall');

        /**
         * Add the last call status to the current symbol table
         */
        $this->addCallStatusFlag($compilationContext);

        /**
         * Call functions must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if (!isset($expression['parameters'])) {
            if ($this->isExpectingReturn()) {
                if ($symbolVariable->getName() == 'return_value') {
                    $codePrinter->output('ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(' . $variable->getName() . ', NULL, 0);');
                } else {
                    if ($this->mustInitSymbolVariable()) {
                        $symbolVariable->setMustInitNull(true);
                        $symbolVariable->trackVariant($compilationContext);
                    }
                    $codePrinter->output('ZEPHIR_CALL_ZVAL_FUNCTION(&' . $symbolVariable->getName() . ', ' . $variable->getName() . ', NULL, 0);');
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_ZVAL_FUNCTION(NULL, ' . $variable->getName() . ', NULL, 0);');
            }
        } else {
            if (count($params)) {
                if ($this->isExpectingReturn()) {
                    if ($symbolVariable->getName() == 'return_value') {
                        $codePrinter->output('ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(' . $variable->getName() . ', NULL, 0, ' . join(', ', $params) . ');');
                    } else {
                        if ($this->mustInitSymbolVariable()) {
                            $symbolVariable->setMustInitNull(true);
                            $symbolVariable->trackVariant($compilationContext);
                        }
                        $codePrinter->output('ZEPHIR_CALL_ZVAL_FUNCTION(&' . $symbolVariable->getName() . ', ' . $variable->getName() . ', NULL, 0, ' . join(', ', $params) . ');');
                    }
                } else {
                    $codePrinter->output('ZEPHIR_CALL_ZVAL_FUNCTION(NULL, ' . $variable->getName() . ', NULL, 0, ' . join(', ', $params) . ');');
                }
            } else {
                if ($this->isExpectingReturn()) {
                    if ($symbolVariable->getName() == 'return_value') {
                        $codePrinter->output('ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(' . $variable->getName() . ', NULL, 0);');
                    } else {
                        if ($this->mustInitSymbolVariable()) {
                            $symbolVariable->setMustInitNull(true);
                            $symbolVariable->trackVariant($compilationContext);
                        }
                        $codePrinter->output('ZEPHIR_CALL_ZVAL_FUNCTION(&' . $symbolVariable->getName() . ', ' . $variable->getName() . ', NULL, 0);');
                    }
                } else {
                    $codePrinter->output('ZEPHIR_CALL_ZVAL_FUNCTION(NULL, ' . $variable->getName() . ', NULL, 0);');
                }
            }
        }

        /**
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }

        $this->addCallStatusOrJump($compilationContext);

        /**
         * We can mark temporary variables generated as idle
         */
        foreach ($this->getTemporalVariables() as $tempVariable) {
            $tempVariable->setIdle(true);
        }

        if ($this->isExpectingReturn()) {
            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        return new CompiledExpression('null', null, $expression);
    }

    /**
     * Compiles a function
     *
     * @param Expression $expr
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(Expression $expr, CompilationContext $compilationContext)
    {
        $this->_expression = $expr;
        $expression = $expr->getExpression();

        switch ($expression['call-type']) {

            case self::CALL_NORMAL:
                return $this->_callNormal($expression, $compilationContext);

            case self::CALL_DYNAMIC:
                return $this->_callDynamic($expression, $compilationContext);
        }

        return new CompiledExpression('null', null, $expression);
    }

    /**
     * Appends an optimizer directory to the directory list
     *
     * @param string $directory
     */
    public static function addOptimizerDir($directory)
    {
        self::$_optimizerDirectories[] = $directory;
    }
}
