<?php
namespace Zephir\Backends\ZendEngine3;

use Zephir\Variable;
use Zephir\CompiledExpression;
use Zephir\Compiler;
use Zephir\CompilerException;
use Zephir\CompilationContext;
use Zephir\ClassMethod;
use Zephir\Backends\ZendEngine2\Backend as BackendZendEngine2;
use Zephir\BaseBackend;

class Backend extends BackendZendEngine2
{
    protected $name = 'ZendEngine3';

    public function getVariableCode(Variable $variable)
    {
        if (in_array($variable->getName(), array('this_ptr'))) {
            return $variable->getName();
        }
        return '&' . $variable->getName();
    }

    public function getStringsManager()
    {
        return new StringsManager();
    }

    public function getTypeDefinition($type)
    {
        list ($pointer, $code) = parent::getTypeDefinition($type);
        if ($code == 'zval') {
            return array('', 'zval');
        }
        return array($pointer, $code);
    }

    public function onPreInitVar(ClassMethod $method, CompilationContext $context)
    {
        if (!$method->isInternal()) {
            return "\t" . 'zval *this_ptr = getThis();' . PHP_EOL; //TODO: think about a better way to solve this.
        }
    }

    public function onPreCompile(ClassMethod $method, CompilationContext $context)
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

    public function onPostCompile(ClassMethod $method, CompilationContext $context)
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
        if ($isComplex) { /* && $variable->mustInitNull() */
            $groupVariables[] = $variable->getName();
            return "\t".'ZVAL_UNDEF(&' . $variable->getName() . ');';
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

    /**
     * Returns the signature of an internal method
     */
    public function getInternalSignature(ClassMethod $method, CompilationContext $context)
    {
        if ($method->isInitializer() && !$method->isStatic()) {
            return 'static zend_object *' . $method->getName() . '(zend_class_entry *class_type TSRMLS_DC)';
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
                        $signatureParameters[] = 'zval *' . $parameter['name'] . '_param_ext';
                        break;

                    default:
                        $signatureParameters[] = 'zval *' . $parameter['name'] . '_ext';
                        break;
                }
            }
        }

        if (count($signatureParameters)) {
            return 'static void ' . $method->getInternalName() . '(int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used, ' . join(', ', $signatureParameters) . ' TSRMLS_DC)';
        }

