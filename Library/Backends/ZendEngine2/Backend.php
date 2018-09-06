<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */

namespace Zephir\Backends\ZendEngine2;

use Zephir\Utils;
use Zephir\Variable;
use Zephir\Compiler;
use Zephir\CodePrinter;
use Zephir\ClassMethod;
use Zephir\BaseBackend;
use Zephir\GlobalConstant;
use Zephir\CompiledExpression;
use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\Fcall\FcallManagerInterface;

/**
 * Zephir\Backends\ZendEngine2\Backend
 *
 * @package Zephir\Backends\ZendEngine2
 */
class Backend extends BaseBackend
{
    protected $name = 'ZendEngine2';

    /* TODO: This should not be used, temporary (until its completely refactored) */
    public function isZE3()
    {
        return false;
    }

    /**
     * {@inheritdoc}
     *
     * @return FcallManagerInterface
     */
    public function getFcallManager()
    {
        if (!$this->fcallManager) {
            $this->setFcallManager(new FcallManager());
        }

        return $this->fcallManager;
    }

    /**
     * {@inheritdoc}
     */
    public function getVariableCodePointer(Variable $variable)
    {
        if ($variable->isLocalOnly()) {
            throw new CompilerException('Double pointer of local zval requested');
        }
        return '&' . $variable->getName();
    }

    /**
     * {@inheritdoc}
     */
    public function getVariableCode(Variable $variable)
    {
        if ($variable->isLocalOnly() && !in_array($variable->getType(), array('int'))) {
            return '&' . $variable->getName();
        }
        return $variable->getName();
    }

    public function getBoolCode(Variable $variable, CompilationContext $context, $useCodePrinter = true)
    {
        $output = 'Z_BVAL_P(' . $this->getVariableCode($variable) . ')';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    /**
     * {@inheritdoc}
     */
    public function getStringsManager()
    {
        return new StringsManager();
    }

    public function getTypeDefinition($type)
    {
        $code = null;
        $pointer = null;
        switch ($type) {
            case 'int':
                $code = 'zend_long';
                break;

            case 'uint':
                $code = 'zend_ulong';
                break;

            case 'char':
                $code = 'char';
                break;

            case 'uchar':
                $code = 'unsigned char';
                break;

            case 'long':
                $code = 'long';
                break;

            case 'ulong':
                $code = 'unsigned long';
                break;

            case 'bool':
                $code = 'zend_bool';
                break;

            case 'double':
                $code = 'double';
                break;

            case 'string':
            case 'variable':
            case 'array':
            case 'null':
                $pointer = '*';
                $code = 'zval';
                break;

            case 'HashTable':
                $pointer = '*';
                $code = 'HashTable';
                break;

            case 'HashPosition':
                $code = 'HashPosition';
                break;

            case 'zend_class_entry':
                $pointer = '*';
                $code = 'zend_class_entry';
                break;

            case 'zend_function':
                $pointer = '*';
                $code = 'zend_function';
                break;

            case 'zend_object_iterator':
                $pointer = '*';
                $code = 'zend_object_iterator';
                break;

            case 'zend_property_info':
                $pointer = '*';
                $code = 'zend_property_info';
                break;

            case 'zephir_fcall_cache_entry':
                $pointer = '*';
                $code = 'zephir_fcall_cache_entry';
                break;

            case 'static_zephir_fcall_cache_entry':
                $pointer = '*';
                $code = 'zephir_fcall_cache_entry';
                break;

            case 'static_zend_class_entry':
                $pointer = '*';
                $code = 'zend_class_entry';
                break;

            case 'zephir_ce_guard':
                $code = 'zend_bool';
                break;

            default:
                throw new CompilerException("Unsupported type in declare: " . $type);
        }
        return array($pointer, $code);
    }

    /**
     * Checks the type of a variable using the ZendEngine constants
     *
     * @param Variable $variableVariable
     * @param string $operator
     * @param string $value
     * @param CompilationContext $context
     * @return string
     *
     * @throws CompilerException
     */
    public function getTypeofCondition(Variable $variableVariable, $operator, $value, CompilationContext $context)
    {
        $variableName = $this->getVariableCode($variableVariable);
        switch ($value) {
            case 'array':
                $condition = 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_ARRAY';
                break;

            case 'object':
                $condition = 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_OBJECT';
                break;

            case 'null':
                $condition = 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_NULL';
                break;

            case 'string':
                $condition = 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_STRING';
                break;

            case 'int':
            case 'long':
            case 'integer':
                $condition = 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_LONG';
                break;

            case 'double':
            case 'float':
                $condition = 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_DOUBLE';
                break;

            case 'boolean':
            case 'bool':
                $condition = 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_BOOL';
                break;

            case 'resource':
                $condition = 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_RESOURCE';
                break;

            case 'callable':
                $condition = 'zephir_is_callable(' . $variableName . ' TSRMLS_CC) ' . $operator . ' 1';
                break;

            default:
                throw new CompilerException('Unknown type: "' . $value . '" in typeof comparison', $expr['right']);
        }

        return $condition;
    }

    public function onPreInitVar($method, CompilationContext $context)
    {
        return '';
    }

    public function onPreCompile($method, CompilationContext $context)
    {
        $codePrinter = $context->codePrinter;
        /**
         * Initialize the properties within create_object, handler code
         */
        if (preg_match('/^zephir_init_properties/', $method->getName())) {
            $codePrinter->increaseLevel();
            $codePrinter->output('{');
            $codePrinter->increaseLevel();
            $codePrinter->output('zval zthis       = zval_used_for_init;');
            $codePrinter->output('zval *this_ptr   = &zthis;');
            $codePrinter->output('zend_object* obj = ecalloc(1, sizeof(zend_object));');
            $codePrinter->output('zend_object_value retval;');
            $codePrinter->outputBlankLine();
            $codePrinter->output('zend_object_std_init(obj, class_type TSRMLS_CC);');
            $codePrinter->output('object_properties_init(obj, class_type);');
            $codePrinter->output('retval.handle   = zend_objects_store_put(obj, (zend_objects_store_dtor_t)zend_objects_destroy_object, zephir_free_object_storage, NULL TSRMLS_CC);');
            $codePrinter->output('retval.handlers = zend_get_std_object_handlers();');
            $codePrinter->outputBlankLine();
            $codePrinter->output('Z_TYPE(zthis)   = IS_OBJECT;');
            $codePrinter->output('Z_OBJVAL(zthis) = retval;');
            $codePrinter->outputBlankLine();
            $codePrinter->decreaseLevel();
        }
    }

    public function onPostCompile($method, CompilationContext $context)
    {
        $codePrinter = $context->codePrinter;
        if (preg_match('/^zephir_init_properties/', $method->getName())) {
            $codePrinter->increaseLevel();
            $codePrinter->output('return retval;');
            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
            $codePrinter->decreaseLevel();
        }
    }

    public function generateInitCode(&$groupVariables, $type, $pointer, Variable $variable)
    {
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
            return;
        }

        if ($variable->isLocalOnly()) {
            $groupVariables[] = $variable->getName();
            return;
        }

        if ($variable->isDoublePointer()) {
            if ($variable->mustInitNull()) {
                $groupVariables[] = $pointer . $pointer . $variable->getName() . ' = NULL';
            } else {
                $groupVariables[] = $pointer . $pointer . $variable->getName();
            }
            return;
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

            return;
        }

        if ($variable->mustInitNull() && $pointer) {
            $groupVariables[] = $pointer . $variable->getName() . ' = NULL';
            return;
        }

        $groupVariables[] = $pointer . $variable->getName();
    }

