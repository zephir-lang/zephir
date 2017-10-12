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

namespace Zephir\Backends\ZendEngine3;

use Zephir\Utils;
use Zephir\Variable;
use Zephir\Compiler;
use Zephir\ClassMethod;
use Zephir\GlobalConstant;
use Zephir\ClassDefinition;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\FunctionDefinition;
use Zephir\Fcall\FcallManagerInterface;
use Zephir\Compiler\CompilerException;
use Zephir\Backends\ZendEngine2\Backend as BackendZendEngine2;

/**
 * Zephir\Backends\ZendEngine3\Backend
 *
 * @package Zephir\Backends\ZendEngine3
 */
class Backend extends BackendZendEngine2
{
    protected $name = 'ZendEngine3';

    /* TODO: This should not be used, temporary (until its completely refactored) */
    public function isZE3()
    {
        return true;
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
    public function getVariableCode(Variable $variable)
    {
        if ($variable->isDoublePointer() ||
            $variable->isSuperGlobal() ||
            in_array($variable->getName(), array('this_ptr', 'return_value')) ||
            in_array($variable->getType(), array('int', 'long'))) {
            return $variable->getName();
        }

        return '&' . $variable->getName();
    }

    /**
     * {@inheritdoc}
     */
    public function getVariableCodePointer(Variable $variable)
    {
        return $this->getVariableCode($variable);
    }

    public function getBoolCode(Variable $variable, CompilationContext $context, $useCodePrinter = true)
    {
        $code = '(Z_TYPE_P(' . $this->getVariableCode($variable) . ') == IS_TRUE)';
        if ($useCodePrinter) {
            $context->codePrinter->output($code);
        }

        return $code;
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
        switch ($type) {
            case 'zend_ulong':
                return array('', 'zend_ulong');

            case 'zend_string':
                return array('*', 'zend_string');
        }

        list ($pointer, $code) = parent::getTypeDefinition($type);

        return array($pointer, $code);
    }

    /**
     * Checks the type of a variable using the ZendEngine constants
     *
     * @param Variable $variableVariable
     * @param string $operator
     * @param string $value
     * @param CompilationContext $context
     *
     * @return string
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
                $condition = '((Z_TYPE_P(' . $variableName . ') == IS_TRUE || Z_TYPE_P(' . $variableName . ') == IS_FALSE) ' . $operator . ' 1)';
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
        if ($method instanceof FunctionDefinition) {
            return;
        }
        if (!$method->isInternal()) {
            return "zval *this_ptr = getThis();\n"; //TODO: think about a better way to solve this.
        }
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
            $codePrinter->output('zval local_this_ptr, *this_ptr = &local_this_ptr;');
            $codePrinter->output('ZEPHIR_CREATE_OBJECT(this_ptr, class_type);');
            $codePrinter->decreaseLevel();
        }
    }

    public function onPostCompile($method, CompilationContext $context)
    {
        $codePrinter = $context->codePrinter;
        if (preg_match('/^zephir_init_properties/', $method->getName())) {
            $codePrinter->increaseLevel();
            $codePrinter->output('return Z_OBJ_P(this_ptr);');
            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
            $codePrinter->decreaseLevel();
        }
    }

    public function generateInitCode(&$groupVariables, $type, $pointer, Variable $variable)
    {

        $isComplex = ($type == 'variable' || $type == 'string' || $type == 'array' || $type == 'resource' || $type == 'callable' || $type == 'object');

        if ($isComplex && !$variable->isDoublePointer() && !$variable->isSuperGlobal()) { /* && $variable->mustInitNull() */
            $groupVariables[] = $variable->getName();
            if ($variable->getRealname() == '__$null') {
                return "\t" . 'ZVAL_NULL(&' . $variable->getName() . ');';
            } else if ($variable->getRealname() == '__$true') {
                return "\t" . 'ZVAL_BOOL(&' . $variable->getName() . ', 1);';
            } else if ($variable->getRealname() == '__$false') {
                return "\t" . 'ZVAL_BOOL(&' . $variable->getName() . ', 0);';
            }
            return "\t".'ZVAL_UNDEF(&' . $variable->getName() . ');';
        }

        if ($variable->isLocalOnly()) {
            $groupVariables[] = $variable->getName();
            return;
        }

        if ($variable->isDoublePointer() || $variable->isSuperGlobal()) {
            /* Double pointers for ZE3 are used as zval * */
            $ptr = $isComplex ? $pointer : $pointer . $pointer;
            if ($variable->mustInitNull()) {
                $groupVariables[] = $ptr . $variable->getName() . ' = NULL';
            } else {
                $groupVariables[] = $ptr . $variable->getName();
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
            $context->codePrinter->output('zephir_declare_class_constant_null(' . $ce . ', SL("' . $name . '"));');
        } else {
            $context->codePrinter->output('zephir_declare_class_constant_' . $dType . '(' . $ce . ', SL("' . $name . '"), ' . $value . ');');
        }
    }

