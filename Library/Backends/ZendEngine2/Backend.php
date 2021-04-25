<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Backends\ZendEngine2;

use Zephir\BaseBackend;
use Zephir\ClassMethod;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Compiler;
use Zephir\Exception\CompilerException;
use Zephir\Fcall\FcallManagerInterface;
use Zephir\GlobalConstant;
use Zephir\Variable;

/**
 * Zephir\Backends\ZendEngine2\Backend.
 *
 * NOTE: ZendEngine2 backend is no longer supported
 * and this class will be removed in the future.
 */
class Backend extends BaseBackend
{
    protected $name = 'ZendEngine2';

    /**
     * {@inheritdoc}
     *
     * @return bool
     */
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
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    /**
     * {@inheritdoc}
     */
    public function getVariableCodePointer(Variable $variable)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    /**
     * {@inheritdoc}
     */
    public function getVariableCode(Variable $variable): string
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function getBoolCode(Variable $variable, CompilationContext $context, $useCodePrinter = true)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    /**
     * {@inheritdoc}
     */
    public function getStringsManager()
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function getTypeDefinition($type)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    /**
     * {@inheritdoc}
     */
    public function getTypeofCondition(
        Variable $variableVariable,
        string $operator,
        string $value,
        CompilationContext $context
    ): string {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function onPreInitVar(ClassMethod $method, CompilationContext $context): string
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function onPreCompile(ClassMethod $method, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function onPostCompile(ClassMethod $method, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function generateInitCode(&$groupVariables, $type, $pointer, Variable $variable)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    /**
     * {@inheritdoc}
     */
    public function initializeVariableDefaults(array $variables, CompilationContext $context): string
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function declareConstant($type, $name, $value, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function declareVariables($method, $typeToVariables, CompilationContext $compilationContext)
    {
        $varInitCode = [];
        $additionalCode = $method ? $this->onPreInitVar($method, $compilationContext) : '';

        foreach ($typeToVariables as $type => $variables) {
            list($pointer, $code) = $this->getTypeDefinition($type);
            $code .= ' ';
            $groupVariables = [];

            /**
             * @var Variable[]
             */
            foreach ($variables as $variable) {
                $nextCode = $this->generateInitCode($groupVariables, $type, $pointer, $variable);
                if ($nextCode && $additionalCode) {
                    $additionalCode .= PHP_EOL.$nextCode;
                } else {
                    $additionalCode .= $nextCode;
                }
            }

            $varInitCode[] = $code.implode(', ', $groupVariables).';';
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
        $code = $macro.'('.$this->getVariableCode($variable).');';
        if ($useCodePrinter) {
            $context->codePrinter->output($code);
        }

        return $code;
    }

    /**
     * {@inheritdoc}
     */
    public function getInternalSignature(ClassMethod $method, CompilationContext $context): string
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
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
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
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
        $output = 'ZVAL_NULL('.$this->getVariableCode($variable).');';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $output;
    }

    public function assignZval(Variable $variable, $code, CompilationContext $context)
    {
        $code = $this->resolveValue($code, $context);
        $context->codePrinter->output($this->getVariableCode($variable).' = '.$code.';');
    }

    public function concatSelf(Variable $variable, Variable $itemVariable, CompilationContext $context)
    {
        $variable = $this->getVariableCodePointer($variable);
        $itemVariable = $this->getVariableCode($itemVariable);
        $context->codePrinter->output('zephir_concat_self('.$variable.', '.$itemVariable.');');
    }

    public function initArray(Variable $variable, CompilationContext $context, int $size = null)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function createClosure(Variable $variable, $classDefinition, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
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
                $context->codePrinter->output('zephir_array_append('.$this->getVariableCodePointer($variable).', ZEPHIR_GLOBAL(global_null), PH_SEPARATE, "'.Compiler::getShortUserPath($statement['file']).'", '.$statement['line'].');');

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
            throw new CompilerException('Unknown type mapping: '.$value->getType());
        }

        if (isset($key)) {
            if ('variable' == $key->getType()) {
                $keyStr = 'string' == $typeKey ? 'Z_STRVAL_P('.$this->getVariableCode($var).'), Z_STRLEN_P('.$this->getVariableCode($var).') + 1' : $this->getVariableCode($var);
            } else {
                $keyStr = 'string' == $key->getType() ? 'SS("'.$key->getCode().'")' : $key->getCode();
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
        $doCopy = 'stringl' == $type ? ', 1' : '';

        if ('assoc' == $keyType) {
            $output = 'add_assoc_'.$type.'_ex('.$this->getVariableCode($variable).', '.$keyStr.', '.$valueStr.$doCopy.');';
        } elseif ('append' == $keyType) {
            $output = 'zephir_array_append('.$this->getVariableCodePointer($variable).', '.$this->resolveValue($value, $context).', PH_SEPARATE, "'.Compiler::getShortUserPath($statement['file']).'", '.$statement['line'].');';
        } else {
            $output = 'add_index_'.$type.'('.$this->getVariableCode($variable).', '.$keyStr.', '.$valueStr.$doCopy.');';
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
                    $compilationContext->codePrinter->output('zephir_array_update_zval('.$this->getVariableCodePointer($symbolVariable).', '.$this->getVariableCode($key).', '.$value.', '.$flags.');');
                    break;

                case 'int':
                case 'uint':
                case 'long':
                    $compilationContext->codePrinter->output('zephir_array_update_long('.$this->getVariableCodePointer($symbolVariable).', '.$key->getName().', '.$value.', '.$flags.' ZEPHIR_DEBUG_PARAMS_DUMMY);');
                    break;

                default:
                    throw new CompilerException('updateArray: Found a variable with unsupported type '.$key->getType());
            }
        } elseif ($key instanceof CompiledExpression) {
            switch ($key->getType()) {
                case 'string':
                    $compilationContext->codePrinter->output('zephir_array_update_string('.$this->getVariableCodePointer($symbolVariable).', SL("'.$key->getCode().'"), '.$value.', '.$flags.');');
                    break;

                case 'int':
                    $compilationContext->codePrinter->output('zephir_array_update_long('.$this->getVariableCodePointer($symbolVariable).', '.$key->getCode().', '.$value.', '.$flags.' ZEPHIR_DEBUG_PARAMS_DUMMY);');
                    break;

                case 'variable':
                    $this->updateArray($symbolVariable, $compilationContext->symbolTable->getVariableForRead($key->getCode()), $value, $compilationContext, $flags);
                    break;

                default:
                    throw new CompilerException('updateArray: Found an expression with unsupported type '.$key->getType());
            }
        } else {
            throw new CompilerException('?');
        }
    }

    public function initObject(Variable $variable, $ce, CompilationContext $context, $useCodePrinter = true)
    {
        $variableAccess = $this->getVariableCode($variable);
        if (!isset($ce)) {
            $output = 'object_init('.$variableAccess.');';
        } else {
            $output = 'object_init_ex('.$variableAccess.', '.$ce.');';
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
                throw new CompilerException(
                    sprintf('Variable type: %s cannot be used as array index without cast', $index->getType()),
                    $arrayAccess['right']
                );
        }
        if ($isVariable && \in_array($index->getType(), ['variable', 'string'])) {
            $output = 'zephir_array_fetch('.$this->getVariableCodePointer($var).', '.$this->getVariableCode($src).', '.$this->getVariableCode($index).', '.$flags.', "'.Compiler::getShortUserPath($arrayAccess['file']).'", '.$arrayAccess['line'].');';
        } else {
            if ($isVariable) {
                $indexAccess = $this->getVariableCode($index);
            } else {
                $indexAccess = $index->getCode();
                if ('string' == $type) {
                    $indexAccess = 'SL("'.$indexAccess.'")';
                }
            }
            $output = 'zephir_array_fetch_'.$type.'('.$this->getVariableCodePointer($var).', '.$this->getVariableCode($src).', '.$indexAccess.', '.$flags.', "'.Compiler::getShortUserPath($arrayAccess['file']).'", '.$arrayAccess['line'].');';
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
                return new CompiledExpression('bool', 'zephir_array_isset_string('.$this->getVariableCode($var).', SS("'.$resolvedExpr->getCode().'"))', $expression);
            } else {
                return new CompiledExpression('bool', 'zephir_array_isset_long('.$this->getVariableCode($var).', '.$resolvedExpr->getCode().')', $expression);
            }
        }

        if ('int' == $resolvedExpr->getType() || 'long' == $resolvedExpr->getType()) {
            return new CompiledExpression('bool', 'zephir_array_isset_long('.$this->getVariableCode($var).', '.$this->getVariableCode($resolvedExpr).')', $expression);
        } elseif ('variable' == $resolvedExpr->getType() || 'string' == $resolvedExpr->getType()) {
            return new CompiledExpression('bool', 'zephir_array_isset('.$this->getVariableCode($var).', '.$this->getVariableCode($resolvedExpr).')', $expression);
        }

        throw new CompilerException('['.$resolvedExpr->getType().']', $expression);
    }

    public function arrayIssetFetch(Variable $target, Variable $var, $resolvedExpr, $flags, $expression, CompilationContext $context)
    {
        $code = $this->getVariableCodePointer($target).', '.$this->getVariableCode($var);

        if (!($resolvedExpr instanceof Variable)) {
            if ('string' == $resolvedExpr->getType()) {
                return new CompiledExpression('bool', 'zephir_array_isset_string_fetch('.$code.', SS("'.$resolvedExpr->getCode().'"), '.$flags.')', $expression);
            } elseif (\in_array($resolvedExpr->getType(), ['int', 'uint', 'long'])) {
                return new CompiledExpression('bool', 'zephir_array_isset_long_fetch('.$code.', '.$resolvedExpr->getCode().', '.$flags.')', $expression);
            } else {
                $resolvedExpr = $context->symbolTable->getVariableForRead($resolvedExpr->getCode(), $context);
            }
        }

        if ('int' == $resolvedExpr->getType() || 'long' == $resolvedExpr->getType()) {
            return new CompiledExpression('bool', 'zephir_array_isset_long_fetch('.$code.', '.$this->getVariableCode($resolvedExpr).', '.$flags.')', $expression);
        } elseif ('variable' == $resolvedExpr->getType() || 'string' == $resolvedExpr->getType()) {
            return new CompiledExpression('bool', 'zephir_array_isset_fetch('.$code.', '.$this->getVariableCode($resolvedExpr).', '.$flags.')', $expression);
        }
        throw new CompilerException('arrayIssetFetch ['.$resolvedExpr->getType().']', $expression);
    }

    public function propertyIsset(Variable $var, $key, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function arrayUnset(Variable $variable, $exprIndex, $flags, CompilationContext $context)
    {
        $context->headersManager->add('kernel/array');
        $variableCode = $this->getVariableCodePointer($variable);
        switch ($exprIndex->getType()) {
            case 'int':
            case 'uint':
            case 'long':
                $context->codePrinter->output('zephir_array_unset_long('.$variableCode.', '.$exprIndex->getCode().', '.$flags.');');
                break;

            case 'string':
                $context->codePrinter->output('zephir_array_unset_string('.$variableCode.', SS("'.$exprIndex->getCode().'"), '.$flags.');');
                break;

            case 'variable':
                $variableIndex = $context->symbolTable->getVariableForRead($exprIndex->getCode(), $context, $exprIndex->getOriginal());
                $indexCode = $this->getVariableCode($variableIndex);
                switch ($variableIndex->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                        $context->codePrinter->output('zephir_array_unset_long('.$variableCode.', '.$indexCode.', '.$flags.');');
                        break;

                    case 'string':
                    case 'variable':
                        $context->codePrinter->output('zephir_array_unset('.$variableCode.', '.$indexCode.', '.$flags.');');
                        break;

                    default:
                        throw new CompilerException('Variable type: '.$variableIndex->getType().' cannot be used as array index without cast');
                }
                break;

            default:
                throw new CompilerException('Cannot use expression: '.$exprIndex->getType().' as array index without cast');
        }
    }

    public function assignArrayMulti(Variable $variable, $symbolVariable, $offsetExprs, CompilationContext $compilationContext)
    {
        list($keys, $offsetItems, $numberParams) = $this->resolveOffsetExprs($offsetExprs, $compilationContext);

        $symbol = $this->resolveValue($symbolVariable, $compilationContext, true);
        $varCode = $this->getVariableCodePointer($variable);
        $compilationContext->codePrinter->output(
            sprintf(
                'zephir_array_update_multi(%s, %s, SL("%s"), %d, %s);',
                $varCode,
                $symbol,
                $keys,
                $numberParams,
                implode(', ', $offsetItems)
            )
        );
    }

    public function assignPropertyArrayMulti(Variable $variable, $valueVariable, $propertyName, $offsetExprs, CompilationContext $compilationContext)
    {
        list($keys, $offsetItems, $numberParams) = $this->resolveOffsetExprs($offsetExprs, $compilationContext);
        $valueVariable = $this->resolveValue($valueVariable, $compilationContext, true);

        $compilationContext->codePrinter->output(
            sprintf(
                'zephir_update_property_array_multi(%s, SL("%s"), %s, SL("%s"), %d, %s);',
                $variable->getName(),
                $propertyName,
                $valueVariable,
                $keys,
                $numberParams,
                implode(', ', $offsetItems)
            )
        );
    }

    public function assignStaticPropertyArrayMulti($classEntry, $valueVariable, $propertyName, $offsetExprs, CompilationContext $compilationContext)
    {
        list($keys, $offsetItems, $numberParams) = $this->resolveOffsetExprs($offsetExprs, $compilationContext);
        $valueVariable = $this->resolveValue($valueVariable, $compilationContext, true);

        $offsetStr = $offsetItems ? ', '.implode(', ', $offsetItems) : '';
        $compilationContext->codePrinter->output(
            sprintf(
                'zephir_update_static_property_array_multi_ce(%s, SL("%s"), %s, SL("%s"), %d%s);',
                $classEntry,
                $propertyName,
                $valueVariable,
                $keys,
                $numberParams,
                $offsetStr
            )
        );
    }

    public function fetchGlobal(Variable $globalVar, CompilationContext $compilationContext, $useCodePrinter = true)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function fetchClass(Variable $zendClassEntry, $className, $guarded, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function fetchProperty(
        Variable $symbolVariable,
        Variable $variableVariable,
        $property,
        $readOnly,
        CompilationContext $context
    ) {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function fetchStaticProperty(
        Variable $symbolVariable,
        $classDefinition,
        $property,
        $readOnly,
        CompilationContext $context
    ) {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function resolveValue($value, CompilationContext $context, $usePointer = false)
    {
        if ('null' == $value) {
            $value = ($usePointer ? '&' : '').'ZEPHIR_GLOBAL(global_null)';
        } elseif ('true' == $value) {
            $value = ($usePointer ? '&' : '').'ZEPHIR_GLOBAL(global_true)';
        } elseif ('false' == $value) {
            $value = ($usePointer ? '&' : '').'ZEPHIR_GLOBAL(global_false)';
        } elseif ($value instanceof GlobalConstant) {
            return ($usePointer ? '&' : '').$value->getName();
        } elseif ($value instanceof CompiledExpression) {
            if ('array' == $value->getType()) {
                $var = $context->symbolTable->getVariableForWrite($value->getCode(), $context, null);
                $value = $usePointer ? $this->getVariableCodePointer($var) : $this->getVariableCode($var);
            } elseif ('variable' == $value->getType()) {
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
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function updateStaticProperty($classEntry, $property, $value, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function addStaticProperty($classEntry, $property, $value, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function subStaticProperty($classEntry, $property, $value, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function assignArrayProperty(Variable $variable, $property, $key, $value, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function checkConstructor(Variable $var, CompilationContext $context)
    {
        $context->codePrinter->output('if (zephir_has_constructor('.$this->getVariableCode($var).')) {');
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
            $context->codePrinter->output('ZEPHIR_'.$macro.'(NULL, '.$variable->getName().', '.$methodName.', '.$cachePointer.$paramStr.');');
        } elseif ('return_value' == $symbolVariable->getName()) {
            $context->codePrinter->output('ZEPHIR_RETURN_'.$macro.'('.$variable->getName().', '.$methodName.', '.$cachePointer.$paramStr.');');
        } else {
            $context->codePrinter->output('ZEPHIR_'.$macro.'(&'.$symbolVariable->getName().', '.$variable->getName().', '.$methodName.', '.$cachePointer.$paramStr.');');
        }
    }

    public function callDynamicFunction($symbolVariable, Variable $variable, CompilationContext $context, $params = [], $cache = 'NULL', $cacheSlot = 0)
    {
        $paramStr = null != $params ? ', '.implode(', ', $params) : '';
        if (!isset($symbolVariable)) {
            $context->codePrinter->output('ZEPHIR_CALL_ZVAL_FUNCTION(NULL, '.$this->getVariableCode($variable).', '.$cache.', '.$cacheSlot.$paramStr.');');
        } elseif ('return_value' == $symbolVariable->getName()) {
            $context->codePrinter->output('ZEPHIR_RETURN_CALL_ZVAL_FUNCTION('.$this->getVariableCode($variable).', '.$cache.', '.$cacheSlot.$paramStr.');');
        } else {
            $context->codePrinter->output('ZEPHIR_CALL_ZVAL_FUNCTION('.$this->getVariableCodePointer($symbolVariable).', '.$this->getVariableCode($variable).', '.$cache.', '.$cacheSlot.$paramStr.');');
        }
    }

    public function zvalOperator($zvalOperator, Variable $expected, Variable $variableLeft, Variable $variableRight, CompilationContext $compilationContext)
    {
        $expected = $this->getVariableCode($expected);
        $op1 = $this->getVariableCode($variableLeft);
        $op2 = $this->getVariableCode($variableRight);

        $compilationContext->codePrinter->output($zvalOperator.'('.$expected.', '.$op1.', '.$op2.');');
    }

    public function maybeSeparate(Variable $variableTempSeparated, Variable $variable, CompilationContext $context)
    {
        $context->codePrinter->output($variableTempSeparated->getName().' = zephir_maybe_separate_zval('.$this->getVariableCodePointer($variable).');');
    }

    public function setSymbolIfSeparated(Variable $variableTempSeparated, Variable $variable, CompilationContext $context)
    {
        $context->codePrinter->output('if ('.$variableTempSeparated->getName().') {');
        $context->codePrinter->output("\t".'ZEND_SET_SYMBOL(&EG(symbol_table), "'.$variable->getName().'", '.$variable->getName().');');
        $context->codePrinter->output('}');
    }

    public function copyOnWrite(Variable $target, $var, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function forStatement(Variable $exprVariable, $keyVariable, $variable, $duplicateKey, $duplicateHash, $statement, $statementBlock, CompilationContext $compilationContext)
    {
        /*
         * Create a hash table and hash pointer temporary variables.
         */
        $arrayPointer = $compilationContext->symbolTable->addTemp('HashPosition', $compilationContext);
        $arrayHash = $compilationContext->symbolTable->addTemp('HashTable', $compilationContext);
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
        $tempValidVariable->setMustInitNull(true);
        $codePrinter = $compilationContext->codePrinter;

        $codePrinter->output('if (Z_TYPE_P('.$this->getVariableCode($exprVariable).') == IS_ARRAY) {');
        $codePrinter->increaseLevel();

        $reverse = $statement['reverse'] ? 1 : 0;

        $codePrinter->output('zephir_is_iterable('.$this->getVariableCode($exprVariable).', &'.$arrayHash->getName().', &'.$arrayPointer->getName().', '.$duplicateHash.', '.$reverse.', "'.Compiler::getShortUserPath($statement['file']).'", '.$statement['line'].');');

        $codePrinter->output('for (');
        $codePrinter->output('  ; zend_hash_get_current_data_ex('.$arrayHash->getName().', (void**) &'.$tempVariable->getName().', &'.$arrayPointer->getName().') == SUCCESS');
        if ($reverse) {
            $codePrinter->output('  ; zend_hash_move_backwards_ex('.$arrayHash->getName().', &'.$arrayPointer->getName().')');
        } else {
            $codePrinter->output('  ; zend_hash_move_forward_ex('.$arrayHash->getName().', &'.$arrayPointer->getName().')');
        }
        $codePrinter->output(') {');

        if (isset($keyVariable)) {
            if ($duplicateKey) {
                $compilationContext->symbolTable->mustGrownStack(true);
                $codePrinter->output("\t".'ZEPHIR_GET_HMKEY('.$keyVariable->getName().', '.$arrayHash->getName().', '.$arrayPointer->getName().');');
            } else {
                $codePrinter->output("\t".'ZEPHIR_GET_HKEY('.$keyVariable->getName().', '.$arrayHash->getName().', '.$arrayPointer->getName().');');
            }
        }

        if (isset($variable)) {
            $compilationContext->symbolTable->mustGrownStack(true);
            $codePrinter->output("\t".'ZEPHIR_GET_HVALUE('.$variable->getName().', '.$tempVariable->getName().');');
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

        $codePrinter->output('}');
        /* Make sure to free the duplicated hash table */
        if ($duplicateHash) {
            $codePrinter->output('zend_hash_destroy('.$arrayHash->getName().');');
            $codePrinter->output('FREE_HASHTABLE('.$arrayHash->getName().');');
        }
        $codePrinter->decreaseLevel();

        $codePrinter->output('} else {');
        $codePrinter->increaseLevel();

        $codePrinter->output('ZEPHIR_CALL_METHOD(NULL, '.$this->getVariableCode($exprVariable).', "rewind", NULL, 0);');
        $codePrinter->output('zephir_check_call_status();');

        $codePrinter->output('while (1) {');
        $codePrinter->increaseLevel();
        $compilationContext->headersManager->add('kernel/operators');

        $codePrinter->output('ZEPHIR_CALL_METHOD(&'.$tempValidVariable->getName().', '.$this->getVariableCode($exprVariable).', "valid", NULL, 0);');
        $codePrinter->output('zephir_check_call_status();');
        $codePrinter->output('if (!zephir_is_true('.$tempValidVariable->getName().')) {');
        $codePrinter->increaseLevel();
        $codePrinter->output('break;');
        $codePrinter->decreaseLevel();
        $codePrinter->output('}');

        if (isset($keyVariable)) {
            $codePrinter->output('ZEPHIR_CALL_METHOD(&'.$this->getVariableCode($keyVariable).', '.$this->getVariableCode($exprVariable).', "key", NULL, 0);');
            $codePrinter->output('zephir_check_call_status();');
        }

        if (isset($variable)) {
            $codePrinter->output('ZEPHIR_CALL_METHOD(&'.$this->getVariableCode($variable).', '.$this->getVariableCode($exprVariable).', "current", NULL, 0);');
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
    }

    public function forStatementIterator(Variable $iteratorVariable, Variable $targetVariable, CompilationContext $compilationContext)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function destroyIterator(Variable $iteratorVariable, CompilationContext $context)
    {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    public function ifVariableValueUndefined(Variable $var, CompilationContext $context, $onlyBody = false, $useCodePrinter = true)
    {
        $body = '!'.$var->getName();
        $output = 'if ('.$body.') {';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $onlyBody ? $body : $output;
    }

    public function checkStrictType($type, $var, CompilationContext $context)
    {
        $codePrinter = $context->codePrinter;
        $conditions = [];

        $inputParamVariable = $context->symbolTable->getVariableForWrite($var['name'], $context);
        $inputParamCode = $this->getVariableCode($inputParamVariable);
        $cond = 'Z_TYPE_P('.$inputParamCode.') != ';
        if ($context->symbolTable->hasVariable($var['name'].'_param')) {
            $parameterVariable = $context->symbolTable->getVariableForWrite($var['name'].'_param', $context);
            $parameterCode = $context->backend->getVariableCode($parameterVariable);
            $cond = 'Z_TYPE_P('.$parameterCode.') != ';
        }

        switch ($type) {
            case 'int':
            case 'uint':
            case 'long':
                $conditions[] = $cond.'IS_LONG';
                break;
            case 'bool':
                if (!$this->isZE3()) {
                    $conditions[] = $cond.'IS_BOOL';
                } else {
                    $conditions[] = $cond.'IS_TRUE';
                    $conditions[] = $cond.'IS_FALSE';
                }
                break;
            case 'double':
                $conditions[] = $cond.'IS_DOUBLE';
                break;
            case 'string':
            case 'ulong':
                $conditions[] = $cond.'IS_STRING';
                $conditions[] = $cond.'IS_NULL';
                break;
            case 'array':
                break;
            case 'object':
            case 'resource':
                $conditions[] = $cond.'IS_'.strtoupper($type);
                break;
            case 'callable':
                $conditions[] = 'zephir_is_callable('.$inputParamCode.') != 1';
                break;
            default:
                throw new CompilerException('Unknown type '.$type);
        }

        /* Generate verification code */
        if (\count($conditions)) {
            $codePrinter->output('if (UNEXPECTED('.implode(' && ', $conditions).')) {');
            $codePrinter->increaseLevel();

            $exceptionMessage = sprintf('SL("Parameter \'%s\' must be of the type %s")', $var['name'], $type);
            $codePrinter->output(
                sprintf(
                    'zephir_throw_exception_string(spl_ce_InvalidArgumentException, %s);',
                    $exceptionMessage
                )
            );

            $codePrinter->output('RETURN_MM_NULL();');
            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
        }

        /* Assign param */
        switch ($type) {
            case 'int':
            case 'long':
                $codePrinter->output($var['name'].' = Z_LVAL_P('.$parameterCode.');');
                break;
            case 'uint':
                $codePrinter->output($var['name'].' = ZEND_ABS(Z_LVAL_P('.$parameterCode.'));');
                break;
            case 'bool':
                $codePrinter->output($var['name'].' = '.$this->getBoolCode($parameterVariable, $context, false).';');
                break;
            case 'double':
                $codePrinter->output($var['name'].' = Z_DVAL_P('.$parameterCode.');');
                break;
            case 'string':
            case 'ulong':
                $context->headersManager->add('kernel/operators');
                $context->symbolTable->mustGrownStack(true);
                $codePrinter->output('if (EXPECTED(Z_TYPE_P('.$parameterCode.') == IS_STRING)) {');
                $codePrinter->increaseLevel();
                $targetVar = $var['name'];
                if ($this->isZE3()) {
                    $targetVar = '&'.$targetVar;
                }
                $codePrinter->output('zephir_get_strval('.$targetVar.', '.$var['name'].'_param);');
                $codePrinter->decreaseLevel();
                $codePrinter->output('} else {');
                $codePrinter->increaseLevel();
                $this->initVar($inputParamVariable, $context);
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
                throw new CompilerException('Unknown type: '.$type);
        }
    }

    public function fetchClassEntry($str)
    {
        return 'zephir_get_internal_ce(SS("'.$str.'"))';
    }

    public function getScalarTempVariable(
        string $type,
        CompilationContext $compilationContext,
        $isLocal = false
    ): Variable {
        throw new CompilerException(
            'ZendEngine2 backend is no longer supported'
        );
    }

    /**
     * Assign value to variable helper.
     *
     * @param string                $macro
     * @param string                $variableName
     * @param string|null|Variable  $value
     * @param CompilationContext    $context
     * @param bool                  $useCodePrinter
     *
     * @return string
     */
    protected function assignHelper(
        string $macro,
        string $variableName,
        $value,
        CompilationContext $context,
        bool $useCodePrinter
    ): string {
        if ($value === null && $macro === 'ZVAL_STRING') {
            return '';
        }

        if ($value instanceof Variable) {
            $value = $value->getName();
        } else {
            $value = 'ZVAL_STRING' === $macro ? '"'.$value.'"' : $value;
        }

        $output = $macro.'('.$variableName.', '.$value.');';
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
            $value = 'RETURN_MM_STRING' == $macro ? '"'.$value.'"' : $value;
        }

        $copyStr = '';
        if (true === $doCopy) {
            $copyStr = ', 1';
        } elseif (false === $doCopy) {
            $copyStr = ', 0';
        } elseif (isset($doCopy)) {
            $copyStr = ', '.$doCopy;
        }

        $output = $macro.'('.$value.$copyStr.');';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $output;
    }

    /**
     * Resolve expressions.
     *
     * @param CompiledExpression[]|string[] $offsetExprs
     * @param CompilationContext            $compilationContext
     *
     * @throws CompilerException
     *
     * @return array
     */
    private function resolveOffsetExprs($offsetExprs, CompilationContext $compilationContext)
    {
        $keys = '';
        $offsetItems = [];
        $numberParams = 0;

        foreach ($offsetExprs as $offsetExpr) {
            if ('a' == $offsetExpr) {
                $keys .= 'a';
                ++$numberParams;
                continue;
            }
            switch ($offsetExpr->getType()) {
                case 'int':
                case 'uint':
                case 'long':
                case 'ulong':
                    $keys .= 'l';
                    $offsetItems[] = $offsetExpr->getCode();
                    ++$numberParams;
                    break;

                case 'string':
                    $keys .= 's';
                    $offsetItems[] = 'SL("'.$offsetExpr->getCode().'")';
                    $numberParams += 2;
                    break;

                case 'variable':
                    $variableIndex = $compilationContext->symbolTable->getVariableForRead(
                        $offsetExpr->getCode(),
                        $compilationContext,
                        null
                    );

                    switch ($variableIndex->getType()) {
                        case 'int':
                        case 'uint':
                        case 'long':
                        case 'ulong':
                            $keys .= 'l';
                            $offsetItems[] = $this->getVariableCode($variableIndex);
                            ++$numberParams;
                            break;
                        case 'string':
                        case 'variable':
                            $keys .= 'z';
                            $offsetItems[] = $this->getVariableCode($variableIndex);
                            ++$numberParams;
                            break;
                        default:
                            throw new CompilerException(
                                sprintf('Variable: %s cannot be used as array index', $variableIndex->getType()),
                                $offsetExpr->getOriginal()
                            );
                    }
                    break;

                default:
                    throw new CompilerException(
                        sprintf('Value: %s cannot be used as array index', $offsetExpr->getType()),
                        $offsetExpr->getOriginal()
                    );
            }
        }

        return [$keys, $offsetItems, $numberParams];
    }
}