        return 'static void ' . $method->getInternalName() . '(int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used TSRMLS_DC)';
    }

    /* Assign value to variable */
    public function assignString(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true, $doCopy = null)
    {
        return $this->assignHelper('ZVAL_STRING', '&' . $variable->getName(), $value, $context, $useCodePrinter, null);
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

        $keyStr = $key->getType() == 'string' ? 'SL("' . $key->getCode() . '")' : $key->getCode();
        $valueStr = $type == 'stringl' ? 'SL("' . $value->getCode()  . '")' : $value->getCode();
        $output = 'add_assoc_' . $type . '_ex(&' . $variable->getName() . ', ' . $keyStr . ', ' . $valueStr . ');';

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

    public function arrayUnset(Variable $variable, $exprIndex, $flags, CompilationContext $context)
    {
        $context->headersManager->add('kernel/array');
        if ($exprIndex->getType()) {
            $context->codePrinter->output('zephir_array_unset_string(&' . $variable->getName() . ', SL("' . $exprIndex->getCode() . '"), ' . $flags . ');');
            return;
        }
        return parent::arrayUnset($variable, $exprIndex, $flags, $context);
    }

    public function fetchGlobal(Variable $globalVar, CompilationContext $compilationContext, $useCodePrinter = true)
    {
        $name = $globalVar->getName();
        $output = 'zephir_get_global(&' . $name . ', SL("' . $name . '"));';
        if ($useCodePrinter) {
            $codePrinter->output($output);
        }
        return $output;
    }

    public function fetchClass(Variable $zendClassEntry, $className, $guarded, CompilationContext $context)
    {
        $context->headersManager->add('kernel/object');
        if ($guarded) {
            $context->codePrinter->output('if (!' . $zendClassEntry->getName() . ') {');
        }
        $context->codePrinter->output("\t" . $zendClassEntry->getName() . ' = zephir_fetch_class_str_ex(' . $className . ', ZEND_FETCH_CLASS_AUTO);');
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
        //TODO: maybe optimizations (read_nproperty/quick) for thisScope access in NG (as in ZE2 - if necessary)
        $context->codePrinter->output('zephir_read_property(&' . $symbolVariable->getName() . ', ' . $variableVariable->getName() . ', SL("' . $property . '"), ' . $flags . ');');
    }

    public function fetchStaticProperty(Variable $symbolVariable, $classDefinition, $property, $readOnly, CompilationContext $context)
    {
        $flags = 'PH_NOISY_CC';
        if ($readOnly) {
            $flags .= ' | PH_READONLY';
        }
        //TODO: maybe optimizations aswell as above
        $context->codePrinter->output('zephir_read_static_property_ce(&' . $symbolVariable->getName() . ', ' . $classDefinition->getClassEntry() . ', SL("' . $property . '"), ' . $flags . ');');
    }

    public function resolveValue($value, CompilationContext $compilationContext)
    {
        if ($value == 'null') {
            $tempVariable = $context->symbolTable->getTempVariableForWrite('variable', $compilationContext);
            //TODO: assignNull?
            $value = $this->getVariableCode($tempVariable);
        } else if ($value == 'true' || $value == 'false') {
            $tempVariable = $context->symbolTable->getTempVariableForWrite('variable', $compilationContext);
            $this->assignBool($tempVariable, $value, $context);
            $value = $this->getVariableCode($tempVariable);
        } else if ($value instanceof Variable) {
            $value = $this->getVariableCode($value);
        }
        return $value;
    }

    public function updateProperty(Variable $symbolVariable, $propertyName, $value, CompilationContext $context)
    {
        //TODO: maybe optimizations aswell as above
        $value = $this->resolveValue($value, $context);
        $context->codePrinter->output('zephir_update_property_zval(' . $this->getVariableCode($symbolVariable) . ', SL("' . $propertyName . '"), ' . $value . ');');
    }

    public function updateStaticProperty($classEntry, $property, $value, CompilationContext $context)
    {
        $value = $this->resolveValue($value, $context);
        $context->codePrinter->output('zephir_update_static_property_ce(' . $classEntry .', SL("' . $property . '"), ' . $value . ');');
    }

    public function callMethod($symbolVariable, Variable $variable, $methodName, $cachePointer, $params, CompilationContext $context)
    {
        $paramStr = $params != null ? ', ' . join(', ', $params) : '';
        if (!isset($symbolVariable)) {
            $context->codePrinter->output('ZEPHIR_CALL_METHOD(NULL, &' . $variable->getName() . ', "' . $methodName . '", ' . $cachePointer . $paramStr . ');');
        } else if ($symbolVariable->getName() == 'return_value') {
            $context->codePrinter->output('ZEPHIR_RETURN_CALL_METHOD(&' . $variable->getName() . ', "' . $methodName . '", ' . $cachePointer . $paramStr . ');');
        } else {
            $context->codePrinter->output('ZEPHIR_CALL_METHOD(&' . $symbolVariable->getName() . ', &' . $variable->getName() . ', "' . $methodName . '", ' . $cachePointer . $paramStr . ');');
        }
    }

    public function setSymbolIfSeparated(Variable $variableTempSeparated, Variable $variable, CompilationContext $context)
    {
        $context->codePrinter->output('if (' . $variableTempSeparated->getName() . ') {');
        $context->codePrinter->output("\t" . 'ZEPHIR_SET_SYMBOL(&EG(symbol_table), "' . $variable->getName() . '", &' . $variable->getName() . ');');
        $context->codePrinter->output('}');
    }
}
