<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir;

use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * Zephir\FunctionCall.
 *
 * Call functions. By default functions are called in the PHP userland if an optimizer
 * was not found or there is not a user-handler for it
 */
class FunctionCall extends Call
{
    /**
     * Function is called using a normal method name.
     */
    const CALL_NORMAL = 1;

    /**
     * Function is called using a dynamic variable as method name.
     */
    const CALL_DYNAMIC = 2;

    /**
     * Function is called using a dynamic string as method name.
     */
    const CALL_DYNAMIC_STRING = 3;

    protected static $optimizers = [];

    protected static $functionReflection = [];

    protected static $optimizerDirectories = [];

    /**
     * Process the ReflectionFunction for the specified function name.
     *
     * @param string $funcName
     *
     * @return \ReflectionFunction|null
     */
    public function getReflector($funcName)
    {
        /*
         * Check if the optimizer is already cached
         */
        if (!isset(self::$functionReflection[$funcName])) {
            try {
                $reflectionFunction = new \ReflectionFunction($funcName);
            } catch (\ReflectionException $e) {
                $reflectionFunction = null;
            }
            self::$functionReflection[$funcName] = $reflectionFunction;
            $this->reflection = $reflectionFunction;

            return $reflectionFunction;
        }
        $reflectionFunction = self::$functionReflection[$funcName];
        $this->reflection = $reflectionFunction;

        return $reflectionFunction;
    }

    /**
     * Checks if the function is a built-in provided by Zephir.
     *
     * @param string $functionName
     *
     * @return bool
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
     * Checks if a function exists or is a built-in Zephir function.
     *
     * @param string             $functionName
     * @param CompilationContext $context
     *
     * @return bool
     */
    public function functionExists($functionName, CompilationContext $context)
    {
        if (\function_exists($functionName)) {
            return true;
        }
        if ($this->isBuiltInFunction($functionName)) {
            return true;
        }

        $internalName = ['f__'.$functionName];
        if (isset($context->classDefinition)) {
            $lowerNamespace = strtolower($context->classDefinition->getNamespace());
            $prefix = 'f_'.str_replace('\\', '_', $lowerNamespace);

            $internalName[] = $prefix.'_'.$functionName;
        }

        foreach ($internalName as $name) {
            if (isset($context->compiler->functionDefinitions[$name])) {
                return true;
            }
        }

        return false;
    }

