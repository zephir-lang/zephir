<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Backends\ZendEngine3;

use function Zephir\add_slashes;
use Zephir\Backends\ZendEngine2\Backend as BackendZendEngine2;
use Zephir\ClassDefinition;
use Zephir\ClassMethod;
use Zephir\CodePrinter;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Compiler;
use Zephir\Exception\CompilerException;
use Zephir\Fcall\FcallManagerInterface;
use Zephir\FunctionDefinition;
use Zephir\GlobalConstant;
use Zephir\Variable;
use Zephir\Variable\Globals;

/**
 * Zephir\Backends\ZendEngine3\Backend.
 */
class Backend extends BackendZendEngine2
{
    protected $name = 'ZendEngine3';

    /**
     * {@inheritdoc}
     *
     * @return bool
     */
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
    public function getVariableCode(Variable $variable): string
    {
        if ($variable->isDoublePointer() ||
            \in_array($variable->getName(), ['this_ptr', 'return_value']) ||
            \in_array($variable->getType(), ['int', 'long'])) {
            return $variable->getName();
        }

        return '&'.$variable->getName();
    }

    /**
     * {@inheritdoc}
     *
     * @todo Do we need this method at all?
     */
    public function getVariableCodePointer(Variable $variable)
    {
        return $this->getVariableCode($variable);
    }

    /**
     * {@inheritdoc}
     */
    public function getBoolCode(Variable $variable, CompilationContext $context, $useCodePrinter = true)
    {
        $code = '(Z_TYPE_P('.$this->getVariableCode($variable).') == IS_TRUE)';
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

    /**
     * {@inheritdoc}
     */
    public function getTypeDefinition($type)
    {
        switch ($type) {
            case 'zend_ulong':
                return ['', 'zend_ulong'];

            case 'zend_string':
                return ['*', 'zend_string'];
        }

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
            case 'zephir_ce_guard':
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
            case 'static_zend_class_entry':
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
            case 'static_zephir_fcall_cache_entry':
                $pointer = '*';
                $code = 'zephir_fcall_cache_entry';
                break;

            case 'zephir_method_globals':
                $pointer = '*';
                $code = 'zephir_method_globals';
                break;

            default:
                throw new CompilerException('Unsupported type in declare: '.$type);
        }

        return [$pointer, $code];
    }

    /**
     * {@inheritdoc}
     *
     * @param Variable           $variableVariable
     * @param string             $operator
     * @param string             $value
     * @param CompilationContext $context
     *
     * @throws CompilerException
     *
     * @return string
     */
    public function getTypeofCondition(
        Variable $variableVariable,
        string $operator,
        string $value,
        CompilationContext $context
    ): string {
        $variableName = $this->getVariableCode($variableVariable);

        switch ($value) {
            case 'array':
                $condition = 'Z_TYPE_P('.$variableName.') '.$operator.' IS_ARRAY';
                break;

            case 'object':
                $condition = 'Z_TYPE_P('.$variableName.') '.$operator.' IS_OBJECT';
                break;

            case 'null':
                $condition = 'Z_TYPE_P('.$variableName.') '.$operator.' IS_NULL';
                break;

            case 'string':
                $condition = 'Z_TYPE_P('.$variableName.') '.$operator.' IS_STRING';
                break;

            case 'int':
            case 'long':
            case 'integer':
                $condition = 'Z_TYPE_P('.$variableName.') '.$operator.' IS_LONG';
                break;

            case 'double':
            case 'float':
                $condition = 'Z_TYPE_P('.$variableName.') '.$operator.' IS_DOUBLE';
                break;

            case 'boolean':
            case 'bool':
                $condition = '((Z_TYPE_P('.$variableName.') == IS_TRUE || Z_TYPE_P('.$variableName.') == IS_FALSE) '.$operator.' 1)';
                break;

            case 'resource':
                $condition = 'Z_TYPE_P('.$variableName.') '.$operator.' IS_RESOURCE';
                break;

            case 'callable':
                $condition = 'zephir_is_callable('.$variableName.') '.$operator.' 1';
                break;

            default:
                throw new CompilerException(sprintf('Unknown type: "%s" in typeof comparison', $value));
        }

        return $condition;
    }

