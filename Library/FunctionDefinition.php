<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team                                  |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the MIT license,       |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the MIT license and are unable      |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

namespace Zephir;

use Zephir\Passes\LocalContextPass;
use Zephir\Passes\StaticTypeInference;
use Zephir\Passes\CallGathererPass;
use Zephir\Builder\VariableBuilder;
use Zephir\Builder\LiteralBuilder;
use Zephir\Builder\ParameterBuilder;
use Zephir\Builder\StatementsBlockBuilder;
use Zephir\Builder\Statements\LetStatementBuilder;
use Zephir\Builder\Operators\UnaryOperatorBuilder;
use Zephir\Builder\Operators\BinaryOperatorBuilder;
use Zephir\Builder\Operators\TypeOfOperatorBuilder;
use Zephir\Builder\Operators\NewInstanceOperatorBuilder;
use Zephir\Builder\Statements\IfStatementBuilder;
use Zephir\Builder\Statements\ThrowStatementBuilder;
use Zephir\Statements\IfStatement;
use Zephir\Detectors\WriteDetector;

/**
 * FunctionDefinition
 *
 * Represents a function (method)
 */
class FunctionDefinition
{
    /**
     * @var string
     */
    protected $namespace;

    /**
     * @var string
     */
    protected $name;

    /**
     * @var ClassMethodParameters
     */
    protected $parameters;

    protected $statements;

    /**
     * Types returned by the method
     *
     * @var array
     */
    protected $returnTypes;

    /**
     * Class type hints returned by the method
     */
    protected $returnClassTypes;

    /**
     * Whether the variable is void
     *
     * @var boolean
     */
    protected $void = false;

    /**
     * Whether the variable is global
     * or namespaced
     *
     * @var boolean
     */
    protected $isGlobal = false;

    /**
     * @var array|null
     *
     * @var boolean
     */
    protected $expression;

    /**
     * LocalContextPass
     *
     * @var LocalContextPass
     */
    protected $localContext;

    /**
     * Static Type Inference Pass
     *
     * @var StaticTypeInferencePass
     */
    protected $typeInference;

    /**
     * Call Gatherer Pass
     *
     * @var CallGathererPass
     */
    protected $callGathererPass;

    /**
     * FunctionDefinition constructor
     *
     * @param $name
     * @param $parameters
     * @param StatementsBlock $statements
     * @param null $returnType
     * @param array $original
     */
    public function __construct($namespace, $name, $parameters, StatementsBlock $statements = null, $returnType = null, array $original = null)
    {
        $this->namespace =  $namespace;
        $this->name = $name;
        $this->parameters = $parameters;
        $this->statements = $statements;
        $this->expression = $original;

        if ($returnType['void']) {
            $this->void = true;
            return;
        }

        if (isset($returnType['list'])) {
            $types = array();
            $castTypes = array();
            foreach ($returnType['list'] as $returnTypeItem) {
                if (isset($returnTypeItem['cast'])) {
                    if (isset($returnTypeItem['cast']['collection'])) {
                        continue;
                    }
                    $castTypes[$returnTypeItem['cast']['value']] = $returnTypeItem['cast']['value'];
                } else {
                    $types[$returnTypeItem['data-type']] = $returnTypeItem;
                }
            }
            if (count($castTypes)) {
                $types['object'] = array();
                $this->returnClassTypes = $castTypes;
            }
            if (count($types)) {
                $this->returnTypes = $types;
            }
        }
    }

    /**
     * Getter for statements block
     *
     * @return StatementsBlock $statements Statements block
     */
    public function getStatementsBlock()
    {
        return $this->statements;
    }

    /**
     * Setter for statements block
     *
     * @param StatementsBlock $statementsBlock
     */
    public function setStatementsBlock(StatementsBlock $statementsBlock)
    {
        $this->statements = $statementsBlock;
    }

    public function getNamespace()
    {
        return $this->namespace;
    }

    public function setNamespace($namespace)
    {
        $this->namespace = $namespace;
    }

    /**
     * Returns the method name
     *
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }
    
    /**
     * Get the internal name used in generated C code
     */
    public function getInternalName()
    {
        return ($this->isGlobal() ? 'g_' : 'f_') . str_replace('\\', '_', $this->namespace) . '_' . $this->getName();
    }

    public function isGlobal()
    {
        return $this->isGlobal;
    }

    public function setGlobal($global)
    {
        $this->isGlobal = $global;
    }

    /**
     * Returns the class name including its namespace
     *
     * @return string
     */
    public function getCompleteName()
    {
        return $this->namespace . '\\' . $this->name;
    }

    /**
     * Returns the parameters
     *
     * @return ClassMethodParameters
     */
    public function getParameters()
    {
        return $this->parameters;
    }

    /**
     * Checks if the method has return-type or cast hints
     *
     * @return boolean
     */
    public function hasReturnTypes()
    {
        if (count($this->returnTypes)) {
            return true;
        }

        if (count($this->returnClassTypes)) {
            return true;
        }

        return false;
    }