    public function initializeVariableDefaults($variables, CompilationContext $compilationContext)
    {
        $codePrinter = new CodePrinter();
        $codePrinter->increaseLevel();
        $oldCodePrinter = $compilationContext->codePrinter;
        $compilationContext->codePrinter = $codePrinter;

        /* Initialize default values in dynamic variables */
        foreach ($variables as $variable) {
            /**
             * Initialize 'dynamic' variables with default values
             */
            if ($variable->getType() == 'variable') {
                if ($variable->getNumberUses() > 0) {
                    if ($variable->getName() != 'this_ptr' && $variable->getName() != 'return_value' && $variable->getName() != 'return_value_ptr') {
                        $defaultValue = $variable->getDefaultInitValue();
                        if (is_array($defaultValue)) {
                            $compilationContext->symbolTable->mustGrownStack(true);
                            $compilationContext->backend->initVar($variable, $compilationContext);
                            switch ($defaultValue['type']) {
                                case 'int':
                                case 'uint':
                                case 'long':
                                    $compilationContext->backend->assignLong($variable, $defaultValue['value'], $compilationContext);
                                    break;

                                case 'bool':
                                    $compilationContext->backend->assignBool($variable, $defaultValue['value'], $compilationContext);
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
                                    $compilationContext->backend->assignLong($variable, '\'' . $defaultValue['value'] . '\'', $compilationContext);
                                    break;

                                case 'null':
                                    $compilationContext->backend->assignNull($variable, $compilationContext);
                                    break;

                                case 'double':
                                    $compilationContext->backend->assignDouble($variable, $defaultValue['value'], $compilationContext);
                                    break;

                                case 'string':
                                    $compilationContext->backend->assignString($variable, Utils::addSlashes($defaultValue['value'], true), $compilationContext);
                                    break;

                                case 'array':
                                case 'empty-array':
                                    $compilationContext->backend->initArray($variable, $compilationContext, null);
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
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->backend->initVar($variable, $compilationContext);
                        switch ($defaultValue['type']) {
                            case 'string':
                                $compilationContext->backend->assignString($variable, Utils::addSlashes($defaultValue['value'], true), $compilationContext);
                                break;

                            case 'null':
                                $compilationContext->backend->assignString($variable, null, $compilationContext);
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
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->backend->initVar($variable, $compilationContext);
                        switch ($defaultValue['type']) {
                            case 'null':
                                $compilationContext->backend->assignNull($variable, $compilationContext);
                                break;

                            case 'array':
                            case 'empty-array':
                                $compilationContext->backend->initArray($variable, $compilationContext, null);
                                break;

                            default:
                                throw new CompilerException('Invalid default type: ' . $defaultValue['type'] . ' for data type: ' . $variable->getType(), $variable->getOriginal());
                        }
                    }
                }
            }
        }
        $compilationContext->codePrinter = $oldCodePrinter;
        return $codePrinter->getOutput();
    }

    public function declareVariables($method, $typeToVariables, CompilationContext $compilationContext)
    {
        $varInitCode = array();
        $additionalCode = $method ? $this->onPreInitVar($method, $compilationContext) : '';

        foreach ($typeToVariables as $type => $variables) {
            list ($pointer, $code) = $this->getTypeDefinition($type);
            $code .= ' ';
            $groupVariables = array();

            /**
             * @var $variables Variable[]
             */
            foreach ($variables as $variable) {
                $nextCode = $this->generateInitCode($groupVariables, $type, $pointer, $variable);
                if ($nextCode && $additionalCode) {
                    $additionalCode .= PHP_EOL . $nextCode;
                } else {
                    $additionalCode .= $nextCode;
                }
            }

            $varInitCode[] = $code . join(', ', $groupVariables) . ';';
        }
        /* Keep order consistent with previous zephir versions (BC-only) */
        $varInitCode = array_reverse($varInitCode);
        if ($additionalCode) {
            $varInitCode[] = $additionalCode;
        }
        return $varInitCode;
    }

    public function initVar(Variable $variable, CompilationContext $context, $useCodePrinter = true, $second = false)
    {
        $macro = !$second ? 'ZEPHIR_INIT_VAR' : 'ZEPHIR_INIT_NVAR';
        $code = $macro . '(' . $this->getVariableCode($variable) . ');';
        if ($useCodePrinter) {
            $context->codePrinter->output($code);
        }
        return $code;
    }

    /**
     * Returns the signature of an internal method
     */
    public function getInternalSignature(ClassMethod $method, CompilationContext $context)
    {
        if ($method->isInitializer() && !$method->isStatic()) {
            return 'zend_object_value ' . $method->getName() . '(zend_class_entry *class_type TSRMLS_DC)';
        }

        if ($method->isInitializer() && $method->isStatic()) {
            return 'void ' . $method->getName() . '(TSRMLS_D)';
        }

        $signatureParameters = array();
        $parameters = $method->getParameters();
        if (is_object($parameters)) {
            foreach ($parameters->getParameters() as $parameter) {
                switch ($parameter['data-type']) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'double':
                    case 'bool':
                    case 'char':
                    case 'uchar':
                    case 'string':
                    case 'array':
                        $signatureParameters[] = 'zval *' . $parameter['name'] . '_param_ext';
                        break;

                    default:
                        $signatureParameters[] = 'zval *' . $parameter['name'] . '_ext';
                        break;
                }
            }
        }

        if (count($signatureParameters)) {
            return 'void ' . $method->getInternalName() . '(int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used, ' . join(', ', $signatureParameters) . ' TSRMLS_DC)';
        }

        return 'void ' . $method->getInternalName() . '(int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used TSRMLS_DC)';
    }

    public function declareConstant($type, $name, $value, CompilationContext $context)
    {
        $ce = $context->classDefinition->getClassEntry($context);
        $dType = null;
        switch ($type) {
            case 'bool':
                $value = $value == 'false' ? '0' : 1;
                break;
            case 'long':
            case 'int':
                $dType = 'long';
                break;
            case 'double':
                break;
            case 'string':
            case 'char':
                if ($type == 'string' || $type == 'char') {
                    $value = "\"" . Utils::addSlashes($value) . "\"";
                }
                $dType = 'string';
                break;
        }
        if (!isset($dType)) {
            $dType = $type;
        }
        if ($dType == 'null') {
            $context->codePrinter->output('zend_declare_class_constant_null(' . $ce . ', SL("' . $name . '") TSRMLS_CC);');
        } else {
            $context->codePrinter->output('zend_declare_class_constant_' . $dType . '(' . $ce . ', SL("' . $name . '"), ' . $value . ' TSRMLS_CC);');
        }
    }

    /* Assign value to variable */
    protected function assignHelper($macro, $variableName, $value, CompilationContext $context, $useCodePrinter, $doCopy = null)
    {
        if ($value instanceof Variable) {
            $value = $value->getName();
        } else {
            $value = $macro == 'ZVAL_STRING' ? '"' . $value . '"' : $value;
        }

        $copyStr = '';
        if ($doCopy === true) {
            $copyStr = ', 1';
        } else if ($doCopy === false) {
            $copyStr = ', 0';
        } else if (isset($doCopy)) {
            $copyStr = ', ' . $doCopy;
        }

        $output = $macro . '(' . $variableName . ', ' . $value . $copyStr . ');';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    protected function returnHelper($macro, $value, CompilationContext $context, $useCodePrinter, $doCopy = null)
    {
        if ($value instanceof Variable) {
            $value = $value->getName();
        } else {
            $value = $macro == 'RETURN_MM_STRING' ? '"' . $value . '"' : $value;
        }

        $copyStr = '';
        if ($doCopy === true) {
            $copyStr = ', 1';
        } else if ($doCopy === false) {
            $copyStr = ', 0';
        } else if (isset($doCopy)) {
            $copyStr = ', ' . $doCopy;
        }

        $output = $macro . '(' . $value . $copyStr . ');';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function assignString(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true, $doCopy = true)
    {
        if ($value === null) {
            $output = 'ZVAL_EMPTY_STRING(' . $this->getVariableCode($variable) . ');';
            if ($useCodePrinter) {
                $context->codePrinter->output($output);
            }
            return $output;
        }
        return $this->assignHelper('ZVAL_STRING', $this->getVariableCode($variable), $value, $context, $useCodePrinter, $doCopy);
    }

    public function returnString($value, CompilationContext $context, $useCodePrinter = true, $doCopy = true)
    {
        return $this->returnHelper('RETURN_MM_STRING', $value, $context, $useCodePrinter, $doCopy);
    }

    public function assignLong(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->assignHelper('ZVAL_LONG', $this->getVariableCode($variable), $value, $context, $useCodePrinter);
    }

    public function assignDouble(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->assignHelper('ZVAL_DOUBLE', $this->getVariableCode($variable), $value, $context, $useCodePrinter);
    }

    public function assignBool(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->assignHelper('ZVAL_BOOL', $this->getVariableCode($variable), $value, $context, $useCodePrinter);
    }

    public function assignNull(Variable $variable, CompilationContext $context, $useCodePrinter = true)
    {
        $output = 'ZVAL_NULL(' . $this->getVariableCode($variable) . ');';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function assignZval(Variable $variable, $code, CompilationContext $context)
    {
        $code = $this->resolveValue($code, $context);
        $context->codePrinter->output($this->getVariableCode($variable) . ' = ' . $code . ';');
    }

    public function concatSelf(Variable $variable, Variable $itemVariable, CompilationContext $context)
    {
        $variable = $this->getVariableCodePointer($variable);
        $itemVariable = $this->getVariableCode($itemVariable);
        $context->codePrinter->output('zephir_concat_self(' . $variable . ', ' . $itemVariable . ' TSRMLS_CC);');
    }

    public function initArray(Variable $variable, CompilationContext $context, $size = null, $useCodePrinter = true)
    {
        if (!isset($size)) {
            $output = 'array_init(' . $this->getVariableCode($variable) . ');';
        } else {
            $output = 'zephir_create_array(' . $this->getVariableCode($variable) . ', ' . $size . ', 0 TSRMLS_CC);';
        }
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function createClosure(Variable $variable, $classDefinition, CompilationContext $context)
    {
        $symbol = $this->getVariableCode($variable);
        $context->codePrinter->output('zephir_create_closure_ex(' . $symbol . ', NULL, ' . $classDefinition->getClassEntry() . ', SS("__invoke") TSRMLS_CC);');
    }

    public function addArrayEntry(Variable $variable, $key, $value, CompilationContext $context, $statement = null, $useCodePrinter = true)
    {
        $type = null;
        $keyType = 'assoc';
        if (!isset($key)) {
            $keyType = 'append';
        } else if ($key instanceof CompiledExpression) {
            $typeKey = $key->getType();
            if ($typeKey == 'variable') {
                $var = $context->symbolTable->getVariableForRead($key->getCode(), $context);
                $typeKey = $var->getType();
            }
            if (in_array($typeKey, array('int', 'uint', 'long', 'ulong'))) {
                $keyType = 'index';
            }
        }

        if ($value == 'null') {
            if (!isset($key)) {
                $context->codePrinter->output('zephir_array_append(' . $this->getVariableCodePointer($variable) . ', ZEPHIR_GLOBAL(global_null), PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
                return;
            }
        }
        switch ($value->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                $type = 'long';
                break;
            case 'double':
                $type = 'double';
                break;
            case 'string':
                $type = 'stringl';
                break;
            case 'variable':
            case 'array':
                $type = 'zval';
                break;
        }
        if (!$type) {
            throw new CompilerException("Unknown type mapping: " . $value->getType());
        }

        if (isset($key)) {
            if ($key->getType() == 'variable') {
                $keyStr = $typeKey == 'string' ? 'Z_STRVAL_P(' . $this->getVariableCode($var) . '), Z_STRLEN_P(' . $this->getVariableCode($var) . ') + 1' : $this->getVariableCode($var);
            } else {
                $keyStr = $key->getType() == 'string' ? 'SS("' . $key->getCode() . '")' : $key->getCode();
            }
        }
        if ($type == 'stringl') {
            if ($value instanceof Variable) {
                $valueStr = 'Z_STRVAL_P(' . $this->getVariableCode($value) . '), Z_STRLEN_P(' . $this->getVariableCode($value) . ')';
            } else {
                $valueStr = 'SL("' . $value->getCode()  . '")';
            }
        } else if ($type == 'zval') {
            $valueStr = $this->getVariableCode($value);
        } else {
            $valueStr = $value->getCode();
        }
        $doCopy = $type == 'stringl' ? ', 1' : '';

        if ($keyType == 'assoc') {
            $output = 'add_assoc_' . $type . '_ex(' . $this->getVariableCode($variable) . ', ' . $keyStr . ', ' . $valueStr . $doCopy . ');';
        } else if ($keyType == 'append') {
            $output = 'zephir_array_append(' . $this->getVariableCodePointer($variable) . ', ' . $this->resolveValue($value, $context) . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');';
        } else {
            $output = 'add_index_' . $type . '(' . $this->getVariableCode($variable) . ', ' . $keyStr . ', '. $valueStr . $doCopy . ');';
        }

        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function updateArray(Variable $symbolVariable, $key, $value, CompilationContext $compilationContext, $flags = null)
    {
        $value = $this->resolveValue($value, $compilationContext, true);
        if (!isset($flags)) {
            $flags = 'PH_COPY';
        }

        if ($key instanceof Variable) {
            switch ($key->getType()) {
                case 'string':
                case 'variable':
                    $compilationContext->codePrinter->output('zephir_array_update_zval(' . $this->getVariableCodePointer($symbolVariable) . ', ' . $this->getVariableCode($key) . ', ' . $value . ', ' . $flags . ');');
                    break;

                case 'int':
                case 'uint':
                case 'long':
                    $compilationContext->codePrinter->output('zephir_array_update_long(' . $this->getVariableCodePointer($symbolVariable) . ', ' . $key->getName() . ', ' . $value . ', ' . $flags . ' ZEPHIR_DEBUG_PARAMS_DUMMY);');
                    break;

                default:
                    throw new CompilerException('updateArray: Found a variable with unsupported type ' . $key->getType());
            }
        } else if ($key instanceof CompiledExpression) {
            switch ($key->getType()) {
                case 'string':
                    $compilationContext->codePrinter->output('zephir_array_update_string(' . $this->getVariableCodePointer($symbolVariable) . ', SL("' . $key->getCode() . '"), ' . $value . ', ' . $flags . ');');
                    break;

                case 'int':
                    $compilationContext->codePrinter->output('zephir_array_update_long(' . $this->getVariableCodePointer($symbolVariable) . ', ' . $key->getCode() . ', ' . $value . ', ' . $flags . ' ZEPHIR_DEBUG_PARAMS_DUMMY);');
                    break;

                case 'variable':
                    $this->updateArray($symbolVariable, $compilationContext->symbolTable->getVariableForRead($key->getCode()), $value, $compilationContext, $flags);
                    break;

                default:
                    throw new CompilerException('updateArray: Found an expression with unsupported type ' . $key->getType());
            }
        } else {
            throw new CompilerException("?");
        }
    }

    public function initObject(Variable $variable, $ce, CompilationContext $context, $useCodePrinter = true)
    {
        $variableAccess = $this->getVariableCode($variable);
        if (!isset($ce)) {
            $output = 'object_init(' . $variableAccess . ');';
        } else {
            $output = 'object_init_ex(' . $variableAccess . ', ' . $ce . ');';
        }

        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $output;
    }

    public function arrayFetch(Variable $var, Variable $src, $index, $flags, $arrayAccess, CompilationContext $context, $useCodePrinter = true)
    {
        $context->headersManager->add('kernel/array');
        $isVariable = $index instanceof Variable;

        switch ($index->getType()) {
            case 'int':
            case 'uint':
            case 'long':
                $type = 'long';
                break;

            /* Types which map to the same */
            case 'variable':
            case 'string':
                $type = $index->getType();
                break;

            default:
                throw new CompilerException("Variable type: " . $index->getType() . " cannot be used as array index without cast", $arrayAccess['right']);
        }
        if ($isVariable && in_array($index->getType(), array('variable', 'string'))) {
            $output = 'zephir_array_fetch(' . $this->getVariableCodePointer($var) . ', ' . $this->getVariableCode($src) . ', ' . $this->getVariableCode($index) . ', ' . $flags . ', "' . Compiler::getShortUserPath($arrayAccess['file']) . '", ' . $arrayAccess['line'] . ' TSRMLS_CC);';
        } else {
            if ($isVariable) {
                $indexAccess =  $this->getVariableCode($index);
            } else {
                $indexAccess = $index->getCode();
                if ($type == 'string') {
                    $indexAccess = 'SL("' . $indexAccess . '")';
                }
            }
            $output = 'zephir_array_fetch_' . $type . '(' . $this->getVariableCodePointer($var) . ', ' . $this->getVariableCode($src) . ', ' . $indexAccess . ', ' . $flags . ', "' . Compiler::getShortUserPath($arrayAccess['file']) . '", ' . $arrayAccess['line'] . ' TSRMLS_CC);';
        }

        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $output;
    }

    public function arrayIsset(Variable $var, $resolvedExpr, $expression, CompilationContext $context)
    {
        if (!($resolvedExpr instanceof Variable)) {
            if ($resolvedExpr->getType() == 'string') {
                return new CompiledExpression('bool', 'zephir_array_isset_string(' . $this->getVariableCode($var) . ', SS("' . $resolvedExpr->getCode() . '"))', $expression);
            } else {
                return new CompiledExpression('bool', 'zephir_array_isset_long(' . $this->getVariableCode($var) . ', ' . $resolvedExpr->getCode() . ')', $expression);
            }
        }

        if ($resolvedExpr->getType() == 'int' || $resolvedExpr->getType() == 'long') {
            return new CompiledExpression('bool', 'zephir_array_isset_long(' . $this->getVariableCode($var) . ', ' . $this->getVariableCode($resolvedExpr) . ')', $expression);
        } else if ($resolvedExpr->getType() == 'variable' || $resolvedExpr->getType() == 'string') {
            return new CompiledExpression('bool', 'zephir_array_isset(' . $this->getVariableCode($var) . ', ' . $this->getVariableCode($resolvedExpr) . ')', $expression);
        }

        throw new CompilerException('[' . $resolvedExpr->getType() . ']', $expression);
    }

    public function arrayIssetFetch(Variable $target, Variable $var, $resolvedExpr, $flags, $expression, CompilationContext $context)
    {
        $code = $this->getVariableCodePointer($target) . ', ' . $this->getVariableCode($var);

        if (!($resolvedExpr instanceof Variable)) {
            if ($resolvedExpr->getType() == 'string') {
                return new CompiledExpression('bool', 'zephir_array_isset_string_fetch(' . $code . ', SS("' . $resolvedExpr->getCode() . '"), '. $flags . ' TSRMLS_CC)', $expression);
            } else if (in_array($resolvedExpr->getType(), array('int', 'uint', 'long'))) {
                return new CompiledExpression('bool', 'zephir_array_isset_long_fetch(' . $code . ', ' . $resolvedExpr->getCode() . ', ' . $flags . ' TSRMLS_CC)', $expression);
            } else {
                $resolvedExpr = $context->symbolTable->getVariableForRead($resolvedExpr->getCode(), $context);
            }
        }

        if ($resolvedExpr->getType() == 'int' || $resolvedExpr->getType() == 'long') {
            return new CompiledExpression('bool', 'zephir_array_isset_long_fetch(' . $code . ', ' . $this->getVariableCode($resolvedExpr) . ', ' . $flags . ' TSRMLS_CC)', $expression);
        } else if ($resolvedExpr->getType() == 'variable' || $resolvedExpr->getType() == 'string') {
            return new CompiledExpression('bool', 'zephir_array_isset_fetch(' . $code . ', ' . $this->getVariableCode($resolvedExpr) . ', ' . $flags . ' TSRMLS_CC)', $expression);
        }
        throw new CompilerException('arrayIssetFetch [' . $resolvedExpr->getType() . ']', $expression);
    }

    public function propertyIsset(Variable $var, $key, CompilationContext $context)
    {
        return new CompiledExpression('bool', 'zephir_isset_property(' . $this->getVariableCode($var) . ', SS("' . $key . '") TSRMLS_CC)', null);
    }

    public function arrayUnset(Variable $variable, $exprIndex, $flags, CompilationContext $context)
    {
        $context->headersManager->add('kernel/array');
        $variableCode = $this->getVariableCodePointer($variable);
        switch ($exprIndex->getType()) {
            case 'int':
            case 'uint':
            case 'long':
                $context->codePrinter->output('zephir_array_unset_long(' . $variableCode . ', ' . $exprIndex->getCode() . ', ' . $flags . ');');
                break;

            case 'string':
                $context->codePrinter->output('zephir_array_unset_string(' . $variableCode . ', SS("' . $exprIndex->getCode() . '"), ' . $flags . ');');
                break;

            case 'variable':
                $variableIndex = $context->symbolTable->getVariableForRead($exprIndex->getCode(), $context, $exprIndex->getOriginal());
                $indexCode = $this->getVariableCode($variableIndex);
                switch ($variableIndex->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                        $context->codePrinter->output('zephir_array_unset_long(' . $variableCode . ', ' . $indexCode . ', ' . $flags . ');');
                        break;

                    case 'string':
                    case 'variable':
                        $context->codePrinter->output('zephir_array_unset(' . $variableCode . ', ' . $indexCode . ', ' . $flags . ');');
                        break;

                    default:
                        throw new CompilerException("Variable type: " . $variableIndex->getType() . " cannot be used as array index without cast", $expression['right']);
                }
                break;

            default:
                throw new CompilerException("Cannot use expression: " . $exprIndex->getType() . " as array index without cast", $expression['right']);
        }
    }

    private function resolveOffsetExprs($offsetExprs, $compilationContext)
    {
        $keys = '';
        $offsetItems = array();
        $numberParams = 0;

        foreach ($offsetExprs as $offsetExpr) {
            if ($offsetExpr == 'a') {
                $keys .= 'a';
                $numberParams++;
                continue;
            }
            switch ($offsetExpr->getType()) {
                case 'int':
                case 'uint':
                case 'long':
                case 'ulong':
                    $keys .= 'l';
                    $offsetItems[] = $offsetExpr->getCode();
                    $numberParams++;
                    break;

                case 'string':
                    $keys .= 's';
                    $offsetItems[] = 'SL("' . $offsetExpr->getCode() . '")';
                    $numberParams += 2;
                    break;

                case 'variable':
                    $variableIndex = $compilationContext->symbolTable->getVariableForRead($offsetExpr->getCode(), $compilationContext, null);
                    switch ($variableIndex->getType()) {
                        case 'int':
                        case 'uint':
                        case 'long':
                        case 'ulong':
                            $keys .= 'l';
                            $offsetItems[] = $this->getVariableCode($variableIndex);
                            $numberParams++;
                            break;
                        case 'string':
                        case 'variable':
                            $keys .= 'z';
                            $offsetItems[] = $this->getVariableCode($variableIndex);
                            $numberParams++;
                            break;
                        default:
                            throw new CompilerException("Variable: " . $variableIndex->getType() . " cannot be used as array index", $statement);
                    }
                    break;

                default:
                    throw new CompilerException("Value: " . $offsetExpr->getType() . " cannot be used as array index", $statement);
            }
        }
        return array($keys, $offsetItems, $numberParams);
    }

    public function assignArrayMulti(Variable $variable, $symbolVariable, $offsetExprs, CompilationContext $compilationContext)
    {
        list ($keys, $offsetItems, $numberParams) = $this->resolveOffsetExprs($offsetExprs, $compilationContext);

        $symbol = $this->resolveValue($symbolVariable, $compilationContext, true);
        $varCode = $this->getVariableCodePointer($variable);
        $compilationContext->codePrinter->output('zephir_array_update_multi(' . $varCode . ', ' . $symbol . ' TSRMLS_CC, SL("' . $keys . '"), ' . $numberParams . ', ' . join(', ', $offsetItems) . ');');
    }

    public function assignPropertyArrayMulti(Variable $variable, $valueVariable, $propertyName, $offsetExprs, CompilationContext $compilationContext)
    {
        list ($keys, $offsetItems, $numberParams) = $this->resolveOffsetExprs($offsetExprs, $compilationContext);
        $valueVariable = $this->resolveValue($valueVariable, $compilationContext, true);

        $compilationContext->codePrinter->output('zephir_update_property_array_multi(' . $variable->getName() . ', SL("' . $propertyName . '"), ' . $valueVariable . ' TSRMLS_CC, SL("' . $keys . '"), ' . $numberParams . ', ' . join(', ', $offsetItems) . ');');
    }

    public function assignStaticPropertyArrayMulti($classEntry, $valueVariable, $propertyName, $offsetExprs, CompilationContext $compilationContext)
    {
        list ($keys, $offsetItems, $numberParams) = $this->resolveOffsetExprs($offsetExprs, $compilationContext);
        $valueVariable = $this->resolveValue($valueVariable, $compilationContext, true);

        $offsetStr = $offsetItems ? ', ' . join(', ', $offsetItems) : '';
        $compilationContext->codePrinter->output('zephir_update_static_property_array_multi_ce(' . $classEntry .', SL("' . $propertyName . '"), ' . $valueVariable . ' TSRMLS_CC, SL("' . $keys . '"), ' . $numberParams . $offsetStr . ');');
    }

    public function fetchGlobal(Variable $globalVar, CompilationContext $compilationContext, $useCodePrinter = true)
    {
        $name = $globalVar->getName();
        $output = 'zephir_get_global(&' . $name . ', SS("' . $name . '") TSRMLS_CC);';
        $compilationContext->symbolTable->mustGrownStack(true);
        if ($useCodePrinter) {
            $compilationContext->codePrinter->output($output);
        }
        return $output;
    }

    public function fetchClass(Variable $zendClassEntry, $className, $guarded, CompilationContext $context)
    {
        if ($guarded) {
            $context->codePrinter->output('if (!' . $zendClassEntry->getName() . ') {');
        }
        $context->codePrinter->output("\t" . $zendClassEntry->getName() . ' = zend_fetch_class(' . $className . ', ZEND_FETCH_CLASS_AUTO TSRMLS_CC);');
        if ($guarded) {
            $context->codePrinter->output('}');
        }
    }

    public function fetchProperty(Variable $symbolVariable, Variable $variableVariable, $property, $readOnly, CompilationContext $context, $useOptimized = false)
    {
        if ($useOptimized) {
            if ($readOnly) {
                $context->codePrinter->output($symbolVariable->getName() . ' = zephir_fetch_nproperty_this(' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');
            } else {
                $context->codePrinter->output('zephir_read_property_this(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');
            }
        } else {
            if ($property instanceof Variable) {
                $context->codePrinter->output('zephir_read_property_zval(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', ' . $this->getVariableCode($property) . ', PH_NOISY_CC);');
            } else {
                $context->codePrinter->output('zephir_read_property(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');
            }
        }
    }

    public function fetchStaticProperty(Variable $symbolVariable, $classDefinition, $property, $readOnly, CompilationContext $context)
    {
        if ($readOnly) {
            $context->codePrinter->output($symbolVariable->getName() . ' = zephir_fetch_static_property_ce(' . $classDefinition->getClassEntry() . ', SL("' . $property . '") TSRMLS_CC);');
        } else {
            $context->codePrinter->output('zephir_read_static_property_ce(&' . $symbolVariable->getName() . ', ' . $classDefinition->getClassEntry() . ', SL("' . $property . '") TSRMLS_CC);');
        }
    }

    public function resolveValue($value, CompilationContext $context, $usePointer = false)
    {
        if ($value == 'null') {
            $value = ($usePointer ? '&' : '') . 'ZEPHIR_GLOBAL(global_null)';
        } else if ($value == 'true') {
            $value = ($usePointer ? '&' : '') . 'ZEPHIR_GLOBAL(global_true)';
        } else if ($value == 'false') {
            $value = ($usePointer ? '&' : '') . 'ZEPHIR_GLOBAL(global_false)';
        } else if ($value instanceof GlobalConstant) {
            return ($usePointer ? '&' : '') . $value->getName();
        } else if ($value instanceof CompiledExpression) {
            if ($value->getType() == 'array') {
                $var = $context->symbolTable->getVariableForWrite($value->getCode(), $context, null);
                $value = $usePointer ? $this->getVariableCodePointer($var) : $this->getVariableCode($var);
            } else if ($value->getType() == 'variable') {
                $value = $context->symbolTable->getVariableForWrite($value->getCode(), $context);
            } else {
                return $value->getCode();
            }
        }
        if ($value instanceof Variable) {
            $value = $usePointer ? $this->getVariableCodePointer($value) : $this->getVariableCode($value);
        }
        return $value;
    }

    public function updateProperty(Variable $symbolVariable, $propertyName, $value, CompilationContext $context)
    {
        $value = $this->resolveValue($value, $context);
        if ($symbolVariable->getName() == 'this_ptr') {
            if ($propertyName instanceof Variable) {
                $context->codePrinter->output('zephir_update_property_zval_zval(getThis(), ' . $this->getVariableCode($propertyName) . ', ' . $value . ' TSRMLS_CC);');
            } else {
                $context->codePrinter->output('zephir_update_property_this(getThis(), SL("' . $propertyName . '"), ' . $value . ' TSRMLS_CC);');
            }
        } else {
            if ($propertyName instanceof Variable) {
                $context->codePrinter->output('zephir_update_property_zval_zval(' . $this->getVariableCode($symbolVariable) . ', ' . $this->getVariableCode($propertyName) . ', ' . $value . ' TSRMLS_CC);');
            } else {
                $context->codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $value . ' TSRMLS_CC);');
            }
        }
    }

    public function updateStaticProperty($classEntry, $property, $value, CompilationContext $context)
    {
        $value = $this->resolveValue($value, $context);
        $context->codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), &' . $value . ' TSRMLS_CC);');
    }

    public function assignArrayProperty(Variable $variable, $property, $key, $value, CompilationContext $context)
    {
        $value = $this->resolveValue($value, $context);
        if (isset($key)) {
            $context->codePrinter->output('zephir_update_property_array(' . $this->getVariableCode($variable) . ', SL("' . $property . '"), ' . $this->getVariableCode($key) . ', ' . $value . ' TSRMLS_CC);');
        } else {
            $context->codePrinter->output('zephir_update_property_array_append(' . $this->getVariableCode($variable) . ', SL("' . $property . '"), ' . $value . ' TSRMLS_CC);');
        }
    }

    public function checkConstructor(Variable $var, CompilationContext $context)
    {
        $context->codePrinter->output('if (zephir_has_constructor(' . $this->getVariableCode($var) . ' TSRMLS_CC)) {');
    }

    public function callMethod($symbolVariable, Variable $variable, $methodName, $cachePointer, $params, CompilationContext $context)
    {
        $paramStr = $params != null ? ', ' . join(', ', $params) : '';
        $macro = 'CALL_METHOD';
        if ($methodName instanceof Variable) {
            $macro = 'CALL_METHOD_ZVAL';
            $methodName = $this->getVariableCode($methodName);
        } else {
            $methodName = '"' . $methodName . '"';
        }
        if (!isset($symbolVariable)) {
            $context->codePrinter->output('ZEPHIR_' . $macro . '(NULL, ' . $variable->getName() . ', ' . $methodName . ', ' . $cachePointer . $paramStr . ');');
        } else if ($symbolVariable->getName() == 'return_value') {
            $context->codePrinter->output('ZEPHIR_RETURN_' . $macro . '(' . $variable->getName() . ', ' . $methodName . ', ' . $cachePointer . $paramStr . ');');
        } else {
            $context->codePrinter->output('ZEPHIR_' . $macro . '(&' . $symbolVariable->getName() . ', ' . $variable->getName() . ', ' . $methodName . ', ' . $cachePointer . $paramStr . ');');
        }
    }

    public function callDynamicFunction($symbolVariable, Variable $variable, CompilationContext $context, $params = array(), $cache = 'NULL', $cacheSlot = 0)
    {
        $paramStr = $params != null ? ', ' . join(', ', $params) : '';
        if (!isset($symbolVariable)) {
            $context->codePrinter->output('ZEPHIR_CALL_ZVAL_FUNCTION(NULL, ' . $this->getVariableCode($variable) . ', ' . $cache . ', ' . $cacheSlot . $paramStr . ');');
        } else if ($symbolVariable->getName() == 'return_value') {
            $context->codePrinter->output('ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(' . $this->getVariableCode($variable) . ', ' . $cache . ', ' . $cacheSlot . $paramStr . ');');
        } else {
            $context->codePrinter->output('ZEPHIR_CALL_ZVAL_FUNCTION(' . $this->getVariableCodePointer($symbolVariable) . ', ' . $this->getVariableCode($variable) . ', ' . $cache . ', ' . $cacheSlot . $paramStr . ');');
        }
    }

    public function zvalOperator($zvalOperator, Variable $expected, Variable $variableLeft, Variable $variableRight, CompilationContext $compilationContext)
    {
        $expected = $this->getVariableCode($expected);
        $op1 = $this->getVariableCode($variableLeft);
        $op2 = $this->getVariableCode($variableRight);

        $params = ' TSRMLS_CC';
        if ($zvalOperator == 'zephir_add_function' || $zvalOperator == 'zephir_sub_function') {
            $params = '';
        }
        $compilationContext->codePrinter->output($zvalOperator . '(' . $expected . ', ' . $op1 . ', ' . $op2 . $params . ');');
    }

    public function maybeSeparate(Variable $variableTempSeparated, Variable $variable, CompilationContext $context)
    {
        $context->codePrinter->output($variableTempSeparated->getName().' = zephir_maybe_separate_zval(' . $this->getVariableCodePointer($variable) . ');');
    }

    public function setSymbolIfSeparated(Variable $variableTempSeparated, Variable $variable, CompilationContext $context)
    {
        $context->codePrinter->output('if (' . $variableTempSeparated->getName() . ') {');
        $context->codePrinter->output("\t" . 'ZEND_SET_SYMBOL(&EG(symbol_table), "' . $variable->getName() . '", ' . $variable->getName() . ');');
        $context->codePrinter->output('}');
    }

    public function copyOnWrite(Variable $target, $var, CompilationContext $context)
    {
        $context->codePrinter->output('ZEPHIR_CPY_WRT(' . $this->getVariableCode($target) . ', ' . $this->resolveValue($var, $context) . ');');
    }

    public function forStatement(Variable $exprVariable, $keyVariable, $variable, $duplicateKey, $duplicateHash, $statement, $statementBlock, CompilationContext $compilationContext)
    {
        /**
         * Create a hash table and hash pointer temporary variables
         */
        $arrayPointer = $compilationContext->symbolTable->addTemp('HashPosition', $compilationContext);
        $arrayHash = $compilationContext->symbolTable->addTemp('HashTable', $compilationContext);
        /**
         * Create a temporary zval to fetch the items from the hash
         */
        $tempVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
        $tempVariable->setIsDoublePointer(true);
        $codePrinter = $compilationContext->codePrinter;
        $reverse = $statement['reverse'] ? 1 : 0;

        $codePrinter->output('zephir_is_iterable(' . $this->getVariableCode($exprVariable) . ', &' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ', ' . $duplicateHash . ', ' . $reverse . ', "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');

        $codePrinter->output('for (');
        $codePrinter->output('  ; zend_hash_get_current_data_ex(' . $arrayHash->getName() . ', (void**) &' . $tempVariable->getName() . ', &' . $arrayPointer ->getName() . ') == SUCCESS');
        if ($reverse) {
            $codePrinter->output('  ; zend_hash_move_backwards_ex(' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ')');
        } else {
            $codePrinter->output('  ; zend_hash_move_forward_ex(' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ')');
        }
        $codePrinter->output(') {');

        if (isset($keyVariable)) {
            if ($duplicateKey) {
                $compilationContext->symbolTable->mustGrownStack(true);
                $codePrinter->output("\t" . 'ZEPHIR_GET_HMKEY(' . $keyVariable->getName() . ', ' . $arrayHash->getName() . ', ' . $arrayPointer ->getName() . ');');
            } else {
                $codePrinter->output("\t" . 'ZEPHIR_GET_HKEY(' . $keyVariable->getName() . ', ' . $arrayHash->getName() . ', ' . $arrayPointer ->getName() . ');');
            }
        }

        if (isset($variable)) {
            $compilationContext->symbolTable->mustGrownStack(true);
            $codePrinter->output("\t" . 'ZEPHIR_GET_HVALUE(' . $variable->getName() . ', ' . $tempVariable->getName() . ');');
        }

        /**
         * Compile statements in the 'for' block
         */
        if (isset($statement['statements'])) {
            $statementBlock->isLoop(true);
            if (isset($statement['key'])) {
                $statementBlock->getMutateGatherer()->increaseMutations($statement['key']);
            }
            $statementBlock->getMutateGatherer()->increaseMutations($statement['value']);
            $statementBlock->compile($compilationContext);
        }

        $codePrinter->output('}');
        /* Make sure to free the duplicated hash table */
        if ($duplicateHash) {
            $codePrinter->output('zend_hash_destroy(' . $arrayHash->getName() . ');');
            $codePrinter->output('FREE_HASHTABLE(' . $arrayHash->getName() . ');');
        }
    }

    public function forStatementIterator(Variable $iteratorVariable, Variable $targetVariable, CompilationContext $compilationContext)
    {
        $compilationContext->codePrinter->output('zval **ZEPHIR_TMP_ITERATOR_PTR;');
        $compilationContext->codePrinter->output($iteratorVariable->getName() . '->funcs->get_current_data(' . $iteratorVariable->getName() . ', &ZEPHIR_TMP_ITERATOR_PTR TSRMLS_CC);');
        $this->copyOnWrite($targetVariable, '(*ZEPHIR_TMP_ITERATOR_PTR)', $compilationContext);
    }

    public function destroyIterator(Variable $iteratorVariable, CompilationContext $context)
    {
        $context->codePrinter->output($iteratorVariable->getName() . '->funcs->dtor(' . $iteratorVariable->getName() . ' TSRMLS_CC);');
    }

    public function ifVariableValueUndefined(Variable $var, CompilationContext $context, $onlyBody = false, $useCodePrinter = true)
    {
        $body = '!' . $var->getName();
        $output = 'if (' . $body. ') {';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $onlyBody ? $body : $output;
    }

    public function checkStrictType($type, $var, CompilationContext $context)
    {
        $codePrinter = $context->codePrinter;
        $conditions = array();

        $inputParamVariable = $context->symbolTable->getVariableForWrite($var['name'], $context);
        $inputParamCode = $this->getVariableCode($inputParamVariable);
        $cond = 'Z_TYPE_P(' . $inputParamCode . ') != ';
        if ($context->symbolTable->hasVariable($var['name'] . '_param')) {
            $parameterVariable = $context->symbolTable->getVariableForWrite($var['name'] . '_param', $context);
            $parameterCode = $context->backend->getVariableCode($parameterVariable);
            $cond = 'Z_TYPE_P(' . $parameterCode . ') != ';
        }


        switch ($type) {
            case 'int':
            case 'uint':
            case 'long':
                $conditions[] = $cond . 'IS_LONG';
                break;
            case 'bool':
                if (!$this->isZE3()) {
                    $conditions[] = $cond . 'IS_BOOL';
                } else {
                    $conditions[] = $cond . 'IS_TRUE';
                    $conditions[] = $cond . 'IS_FALSE';
                }
                break;
            case 'double':
                $conditions[] = $cond . 'IS_DOUBLE';
                break;
            case 'string':
            case 'ulong':
                $conditions[] = $cond . 'IS_STRING';
                $conditions[] = $cond . 'IS_NULL';
                break;
            case 'array':
                break;
            case 'object':
            case 'resource':
                $conditions[] = $cond . 'IS_' . strtoupper($type);
                break;
            case 'callable':
                $conditions[] = 'zephir_is_callable(' . $inputParamCode . ' TSRMLS_CC) != 1';
                break;
            default:
                throw new CompilerException('Unknown type ' . $type);
        }

        /* Generate verification code */
        if (count($conditions)) {
            $codePrinter->output('if (UNEXPECTED(' . implode(' && ', $conditions) . ')) {');
            $codePrinter->increaseLevel();
            $codePrinter->output('zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter \'' . $var['name'] . '\' must be a ' . $type . '") TSRMLS_CC);');
            $codePrinter->output('RETURN_MM_NULL();');
            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
        }

        /* Assign param */
        switch ($type) {
            case 'int':
            case 'uint':
            case 'long':
                $codePrinter->output($var['name'] . ' = Z_LVAL_P(' . $parameterCode . ');');
                break;
            case 'bool':
                $codePrinter->output($var['name'] . ' = ' . $this->getBoolCode($parameterVariable, $context, false) . ';');
                break;
            case 'double':
                $codePrinter->output($var['name'] . ' = Z_DVAL_P(' . $parameterCode . ');');
                break;
            case 'string':
            case 'ulong':
                $context->headersManager->add('kernel/operators');
                $context->symbolTable->mustGrownStack(true);
                $codePrinter->output('if (EXPECTED(Z_TYPE_P(' . $parameterCode . ') == IS_STRING)) {');
                $codePrinter->increaseLevel();
                $targetVar = $var['name'];
                if ($this->isZE3()) {
                    $targetVar = '&' . $targetVar;
                }
                $codePrinter->output('zephir_get_strval(' . $targetVar . ', ' . $var['name'] . '_param);');
                $codePrinter->decreaseLevel();
                $codePrinter->output('} else {');
                $codePrinter->increaseLevel();
                $this->initVar($inputParamVariable, $context);
                $codePrinter->output('ZVAL_EMPTY_STRING(' . $inputParamCode . ');');
                $codePrinter->decreaseLevel();
                $codePrinter->output('}');
                break;
            case 'array':
                $context->backend->assignZval($inputParamVariable, $parameterVariable, $context);
                break;
            case 'object':
            case 'resource':
            case 'callable':
                break;
            default:
                throw new CompilerException('Unknown type: ' . $type);
        }
    }

    public function fetchClassEntry($str)
    {
        return 'zephir_get_internal_ce(SS("' . $str . '") TSRMLS_CC)';
    }

    public function getScalarTempVariable($type, CompilationContext $compilationContext, $isLocal = false)
    {
        if ($isLocal) {
            return $compilationContext->symbolTable->getTempLocalVariableForWrite($type, $compilationContext);
        }
        return $compilationContext->symbolTable->getTempVariableForWrite($type, $compilationContext);
    }
}