    /**
     * Compiles a function.
     *
     * @param Expression         $expr
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException|Exception
     *
     * @return CompiledExpression
     */
    public function compile(Expression $expr, CompilationContext $compilationContext)
    {
        $this->expression = $expr;
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
     * Appends an optimizer directory to the directory list.
     *
     * @param string $directory
     */
    public static function addOptimizerDir($directory)
    {
        self::$optimizerDirectories[] = $directory;
    }

    /**
     * This method gets the reflection of a function
     * to check if any of their parameters are passed by reference
     * Built-in functions rarely change the parameters if they aren't passed by reference.
     *
     * @param string $funcName
     * @param array  $expression
     *
     * @throws CompilerException
     *
     * @return bool
     */
    protected function isReadOnly($funcName, array $expression)
    {
        if ($this->isBuiltInFunction($funcName)) {
            return false;
        }

        /*
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
        if (!$reflector instanceof \ReflectionFunction) {
            return false;
        }

        $messageFormat = "The number of parameters passed is lesser than the number of required parameters by '%s'";

        if (isset($expression['parameters'])) {
            /**
             * Check if the number of parameters.
             */
            $numberParameters = \count($expression['parameters']);
            if ('unpack' == $funcName &&
                (0 == version_compare(PHP_VERSION, '7.1.0') ||
                    0 == version_compare(PHP_VERSION, '7.1.1'))
            ) {
                if ($numberParameters < 2) {
                    throw new CompilerException(sprintf($messageFormat, $funcName), $expression);
                }
            } else {
                if ($numberParameters < $reflector->getNumberOfRequiredParameters()) {
                    throw new CompilerException(sprintf($messageFormat, $funcName), $expression);
                }
            }
        } else {
            if ($reflector->getNumberOfRequiredParameters() > 0) {
                throw new CompilerException(sprintf($messageFormat, $funcName), $expression);
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

    /**
     * Once the function processes the parameters we should mark
     * specific parameters to be passed by reference.
     *
     * @param string             $funcName
     * @param array              $parameters
     * @param CompilationContext $compilationContext
     * @param array              $references
     * @param array              $expression
     */
    protected function markReferences(
        $funcName,
        $parameters,
        CompilationContext $compilationContext,
        &$references,
        $expression
    ) {
        if ($this->isBuiltInFunction($funcName)) {
            return;
        }

        $reflector = $this->getReflector($funcName);
        if ($reflector) {
            $numberParameters = \count($parameters);
            if ($numberParameters > 0) {
                $n = 1;
                $funcParameters = $reflector->getParameters();
                $isZendEngine3 = $compilationContext->backend->isZE3();
                foreach ($funcParameters as $parameter) {
                    if ($numberParameters >= $n) {
                        if ($parameter->isPassedByReference()) {
                            /* TODO hack, fix this better */
                            if ($isZendEngine3 && '&' == $parameters[$n - 1][0]) {
                                $parameters[$n - 1] = substr($parameters[$n - 1], 1);
                            }

                            if (!preg_match('/^[a-zA-Z0-9$\_]+$/', $parameters[$n - 1])) {
                                $compilationContext->logger->warning(
                                    'Cannot mark complex expression as reference',
                                    ['invalid-reference', $expression]
                                );
                                continue;
                            }

                            $variable = $compilationContext->symbolTable->getVariable($parameters[$n - 1]);
                            if ($variable) {
                                $variable->setDynamicTypes('undefined');
                                $referenceSymbol = $compilationContext->backend->getVariableCode($variable);
                                $compilationContext->codePrinter->output('ZEPHIR_MAKE_REF('.$referenceSymbol.');');
                                $references[] = $parameters[$n - 1];
                            }
                        }
                    }
                    ++$n;
                }
            }
        }
    }

    /**
     * Tries to find specific an specialized optimizer for function calls.
     *
     * @param string             $funcName
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $compilationContext
     *
     * @throws Exception
     *
     * @return bool|mixed
     */
    protected function optimize($funcName, array $expression, Call $call, CompilationContext $compilationContext)
    {
        $optimizer = false;

        /*
         * Check if the optimizer is already cached
         */
        if (!isset(self::$optimizers[$funcName])) {
            $camelizeFunctionName = camelize($funcName);

            /*
             * Check every optimizer directory for an optimizer
             */
            foreach (self::$optimizerDirectories as $directory) {
                $path = $directory.\DIRECTORY_SEPARATOR.$camelizeFunctionName.'Optimizer.php';
                $className = 'Zephir\Optimizers\FunctionCall\\'.$camelizeFunctionName.'Optimizer';

                if (file_exists($path)) {
                    if (!class_exists($className, false)) {
                        require_once $path;
                    }

                    if (!class_exists($className, false)) {
                        throw new Exception("Class {$className} cannot be loaded");
                    }

                    $optimizer = new $className();

                    if (!($optimizer instanceof OptimizerAbstract)) {
                        throw new Exception("Class {$className} must be instance of OptimizerAbstract");
                    }

                    break;
                }
            }

            self::$optimizers[$funcName] = $optimizer;
        } else {
            $optimizer = self::$optimizers[$funcName];
        }

        if ($optimizer) {
            return $optimizer->optimize($expression, $call, $compilationContext);
        }

        return false;
    }

    /**
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @throws Exception|\Zephir\Exception\CompilerException
     *
     * @return CompiledExpression
     */
    protected function _callNormal(array $expression, CompilationContext $compilationContext)
    {
        $funcName = strtolower($expression['name']);

        if ('array' == $funcName) {
            throw new CompilerException("Cannot use 'array' as a function call", $expression);
        }

        /**
         * Try to optimize function calls using existing optimizers.
         */
        $compiledExpr = $this->optimize($funcName, $expression, $this, $compilationContext);
        if (\is_object($compiledExpr)) {
            return $compiledExpr;
        }

        $exists = $this->functionExists($funcName, $compilationContext);

        if (!$exists) {
            $compilationContext->logger->warning(
                sprintf('Function "%s" does not exist at compile time', $funcName),
                ['nonexistent-function', $expression]
            );
        }

        /*
         * Static variables can be passed using local variables saving memory if the function is read only
         */
        if ($exists) {
            $readOnly = $this->isReadOnly($funcName, $expression);
        } else {
            $readOnly = false;
        }

        /*
         * Resolve parameters
         */
        if (isset($expression['parameters'])) {
            if ($readOnly) {
                $params = $this->getReadOnlyResolvedParams($expression['parameters'], $compilationContext, $expression);
            } else {
                $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
            }
        } else {
            $params = [];
        }

        /*
         * Some functions receive parameters as references
         * We mark those parameters temporary as references to properly pass them
         */
        $this->markReferences($funcName, $params, $compilationContext, $references, $expression);
        $codePrinter = $compilationContext->codePrinter;

        /*
         * Process the expected symbol to be returned
         */
        $this->processExpectedObservedReturn($compilationContext);

        /**
         * At this point the function will be called in the PHP userland.
         * PHP functions only return zvals so we need to validate the target variable is also a zval.
         */
        $symbolVariable = $this->getSymbolVariable();
        if ($symbolVariable) {
            if (!$symbolVariable->isVariable()) {
                throw new CompilerException(
                    'Returned values by functions can only be assigned to variant variables',
                    $expression
                );
            }

            /*
             * We don't know the exact dynamic type returned by the method call
             */
            $symbolVariable->setDynamicTypes('undefined');
            $symbol = $compilationContext->backend->getVariableCode($symbolVariable);
        }

        /*
         * Include fcall header
         */
        $compilationContext->headersManager->add('kernel/fcall');

        /*
         * Call functions must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        /**
         * Check if the function can have an inline cache.
         */
        $functionCache = $compilationContext->cacheManager->getFunctionCache();
        $cachePointer = $functionCache->get($funcName, $compilationContext, $this, $exists);

        /*
         * Add the last call status to the current symbol table
         */
        $this->addCallStatusFlag($compilationContext);

        if (!\count($params)) {
            if ($this->isExpectingReturn()) {
                if ('return_value' == $symbolVariable->getName()) {
                    $codePrinter->output(
                        'ZEPHIR_RETURN_CALL_FUNCTION("'.$funcName.'", '.$cachePointer.');'
                    );
                } else {
                    if ($this->mustInitSymbolVariable()) {
                        $symbolVariable->setMustInitNull(true);
                        $symbolVariable->trackVariant($compilationContext);
                    }
                    $codePrinter->output(
                        'ZEPHIR_CALL_FUNCTION('.$symbol.', "'.$funcName.'", '.$cachePointer.');'
                    );
                }
            } else {
                $codePrinter->output('ZEPHIR_CALL_FUNCTION(NULL, "'.$funcName.'", '.$cachePointer.');');
            }
        } else {
            if ($this->isExpectingReturn()) {
                if ('return_value' == $symbolVariable->getName()) {
                    $codePrinter->output(
                        strtr('ZEPHIR_RETURN_CALL_FUNCTION(":func", :pointer, :params);', [
                            ':func' => $funcName,
                            ':pointer' => $cachePointer,
                            ':params' => implode(', ', $params),
                        ])
                    );
                } else {
                    if ($this->mustInitSymbolVariable()) {
                        $symbolVariable->setMustInitNull(true);
                        $symbolVariable->trackVariant($compilationContext);
                    }

                    $codePrinter->output(
                        strtr('ZEPHIR_CALL_FUNCTION(:symbol, ":func", :pointer, :params);', [
                            ':symbol' => $symbol,
                            ':func' => $funcName,
                            ':pointer' => $cachePointer,
                            ':params' => implode(', ', $params),
                        ])
                    );
                }
            } else {
                $codePrinter->output(
                    strtr('ZEPHIR_CALL_FUNCTION(NULL, ":func", :pointer, :params);', [
                        ':func' => $funcName,
                        ':pointer' => $cachePointer,
                        ':params' => implode(', ', $params),
                    ])
                );
            }
        }

        /*
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $codePrinter->output('zephir_check_temp_parameter('.$checkVariable.');');
        }

        if (\is_array($references)) {
            foreach ($references as $reference) {
                $variable = $compilationContext->symbolTable->getVariable($reference, $compilationContext);
                $compilationContext->codePrinter->output(
                    'ZEPHIR_UNREF('.$compilationContext->backend->getVariableCode($variable).');'
                );
            }
        }

        $this->addCallStatusOrJump($compilationContext);

        /*
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
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     *
     * @return CompiledExpression
     */
    protected function _callDynamic(array $expression, CompilationContext $compilationContext)
    {
        $variable = $compilationContext->symbolTable->getVariableForRead(
            $expression['name'],
            $compilationContext,
            $expression
        );

        switch ($variable->getType()) {
            case 'variable':
            case 'string':
                break;

            default:
                throw new CompilerException(
                    'Variable type: '.$variable->getType().' cannot be used as dynamic caller',
                    $expression['left']
                );
        }

        /*
         * Resolve parameters
         */
        if (isset($expression['parameters'])) {
            $params = $this->getResolvedParams($expression['parameters'], $compilationContext, $expression);
        } else {
            $params = [];
        }

        $codePrinter = $compilationContext->codePrinter;

        /*
         * Process the expected symbol to be returned
         */
        $this->processExpectedObservedReturn($compilationContext);

        /**
         * At this point the function will be called in the PHP userland.
         * PHP functions only return zvals so we need to validate the target variable is also a zval.
         */
        $symbolVariable = $this->getSymbolVariable();
        if ($symbolVariable) {
            if (!$symbolVariable->isVariable()) {
                throw new CompilerException(
                    'Returned values by functions can only be assigned to variant variables',
                    $expression
                );
            }

            /*
             * We don't know the exact dynamic type returned by the method call
             */
            $symbolVariable->setDynamicTypes('undefined');
        }

        /*
         * Include fcall header
         */
        $compilationContext->headersManager->add('kernel/fcall');

        /*
         * Add the last call status to the current symbol table
         */
        $this->addCallStatusFlag($compilationContext);

        /*
         * Call functions must grown the stack
         */
        $compilationContext->symbolTable->mustGrownStack(true);

        if (!isset($expression['parameters'])) {
            if ($this->isExpectingReturn()) {
                if ('return_value' != $symbolVariable->getName()) {
                    if ($this->mustInitSymbolVariable()) {
                        $symbolVariable->setMustInitNull(true);
                        $symbolVariable->trackVariant($compilationContext);
                    }
                }
                $compilationContext->backend->callDynamicFunction($symbolVariable, $variable, $compilationContext);
            } else {
                $compilationContext->backend->callDynamicFunction(null, $variable, $compilationContext);
            }
        } else {
            if (\count($params)) {
                if ($this->isExpectingReturn()) {
                    if ('return_value' != $symbolVariable->getName()) {
                        if ($this->mustInitSymbolVariable()) {
                            $symbolVariable->setMustInitNull(true);
                            $symbolVariable->trackVariant($compilationContext);
                        }
                    }
                    $compilationContext->backend->callDynamicFunction(
                        $symbolVariable,
                        $variable,
                        $compilationContext,
                        $params
                    );
                } else {
                    $compilationContext->backend->callDynamicFunction(null, $variable, $compilationContext, $params);
                }
            } else {
                if ($this->isExpectingReturn()) {
                    if ('return_value' != $symbolVariable->getName()) {
                        if ($this->mustInitSymbolVariable()) {
                            $symbolVariable->setMustInitNull(true);
                            $symbolVariable->trackVariant($compilationContext);
                        }
                    }
                    $compilationContext->backend->callDynamicFunction($symbolVariable, $variable, $compilationContext);
                } else {
                    $compilationContext->backend->callDynamicFunction(null, $variable, $compilationContext);
                }
            }
        }

        /*
         * Temporary variables must be copied if they have more than one reference
         */
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $codePrinter->output('zephir_check_temp_parameter('.$checkVariable.');');
        }

        $this->addCallStatusOrJump($compilationContext);

        /*
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
}
