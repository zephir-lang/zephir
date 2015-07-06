<?php
namespace Zephir\Backends\ZendEngine2;

use Zephir\Variable;
use Zephir\CompilerException;
use Zephir\CompilationContext;
use Zephir\BaseBackend;

class Backend extends BaseBackend
{
    protected $name = 'ZendEngine2';

    public function getStringsManager()
    {
        return new StringsManager();
    }

    public function getTypeDefinition ($type)
    {
        $code = null;
        $pointer = null;
        switch ($type) {
            case 'int':
                $code = 'int';
                break;

            case 'uint':
                $code = 'unsigned int';
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
                $code = 'zephir_nts_static zephir_fcall_cache_entry';
                break;

            case 'static_zend_class_entry':
                $pointer = '*';
                $code = 'zephir_nts_static zend_class_entry';
                break;

            case 'zephir_ce_guard':
                $code = 'zephir_nts_static zend_bool';
                break;

            default:
                throw new CompilerException("Unsupported type in declare: " . $type);
        }
        return array($pointer, $code);
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

    /* Assign value to variable */
    protected function assignHelper($macro, $variableName, $value, CompilationContext $context, $useCodePrinter)
    {
        if ($value instanceof Variable) {
            $value = $value->getName();
        } else {
            $value = '"' . $value . '"';
        }
        $output = $macro . '(' . $variableName . ', ' . $value . ');';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function assignString(Variable $variable, $value, CompilationContext $context, $useCodePrinter=true)
    {
        return $this->assignHelper('ZVAL_STRING', ($symbolVariable->isLocalOnly() ? '&' : '') . $variable->getName(), $value, $context, $useCodePrinter);
    }

    public function assignLong(Variable $variable, $value, CompilationContext $context, $useCodePrinter=true)
    {
        return $this->assignHelper('ZVAL_LONG', ($symbolVariable->isLocalOnly() ? '&' : '') . $variable->getName(), $value, $context, $useCodePrinter);
    }

    public function assignDouble(Variable $variable, $value, CompilationContext $context, $useCodePrinter=true)
    {
        return $this->assignHelper('ZVAL_DOUBLE', ($symbolVariable->isLocalOnly() ? '&' : '') . $variable->getName(), $value, $context, $useCodePrinter);
    }

    public function assignBool(Variable $variable, $value, CompilationContext $context, $useCodePrinter=true)
    {
        return $this->assignHelper('ZVAL_BOOL', ($symbolVariable->isLocalOnly() ? '&' : '') . $variable->getName(), $value, $context, $useCodePrinter);
    }

    public function initArray(Variable $variable, CompilationContext $context, $size = null, $useCodePrinter=true)
    {
        if (!isset($size)) {
            $output = 'array_init(' . $variable->getName() . ');';
        } else {
            $output = 'zephir_create_array(' . $variable->getName() . ', ' . $size . ', 0);';
        }
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return "\t" . $output;
    }

    public function addArrayEntry(Variable $variable, $key, $value, CompilationContext $context, $useCodePrinter=true)
    {
        $type = null;
        switch ($value->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                $type = 'long';
                break;
            case 'string':
                $type = 'stringl';
                break;
        }
        if (!$type) {
            throw new Exception("Unknown type mapping: " . $type);
        }

        $keyStr   = $key->getType() == 'string' ? 'SS("' . $key->getCode() . '")' : $key->getCode();
        $valueStr = $type == 'stringl' ? 'SL("' . $value->getCode()  . '")' : $value->getCode();
        $output = 'add_assoc_' . $value->getType() . '_ex(' . $variable->getName() . ', ' . $keyStr . ', ' . $valueStr . ');';

        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function initObject(Variable $variable, $ce, CompilationContext $context, $useCodePrinter=true)
    {
        if (!isset($ce)) {
            $output = 'object_init(&' . $variable->getName() . ');';
        } else {
            $output = 'object_init_ex(' . $variable->getName() . ', ' . $ce . ');';
        }
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function arrayFetch(Variable $var, Variable $src, $index, $flags, $arrayAccess, CompilationContext $context, $useCodePrinter=true)
    {
        switch ($index->getType()) {
            case 'int':
            case 'uint':
            case 'long':
                $type = 'long';
                break;
            /* Types which map to the same */
            case 'string':
                $type = $index->getType();
                break;
            default:
                 throw new CompilerException("Variable type: " . $index->getType() . " cannot be used as array index without cast", $arrayAccess['right']);
        }
        $output = 'zephir_array_fetch_long(&' . $var->getName() . ', ' . $src->getName() . ', ' . $index->getCode() . ', ' . $flags . ', "' . Compiler::getShortUserPath($arrayAccess['file']) . '", ' . $arrayAccess['line'] . ');';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function fetchClass(Variable $var, $name, $guarded, CompilationContext $context)
    {
        if ($guarded) {
            $context->codePrinter->output('if (!' . $zendClassEntry->getName() . ') {');
        }
        $context->codePrinter->output("\t" . $zendClassEntry->getName() . ' = zend_fetch_class(' . $className . ', ZEND_FETCH_CLASS_AUTO TSRMLS_CC);');
        if ($guarded) {
            $context->codePrinter->output('}');
        }
    }

    public function checkConstructor(Variable $var, CompilationContext $context)
    {
        $context->codePrinter->output('if (zephir_has_constructor(' . $var->getName() . ' TSRMLS_CC)) {');
    }

    public function callMethod($symbolVariable, Variable $variable, $methodName, $cachePointer, $params, CompilationContext $context)
    {
        $paramStr = $params != NULL ? ', ' . join(', ', $params) : '';
        if (!isset($symbolVariable)) {
            $context->codePrinter->output('ZEPHIR_CALL_METHOD(NULL, ' . $variable->getName() . ', "' . $methodName . '", ' . $cachePointer . $paramStr . ');');
        }
        else if ($symbolVariable->getName() == 'return_value') {
            $context->codePrinter->output('ZEPHIR_RETURN_CALL_METHOD(' . $variable->getName() . ', "' . $methodName . '", ' . $cachePointer . $paramStr . ');');
        } else {
            $context->codePrinter->output('ZEPHIR_CALL_METHOD(&' . $symbolVariable->getName() . ', ' . $variable->getName() . ', "' . $methodName . '", ' . $cachePointer . $paramStr . ');');
        }
    }
}