    /**
     * {@inheritdoc}
     */
    public function onPreInitVar(ClassMethod $method, CompilationContext $context): string
    {
        if (!$method instanceof FunctionDefinition && !$method->isInternal()) {
            return "zval *this_ptr = getThis();\n"; //TODO: think about a better way to solve this.
        }

        return '';
    }

    public function onPreCompile(ClassMethod $method, CompilationContext $context)
    {
        $codePrinter = $context->codePrinter;
        /*
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
        $isComplex = \in_array($type, ['variable', 'string', 'array', 'resource', 'callable', 'object'], true);

        if ($isComplex && !$variable->isDoublePointer()) { /* && $variable->mustInitNull() */
            $groupVariables[] = $variable->getName();
            switch ($variable->getRealname()) {
                case '__$null':
                    return "\t".'ZVAL_NULL(&'.$variable->getName().');';
                case '__$true':
                    return "\t".'ZVAL_BOOL(&'.$variable->getName().', 1);';
                case '__$false':
                    return "\t".'ZVAL_BOOL(&'.$variable->getName().', 0);';
                default:
                    return "\t".'ZVAL_UNDEF(&'.$variable->getName().');';
            }
        }

        if ($variable->isLocalOnly()) {
            $groupVariables[] = $variable->getName();

            return;
        }

        if ($variable->isSuperGlobal()) {
            $groupVariables[] = $variable->getName();

            return;
        }

        if ($variable->isDoublePointer()) {
            /* Double pointers for ZE3 are used as zval * */
            $ptr = $isComplex ? $pointer : $pointer.$pointer;
            if ($variable->mustInitNull()) {
                $groupVariables[] = $ptr.$variable->getName().' = NULL';
            } else {
                $groupVariables[] = $ptr.$variable->getName();
            }

            return;
        }

        $defaultValue = $variable->getDefaultInitValue();
        if (null !== $defaultValue) {
            switch ($type) {
                case 'variable':
                case 'string':
                case 'array':
                case 'resource':
                case 'callable':
                case 'object':
                    $groupVariables[] = $pointer.$variable->getName();
                    break;

                case 'char':
                    if (\strlen($defaultValue) > 4) {
                        if (\strlen($defaultValue) > 10) {
                            throw new CompilerException("Invalid char literal: '".substr($defaultValue, 0, 10)."...'", $variable->getOriginal());
                        } else {
                            throw new CompilerException("Invalid char literal: '".$defaultValue."'", $variable->getOriginal());
                        }
                    }
                    /* no break */

                default:
                    $groupVariables[] = $pointer.$variable->getName().' = '.$defaultValue;
                    break;
            }

            return;
        }

        if ($variable->mustInitNull() && $pointer) {
            $groupVariables[] = $pointer.$variable->getName().' = NULL';

            return;
        }