    /**
     * Checks whether at least one return type hint is null compatible
     *
     * @param string $type
     * @return boolean
     */
    public function areReturnTypesNullCompatible($type = null)
    {
        if (count($this->returnTypes)) {
            foreach ($this->returnTypes as $returnType => $definition) {
                switch ($returnType) {
                    case 'null':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Checks whether at least one return type hint is integer compatible
     *
     * @param string $type
     * @return boolean
     */
    public function areReturnTypesIntCompatible($type = null)
    {
        if (count($this->returnTypes)) {
            foreach ($this->returnTypes as $returnType => $definition) {
                switch ($returnType) {
                    case 'int':
                    case 'uint':
                    case 'char':
                    case 'uchar':
                    case 'long':
                    case 'ulong':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Checks whether at least one return type hint is double compatible
     *
     * @param string $type
     * @return boolean
     */
    public function areReturnTypesDoubleCompatible($type = null)
    {
        if (count($this->returnTypes)) {
            foreach ($this->returnTypes as $returnType => $definition) {
                switch ($returnType) {
                    case 'double':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Checks whether at least one return type hint is integer compatible
     *
     * @param string $type
     * @return boolean
     */
    public function areReturnTypesBoolCompatible($type = null)
    {
        if (count($this->returnTypes)) {
            foreach ($this->returnTypes as $returnType => $definition) {
                switch ($returnType) {
                    case 'bool':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Checks whether at least one return type hint is integer compatible
     *
     * @param string $type
     * @return boolean
     */
    public function areReturnTypesStringCompatible($type = null)
    {
        if (count($this->returnTypes)) {
            foreach ($this->returnTypes as $returnType => $definition) {
                switch ($returnType) {
                    case 'string':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Returned type hints by the method
     *
     * @return array
     */
    public function getReturnTypes()
    {
        return $this->returnTypes;
    }

    /**
     * Returned class-type hints by the method
     *
     * @return array
     */
    public function getReturnClassTypes()
    {
        return $this->returnClassTypes;
    }

    public function isConstructor()
    {
        return false;
    }

    /**
     * Returns the number of parameters the method has
     *
     * @return boolean
     */
    public function hasParameters()
    {
        if (is_object($this->parameters)) {
            return count($this->parameters->getParameters()) > 0;
        }
        return false;
    }

    /**
     * Returns the number of parameters the method has
     *
     * @return int
     */
    public function getNumberOfParameters()
    {
        if (is_object($this->parameters)) {
            return count($this->parameters->getParameters());
        }
        return 0;
    }

    /**
     * Returns the number of required parameters the method has
     *
     * @return int
     */
    public function getNumberOfRequiredParameters()
    {
        if (is_object($this->parameters)) {
            $parameters = $this->parameters->getParameters();
            if (count($parameters)) {
                $required = 0;
                foreach ($parameters as $parameter) {
                    if (!isset($parameter['default'])) {
                        $required++;
                    }
                }
                return $required;
            }
        }
        return 0;
    }

    /**
     * Returns the number of required parameters the method has
     *
     * @return string
     */
    public function getInternalParameters()
    {
        if (is_object($this->parameters)) {
            $parameters = $this->parameters->getParameters();
            if (count($parameters)) {
                return count($parameters) . ', ...';
            }
        }
        return "";
    }

    /**
     * Checks if the method must not return any value
     *
     * @return boolean
     */
    public function isVoid()
    {
        return $this->void;
    }

    /**
     * Checks if method is a shortcut
     *
     * @return bool
     */
    public function isShortcut()
    {
        return $this->expression && $this->expression['type'] == 'shortcut';
    }

    /**
     * Return shortcut method name
     *
     * @return mixed
     */
    public function getShortcutName()
    {
        return $this->expression['name'];
    }

    /**
     * Returns the local context pass information
     *
     * @return LocalContextPass
     */
    public function getLocalContextPass()
    {
        return $this->localContext;
    }

    /**
     * Returns the static type inference pass information
     *
     * @return StaticTypeInference
     */
    public function getStaticTypeInferencePass()
    {
        return $this->typeInference;
    }

    /**
     * Returns the call gatherer pass information
     *
     * @return CallGathererPass
     */
    public function getCallGathererPass()
    {
        return $this->callGathererPass;
    }

    /**
     * Replace macros
     *
     * @param SymbolTable $symbolTable
     * @param string $containerCode
     */
    public function removeMemoryStackReferences(SymbolTable $symbolTable, $containerCode)
    {
        if (!$symbolTable->getMustGrownStack()) {
            $containerCode = str_replace('ZEPHIR_THROW_EXCEPTION_STR', 'ZEPHIR_THROW_EXCEPTION_STRW', $containerCode);
            $containerCode = str_replace('ZEPHIR_THROW_EXCEPTION_DEBUG_STR', 'ZEPHIR_THROW_EXCEPTION_DEBUG_STRW', $containerCode);
            $containerCode = str_replace('ZEPHIR_THROW_EXCEPTION_ZVAL', 'ZEPHIR_THROW_EXCEPTION_ZVALW', $containerCode);
            $containerCode = str_replace('RETURN_THIS', 'RETURN_THISW', $containerCode);
            $containerCode = str_replace('RETURN_LCTOR', 'RETURN_LCTORW', $containerCode);
            $containerCode = str_replace('RETURN_CTOR', 'RETURN_CTORW', $containerCode);
            $containerCode = str_replace('RETURN_NCTOR', 'RETURN_NCTORW', $containerCode);
            $containerCode = str_replace('RETURN_CCTOR', 'RETURN_CCTORW', $containerCode);
            $containerCode = str_replace('RETURN_MM_NULL', 'RETURN_NULL', $containerCode);
            $containerCode = str_replace('RETURN_MM_BOOL', 'RETURN_BOOL', $containerCode);
            $containerCode = str_replace('RETURN_MM_FALSE', 'RETURN_FALSE', $containerCode);
            $containerCode = str_replace('RETURN_MM_TRUE', 'RETURN_TRUE', $containerCode);
            $containerCode = str_replace('RETURN_MM_STRING', 'RETURN_STRING', $containerCode);
            $containerCode = str_replace('RETURN_MM_LONG', 'RETURN_LONG', $containerCode);
            $containerCode = str_replace('RETURN_MM_DOUBLE', 'RETURN_DOUBLE', $containerCode);
            $containerCode = str_replace('RETURN_MM_FALSE', 'RETURN_FALSE', $containerCode);
            $containerCode = str_replace('RETURN_MM_EMPTY_STRING', 'RETURN_MM_EMPTY_STRING', $containerCode);
            $containerCode = str_replace('RETURN_MM_EMPTY_ARRAY', 'RETURN_EMPTY_ARRAY', $containerCode);
            $containerCode = str_replace('RETURN_MM_MEMBER', 'RETURN_MEMBER', $containerCode);
            $containerCode = str_replace('RETURN_MM()', 'return', $containerCode);
            $containerCode = preg_replace('/[ \t]+ZEPHIR_MM_RESTORE\(\);' . PHP_EOL . '/s', '', $containerCode);
        }
        return $containerCode;
    }

    /**
     * Assigns a default value
     *
     * @param array $parameter
     * @param CompilationContext $compilationContext
     * @return string
     * @throws CompilerException
     */
    public function assignDefaultValue(array $parameter, CompilationContext $compilationContext)
    {
        if (isset($parameter['data-type'])) {
            $dataType = $parameter['data-type'];
        } else {
            $dataType = 'variable';
        }

        /**
         * Class-Hinted parameters only can be null?
         */
        if (isset($parameter['cast'])) {
            if ($parameter['default']['type'] != 'null') {
                throw new CompilerException('Class-Hinted parameters only can have "null" as default parameter', $parameter);
            }
        }

        $code = '';

        /**
         * @todo Refactoring this place, move to one - static-constant-access
         */
        switch ($dataType) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(
                         * @todo Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext, $parameter['default']);
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);


                        if ($compiledExpression->getType() != 'int') {
                            throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(int)", $parameter);
                        }

                        $parameter['default']['type'] = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        return $this->assignDefaultValue($parameter, $compilationContext);
                        break;

                    case 'null':
                        $code .= "\t\t" . $parameter['name'] . ' = 0;' . PHP_EOL;
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                        $code .= "\t\t" . $parameter['name'] . ' = ' . $parameter['default']['value'] . ';' . PHP_EOL;
                        break;

                    case 'double':
                        $code .= "\t\t" . $parameter['name'] . ' = (int) ' . $parameter['default']['value'] . ';' . PHP_EOL;
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(int)", $parameter);
                }
                break;

            case 'double':

                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(
                         * @todo Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext, $parameter['default']);
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);


                        if ($compiledExpression->getType() != 'double') {
                            throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(double)", $parameter);
                        }

                        $parameter['default']['type'] = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        return $this->assignDefaultValue($parameter, $compilationContext);
                        break;

                    case 'null':
                        $code .= "\t\t" . $parameter['name'] . ' = 0;' . PHP_EOL;
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                        $code .= "\t\t" . $parameter['name'] . ' = (double) ' . $parameter['default']['value'] . ';' . PHP_EOL;
                        break;

                    case 'double':
                        $code .= "\t\t" . $parameter['name'] . ' = ' . $parameter['default']['value'] . ';' . PHP_EOL;
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(double)", $parameter);
                }
                break;

            case 'bool':
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(
                         * @todo Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext, $parameter['default']);
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);


                        if ($compiledExpression->getType() != 'bool') {
                            throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(bool)", $parameter);
                        }

                        $parameter['default']['type'] = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        return $this->assignDefaultValue($parameter, $compilationContext);
                        break;

                    case 'null':
                        $code .= "\t\t" . $parameter['name'] . ' = 0;' . PHP_EOL;
                        break;

                    case 'bool':
                        if ($parameter['default']['value'] == 'true') {
                            $code .= "\t\t" . $parameter['name'] . ' = 1;' . PHP_EOL;
                        } else {
                            $code .= "\t\t" . $parameter['name'] . ' = 0;' . PHP_EOL;
                        }
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(bool)", $parameter);
                }
                break;

            case 'string':
                $compilationContext->symbolTable->mustGrownStack(true);
                $compilationContext->headersManager->add('kernel/memory');
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(
                         * @todo Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext, $parameter['default']);
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);


                        if ($compiledExpression->getType() != 'string') {
                            throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(string)", $parameter);
                        }

                        $parameter['default']['type'] = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        return $this->assignDefaultValue($parameter, $compilationContext);
                        break;

                    case 'null':
                        $code .= "\t\t" . 'ZEPHIR_INIT_VAR(' . $parameter['name'] . ');' . PHP_EOL;
                        $code .= "\t\t" . 'ZVAL_EMPTY_STRING(' . $parameter['name'] . ');' . PHP_EOL;
                        break;

                    case 'string':
                        $code .= "\t\t" . 'ZEPHIR_INIT_VAR(' . $parameter['name'] . ');' . PHP_EOL;
                        $code .= "\t\t" . 'ZVAL_STRING(' . $parameter['name'] . ', "' . Utils::addSlashes($parameter['default']['value'], true) . '", 1);' . PHP_EOL;
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(string)", $parameter);
                }
                break;

            case 'array':
                $compilationContext->symbolTable->mustGrownStack(true);
                $compilationContext->headersManager->add('kernel/memory');
                switch ($parameter['default']['type']) {

                    case 'null':
                        $code .= "\t" . 'ZEPHIR_INIT_VAR(' . $parameter['name'] . ');' . PHP_EOL;
                        $code .= "\t" . 'array_init(' . $parameter['name'] . ');' . PHP_EOL;
                        break;

                    case 'empty-array':
                    case 'array':
                        $code .= "\t\t" . 'ZEPHIR_INIT_VAR(' . $parameter['name'] . ');' . PHP_EOL;
                        $code .= "\t\t" . 'array_init(' . $parameter['name'] . ');' . PHP_EOL;
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(array)", $parameter);
                }
                break;

            case 'variable':
                switch ($parameter['default']['type']) {

                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(
                         * @todo Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext, $parameter['default']);
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);

                        $parameter['default']['type'] = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        return $this->assignDefaultValue($parameter, $compilationContext);
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $code .= "\t\t" . 'ZEPHIR_INIT_VAR(' . $parameter['name'] . ');' . PHP_EOL;
                        $code .= "\t\t" . 'ZVAL_LONG(' . $parameter['name'] . ', ' . $parameter['default']['value'] . ');' . PHP_EOL;
                        break;

                    case 'double':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $code .= "\t\t" . 'ZEPHIR_INIT_VAR(' . $parameter['name'] . ');' . PHP_EOL;
                        $code .= "\t\t" . 'ZVAL_DOUBLE(' . $parameter['name'] . ', ' . $parameter['default']['value'] . ');' . PHP_EOL;
                        break;

                    case 'string':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $code .= "\t\t" . 'ZEPHIR_INIT_VAR(' . $parameter['name'] . ');' . PHP_EOL;
                        $code .= "\t\t" . 'ZVAL_STRING(' . $parameter['name'] . ', "' . Utils::addSlashes($parameter['default']['value'], true) . '", 1);' . PHP_EOL;
                        break;

                    case 'bool':
                        $expectedMutations = $compilationContext->symbolTable->getExpectedMutations($parameter['name']);
                        if ($expectedMutations < 2) {
                            if ($parameter['default']['value'] == 'true') {
                                $code .= "\t\t" . $parameter['name'] . ' = ZEPHIR_GLOBAL(global_true);' . PHP_EOL;
                            } else {
                                $code .= "\t\t" . $parameter['name'] . ' = ZEPHIR_GLOBAL(global_false);' . PHP_EOL;
                            }
                        } else {
                            $compilationContext->symbolTable->mustGrownStack(true);
                            $compilationContext->headersManager->add('kernel/memory');
                            if ($parameter['default']['value'] == 'true') {
                                $code .= "\t\t" . 'ZEPHIR_CPY_WRT(' . $parameter['name'] . ', ZEPHIR_GLOBAL(global_true));' . PHP_EOL;
                            } else {
                                $code .= "\t\t" . 'ZEPHIR_CPY_WRT(' . $parameter['name'] . ', ZEPHIR_GLOBAL(global_false));' . PHP_EOL;
                            }
                        }
                        break;

                    case 'null':
                        $expectedMutations = $compilationContext->symbolTable->getExpectedMutations($parameter['name']);
                        if ($expectedMutations < 2) {
                            $code .= "\t\t" . $parameter['name'] . ' = ZEPHIR_GLOBAL(global_null);' . PHP_EOL;
                        } else {
                            $compilationContext->symbolTable->mustGrownStack(true);
                            $compilationContext->headersManager->add('kernel/memory');
                            $code .= "\t\t" . 'ZEPHIR_CPY_WRT(' . $parameter['name'] . ', ZEPHIR_GLOBAL(global_null));' . PHP_EOL;
                        }
                        break;

                    case 'empty-array':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $code .= "\t\t" . 'ZEPHIR_INIT_VAR(' . $parameter['name'] . ');' . PHP_EOL;
                        $code .= "\t\t" . 'array_init(' . $parameter['name'] . ');' . PHP_EOL;
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(variable)", $parameter);
                }
                break;

            default:
                throw new CompilerException("Default parameter type: " . $dataType, $parameter);
        }

        return $code;
    }

    /**
     * Assigns a zval value to a static low-level type
     *
     * @todo rewrite this to build ifs and throw from builders
     *
     * @param array $parameter
     * @param CompilationContext $compilationContext
     * @return string
     * @throws CompilerException
     */
    public function checkStrictType(array $parameter, CompilationContext $compilationContext)
    {
        if (isset($parameter['data-type'])) {
            $dataType = $parameter['data-type'];
        } else {
            $dataType = 'variable';
        }

        $compilationContext->headersManager->add('ext/spl/spl_exceptions');
        $compilationContext->headersManager->add('kernel/exception');

        switch ($dataType) {

            case 'int':
            case 'uint':
            case 'long':
                $code  = "\tif (unlikely(Z_TYPE_P(" . $parameter['name'] . '_param) != IS_LONG)) {' . PHP_EOL;
                $code .= "\t\t" . 'zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter \'' . $parameter['name'] . '\' must be a long/integer") TSRMLS_CC);' . PHP_EOL;
                $code .= "\t\t" . 'RETURN_MM_NULL();' . PHP_EOL;
                $code .= "\t" . '}' . PHP_EOL;
                $code .= PHP_EOL;
                $code .= "\t" . $parameter['name'] . ' = Z_LVAL_P(' . $parameter['name'] . '_param);' . PHP_EOL;
                return $code;

            case 'bool':
                $code  = "\tif (unlikely(Z_TYPE_P(" . $parameter['name'] . '_param) != IS_BOOL)) {' . PHP_EOL;
                $code .= "\t\t" . 'zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter \'' . $parameter['name'] . '\' must be a bool") TSRMLS_CC);' . PHP_EOL;
                $code .= "\t\t" . 'RETURN_MM_NULL();' . PHP_EOL;
                $code .= "\t" . '}' . PHP_EOL;
                $code .= PHP_EOL;
                $code .= "\t" . $parameter['name'] . ' = Z_BVAL_P(' . $parameter['name'] . '_param);' . PHP_EOL;
                return $code;

            case 'double':
                $code  = "\tif (unlikely(Z_TYPE_P(" . $parameter['name'] . '_param) != IS_DOUBLE)) {' . PHP_EOL;
                $code .= "\t\t" . 'zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter \'' . $parameter['name'] . '\' must be a double") TSRMLS_CC);' . PHP_EOL;
                $code .= "\t\t" . 'RETURN_MM_NULL();' . PHP_EOL;
                $code .= "\t" . '}' . PHP_EOL;
                $code .= PHP_EOL;
                $code .= "\t" . $parameter['name'] . ' = Z_DVAL_P(' . $parameter['name'] . '_param);' . PHP_EOL;
                return $code;

            case 'string':
            case 'ulong':
                $compilationContext->headersManager->add('kernel/operators');
                $compilationContext->symbolTable->mustGrownStack(true);
                $code  = "\tif (unlikely(Z_TYPE_P(" . $parameter['name'] . '_param) != IS_STRING && Z_TYPE_P(' . $parameter['name'] . '_param) != IS_NULL)) {' . PHP_EOL;
                $code .= "\t\t" . 'zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter \'' . $parameter['name'] . '\' must be a string") TSRMLS_CC);' . PHP_EOL;
                $code .= "\t\t" . 'RETURN_MM_NULL();' . PHP_EOL;
                $code .= "\t" . '}' . PHP_EOL;
                $code .= PHP_EOL;
                $code .= "\tif (likely(Z_TYPE_P(" . $parameter['name'] . '_param) == IS_STRING)) {' . PHP_EOL;
                $code .= "\t\tzephir_get_strval(" . $parameter['name'] . ', ' . $parameter['name'] . '_param);' . PHP_EOL;
                $code .= "\t" . '} else {' . PHP_EOL;
                $code .= "\t\tZEPHIR_INIT_VAR(" . $parameter['name'] . ');' . PHP_EOL;
                $code .= "\t\tZVAL_EMPTY_STRING(" . $parameter['name'] . ');' . PHP_EOL;
                $code .= "\t" . '}' . PHP_EOL;
                return $code;

            case 'array':
                /**
                 * We don't need to check array type
                 * because It's already checked with ZEND_ARG_ARRAY_INFO
                 */
                $code = "\t" . $parameter['name'] . ' = ' . $parameter['name'] . '_param;' . PHP_EOL;
                $code .= PHP_EOL;
                return $code;

            case 'object':
            case 'resource':
                $code  = "\tif (unlikely(Z_TYPE_P(" . $parameter['name'] . ') != IS_' . strtoupper($dataType) . ')) {' . PHP_EOL;
                $code .= "\t\t" . 'zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter \'' . $parameter['name'] . '\' must be an ' . $dataType . '") TSRMLS_CC);' . PHP_EOL;
                $code .= "\t\t" . 'RETURN_MM_NULL();' . PHP_EOL;
                $code .= "\t" . '}' . PHP_EOL;
                $code .= PHP_EOL;
                return $code;

            case 'callable':
                $code  = "\tif (unlikely(zephir_is_callable(" . $parameter['name'] . ' TSRMLS_CC) != 1)) {' . PHP_EOL;
                $code .= "\t\t" . 'zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter \'' . $parameter['name'] . '\' must be callable") TSRMLS_CC);' . PHP_EOL;
                $code .= "\t\t" . 'RETURN_MM_NULL();' . PHP_EOL;
                $code .= "\t" . '}' . PHP_EOL;
                $code .= PHP_EOL;
                return $code;

            default:
                throw new CompilerException("Parameter type: " . $dataType, $parameter);
        }
    }

    /**
     * Assigns a zval value to a static low-level type
     *
     * @param array $parameter
     * @param CompilationContext $compilationContext
     * @return string
     * @throws CompilerException
     */
    public function assignZvalValue(array $parameter, CompilationContext $compilationContext)
    {
        if (isset($parameter['data-type'])) {
            $dataType = $parameter['data-type'];
        } else {
            $dataType = 'variable';
        }

        $compilationContext->headersManager->add('kernel/operators');
        switch ($dataType) {

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return "\t" . $parameter['name'] . ' = zephir_get_intval(' . $parameter['name'] . '_param);' . PHP_EOL;

            case 'bool':
                return "\t" . $parameter['name'] . ' = zephir_get_boolval(' . $parameter['name'] . '_param);' . PHP_EOL;

            case 'double':
                return "\t" . $parameter['name'] . ' = zephir_get_doubleval(' . $parameter['name'] . '_param);' . PHP_EOL;

            case 'string':
                $compilationContext->symbolTable->mustGrownStack(true);
                return "\t" . 'zephir_get_strval(' . $parameter['name'] . ', ' . $parameter['name'] . '_param);' . PHP_EOL;

            case 'array':
                $compilationContext->symbolTable->mustGrownStack(true);
                return "\t" . 'zephir_get_arrval(' . $parameter['name'] . ', ' . $parameter['name'] . '_param);' . PHP_EOL;

            case 'variable':
            case 'callable':
            case 'object':
            case 'resource':
                break;

            default:
                throw new CompilerException("Parameter type: " . $dataType, $parameter);

        }
    }

    /**
     * Pre-compiles the method making compilation pass data (static inference, local-context-pass) available to other methods
     *
     * @param CompilationContext $compilationContext
     * @return null
     * @throws CompilerException
     */
    public function preCompile(CompilationContext $compilationContext)
    {
        $localContext = null;
        $typeInference = null;
        $callGathererPass = null;

        if (is_object($this->statements)) {
            /**
             * This pass checks for zval variables than can be potentially
             * used without allocating memory and track it
             * these variables are stored in the stack
             */
            if ($compilationContext->config->get('local-context-pass', 'optimizations')) {
                $localContext = new LocalContextPass();
                $localContext->pass($this->statements);
            }

            /**
             * This pass tries to infer types for dynamic variables
             * replacing them by low level variables
             */
            if ($compilationContext->config->get('static-type-inference', 'optimizations')) {
                $typeInference = new StaticTypeInference();
                $typeInference->pass($this->statements);
                if ($compilationContext->config->get('static-type-inference-second-pass', 'optimizations')) {
                    $typeInference->reduce();
                    $typeInference->pass($this->statements);
                }
            }

            /**
             * This pass counts how many times a specific
             */
            if ($compilationContext->config->get('call-gatherer-pass', 'optimizations')) {
                $callGathererPass = new CallGathererPass($compilationContext);
                $callGathererPass->pass($this->statements);
            }
        }

        $this->localContext = $localContext;
        $this->typeInference = $typeInference;
        $this->callGathererPass = $callGathererPass;
    }

    /**
     * Compiles the method
     *
     * @param CompilationContext $compilationContext
     * @return null
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        /**
         * Set the method currently being compiled
         */
        $compilationContext->currentMethod = $this;

        /**
         * Assign pre-made compilation passses
         */
        $localContext = $this->localContext;
        $typeInference = $this->typeInference;
        $callGathererPass = $this->callGathererPass;

        /**
         * Every method has its own symbol table
         */
        $symbolTable = new SymbolTable($compilationContext);
        if ($localContext) {
            $symbolTable->setLocalContext($localContext);
        }

        /**
         * Parameters has an additional extra mutation
         */
        $parameters = $this->parameters;
        if ($localContext) {
            if (is_object($parameters)) {
                foreach ($parameters->getParameters() as $parameter) {
                    $localContext->increaseMutations($parameter['name']);
                }
            }
        }

        /**
         * Initialization of parameters happens in a fictitious external branch
         */
        $branch = new Branch();
        $branch->setType(Branch::TYPE_EXTERNAL);

        /**
         * BranchManager helps to create graphs of conditional/loop/root/jump branches
         */
        $branchManager = new BranchManager();
        $branchManager->addBranch($branch);

        /**
         * Cache Manager manages function calls, method calls and class entries caches
         */
        $cacheManager = new CacheManager();
        $cacheManager->setGatherer($callGathererPass);

        $compilationContext->branchManager = $branchManager;
        $compilationContext->cacheManager  = $cacheManager;
        $compilationContext->typeInference = $typeInference;
        $compilationContext->symbolTable   = $symbolTable;

        $oldCodePrinter = $compilationContext->codePrinter;

        /**
         * Change the code printer to a single method instance
         */
        $codePrinter = new CodePrinter();
        $compilationContext->codePrinter = $codePrinter;

        /**
         * Set an empty function cache
         */
        $compilationContext->functionCache = null;

        /**
         * Reset try/catch and loop counter
         */
        $compilationContext->insideCycle = 0;
        $compilationContext->insideTryCatch = 0;

        if (is_object($parameters)) {
            /**
             * Round 1. Create variables in parameters in the symbol table
             */
            $classCastChecks = array();
            foreach ($parameters->getParameters() as $parameter) {
                /**
                 * Change dynamic variables to low level types
                 */
                if ($typeInference) {
                    if (isset($parameter['data-type'])) {
                        if ($parameter['data-type'] == 'variable') {
                            $type = $typeInference->getInferedType($parameter['name']);
                            if (is_string($type)) {
                                /* promote polymorphic parameters to low level types */
                            }
                        }
                    } else {
                        $type = $typeInference->getInferedType($parameter['name']);
                        if (is_string($type)) {
                            /* promote polymorphic parameters to low level types */
                        }
                    }
                }

                $symbolParam = null;
                if (isset($parameter['data-type'])) {
                    switch ($parameter['data-type']) {
                        case 'object':
                        case 'callable':
                        case 'resource':
                        case 'variable':
                            $symbol = $symbolTable->addVariable($parameter['data-type'], $parameter['name'], $compilationContext);
                            break;
                        default:
                            $symbol = $symbolTable->addVariable($parameter['data-type'], $parameter['name'], $compilationContext);
                            $symbolParam = $symbolTable->addVariable('variable', $parameter['name'] . '_param', $compilationContext);
                            if ($parameter['data-type'] == 'string' || $parameter['data-type'] == 'array') {
                                $symbol->setMustInitNull(true);
                            }
                            break;
                    }
                } else {
                    $symbol = $symbolTable->addVariable('variable', $parameter['name'], $compilationContext);
                }

                /**
                 * Some parameters can be read-only
                 */
                if (isset($parameter['const']) && $parameter['const']) {
                    $symbol->setReadOnly(true);
                    if (is_object($symbolParam)) {
                        $symbolParam->setReadOnly(true);
                    }
                }

                if (is_object($symbolParam)) {
                    /**
                     * Parameters are marked as 'external'
                     */
                    $symbolParam->setIsExternal(true);

                    /**
                     * Assuming they're initialized
                     */
                    $symbolParam->setIsInitialized(true, $compilationContext, $parameter);

                    /**
                     * Initialize auxiliar parameter zvals to null
                     */
                    $symbolParam->setMustInitNull(true);

                    /**
                     * Increase uses
                     */
                    $symbolParam->increaseUses();
                } else {
                    if (isset($parameter['default'])) {
                        if (isset($parameter['data-type'])) {
                            if ($parameter['data-type'] == 'variable') {
                                $symbol->setMustInitNull(true);
                            }
                        } else {
                            $symbol->setMustInitNull(true);
                        }
                    }
                }

                /**
                 * Original node where the variable was declared
                 */
                $symbol->setOriginal($parameter);

                /**
                 * Parameters are marked as 'external'
                 */
                $symbol->setIsExternal(true);

                /**
                 * Assuming they're initialized
                 */
                $symbol->setIsInitialized(true, $compilationContext, $parameter);

                /**
                 * Variables with class/type must be objects across the execution
                 */
                if (isset($parameter['cast'])) {
                    $symbol->setDynamicTypes('object');
                    $symbol->setClassTypes($compilationContext->getFullName($parameter['cast']['value']));
                    $classCastChecks[] = array($symbol, $parameter);
                } else {
                    if (isset($parameter['data-type'])) {
                        if ($parameter['data-type'] == 'variable') {
                            $symbol->setDynamicTypes('undefined');
                        }
                    } else {
                        $symbol->setDynamicTypes('undefined');
                    }
                }
            }

            $compilationContext->codePrinter->increaseLevel();

            /**
             * Checks that a class-hinted variable meets its declaration
             */
            foreach ($classCastChecks as $classCastCheck) {
                foreach ($classCastCheck[0]->getClassTypes() as $className) {
                    /**
                     * If the parameter is nullable check it must pass the 'instanceof' validation
                     */
                    if (!isset($classCastCheck[1]['default'])) {
                        $evalExpr = new UnaryOperatorBuilder(
                            'not',
                            new BinaryOperatorBuilder(
                                'instanceof',
                                new VariableBuilder($classCastCheck[0]->getName()),
                                new VariableBuilder('\\' . $className)
                            )
                        );
                    } else {
                        $evalExpr = new BinaryOperatorBuilder(
                            'and',
                            new BinaryOperatorBuilder(
                                'not-equals',
                                new TypeOfOperatorBuilder(new VariableBuilder($classCastCheck[0]->getName())),
                                new LiteralBuilder("string", "null")
                            ),
                            new UnaryOperatorBuilder(
                                'not',
                                new BinaryOperatorBuilder(
                                    'instanceof',
                                    new VariableBuilder($classCastCheck[0]->getName()),
                                    new VariableBuilder('\\' . $className)
                                )
                            )
                        );
                    }

                    $ifCheck = new IfStatementBuilder(
                        $evalExpr,
                        new StatementsBlockBuilder(array(
                            new ThrowStatementBuilder(
                                new NewInstanceOperatorBuilder('\InvalidArgumentException', array(
                                    new ParameterBuilder(
                                        new LiteralBuilder(
                                            "string",
                                            "Parameter '" . $classCastCheck[0]->getName() . "' must be an instance of '" . Utils::escapeClassName($className) . "'"
                                        )
                                    )
                                ))
                            )
                        ))
                    );

                    $ifStatement = new IfStatement($ifCheck->get());
                    $ifStatement->compile($compilationContext);
                }
            }

            $compilationContext->codePrinter->decreaseLevel();
        }

        /**
         * Compile the block of statements if any
         */
        if (is_object($this->statements)) {
            $compilationContext->staticContext = true;

            /**
             * Compile the statements block as a 'root' branch
             */
            $this->statements->compile($compilationContext, false, Branch::TYPE_ROOT);
        }

        /**
         * Initialize default values in dynamic variables
         */
        $initVarCode = "";
        foreach ($symbolTable->getVariables() as $variable) {
            /**
             * Initialize 'dynamic' variables with default values
             */
            if ($variable->getType() == 'variable') {
                if ($variable->getNumberUses() > 0) {
                    if ($variable->getName() != 'this_ptr' && $variable->getName() != 'return_value' && $variable->getName() != 'return_value_ptr') {
                        $defaultValue = $variable->getDefaultInitValue();
                        if (is_array($defaultValue)) {
                            $symbolTable->mustGrownStack(true);
                            switch ($defaultValue['type']) {

                                case 'int':
                                case 'uint':
                                case 'long':
                                    $initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
                                    $initVarCode .= "\t" . 'ZVAL_LONG(' . $variable->getName() . ', ' . $defaultValue['value'] . ');' . PHP_EOL;
                                    break;

                                case 'char':
                                case 'uchar':
                                    if (strlen($defaultValue['value']) > 2) {
                                        if (strlen($defaultValue['value']) > 10) {
                                            throw new CompilerException("Invalid char literal: '" . substr($defaultValue['value'], 0, 10) . "...'", $defaultValue);
                                        } else {
                                            throw new CompilerException("Invalid char literal: '" . $defaultValue['value'] . "'", $defaultValue);
                                        }
                                    }
                                    $initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
                                    $initVarCode .= "\t" . 'ZVAL_LONG(' . $variable->getName() . ', \'' . $defaultValue['value'] . '\');' . PHP_EOL;
                                    break;

                                case 'null':
                                    $initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
                                    $initVarCode .= "\t" . 'ZVAL_NULL(' . $variable->getName() . ');' . PHP_EOL;
                                    break;

                                case 'double':
                                    $initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
                                    $initVarCode .= "\t" . 'ZVAL_DOUBLE(' . $variable->getName() . ', ' . $defaultValue['value'] . ');' . PHP_EOL;
                                    break;

                                case 'string':
                                    $initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
                                    $initVarCode .= "\t" . 'ZVAL_STRING(' . $variable->getName() . ', "' . Utils::addSlashes($defaultValue['value'], true) . '", 1);' . PHP_EOL;
                                    break;

                                case 'array':
                                case 'empty-array':
                                    $initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
                                    $initVarCode .= "\t" . 'array_init(' . $variable->getName() . ');' . PHP_EOL;
                                    break;

                                default:
                                    throw new CompilerException('Invalid default type: ' . $defaultValue['type'] . ' for data type: ' . $variable->getType(), $variable->getOriginal());
                            }
                        }
                    }
                }
                continue;
            }

            /**
             * Initialize 'string' variables with default values
             */
            if ($variable->getType() == 'string') {
                if ($variable->getNumberUses() > 0) {
                    $defaultValue = $variable->getDefaultInitValue();
                    if (is_array($defaultValue)) {
                        $symbolTable->mustGrownStack(true);
                        switch ($defaultValue['type']) {

                            case 'string':
                                $initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
                                $initVarCode .= "\t" . 'ZVAL_STRING(' . $variable->getName() . ', "' . Utils::addSlashes($defaultValue['value'], true) . '", 1);' . PHP_EOL;
                                break;

                            case 'null':
                                $initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
                                $initVarCode .= "\t" . 'ZVAL_EMPTY_STRING(' . $variable->getName() . ');' . PHP_EOL;
                                break;

                            default:
                                throw new CompilerException('Invalid default type: ' . $defaultValue['type'] . ' for data type: ' . $variable->getType(), $variable->getOriginal());
                        }
                    }
                }
                continue;
            }

            /**
             * Initialize 'array' variables with default values
             */
            if ($variable->getType() == 'array') {
                if ($variable->getNumberUses() > 0) {
                    $defaultValue = $variable->getDefaultInitValue();
                    if (is_array($defaultValue)) {
                        $symbolTable->mustGrownStack(true);
                        switch ($defaultValue['type']) {

                            case 'null':
                                $initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
                                $initVarCode .= "\t" . 'ZVAL_NULL(' . $variable->getName() . ');' . PHP_EOL;
                                break;

                            case 'array':
                            case 'empty-array':
                                $initVarCode .= "\t" . 'ZEPHIR_INIT_VAR(' . $variable->getName() . ');' . PHP_EOL;
                                $initVarCode .= "\t" . 'array_init(' . $variable->getName() . ');' . PHP_EOL;
                                break;

                            default:
                                throw new CompilerException('Invalid default type: ' . $defaultValue['type'] . ' for data type: ' . $variable->getType(), $variable->getOriginal());
                        }
                    }
                }
            }
        }

        /**
         * Fetch parameters from vm-top
         */
        $initCode = "";
        $code = "";
        if (is_object($parameters)) {
            /**
             * Round 2. Fetch the parameters in the method
             */
            $params = array();
            $requiredParams = array();
            $optionalParams = array();
            $numberRequiredParams = 0;
            $numberOptionalParams = 0;
            foreach ($parameters->getParameters() as $parameter) {
                if (isset($parameter['data-type'])) {
                    $dataType = $parameter['data-type'];
                } else {
                    $dataType = 'variable';
                }

                switch ($dataType) {

                    case 'object':
                    case 'callable':
                    case 'resource':
                    case 'variable':
                        $params[] = '&' . $parameter['name'];
                        break;

                    default:
                        $params[] = '&' . $parameter['name'] . '_param';
                        break;
                }

                if (isset($parameter['default'])) {
                    $optionalParams[] = $parameter;
                    $numberOptionalParams++;
                } else {
                    $requiredParams[] = $parameter;
                    $numberRequiredParams++;
                }
            }

            /**
             * Pass the write detector to the method statement block to check if the parameter
             * variable is modified so as do the proper separation
             */
            $parametersToSeparate = array();
            if (is_object($this->statements)) {
                /**
                 * If local context is not available
                 */
                if (!$localContext) {
                    $writeDetector = new WriteDetector();
                }

                foreach ($parameters->getParameters() as $parameter) {
                    if (isset($parameter['data-type'])) {
                        $dataType = $parameter['data-type'];
                    } else {
                        $dataType = 'variable';
                    }

                    switch ($dataType) {
                        case 'variable':
                        case 'string':
                        case 'array':
                        case 'resource':
                        case 'object':
                        case 'callable':
                            $name = $parameter['name'];
                            if (!$localContext) {
                                if ($writeDetector->detect($name, $this->statements->getStatements())) {
                                    $parametersToSeparate[$name] = true;
                                }
                            } else {
                                if ($localContext->getNumberOfMutations($name) > 1) {
                                    $parametersToSeparate[$name] = true;
                                }
                            }
                            break;
                    }
                }
            }

            /**
             * Initialize required parameters
             */
            foreach ($requiredParams as $parameter) {
                if (isset($parameter['mandatory'])) {
                    $mandatory = $parameter['mandatory'];
                } else {
                    $mandatory = 0;
                }

                if (isset($parameter['data-type'])) {
                    $dataType = $parameter['data-type'];
                } else {
                    $dataType = 'variable';
                }

                if ($dataType != 'variable') {
                    /**
                     * Assign value from zval to low level type
                     */
                    if ($mandatory) {
                        $initCode .= $this->checkStrictType($parameter, $compilationContext);
                    } else {
                        $initCode .= $this->assignZvalValue($parameter, $compilationContext);
                    }
                }

                switch ($dataType) {
                    case 'variable':
                    case 'resource':
                    case 'object':
                    case 'callable':
                        if (isset($parametersToSeparate[$parameter['name']])) {
                            $symbolTable->mustGrownStack(true);
                            $initCode .= "\t" . "ZEPHIR_SEPARATE_PARAM(" . $parameter['name'] . ");" . PHP_EOL;
                        }
                        break;
                }
            }

            /**
             * Initialize optional parameters
             */
            foreach ($optionalParams as $parameter) {
                if (isset($parameter['mandatory'])) {
                    $mandatory = $parameter['mandatory'];
                } else {
                    $mandatory = 0;
                }

                if (isset($parameter['data-type'])) {
                    $dataType = $parameter['data-type'];
                } else {
                    $dataType = 'variable';
                }

                switch ($dataType) {

                    case 'object':
                    case 'callable':
                    case 'resource':
                    case 'variable':
                        $name = $parameter['name'];
                        break;

                    default:
                        $name = $parameter['name'] . '_param';
                        break;
                }

                /**
                 * Assign the default value according to the variable's type
                 */
                $initCode .= "\t" . 'if (!' . $name . ') {' . PHP_EOL;
                $initCode .= $this->assignDefaultValue($parameter, $compilationContext);

                if (isset($parametersToSeparate[$name]) || $dataType != 'variable') {
                    $initCode .= "\t" . '} else {' . PHP_EOL;
                    if (isset($parametersToSeparate[$name])) {
                        $initCode .= "\t\t" . "ZEPHIR_SEPARATE_PARAM(" . $name . ");" . PHP_EOL;
                    } else {
                        if ($mandatory) {
                            $initCode .= $this->checkStrictType($parameter, $compilationContext, $mandatory);
                        } else {
                            $initCode .= "\t" . $this->assignZvalValue($parameter, $compilationContext);
                        }
                    }
                }
                $initCode .= "\t" . '}' . PHP_EOL;
            }

            /**
             * Fetch the parameters to zval pointers
             */
            $codePrinter->preOutputBlankLine();
            $compilationContext->headersManager->add('kernel/memory');
            if ($symbolTable->getMustGrownStack()) {
                $code .= "\t" . 'zephir_fetch_params(1, ' . $numberRequiredParams . ', ' . $numberOptionalParams . ', ' . join(', ', $params) . ');' . PHP_EOL;
            } else {
                $code .= "\t" . 'zephir_fetch_params(0, ' . $numberRequiredParams . ', ' . $numberOptionalParams . ', ' . join(', ', $params) . ');' . PHP_EOL;
            }
            $code .= PHP_EOL;
        }

        $code .= $initCode . $initVarCode;
        $codePrinter->preOutput($code);
        
        /**
         * Fetch used superglobals
         */
        foreach ($symbolTable->getVariables() as $name => $variable) {
            if ($symbolTable->isSuperGlobal($name)) {
                $codePrinter->preOutput("\t" . 'zephir_get_global(&' . $name . ', SS("' . $name . '") TSRMLS_CC);');
            }
        }

        /**
         * Grow the stack if needed
         */
        if ($symbolTable->getMustGrownStack()) {
            $compilationContext->headersManager->add('kernel/memory');
            $codePrinter->preOutput("\t" . 'ZEPHIR_MM_GROW();');
        }

        /**
         * Check if there are unused variables
         */
        $usedVariables = array();
        foreach ($symbolTable->getVariables() as $variable) {
            if ($variable->getNumberUses() <= 0) {
                if ($variable->isExternal() == false) {
                    $compilationContext->logger->warning('Variable "' . $variable->getName() . '" declared but not used in function ' . $this->getName(), "unused-variable", $variable->getOriginal());
                    continue;
                }
                $compilationContext->logger->warning('Variable "' . $variable->getName() . '" declared but not used in function ' . $this->getName(), "unused-variable-external", $variable->getOriginal());
            }

            if ($variable->getName() != 'this_ptr' && $variable->getName() != 'return_value' && $variable->getName() != 'return_value_ptr') {
                $type = $variable->getType();
                if (!isset($usedVariables[$type])) {
                    $usedVariables[$type] = array();
                }
                $usedVariables[$type][] = $variable;
            }
        }

        /**
         * Check if there are assigned but not used variables
         * Warn whenever a variable is unused aside from its declaration.
         */
        foreach ($symbolTable->getVariables() as $variable) {
            if ($variable->isExternal() == true || $variable->isTemporal()) {
                continue;
            }

            if ($variable->getName() == 'this_ptr' || $variable->getName() == 'return_value' || $variable->getName() == 'return_value_ptr' || $variable->getName() == 'ZEPHIR_LAST_CALL_STATUS') {
                continue;
            }

            if (!$variable->isUsed()) {
                $node = $variable->getLastUsedNode();
                if (is_array($node)) {
                    $compilationContext->logger->warning('Variable "' . $variable->getName() . '" assigned but not used in ' . $this->getCompleteName() . '::' . $this->getName(), "unused-variable", $node);
                } else {
                    $compilationContext->logger->warning('Variable "' . $variable->getName() . '" assigned but not used in ' . $this->getCompleteName() . '::' . $this->getName(), "unused-variable", $variable->getOriginal());
                }
            }
        }

        if (count($usedVariables)) {
            $codePrinter->preOutputBlankLine();
        }

        /**
         * Generate the variable definition for variables used
         */
        foreach ($usedVariables as $type => $variables) {
            $pointer = null;
            switch ($type) {

                case 'int':
                    $code = 'int ';
                    break;

                case 'uint':
                    $code = 'unsigned int ';
                    break;

                case 'char':
                    $code = 'char ';
                    break;

                case 'uchar':
                    $code = 'unsigned char ';
                    break;

                case 'long':
                    $code = 'long ';
                    break;

                case 'ulong':
                    $code = 'unsigned long ';
                    break;

                case 'bool':
                    $code = 'zend_bool ';
                    break;

                case 'double':
                    $code = 'double ';
                    break;

                case 'string':
                case 'variable':
                case 'array':
                case 'null':
                    $pointer = '*';
                    $code = 'zval ';
                    break;

                case 'HashTable':
                    $pointer = '*';
                    $code = 'HashTable ';
                    break;

                case 'HashPosition':
                    $code = 'HashPosition ';
                    break;

                case 'zend_class_entry':
                    $pointer = '*';
                    $code = 'zend_class_entry ';
                    break;

                case 'zend_function':
                    $pointer = '*';
                    $code = 'zend_function ';
                    break;

                case 'zend_object_iterator':
                    $pointer = '*';
                    $code = 'zend_object_iterator ';
                    break;

                case 'zend_property_info':
                    $pointer = '*';
                    $code = 'zend_property_info ';
                    break;

                case 'zephir_fcall_cache_entry':
                    $pointer = '*';
                    $code = 'zephir_fcall_cache_entry ';
                    break;

                case 'static_zephir_fcall_cache_entry':
                    $pointer = '*';
                    $code = 'zephir_nts_static zephir_fcall_cache_entry ';
                    break;

                case 'static_zend_class_entry':
                    $pointer = '*';
                    $code = 'zephir_nts_static zend_class_entry ';
                    break;

                case 'zephir_ce_guard':
                    $code = 'zephir_nts_static zend_bool ';
                    break;

                default:
                    throw new CompilerException("Unsupported type in declare: " . $type);
            }

            $groupVariables = array();
            $defaultValues = array();

            /**
             * @var $variables Variable[]
             */
            foreach ($variables as $variable) {
                $isComplex = ($type == 'variable' || $type == 'string' || $type == 'array' || $type == 'resource' || $type == 'callable' || $type == 'object');
                if ($isComplex && $variable->mustInitNull()) {
                    if ($variable->isLocalOnly()) {
                        $groupVariables[] = $variable->getName() . ' = zval_used_for_init';
                    } else {
                        if ($variable->isDoublePointer()) {
                            $groupVariables[] = $pointer . $pointer . $variable->getName() . ' = NULL';
                        } else {
                            $groupVariables[] = $pointer . $variable->getName() . ' = NULL';
                        }
                    }
                    continue;
                }

                if ($variable->isLocalOnly()) {
                    $groupVariables[] = $variable->getName();
                    continue;
                }

                if ($variable->isDoublePointer()) {
                    if ($variable->mustInitNull()) {
                        $groupVariables[] = $pointer . $pointer . $variable->getName() . ' = NULL';
                    } else {
                        $groupVariables[] = $pointer . $pointer . $variable->getName();
                    }
                    continue;
                }

                $defaultValue = $variable->getDefaultInitValue();
                if ($defaultValue !== null) {
                    switch ($type) {

                        case 'variable':
                        case 'string':
                        case 'array':
                        case 'resource':
                        case 'callable':
                        case 'object':
                            $groupVariables[] = $pointer . $variable->getName();
                            break;

                        case 'char':
                            if (strlen($defaultValue) > 4) {
                                if (strlen($defaultValue) > 10) {
                                    throw new CompilerException("Invalid char literal: '" . substr($defaultValue, 0, 10) . "...'", $variable->getOriginal());
                                } else {
                                    throw new CompilerException("Invalid char literal: '" . $defaultValue . "'", $variable->getOriginal());
                                }
                            }
                            /* no break */

                        default:
                            $groupVariables[] = $pointer . $variable->getName() . ' = ' . $defaultValue;
                            break;
                    }

                    continue;
                }

                if ($variable->mustInitNull() && $pointer) {
                    $groupVariables[] = $pointer . $variable->getName() . ' = NULL';
                    continue;
                }

                $groupVariables[] = $pointer . $variable->getName();
            }

            $codePrinter->preOutput("\t" . $code . join(', ', $groupVariables) . ';');
        }

        /**
         * Finalize the method compilation
         */
        if (is_object($this->statements)) {
            /**
             * If the last statement is not a 'return' or 'throw' we need to
             * restore the memory stack if needed
             */
            $lastType = $this->statements->getLastStatementType();

            if ($lastType != 'return' && $lastType != 'throw' && !$this->hasChildReturnStatementType($this->statements->getLastStatement())) {
                if ($symbolTable->getMustGrownStack()) {
                    $compilationContext->headersManager->add('kernel/memory');
                    $codePrinter->output("\t" . 'ZEPHIR_MM_RESTORE();');
                }

                /**
                 * If a method has return-type hints we need to ensure the last statement is a 'return' statement
                 */
                if ($this->hasReturnTypes()) {
                    throw new CompilerException('Reached end of the method without returning a valid type specified in the return-type hints', $this->expression['return-type']);
                }
            }
        }

        /**
         * Remove macros that grow/restore the memory frame stack if it wasn't used
         */
        $code = $this->removeMemoryStackReferences($symbolTable, $codePrinter->getOutput());

        /**
         * Restore the compilation context
         */
        $oldCodePrinter->output($code);
        $compilationContext->codePrinter = $oldCodePrinter;

        $compilationContext->branchManager = null;
        $compilationContext->cacheManager = null;
        $compilationContext->typeInference = null;

        $codePrinter->clear();

        return null;
    }

    /**
     * Simple method to check if one of the paths are returning the right expected type
     *
     * @param array $statement
     * @return boolean
     */
    public function hasChildReturnStatementType($statement)
    {
        if (!isset($statement['statements']) || !is_array($statement['statements'])) {
            return false;
        }

        if ($statement['type'] == 'if') {
            $ret = false;

            $statements = $statement['statements'];
            foreach ($statements as $item) {
                $type = isset($item['type']) ? $item['type'] : null;
                if ($type == 'return' || $type == 'throw') {
                    $ret = true;
                } else {
                    $ret = $this->hasChildReturnStatementType($item);
                }
            }

            if (!$ret || !isset($statement['else_statements'])) {
                return false;
            }

            $statements = $statement['else_statements'];
            foreach ($statements as $item) {
                $type = isset($item['type']) ? $item['type'] : null;
                if ($type == 'return' || $type == 'throw') {
                    return true;
                } else {
                    return $this->hasChildReturnStatementType($item);
                }
            }
        } else {
            $statements = $statement['statements'];
            foreach ($statements as $item) {
                $type = isset($item['type']) ? $item['type'] : null;
                if ($type == 'return' || $type == 'throw') {
                    return true;
                } else {
                    return $this->hasChildReturnStatementType($item);
                }
            }
        }

        return false;
    }
}