    /**
     * Returns the signature of an internal method
     */
    public function getInternalSignature(ClassMethod $method, CompilationContext $context)
    {
        if ($method->isInitializer() && !$method->isStatic()) {
            return 'zend_object *' . $method->getName() . '(zend_class_entry *class_type TSRMLS_DC)';
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
                        $signatureParameters[] = 'zval *' . $parameter['name'] . '_ext ';
                        break;
                }
            }
        }

        if (count($signatureParameters)) {
            return 'void ' . $method->getInternalName() . '(int ht, zval *return_value, zval *this_ptr, int return_value_used, ' . join(', ', $signatureParameters) . ')';
        }

        return 'void ' . $method->getInternalName() . '(int ht, zval *return_value, zval *this_ptr, int return_value_used)';
    }

    /* Assign value to variable */
    public function assignString(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true, $doCopy = null)
    {
        return $this->assignHelper('ZVAL_STRING', $this->getVariableCode($variable), $value, $context, $useCodePrinter, null);
    }

    /**
     * Assigns a zval to another
     *
     * @param Variable $variable
     * @param string $code
     * @param CompilationContext $context
     */
    public function assignZval(Variable $variable, $code, CompilationContext $context)
    {
        $code = $this->resolveValue($code, $context);
        if (!$variable->isDoublePointer()) {
            $context->symbolTable->mustGrownStack(true);
            $symbolVariable = $this->getVariableCode($variable);
            $context->codePrinter->output('ZEPHIR_OBS_COPY_OR_DUP(' . $symbolVariable . ', ' . $code . ');');
        } else {
            $context->codePrinter->output($variable->getName() . ' = ' . $code . ';');
        }
    }

    public function returnString($value, CompilationContext $context, $useCodePrinter = true, $doCopy = true)
    {
        return $this->returnHelper('RETURN_MM_STRING', $value, $context, $useCodePrinter, null);
    }

    public function createClosure(Variable $variable, $classDefinition, CompilationContext $context)
    {
        $symbol = $this->getVariableCode($variable);
        $context->codePrinter->output('zephir_create_closure_ex(' . $symbol . ', NULL, ' . $classDefinition->getClassEntry() . ', SL("__invoke"));');
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
                $value = $this->resolveValue('null', $context);
                $context->codePrinter->output('zephir_array_append(' . $this->getVariableCode($variable) . ', '. $value . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');
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

        if ($type === null) {
            throw new CompilerException("Unknown type mapping: " . $value->getType());
        }

        if (isset($key)) {
            if ($key->getType() == 'variable') {
                $var = $context->symbolTable->getVariableForRead($key->getCode(), $context);
                $keyStr = $typeKey == 'string' ? 'Z_STRVAL_P(' . $this->getVariableCode($var) . '), Z_STRLEN_P(' . $this->getVariableCode($var) . ')' : $this->getVariableCode($var);
            } else {
                $keyStr = $key->getType() == 'string' ? 'SL("' . $key->getCode() . '")' : $key->getCode();
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

        if ($keyType == 'assoc') {
            $output = 'add_assoc_' . $type . '_ex(' . $this->getVariableCode($variable) . ', ' . $keyStr . ', ' . $valueStr . ');';
        } else if ($keyType == 'append') {
            $output = 'zephir_array_append(' . $this->getVariableCode($variable) . ', ' . $this->resolveValue($value, $context) . ', PH_SEPARATE, "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');';
        } else {
            $output = 'add_index_' . $type . '(' . $this->getVariableCode($variable) . ', ' . $keyStr . ', '. $valueStr . ');';
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
                return new CompiledExpression('bool', 'zephir_array_isset_string(' . $this->getVariableCode($var) . ', SL("' . $resolvedExpr->getCode() . '"))', $expression);
            }
        }
        return parent::arrayIsset($var, $resolvedExpr, $expression, $context);
    }

    public function arrayIssetFetch(Variable $target, Variable $var, $resolvedExpr, $flags, $expression, CompilationContext $context)
    {
        if (!($resolvedExpr instanceof Variable)) {
            $code = $this->getVariableCode($target) . ', ' . $this->getVariableCode($var);
            if ($resolvedExpr->getType() == 'string') {
                return new CompiledExpression('bool', 'zephir_array_isset_string_fetch(' . $code . ', SL("' . $resolvedExpr->getCode() . '"), ' . $flags . ')', $expression);
            }
        }
        return parent::arrayIssetFetch($target, $var, $resolvedExpr, $flags, $expression, $context);
    }

    public function propertyIsset(Variable $var, $key, CompilationContext $context)
    {
        return new CompiledExpression('bool', 'zephir_isset_property(' . $this->getVariableCode($var) . ', SL("' . $key . '"))', null);
    }

    public function arrayUnset(Variable $variable, $exprIndex, $flags, CompilationContext $context)
    {
        $context->headersManager->add('kernel/array');
        $variableCode = $this->getVariableCodePointer($variable);
        if ($exprIndex->getType() == 'string') {
            $context->codePrinter->output('zephir_array_unset_string(' . $variableCode . ', SL("' . $exprIndex->getCode() . '"), ' . $flags . ');');
            return;
        }

        parent::arrayUnset($variable, $exprIndex, $flags, $context);
    }

    public function fetchGlobal(Variable $globalVar, CompilationContext $compilationContext, $useCodePrinter = true)
    {
        $name    = $globalVar->getName();
        $lines   = array();
        $lines[] = 'zephir_get_global(&' . $name . ', SL("' . $name . '"));';
        $lines[] = 'if (!' . $name . ') {';
        $lines[] = "\t" . 'ZEPHIR_THROW_EXCEPTION_STR(zend_exception_get_default(), "Invalid superglobal");';
        $lines[] = "\t" . 'return;';
        $lines[] = '}';
        if ($useCodePrinter) {
            foreach ($lines as $line) {
                $compilationContext->codePrinter->output($line);
            }
        }
        return join("\n\t", $lines);
    }

    public function fetchClass(Variable $zendClassEntry, $className, $guarded, CompilationContext $context)
    {
        $context->headersManager->add('kernel/object');
        if ($guarded) {
            $context->codePrinter->output('if (!' . $zendClassEntry->getName() . ') {');
        }
        $context->codePrinter->output($zendClassEntry->getName() . ' = zephir_fetch_class_str_ex(' . $className . ', ZEND_FETCH_CLASS_AUTO);');
        if ($guarded) {
            $context->codePrinter->output('}');
        }
    }

    public function fetchProperty(Variable $symbolVariable, Variable $variableVariable, $property, $readOnly, CompilationContext $context, $useOptimized = false)
    {
        $flags = 'PH_NOISY_CC';
        if ($readOnly) {
            $flags .= ' | PH_READONLY';
        }
        $variableCode = $this->getVariableCode($variableVariable);
        $symbol = $this->getVariableCode($symbolVariable);
        //TODO: maybe optimizations (read_nproperty/quick) for thisScope access in NG (as in ZE2 - if necessary)
        if ($property instanceof Variable) {
            $context->codePrinter->output('zephir_read_property_zval(' . $symbol . ', ' . $variableCode . ', ' . $this->getVariableCode($property) . ', ' . $flags . ');');
        } else {
            $context->codePrinter->output('zephir_read_property(' . $symbol . ', ' . $variableCode . ', SL("' . $property . '"), ' . $flags . ');');
        }
    }

    /**
     * @param Variable           $symbolVariable
     * @param ClassDefinition    $classDefinition
     * @param                    $property
     * @param bool               $readOnly
     * @param CompilationContext $context
     */
    public function fetchStaticProperty(Variable $symbolVariable, $classDefinition, $property, $readOnly, CompilationContext $context)
    {
        // TODO: maybe optimizations as well as above
        $context->codePrinter->output(
            sprintf(
                'zephir_read_static_property_ce(%s%s, %s, SL("%s"), PH_NOISY_CC%s);',
                $symbolVariable->isDoublePointer() ? '' : '&',
                $symbolVariable->getName(),
                $classDefinition->getClassEntry(),
                $property,
                $readOnly ? ' | PH_READONLY' : ''
            )
        );
    }

    /**
     * @param                    $value
     * @param CompilationContext $context
     * @param bool               $usePointer
     * @return bool|string|Variable
     * @throws CompilerException
     */
    public function resolveValue($value, CompilationContext $context, $usePointer = false)
    {
        if ($value instanceof GlobalConstant) {
            switch ($value->getName()) {
                case 'ZEPHIR_GLOBAL(global_null)':
                    $value = 'null';
                    break;
                case 'ZEPHIR_GLOBAL(global_true)':
                    $value = 'true';
                    break;
                case 'ZEPHIR_GLOBAL(global_false)':
                    $value = 'false';
                    break;
                default:
                    throw new CompilerException(
                        $this->name . ': Unknown constant ' . $value->getName()
                    );
            }
        }

        if ($value == 'null' || $value == 'true' || $value == 'false') {
            $varName = '__$' . $value;
            if (!$context->symbolTable->hasVariable($varName)) {
                $tempVariable = new Variable('variable', $varName, $context->currentBranch);
                $context->symbolTable->addRawVariable($tempVariable);
            }
            $tempVariable = $context->symbolTable->getVariableForWrite($varName, $context);
            $tempVariable->increaseUses();
            $tempVariable->setUsed(true, null);
            if ($value == 'null') {
                $tempVariable->setDynamicTypes('null');
            } else {
                $tempVariable->setDynamicTypes('bool');
            }
            $value = $this->getVariableCode($tempVariable);
        } else {
            if ($value instanceof CompiledExpression) {
                if ($value->getType() == 'array') {
                    $value = $context->symbolTable->getVariableForWrite($value->getCode(), $context, null);
                } else if ($value->getType() == 'variable') {
                    $value = $context->symbolTable->getVariableForWrite($value->getCode(), $context);
                } else {
                    return $value->getCode();
                }
            }
        }

        if ($value instanceof Variable) {
            $value = $this->getVariableCode($value);
        }

        return $value;
    }

    public function updateProperty(Variable $symbolVariable, $propertyName, $value, CompilationContext $context)
    {
        //TODO: maybe optimizations as well as above
        $value = $this->resolveValue($value, $context);
        if ($propertyName instanceof Variable) {
            $context->codePrinter->output('zephir_update_property_zval_zval(' . $this->getVariableCode($symbolVariable) . ', ' . $this->getVariableCode($propertyName) . ', ' . $value . ' TSRMLS_CC);');
        } else {
            $context->codePrinter->output('zephir_update_property_zval(' . $this->getVariableCode($symbolVariable) . ', SL("' . $propertyName . '"), ' . $value . ');');
        }
    }

    public function updateStaticProperty($classEntry, $property, $value, CompilationContext $context)
    {
        $value = $this->resolveValue($value, $context);
        $context->codePrinter->output('zend_update_static_property(' . $classEntry .', ZEND_STRL("' . $property . '"), ' . $value . ');');
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
            $context->codePrinter->output('ZEPHIR_' . $macro . '(NULL, ' . $this->getVariableCode($variable) . ', ' . $methodName . ', ' . $cachePointer . $paramStr . ');');
        } else if ($symbolVariable->getName() == 'return_value') {
            $context->codePrinter->output('ZEPHIR_RETURN_' . $macro . '(' . $this->getVariableCode($variable) . ', ' . $methodName . ', ' . $cachePointer . $paramStr . ');');
        } else {
            $symbol = $this->getVariableCode($symbolVariable);
            $context->codePrinter->output('ZEPHIR_' . $macro . '(' . $symbol . ', ' . $this->getVariableCode($variable) . ', ' . $methodName . ', ' . $cachePointer . $paramStr . ');');
        }
    }

    public function setSymbolIfSeparated(Variable $variableTempSeparated, Variable $variable, CompilationContext $context)
    {
        $context->codePrinter->output('if (' . $variableTempSeparated->getName() . ') {');
        $context->codePrinter->output("\t" . 'ZEPHIR_SET_SYMBOL(&EG(symbol_table), "' . $variable->getName() . '", &' . $variable->getName() . ');');
        $context->codePrinter->output('}');
    }

    public function copyOnWrite(Variable $target, $var, CompilationContext $context)
    {
        if ($var == 'EG(exception)') {
            $context->codePrinter->output('ZVAL_OBJ(' . $this->getVariableCode($target) . ', EG(exception));');
            $context->codePrinter->output('Z_ADDREF_P(' . $this->getVariableCode($target) . ');');
            return;
        }
        return parent::copyOnWrite($target, $var, $context);
    }

    public function forStatement(Variable $exprVariable, $keyVariable, $variable, $duplicateKey, $duplicateHash, $statement, $statementBlock, CompilationContext $compilationContext)
    {
        /**
         * Create a hash table and hash pointer temporary variables
         */
        //$arrayPointer = $compilationContext->symbolTable->addTemp('HashPosition', $compilationContext);
        //$arrayHash = $compilationContext->symbolTable->addTemp('HashTable', $compilationContext);
        /**
         * Create a temporary zval to fetch the items from the hash
         */
        $tempVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
        $tempVariable->setIsDoublePointer(true);
        $codePrinter = $compilationContext->codePrinter;

        $codePrinter->output('zephir_is_iterable(' . $this->getVariableCode($exprVariable) . ', ' . $duplicateHash . ', "' . Compiler::getShortUserPath($statement['file']) . '", ' . $statement['line'] . ');');

        $macro = null;
        $reverse = $statement['reverse'] ? 'REVERSE_' : '';

        if (isset($keyVariable)) {
            $arrayNumKey = $compilationContext->symbolTable->addTemp('zend_ulong', $compilationContext);
            $arrayStrKey = $compilationContext->symbolTable->addTemp('zend_string', $compilationContext);
        }

        if (isset($keyVariable) && isset($variable)) {
            $macro = 'ZEND_HASH_' . $reverse . 'FOREACH_KEY_VAL';
            $codePrinter->output($macro . '(Z_ARRVAL_P(' . $this->getVariableCode($exprVariable) . '), ' . $arrayNumKey->getName() . ', ' . $arrayStrKey->getName() . ', ' . $tempVariable->getName() . ')');
        } else if (isset($keyVariable)) {
            $macro = 'ZEND_HASH_' . $reverse . 'FOREACH_KEY';
            $codePrinter->output($macro . '(Z_ARRVAL_P(' . $this->getVariableCode($exprVariable) . '), ' . $arrayNumKey->getName() . ', ' . $arrayStrKey->getName() . ')');
        } else {
            $macro = 'ZEND_HASH_' . $reverse . 'FOREACH_VAL';
            $codePrinter->output($macro . '(Z_ARRVAL_P(' . $this->getVariableCode($exprVariable) . '), ' . $tempVariable->getName() . ')');
        }

        $codePrinter->output('{');

        if (isset($keyVariable)) {
            $codePrinter->increaseLevel();
            if ($duplicateKey) {
                $compilationContext->symbolTable->mustGrownStack(true);
                $keyVariable->initVariant($compilationContext);
            }
            $codePrinter->output('if (' . $arrayStrKey->getName() . ' != NULL) { ');
            $codePrinter->increaseLevel();
            if ($duplicateKey) {
                $codePrinter->output('ZVAL_STR_COPY(' . $this->getVariableCode($keyVariable) . ', ' . $arrayStrKey->getName() . ');');
            } else {
                $codePrinter->output('ZVAL_STR(' . $this->getVariableCode($keyVariable) . ', ' . $arrayStrKey->getName() . ');');
            }
            $codePrinter->decreaseLevel();
            $codePrinter->output('} else {');
            $codePrinter->increaseLevel();
            $codePrinter->output('ZVAL_LONG(' . $this->getVariableCode($keyVariable) . ', ' . $arrayNumKey->getName() . ');');
            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
            $codePrinter->decreaseLevel();
        }

        if (isset($variable)) {
            $compilationContext->symbolTable->mustGrownStack(true);
            $codePrinter->increaseLevel();
            $variable->initVariant($compilationContext);
            $codePrinter->output('ZVAL_COPY(' . $this->getVariableCode($variable) . ', ' . $this->getVariableCode($tempVariable) . ');');
            $codePrinter->decreaseLevel();
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

        $codePrinter->output('} ZEND_HASH_FOREACH_END();');

        /* Since we do not observe, still do cleanup */
        if (isset($variable)) {
            $variable->initVariant($compilationContext);
        }
        if (isset($keyVariable)) {
            $keyVariable->initVariant($compilationContext);
        }
    }

    public function forStatementIterator(Variable $iteratorVariable, Variable $targetVariable, CompilationContext $compilationContext)
    {
        $compilationContext->symbolTable->mustGrownStack(true);
        $compilationContext->codePrinter->output('ZEPHIR_ITERATOR_COPY(' . $this->getVariableCode($targetVariable) . ', ' . $iteratorVariable->getName() . ');');
    }

    public function destroyIterator(Variable $iteratorVariable, CompilationContext $context)
    {
        $context->codePrinter->output('zend_iterator_dtor(' . $iteratorVariable->getName() . ');');
    }

    public function ifVariableValueUndefined(Variable $var, CompilationContext $context, $useBody = false, $useCodePrinter = true)
    {
        if ($var->isDoublePointer()) {
            return parent::ifVariableValueUndefined($var, $context, $useBody, $useCodePrinter);
        }
        $body = 'Z_TYPE_P(' . $this->getVariableCode($var) . ') == IS_UNDEF';
        $output = 'if (' . $body . ') {';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $useBody ? $body : $output;
    }

    public function fetchClassEntry($str)
    {
        return 'zephir_get_internal_ce(SL("' . $str . '"))';
    }

    public function getScalarTempVariable($type, CompilationContext $compilationContext, $isLocal = true)
    {
        return $compilationContext->symbolTable->getTempNonTrackedVariable($type, $compilationContext);
    }
}