        $groupVariables[] = $pointer.$variable->getName();
    }

    /**
     * {@inheritdoc}
     *
     * @param Variable[]         $variables
     * @param CompilationContext $context
     *
     * @throws CompilerException
     *
     * @return string
     */
    public function initializeVariableDefaults(array $variables, CompilationContext $context): string
    {
        $codePrinter = new CodePrinter();
        $codePrinter->increaseLevel();

        $oldCodePrinter = $context->codePrinter;
        $context->codePrinter = $codePrinter;

        $variablesManager = new VariablesManager();

        /* Initialize default values in dynamic variables */
        foreach ($variables as $variable) {
            /* Do not initialize unused variable */
            if ($variable->getNumberUses() < 1) {
                continue;
            }

            /* The default init value to be used bellow.
               Actually this value should be in array form and
               provide 'type' and 'value' keys. */
            $value = $variable->getDefaultInitValue();
            if (!\is_array($value)) {
                continue;
            }

            $variablesManager->initializeDefaults($variable, $value, $context);
        }

        $context->codePrinter = $oldCodePrinter;

        return $codePrinter->getOutput();
    }

    public function declareConstant($type, $name, $value, CompilationContext $context)
    {
        $ce = $context->classDefinition->getClassEntry($context);

        $dType = null;
        switch ($type) {
            case 'bool':
                $value = 'false' == $value ? '0' : 1;
                break;

            case 'long':
            case 'int':
                $dType = 'long';
                break;

            case 'double':
                break;

            case 'string':
            case 'char':
                if ('string' == $type || 'char' == $type) {
                    $value = '"'.add_slashes($value).'"';
                }
                $dType = 'string';
                break;
        }

        if (!isset($dType)) {
            $dType = $type;
        }

        if ('null' == $dType) {
            $context->codePrinter->output('zephir_declare_class_constant_null('.$ce.', SL("'.$name.'"));');
        } else {
            $context->codePrinter->output('zephir_declare_class_constant_'.$dType.'('.$ce.', SL("'.$name.'"), '.$value.');');
        }
    }

    /**
     * {@inheritdoc}
     *
     * @param ClassMethod        $method
     * @param CompilationContext $context
     *
     * @return string
     */
    public function getInternalSignature(ClassMethod $method, CompilationContext $context): string
    {
        if ($method->isInitializer() && !$method->isStatic()) {
            return 'zend_object *'.$method->getName().'(zend_class_entry *class_type TSRMLS_DC)';
        }

        if ($method->isInitializer() && $method->isStatic()) {
            return 'void '.$method->getName().'(TSRMLS_D)';
        }

        $signatureParameters = [];
        $parameters = $method->getParameters();
        if (\is_object($parameters)) {
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
                        $signatureParameters[] = 'zval *'.$parameter['name'].'_param_ext';
                        break;

                    default:
                        $signatureParameters[] = 'zval *'.$parameter['name'].'_ext ';
                        break;
                }
            }
        }

        $signaturePattern =
            'int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used';

        if (\count($signatureParameters)) {
            $signaturePattern = sprintf('%s, %s', $signaturePattern, implode(', ', $signatureParameters));
        }

        return "void {$method->getInternalName()}({$signaturePattern})";
    }

    /**
     * {@inheritdoc}
     *
     * @param Variable           $variable
     * @param string|Variable    $value
     * @param CompilationContext $context
     * @param bool               $useCodePrinter
     *
     * @return string
     */
    public function assignString(
        Variable $variable,
        $value,
        CompilationContext $context,
        bool $useCodePrinter = true
    ): string {
        return $this->assignHelper(
            'ZVAL_STRING',
            $this->getVariableCode($variable),
            $value,
            $context,
            $useCodePrinter
        );
    }

    /**
     * Assigns a zval to another.
     *
     * @param Variable           $variable
     * @param string             $code
     * @param CompilationContext $context
     */
    public function assignZval(Variable $variable, $code, CompilationContext $context)
    {
        $code = $this->resolveValue($code, $context);
        if (!$variable->isDoublePointer()) {
            $context->symbolTable->mustGrownStack(true);
            $symbolVariable = $this->getVariableCode($variable);
            $context->codePrinter->output('ZEPHIR_OBS_COPY_OR_DUP('.$symbolVariable.', '.$code.');');
        } else {
            $context->codePrinter->output($variable->getName().' = '.$code.';');
        }
    }

    public function returnString($value, CompilationContext $context, $useCodePrinter = true, $doCopy = true)
    {
        return $this->returnHelper('RETURN_MM_STRING', $value, $context, $useCodePrinter, null);
    }

    public function createClosure(Variable $variable, $classDefinition, CompilationContext $context)
    {
        $symbol = $this->getVariableCode($variable);
        $context->codePrinter->output(
            'zephir_create_closure_ex('.$symbol.', NULL, '.$classDefinition->getClassEntry().', SL("__invoke"));'
        );
    }

    public function addArrayEntry(Variable $variable, $key, $value, CompilationContext $context, $statement = null, $useCodePrinter = true)
    {
        $type = null;
        $keyType = 'assoc';

        if (!isset($key)) {
            $keyType = 'append';
        } elseif ($key instanceof CompiledExpression) {
            $typeKey = $key->getType();
            if ('variable' == $typeKey) {
                $var = $context->symbolTable->getVariableForRead($key->getCode(), $context);
                $typeKey = $var->getType();
            }
            if (\in_array($typeKey, ['int', 'uint', 'long', 'ulong'])) {
                $keyType = 'index';
            }
        }
        if ('null' == $value) {
            if (!isset($key)) {
                $value = $this->resolveValue('null', $context);
                $context->codePrinter->output('zephir_array_append('.$this->getVariableCode($variable).', '.$value.', PH_SEPARATE, "'.Compiler::getShortUserPath($statement['file']).'", '.$statement['line'].');');

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

        if (null === $type) {
            throw new CompilerException('Unknown type mapping: '.$value->getType());
        }

        if (isset($key)) {
            if ('variable' == $key->getType()) {
                $var = $context->symbolTable->getVariableForRead($key->getCode(), $context);
                $keyStr = 'string' == $typeKey ? 'Z_STRVAL_P('.$this->getVariableCode($var).'), Z_STRLEN_P('.$this->getVariableCode($var).')' : $this->getVariableCode($var);
            } else {
                $keyStr = 'string' == $key->getType() ? 'SL("'.$key->getCode().'")' : $key->getCode();
            }
        }

        if ('stringl' == $type) {
            if ($value instanceof Variable) {
                $valueStr = 'Z_STRVAL_P('.$this->getVariableCode($value).'), Z_STRLEN_P('.$this->getVariableCode($value).')';
            } else {
                $valueStr = 'SL("'.$value->getCode().'")';
            }
        } elseif ('zval' == $type) {
            $valueStr = $this->getVariableCode($value);
        } else {
            $valueStr = $value->getCode();
        }

        if ('assoc' == $keyType) {
            $output = 'add_assoc_'.$type.'_ex('.$this->getVariableCode($variable).', '.$keyStr.', '.$valueStr.');';
        } elseif ('append' == $keyType) {
            $output = 'zephir_array_append('.$this->getVariableCode($variable).', '.$this->resolveValue($value, $context).', PH_SEPARATE, "'.Compiler::getShortUserPath($statement['file']).'", '.$statement['line'].');';
        } else {
            $output = 'add_index_'.$type.'('.$this->getVariableCode($variable).', '.$keyStr.', '.$valueStr.');';
        }

        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $output;
    }

    public function arrayIsset(Variable $var, $resolvedExpr, $expression, CompilationContext $context)
    {
        if (!($resolvedExpr instanceof Variable)) {
            if ('string' == $resolvedExpr->getType()) {
                return new CompiledExpression('bool', 'zephir_array_isset_string('.$this->getVariableCode($var).', SL("'.$resolvedExpr->getCode().'"))', $expression);
            }
        }

        return parent::arrayIsset($var, $resolvedExpr, $expression, $context);
    }

    public function arrayIssetFetch(Variable $target, Variable $var, $resolvedExpr, $flags, $expression, CompilationContext $context)
    {
        if (!($resolvedExpr instanceof Variable)) {
            $code = $this->getVariableCode($target).', '.$this->getVariableCode($var);
            if ('string' == $resolvedExpr->getType()) {
                return new CompiledExpression('bool', 'zephir_array_isset_string_fetch('.$code.', SL("'.$resolvedExpr->getCode().'"), '.$flags.')', $expression);
            }
        }

        return parent::arrayIssetFetch($target, $var, $resolvedExpr, $flags, $expression, $context);
    }

    public function propertyIsset(Variable $var, $key, CompilationContext $context)
    {
        return new CompiledExpression('bool', 'zephir_isset_property('.$this->getVariableCode($var).', SL("'.$key.'"))', null);
    }

    public function arrayUnset(Variable $variable, $exprIndex, $flags, CompilationContext $context)
    {
        $context->headersManager->add('kernel/array');
        $variableCode = $this->getVariableCodePointer($variable);
        if ('string' == $exprIndex->getType()) {
            $context->codePrinter->output('zephir_array_unset_string('.$variableCode.', SL("'.$exprIndex->getCode().'"), '.$flags.');');

            return;
        }

        parent::arrayUnset($variable, $exprIndex, $flags, $context);
    }

    public function fetchGlobal(Variable $globalVar, CompilationContext $compilationContext, $useCodePrinter = true)
    {
        $name = $globalVar->getName();
        $output = strtr('zephir_get_global(&:name, SL(":name"));', [':name' => $name]);

        if ($useCodePrinter) {
            $compilationContext->codePrinter->output($output);
        }

        return $output;
    }

    public function fetchClass(Variable $zendClassEntry, $className, $guarded, CompilationContext $context)
    {
        $context->headersManager->add('kernel/object');
        if ($guarded) {
            $context->codePrinter->output('if (!'.$zendClassEntry->getName().') {');
        }
        $context->codePrinter->output($zendClassEntry->getName().' = zephir_fetch_class_str_ex('.$className.', ZEND_FETCH_CLASS_AUTO);');
        if ($guarded) {
            $context->codePrinter->output('}');
        }
    }

    /**
     * {@inheritdoc}
     *
     * @param Variable           $symbolVariable
     * @param Variable           $variableVariable
     * @param Variable|string    $property
     * @param bool               $readOnly
     * @param CompilationContext $context
     *
     * @return void
     */
    public function fetchProperty(
        Variable $symbolVariable,
        Variable $variableVariable,
        $property,
        $readOnly,
        CompilationContext $context
    ) {
        $flags = 'PH_NOISY_CC';
        if ($readOnly) {
            $flags .= ' | PH_READONLY';
        }

        $variableCode = $this->getVariableCode($variableVariable);
        $symbol = $this->getVariableCode($symbolVariable);

        if ($property instanceof Variable) {
            $context->codePrinter->output(
                sprintf(
                    'zephir_read_property_zval(%s, %s, %s, %s);',
                    $symbol,
                    $variableCode,
                    $this->getVariableCode($property),
                    $flags
                )
            );

            return;
        }

        $template = 'zephir_read_property(%s, %s, ZEND_STRL("%s"), %s);';
        /* Are we going to init default object property value? */
        if ($context->currentMethod && $context->currentMethod->isInitializer()) {
            $template = 'zephir_read_property_ex(%s, %s, ZEND_STRL("%s"), %s);';
        }

        $context->codePrinter->output(
            sprintf($template, $symbol, $variableCode, $property, $flags)
        );
    }

    /**
     * @param Variable        $symbolVariable
     * @param ClassDefinition $classDefinition
     * @param $property
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
     * @param $value
     * @param CompilationContext $context
     * @param bool               $usePointer
     *
     * @throws CompilerException
     *
     * @return bool|string|Variable
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
                        $this->name.': Unknown constant '.$value->getName()
                    );
            }
        }

        if ('null' == $value || 'true' == $value || 'false' == $value) {
            $varName = '__$'.$value;
            if (!$context->symbolTable->hasVariable($varName)) {
                $tempVariable = new Variable('variable', $varName, $context->branchManager->getCurrentBranch());
                $context->symbolTable->addRawVariable($tempVariable);
            }
            $tempVariable = $context->symbolTable->getVariableForWrite($varName, $context);
            $tempVariable->increaseUses();
            $tempVariable->setUsed(true, null);
            if ('null' == $value) {
                $tempVariable->setDynamicTypes('null');
            } else {
                $tempVariable->setDynamicTypes('bool');
            }
            $value = $this->getVariableCode($tempVariable);
        } else {
            if ($value instanceof CompiledExpression) {
                if ('array' == $value->getType()) {
                    $value = $context->symbolTable->getVariableForWrite($value->getCode(), $context, null);
                } elseif ('variable' == $value->getType()) {
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

    /**
     * {@inheritdoc}
     *
     * @param Variable           $variable
     * @param string|Variable    $property
     * @param mixed              $value
     * @param CompilationContext $context
     *
     * @return void
     */
    public function updateProperty(Variable $variable, $property, $value, CompilationContext $context)
    {
        $value = $this->resolveValue($value, $context);

        if ($property instanceof Variable) {
            $context->codePrinter->output(
                sprintf(
                    'zephir_update_property_zval_zval(%s, %s, %s);',
                    $this->getVariableCode($variable),
                    $this->getVariableCode($property),
                    $value
                )
            );

            return;
        }

        $template = 'zephir_update_property_zval(%s, ZEND_STRL("%s"), %s);';
        /* Are we going to init default object property value? */
        if ($context->currentMethod && $context->currentMethod->isInitializer()) {
            $template = 'zephir_update_property_zval_ex(%s, ZEND_STRL("%s"), %s);';
        }

        $context->codePrinter->output(
            sprintf($template, $this->getVariableCode($variable), $property, $value)
        );
    }

    public function updateStaticProperty($classEntry, $property, $value, CompilationContext $context)
    {
        // TODO(serghei): Sort out as well as above
        $value = $this->resolveValue($value, $context);
        $context->codePrinter->output('zephir_update_static_property_ce('.$classEntry.', ZEND_STRL("'.$property.'"), '.$value.');');
    }

    public function addStaticProperty($classEntry, $property, $value, CompilationContext $context)
    {
        $value = $this->resolveValue($value, $context);
        $context->codePrinter->output('zephir_add_static_property_ce('.$classEntry.', ZEND_STRL("'.$property.'"), '.$value.');');
    }

    public function subStaticProperty($classEntry, $property, $value, CompilationContext $context)
    {
        $value = $this->resolveValue($value, $context);
        $context->codePrinter->output('zephir_sub_static_property_ce('.$classEntry.', ZEND_STRL("'.$property.'"), '.$value.');');
    }

    public function assignArrayProperty(Variable $variable, $property, $key, $value, CompilationContext $context)
    {
        $resolveValue = $this->resolveValue($value, $context);
        if (isset($key)) {
            $context->codePrinter->output('zephir_update_property_array('.$this->getVariableCode($variable).', SL("'.$property.'"), '.$this->getVariableCode($key).', '.$resolveValue.');');
        } else {
            $context->codePrinter->output('zephir_update_property_array_append('.$this->getVariableCode($variable).', SL("'.$property.'"), '.$resolveValue.');');
        }
    }

    public function callMethod($symbolVariable, Variable $variable, $methodName, $cachePointer, $params, CompilationContext $context)
    {
        $paramStr = null != $params ? ', '.implode(', ', $params) : '';
        $macro = 'CALL_METHOD';
        if ($methodName instanceof Variable) {
            $macro = 'CALL_METHOD_ZVAL';
            $methodName = $this->getVariableCode($methodName);
        } else {
            $methodName = '"'.$methodName.'"';
        }
        if (!isset($symbolVariable)) {
            $context->codePrinter->output('ZEPHIR_'.$macro.'(NULL, '.$this->getVariableCode($variable).', '.$methodName.', '.$cachePointer.$paramStr.');');
        } elseif ('return_value' == $symbolVariable->getName()) {
            $context->codePrinter->output('ZEPHIR_RETURN_'.$macro.'('.$this->getVariableCode($variable).', '.$methodName.', '.$cachePointer.$paramStr.');');
        } else {
            $symbol = $this->getVariableCode($symbolVariable);
            $context->codePrinter->output('ZEPHIR_'.$macro.'('.$symbol.', '.$this->getVariableCode($variable).', '.$methodName.', '.$cachePointer.$paramStr.');');
        }
    }

    public function setSymbolIfSeparated(Variable $variableTempSeparated, Variable $variable, CompilationContext $context)
    {
        $context->codePrinter->output('if ('.$variableTempSeparated->getName().') {');
        $context->codePrinter->output("\t".'ZEPHIR_SET_SYMBOL(&EG(symbol_table), "'.$variable->getName().'", &'.$variable->getName().');');
        $context->codePrinter->output('}');
    }

    public function copyOnWrite(Variable $target, $var, CompilationContext $context)
    {
        if ('EG(exception)' == $var) {
            $context->codePrinter->output('ZVAL_OBJ('.$this->getVariableCode($target).', EG(exception));');
            $context->codePrinter->output('Z_ADDREF_P('.$this->getVariableCode($target).');');

            return;
        }

        $globalsManager = new Globals();

        if ($globalsManager->isSuperGlobal($target->getName())) {
            $context->codePrinter->output(sprintf(
                'ZEPHIR_HASH_COPY(%s, %s);',
                $this->getVariableCode($target),
                $this->resolveValue($var, $context)
            ));
        } else {
            $context->codePrinter->output(sprintf(
                'ZEPHIR_CPY_WRT(%s, %s);',
                $this->getVariableCode($target),
                $this->resolveValue($var, $context)
            ));
        }
    }

    public function forStatement(Variable $exprVariable, $keyVariable, $variable, $duplicateKey, $duplicateHash, $statement, $statementBlock, CompilationContext $compilationContext)
    {
        /*
         * Create a hash table and hash pointer temporary variables.
         */
        //$arrayPointer = $compilationContext->symbolTable->addTemp('HashPosition', $compilationContext);
        //$arrayHash = $compilationContext->symbolTable->addTemp('HashTable', $compilationContext);
        /*
         * Create a temporary zval to fetch the items from the hash.
         */
        $compilationContext->headersManager->add('kernel/fcall');
        $compilationContext->symbolTable->mustGrownStack(true);
        if (!$compilationContext->symbolTable->hasVariable('ZEPHIR_LAST_CALL_STATUS')) {
            $callStatus = new Variable('int', 'ZEPHIR_LAST_CALL_STATUS', $compilationContext->branchManager->getCurrentBranch());
            $callStatus->setIsInitialized(true, $compilationContext);
            $callStatus->increaseUses();
            $callStatus->setReadOnly(true);
            $compilationContext->symbolTable->addRawVariable($callStatus);
        }
        $tempVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
        $tempVariable->setIsDoublePointer(true);
        $tempValidVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
        $codePrinter = $compilationContext->codePrinter;

        $codePrinter->output('zephir_is_iterable('.$this->getVariableCode($exprVariable).', '.$duplicateHash.', "'.Compiler::getShortUserPath($statement['file']).'", '.$statement['line'].');');

        $codePrinter->output('if (Z_TYPE_P('.$this->getVariableCode($exprVariable).') == IS_ARRAY) {');
        $codePrinter->increaseLevel();

        $macro = null;
        $reverse = $statement['reverse'] ? 'REVERSE_' : '';

        if (isset($keyVariable)) {
            $arrayNumKey = $compilationContext->symbolTable->addTemp('zend_ulong', $compilationContext);
            $arrayStrKey = $compilationContext->symbolTable->addTemp('zend_string', $compilationContext);
        }

        if (isset($keyVariable) && isset($variable)) {
            $macro = 'ZEND_HASH_'.$reverse.'FOREACH_KEY_VAL';
            $codePrinter->output($macro.'(Z_ARRVAL_P('.$this->getVariableCode($exprVariable).'), '.$arrayNumKey->getName().', '.$arrayStrKey->getName().', '.$tempVariable->getName().')');
        } elseif (isset($keyVariable)) {
            $macro = 'ZEND_HASH_'.$reverse.'FOREACH_KEY';
            $codePrinter->output($macro.'(Z_ARRVAL_P('.$this->getVariableCode($exprVariable).'), '.$arrayNumKey->getName().', '.$arrayStrKey->getName().')');
        } else {
            $macro = 'ZEND_HASH_'.$reverse.'FOREACH_VAL';
            $codePrinter->output($macro.'(Z_ARRVAL_P('.$this->getVariableCode($exprVariable).'), '.$tempVariable->getName().')');
        }

        $codePrinter->output('{');

        if (isset($keyVariable)) {
            $codePrinter->increaseLevel();
            if ($duplicateKey) {
                $compilationContext->symbolTable->mustGrownStack(true);
                $keyVariable->initVariant($compilationContext);
            }
            $codePrinter->output('if ('.$arrayStrKey->getName().' != NULL) { ');
            $codePrinter->increaseLevel();
            if ($duplicateKey) {
                $codePrinter->output('ZVAL_STR_COPY('.$this->getVariableCode($keyVariable).', '.$arrayStrKey->getName().');');
            } else {
                $codePrinter->output('ZVAL_STR('.$this->getVariableCode($keyVariable).', '.$arrayStrKey->getName().');');
            }
            $codePrinter->decreaseLevel();
            $codePrinter->output('} else {');
            $codePrinter->increaseLevel();
            $codePrinter->output('ZVAL_LONG('.$this->getVariableCode($keyVariable).', '.$arrayNumKey->getName().');');
            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
            $codePrinter->decreaseLevel();
        }

        if (isset($variable)) {
            $compilationContext->symbolTable->mustGrownStack(true);
            $codePrinter->increaseLevel();
            $variable->initVariant($compilationContext);
            $codePrinter->output('ZVAL_COPY('.$this->getVariableCode($variable).', '.$this->getVariableCode($tempVariable).');');
            $codePrinter->decreaseLevel();
        }

        /*
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
        $codePrinter->decreaseLevel();

        $codePrinter->output('} else {');
        $codePrinter->increaseLevel();

        $codePrinter->output('ZEPHIR_CALL_METHOD(NULL, '.$this->getVariableCode($exprVariable).', "rewind", NULL, 0);');
        $codePrinter->output('zephir_check_call_status();');

        $codePrinter->output('while (1) {');
        $codePrinter->increaseLevel();

        $codePrinter->output('ZEPHIR_CALL_METHOD(&'.$tempValidVariable->getName().', '.$this->getVariableCode($exprVariable).', "valid", NULL, 0);');
        $codePrinter->output('zephir_check_call_status();');
        $codePrinter->output('if (!zend_is_true(&'.$tempValidVariable->getName().')) {');
        $codePrinter->increaseLevel();
        $codePrinter->output('break;');
        $codePrinter->decreaseLevel();
        $codePrinter->output('}');

        if (isset($keyVariable)) {
            $codePrinter->output('ZEPHIR_CALL_METHOD('.$this->getVariableCode($keyVariable).', '.$this->getVariableCode($exprVariable).', "key", NULL, 0);');
            $codePrinter->output('zephir_check_call_status();');
        }

        if (isset($variable)) {
            $codePrinter->output('ZEPHIR_CALL_METHOD('.$this->getVariableCode($variable).', '.$this->getVariableCode($exprVariable).', "current", NULL, 0);');
            $codePrinter->output('zephir_check_call_status();');
        }

        if (isset($statement['statements'])) {
            $statementBlock->isLoop(true);
            if (isset($statement['key'])) {
                $statementBlock->getMutateGatherer()->increaseMutations($statement['key']);
            }
            $statementBlock->getMutateGatherer()->increaseMutations($statement['value']);
            $statementBlock->compile($compilationContext);
        }

        $codePrinter->output('ZEPHIR_CALL_METHOD(NULL, '.$this->getVariableCode($exprVariable).', "next", NULL, 0);');
        $codePrinter->output('zephir_check_call_status();');

        $codePrinter->decreaseLevel();
        $codePrinter->output('}');

        $codePrinter->decreaseLevel();
        $codePrinter->output('}');

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
        $compilationContext->codePrinter->output('ZEPHIR_ITERATOR_COPY('.$this->getVariableCode($targetVariable).', '.$iteratorVariable->getName().');');
    }

    public function destroyIterator(Variable $iteratorVariable, CompilationContext $context)
    {
        $context->codePrinter->output('zend_iterator_dtor('.$iteratorVariable->getName().');');
    }

    public function ifVariableValueUndefined(Variable $var, CompilationContext $context, $useBody = false, $useCodePrinter = true)
    {
        if ($var->isDoublePointer()) {
            return parent::ifVariableValueUndefined($var, $context, $useBody, $useCodePrinter);
        }
        $body = 'Z_TYPE_P('.$this->getVariableCode($var).') == IS_UNDEF';
        $output = 'if ('.$body.') {';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $useBody ? $body : $output;
    }

    public function fetchClassEntry($str)
    {
        return 'zephir_get_internal_ce(SL("'.$str.'"))';
    }

    /**
     * {@inheritdoc}
     *
     * @param string             $type
     * @param CompilationContext $context
     * @param bool               $isLocal
     *
     * @return Variable
     */
    public function getScalarTempVariable(
        string $type,
        CompilationContext $context,
        $isLocal = true
    ): Variable {
        return $context->symbolTable->getTempNonTrackedVariable($type, $context);
    }

    /**
     * {@inheritdoc}
     *
     * @param Variable           $variable
     * @param CompilationContext $context
     * @param int                $size
     *
     * @return void
     */
    public function initArray(Variable $variable, CompilationContext $context, int $size = null)
    {
        $code = $this->getVariableCode($variable);

        if (null === $size) {
            $output = "array_init({$code});";
        } else {
            $output = "zephir_create_array({$code}, {$size}, 0);";
        }

        $context->codePrinter->output($output);
    }
}
