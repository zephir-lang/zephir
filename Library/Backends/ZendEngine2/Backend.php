<?php
namespace Zephir\Backends\ZendEngine2;

use Zephir\Variable;
use Zephir\Compiler;
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

    public function getTypeDefinition($type)
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

    public function onPreInitVar(CompilationContext $context)
    {
        return '';
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
        }

        $output = $macro . '(' . $variableName . ', ' . $value . $copyStr . ');';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function assignString(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->assignHelper('ZVAL_STRING', ($variable->isLocalOnly() ? '&' : '') . $variable->getName(), $value, $context, $useCodePrinter, true);
    }

    public function assignLong(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->assignHelper('ZVAL_LONG', ($variable->isLocalOnly() ? '&' : '') . $variable->getName(), $value, $context, $useCodePrinter);
    }

    public function assignDouble(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->assignHelper('ZVAL_DOUBLE', ($variable->isLocalOnly() ? '&' : '') . $variable->getName(), $value, $context, $useCodePrinter);
    }

    public function assignBool(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->assignHelper('ZVAL_BOOL', ($variable->isLocalOnly() ? '&' : '') . $variable->getName(), $value, $context, $useCodePrinter);
    }

    public function initArray(Variable $variable, CompilationContext $context, $size = null, $useCodePrinter = true)
    {
        if (!isset($size)) {
            $output = 'array_init(' . $variable->getName() . ');';
        } else {
            $output = 'zephir_create_array(' . $variable->getName() . ', ' . $size . ', 0 TSRMLS_CC);';
        }
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function addArrayEntry(Variable $variable, $key, $value, CompilationContext $context, $useCodePrinter = true)
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


        if ($key->getType() == 'variable') {
            $keyStr = 'Z_STRVAL_P(' . $key->getCode() . '), Z_STRLEN_P(' . $key->getCode() . ') + 1';
        } else {
            $keyStr = $key->getType() == 'string' ? 'SS("' . $key->getCode() . '")' : $key->getCode();
        }
        $valueStr = $type == 'stringl' ? 'SL("' . $value->getCode()  . '")' : $value->getCode();
        $doCopy = $type == 'stringl' ? ', 1' : '';
        $output = 'add_assoc_' . $type . '_ex(' . $variable->getName() . ', ' . $keyStr . ', ' . $valueStr . $doCopy . ');';

        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function initObject(Variable $variable, $ce, CompilationContext $context, $useCodePrinter = true)
    {
        if ($variable->getName() == 'return_value' || !$variable->isLocalOnly()) {
            $variableAccess = $variable->getName();
        } else {
            $variableAccess = '&' . $variable->getName();
        }
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
            if ($index->isLocalOnly()) {
                $output = 'zephir_array_fetch(&' . $var->getName() . ', ' . $src->getName() . ', &' . $index->getName() . ', ' . $flags . ', "' . Compiler::getShortUserPath($arrayAccess['file']) . '", ' . $arrayAccess['line'] . ' TSRMLS_CC);';
            } else {
                $output = 'zephir_array_fetch(&' . $var->getName() . ', ' . $src->getName() . ', ' . $index->getName() . ', ' . $flags . ', "' . Compiler::getShortUserPath($arrayAccess['file']) . '", ' . $arrayAccess['line'] . ' TSRMLS_CC);';
            }
        } else {
            if ($isVariable) {
                $indexAccess =  $index->getName();
            } else {
                $indexAccess = $index->getCode();
                if ($type == 'string') {
                    $indexAccess = 'SL("' . $indexAccess . '")';
                }
            }
            $output = 'zephir_array_fetch_' . $type . '(&' . $var->getName() . ', ' . $src->getName() . ', ' . $indexAccess . ', ' . $flags . ', "' . Compiler::getShortUserPath($arrayAccess['file']) . '", ' . $arrayAccess['line'] . ' TSRMLS_CC);';
        }

        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
        return $output;
    }

    public function arrayUnset(Variable $variable, $exprIndex, $flags, CompilationContext $context)
    {
        $context->headersManager->add('kernel/array');
        switch ($exprIndex->getType()) {
            case 'int':
            case 'uint':
            case 'long':
                $context->codePrinter->output('zephir_array_unset_long(&' . $variable->getName() . ', ' . $exprIndex->getCode() . ', ' . $flags . ');');
                break;

            case 'string':
                $context->codePrinter->output('zephir_array_unset_string(&' . $variable->getName() . ', SS("' . $exprIndex->getCode() . '"), ' . $flags . ');');
                break;

            case 'variable':
                $variableIndex = $context->symbolTable->getVariableForRead($exprIndex->getCode(), $context, $exprIndex->getOriginal());
                switch ($variableIndex->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                        $context->codePrinter->output('zephir_array_unset_long(&' . $variable->getName() . ', ' . $variableIndex->getName() . ', ' . $flags . ');');
                        break;

                    case 'string':
                    case 'variable':
                        $context->codePrinter->output('zephir_array_unset(&' . $variable->getName() . ', ' . $variableIndex->getName() . ', ' . $flags . ');');
                        break;

                    default:
                        throw new CompilerException("Variable type: " . $variableIndex->getType() . " cannot be used as array index without cast", $expression['right']);
                }
                break;

            default:
                throw new CompilerException("Cannot use expression: " . $exprIndex->getType() . " as array index without cast", $expression['right']);
        }
    }

    public function assignArrayMulti(Variable $variable, $symbolVariable, $offsetExprs, CompilationContext $compilationContext)
    {
        $keys = '';
        $offsetItems = array();
        $numberParams = 0;

        foreach ($offsetExprs as $offsetExpr) {
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
                            $offsetItems[] = $variableIndex->getName();
                            $numberParams++;
                            break;
                        case 'string':
                        case 'variable':
                            $keys .= 'z';
                            $offsetItems[] = $variableIndex->getName();
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

        $compilationContext->codePrinter->output('zephir_array_update_multi(&' . $variable->getName() . ', &' . $symbolVariable->getName() . ' TSRMLS_CC, SL("' . $keys . '"), ' . $numberParams . ', ' . join(', ', $offsetItems) . ');');
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
            $context->codePrinter->output('zephir_read_property(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $property . '"), PH_NOISY_CC);');
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

    public function updateProperty(Variable $symbolVariable, $propertyName, Variable $value, CompilationContext $context)
    {
        if ($symbolVariable->getName() == 'this_ptr') {
            $context->codePrinter->output('zephir_update_property_this(this_ptr, SL("' . $propertyName . '"), ' . $value->getName() . ' TSRMLS_CC);');
        } else {
            $context->codePrinter->output('zephir_update_property_zval(' . $symbolVariable->getName() . ', SL("' . $propertyName . '"), ' . $value->getName() . ' TSRMLS_CC);');
        }
    }

    public function checkConstructor(Variable $var, CompilationContext $context)
    {
        $context->codePrinter->output('if (zephir_has_constructor(' . $var->getName() . ' TSRMLS_CC)) {');
    }

    public function callMethod($symbolVariable, Variable $variable, $methodName, $cachePointer, $params, CompilationContext $context)
    {
        $paramStr = $params != null ? ', ' . join(', ', $params) : '';
        if (!isset($symbolVariable)) {
            $context->codePrinter->output('ZEPHIR_CALL_METHOD(NULL, ' . $variable->getName() . ', "' . $methodName . '", ' . $cachePointer . $paramStr . ');');
        } else if ($symbolVariable->getName() == 'return_value') {
            $context->codePrinter->output('ZEPHIR_RETURN_CALL_METHOD(' . $variable->getName() . ', "' . $methodName . '", ' . $cachePointer . $paramStr . ');');
        } else {
            $context->codePrinter->output('ZEPHIR_CALL_METHOD(&' . $symbolVariable->getName() . ', ' . $variable->getName() . ', "' . $methodName . '", ' . $cachePointer . $paramStr . ');');
        }
    }
}
