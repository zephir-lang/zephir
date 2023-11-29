<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Backend;

use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Code\Printer;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Compiler;
use Zephir\Config;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\FunctionDefinition;
use Zephir\GlobalConstant;
use Zephir\Name;
use Zephir\Variable\Globals;
use Zephir\Variable\Variable;

use function array_reverse;
use function count;
use function file_get_contents;
use function implode;
use function in_array;
use function is_array;
use function is_object;
use function rtrim;
use function sprintf;
use function strlen;
use function strtoupper;
use function substr;

use const PHP_EOL;

class Backend
{
    protected FcallManagerInterface $fcallManager;

    public function __construct(
        protected Config $config,
        protected string $kernelsPath,
        protected string $templatesPath
    ) {
        $this->fcallManager = new FcallManager();
    }

    public function addArrayEntry(
        Variable $variable,
        $key,
        $value,
        CompilationContext $context,
        $statement = null,
        $useCodePrinter = true
    ): void {
        $type    = null;
        $keyType = 'assoc';

        if (!isset($key)) {
            $keyType = 'append';
        } elseif ($key instanceof CompiledExpression) {
            $typeKey = $key->getType();
            if ('variable' === $typeKey || 'mixed' === $typeKey) {
                $var     = $context->symbolTable->getVariableForRead($key->getCode(), $context);
                $typeKey = $var->getType();
            }
            if (in_array($typeKey, ['int', 'uint', 'long', 'ulong'])) {
                $keyType = 'index';
            }
        }
        if ('null' == $value) {
            if (!isset($key)) {
                $value = $this->resolveValue('null', $context);
                $context->codePrinter->output(
                    'zephir_array_append(' . $this->getVariableCode(
                        $variable
                    ) . ', ' . $value . ', PH_SEPARATE, "' . Compiler::getShortUserPath(
                        $statement['file']
                    ) . '", ' . $statement['line'] . ');'
                );

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
            case 'mixed':
                $type = 'zval';
                break;
        }

        if (null === $type) {
            throw new CompilerException('Unknown type mapping: ' . $value->getType());
        }

        if (isset($key)) {
            if ('variable' == $key->getType()) {
                $var    = $context->symbolTable->getVariableForRead($key->getCode(), $context);
                $keyStr = 'string' == $typeKey ?
                    'Z_STRVAL_P('
                    . $this->getVariableCode($var)
                    . '), Z_STRLEN_P('
                    . $this->getVariableCode($var)
                    . ')' :
                    $this->getVariableCode($var);
            } else {
                $keyStr = 'string' == $key->getType() ? 'SL("' . $key->getCode() . '")' : $key->getCode();
            }
        }

        if ('stringl' == $type) {
            if ($value instanceof Variable) {
                $valueStr = 'Z_STRVAL_P('
                    . $this->getVariableCode($value)
                    . '), Z_STRLEN_P('
                    . $this->getVariableCode($value)
                    . ')';
            } else {
                $valueStr = 'SL("' . $value->getCode() . '")';
            }
        } elseif ('zval' == $type) {
            $valueStr = $this->getVariableCode($value);
        } else {
            $valueStr = $value->getCode();
        }

        if ('assoc' == $keyType) {
            $output = 'add_assoc_'
                . $type
                . '_ex('
                . $this->getVariableCode($variable)
                . ', '
                . $keyStr
                . ', '
                . $valueStr
                . ');';
        } elseif ('append' == $keyType) {
            $output = 'zephir_array_append('
                . $this->getVariableCode($variable)
                . ', '
                . $this->resolveValue($value, $context)
                . ', PH_SEPARATE, "'
                . Compiler::getShortUserPath($statement['file'])
                . '", '
                . $statement['line']
                . ');';
        } else {
            $output = 'add_index_'
                . $type
                . '('
                . $this->getVariableCode($variable)
                . ', '
                . $keyStr
                . ', '
                . $valueStr
                . ');';
        }

        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }
    }

    public function addStaticProperty($classEntry, $property, $value, CompilationContext $context): void
    {
        $value = $this->resolveValue($value, $context);
        $context->codePrinter->output(
            'zephir_add_static_property_ce('
            . $classEntry
            . ', ZEND_STRL("'
            . $property
            . '"), '
            . $value
            . ');'
        );
    }

    public function arrayFetch(
        Variable $var,
        Variable $src,
        $index,
        $flags,
        $arrayAccess,
        CompilationContext $context,
        $useCodePrinter = true
    ) {
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
            case 'mixed':
            case 'string':
                $type = $index->getType();
                break;

            default:
                throw new CompilerException(
                    sprintf('Variable type: %s cannot be used as array index without cast', $index->getType()),
                    $arrayAccess['right']
                );
        }
        if ($isVariable && in_array($index->getType(), ['variable', 'string', 'mixed'])) {
            $output = 'zephir_array_fetch('
                . $this->getVariableCode($var)
                . ', '
                . $this->getVariableCode($src)
                . ', '
                . $this->getVariableCode($index)
                . ', '
                . $flags
                . ', "'
                . Compiler::getShortUserPath($arrayAccess['file'])
                . '", '
                . $arrayAccess['line']
                . ');';
        } else {
            if ($isVariable) {
                $indexAccess = $this->getVariableCode($index);
            } else {
                $indexAccess = $index->getCode();
                if ('string' == $type) {
                    $indexAccess = 'SL("' . $indexAccess . '")';
                }
            }
            $output = 'zephir_array_fetch_'
                . $type
                . '('
                . $this->getVariableCode($var)
                . ', '
                . $this->getVariableCode($src)
                . ', '
                . $indexAccess
                . ', '
                . $flags
                . ', "'
                . Compiler::getShortUserPath($arrayAccess['file'])
                . '", '
                . $arrayAccess['line']
                . ');';
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
                return new CompiledExpression(
                    'bool',
                    'zephir_array_isset_string('
                    . $this->getVariableCode($var)
                    . ', SL("'
                    . $resolvedExpr->getCode()
                    . '"))',
                    $expression
                );
            }
        }

        return $this->arrayIsset2($var, $resolvedExpr, $expression, $context);
    }

    public function arrayIsset2(Variable $var, $resolvedExpr, $expression, CompilationContext $context)
    {
        if (!($resolvedExpr instanceof Variable)) {
            if ('string' == $resolvedExpr->getType()) {
                return new CompiledExpression(
                    'bool',
                    'zephir_array_isset_string('
                    . $this->getVariableCode($var)
                    . ', SS("'
                    . $resolvedExpr->getCode()
                    . '"))',
                    $expression
                );
            } else {
                return new CompiledExpression(
                    'bool',
                    'zephir_array_isset_long('
                    . $this->getVariableCode($var)
                    . ', '
                    . $resolvedExpr->getCode() . ')',
                    $expression
                );
            }
        }

        if ('int' == $resolvedExpr->getType() || 'long' == $resolvedExpr->getType()) {
            return new CompiledExpression(
                'bool',
                'zephir_array_isset_long('
                . $this->getVariableCode($var)
                . ', '
                . $this->getVariableCode($resolvedExpr)
                . ')',
                $expression
            );
        } elseif (
            'variable' == $resolvedExpr->getType() ||
            'string' == $resolvedExpr->getType() ||
            'mixed' === $resolvedExpr->getType()
        ) {
            return new CompiledExpression(
                'bool',
                'zephir_array_isset(' . $this->getVariableCode($var) . ', ' . $this->getVariableCode(
                    $resolvedExpr
                ) . ')',
                $expression
            );
        }

        throw new CompilerException('[' . $resolvedExpr->getType() . ']', $expression);
    }

    public function arrayIssetFetch(
        Variable $target,
        Variable $var,
        $resolvedExpr,
        $flags,
        $expression,
        CompilationContext $context
    ) {
        if (!($resolvedExpr instanceof Variable)) {
            $code = $this->getVariableCode($target) . ', ' . $this->getVariableCode($var);
            if ('string' == $resolvedExpr->getType()) {
                return new CompiledExpression(
                    'bool',
                    'zephir_array_isset_string_fetch('
                    . $code
                    . ', SL("'
                    . $resolvedExpr->getCode()
                    . '"), '
                    . $flags
                    . ')',
                    $expression
                );
            }
        }

        return $this->arrayIssetFetch2($target, $var, $resolvedExpr, $flags, $expression, $context);
    }

    public function arrayIssetFetch2(
        Variable $target,
        Variable $var,
        $resolvedExpr,
        $flags,
        $expression,
        CompilationContext $context
    ) {
        $code = $this->getVariableCode($target) . ', ' . $this->getVariableCode($var);

        if (!($resolvedExpr instanceof Variable)) {
            if ('string' === $resolvedExpr->getType()) {
                return new CompiledExpression(
                    'bool',
                    'zephir_array_isset_string_fetch(' . $code . ', SS("' . $resolvedExpr->getCode(
                    ) . '"), ' . $flags . ')',
                    $expression
                );
            } elseif (in_array($resolvedExpr->getType(), ['int', 'uint', 'long'])) {
                return new CompiledExpression(
                    'bool',
                    'zephir_array_isset_long_fetch(' . $code . ', ' . $resolvedExpr->getCode() . ', ' . $flags . ')',
                    $expression
                );
            } else {
                $resolvedExpr = $context->symbolTable->getVariableForRead($resolvedExpr->getCode(), $context);
            }
        }

        if (in_array($resolvedExpr->getType(), ['int', 'long'])) {
            return new CompiledExpression(
                'bool',
                'zephir_array_isset_long_fetch(' . $code . ', ' . $this->getVariableCode(
                    $resolvedExpr
                ) . ', ' . $flags . ')',
                $expression
            );
        } elseif (in_array($resolvedExpr->getType(), ['variable', 'mixed', 'string'])) {
            return new CompiledExpression(
                'bool',
                'zephir_array_isset_fetch(' . $code . ', ' . $this->getVariableCode(
                    $resolvedExpr
                ) . ', ' . $flags . ')',
                $expression
            );
        }

        throw new CompilerException('arrayIssetFetch [' . $resolvedExpr->getType() . ']', $expression);
    }

    public function arrayUnset(Variable $variable, $exprIndex, $flags, CompilationContext $context): void
    {
        $context->headersManager->add('kernel/array');
        $variableCode = $this->getVariableCode($variable);
        if ('string' == $exprIndex->getType()) {
            $context->codePrinter->output(
                'zephir_array_unset_string(' . $variableCode . ', SL("' . $exprIndex->getCode() . '"), ' . $flags . ');'
            );

            return;
        }

        $this->arrayUnset2($variable, $exprIndex, $flags, $context);
    }

    public function arrayUnset2(Variable $variable, $exprIndex, $flags, CompilationContext $context): void
    {
        $context->headersManager->add('kernel/array');
        $variableCode = $this->getVariableCode($variable);
        switch ($exprIndex->getType()) {
            case 'int':
            case 'uint':
            case 'long':
                $context->codePrinter->output(
                    'zephir_array_unset_long(' . $variableCode . ', ' . $exprIndex->getCode() . ', ' . $flags . ');'
                );
                break;

            case 'string':
                $context->codePrinter->output(
                    'zephir_array_unset_string(' . $variableCode . ', SS("' . $exprIndex->getCode(
                    ) . '"), ' . $flags . ');'
                );
                break;

            case 'variable':
                $variableIndex = $context->symbolTable->getVariableForRead(
                    $exprIndex->getCode(),
                    $context,
                    $exprIndex->getOriginal()
                );
                $indexCode     = $this->getVariableCode($variableIndex);
                switch ($variableIndex->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                        $context->codePrinter->output(
                            'zephir_array_unset_long(' . $variableCode . ', ' . $indexCode . ', ' . $flags . ');'
                        );
                        break;

                    case 'string':
                    case 'variable':
                    case 'mixed':
                        $context->codePrinter->output(
                            'zephir_array_unset(' . $variableCode . ', ' . $indexCode . ', ' . $flags . ');'
                        );
                        break;

                    default:
                        throw new CompilerException(
                            'Variable type: ' . $variableIndex->getType(
                            ) . ' cannot be used as array index without cast'
                        );
                }
                break;

            default:
                throw new CompilerException(
                    'Cannot use expression: ' . $exprIndex->getType() . ' as array index without cast'
                );
        }
    }

    public function assignArrayMulti(
        Variable $variable,
        $symbolVariable,
        $offsetExprs,
        CompilationContext $compilationContext
    ): void {
        [$keys, $offsetItems, $numberParams] = $this->resolveOffsetExprs($offsetExprs, $compilationContext);

        $symbol  = $this->resolveValue($symbolVariable, $compilationContext, true);
        $varCode = $this->getVariableCode($variable);
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

    public function assignArrayProperty(Variable $variable, $property, $key, $value, CompilationContext $context): void
    {
        $resolveValue = $this->resolveValue($value, $context);
        if (isset($key)) {
            $context->codePrinter->output(
                'zephir_update_property_array(' . $this->getVariableCode(
                    $variable
                ) . ', SL("' . $property . '"), ' . $this->getVariableCode($key) . ', ' . $resolveValue . ');'
            );
        } else {
            $context->codePrinter->output(
                'zephir_update_property_array_append(' . $this->getVariableCode(
                    $variable
                ) . ', SL("' . $property . '"), ' . $resolveValue . ');'
            );
        }
    }

    public function assignBool(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->assignHelper('ZVAL_BOOL', $this->getVariableCode($variable), $value, $context, $useCodePrinter);
    }

    public function assignDouble(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->assignHelper('ZVAL_DOUBLE', $this->getVariableCode($variable), $value, $context, $useCodePrinter);
    }

    public function assignLong(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->assignHelper('ZVAL_LONG', $this->getVariableCode($variable), $value, $context, $useCodePrinter);
    }

    public function assignNull(Variable $variable, CompilationContext $context, $useCodePrinter = true)
    {
        $output = 'ZVAL_NULL(' . $this->getVariableCode($variable) . ');';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $output;
    }

    public function assignPropertyArrayMulti(
        Variable $variable,
        $valueVariable,
        $propertyName,
        $offsetExprs,
        CompilationContext $compilationContext
    ): void {
        [$keys, $offsetItems, $numberParams] = $this->resolveOffsetExprs($offsetExprs, $compilationContext);
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

    public function assignStaticPropertyArrayMulti(
        $classEntry,
        $valueVariable,
        $propertyName,
        $offsetExprs,
        CompilationContext $compilationContext
    ): void {
        [$keys, $offsetItems, $numberParams] = $this->resolveOffsetExprs($offsetExprs, $compilationContext);
        $valueVariable = $this->resolveValue($valueVariable, $compilationContext, true);

        $offsetStr = $offsetItems ? ', ' . implode(', ', $offsetItems) : '';
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

    /**
     * @param Variable             $variable
     * @param string|Variable|null $value
     * @param CompilationContext   $context
     * @param bool                 $useCodePrinter
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
    public function assignZval(Variable $variable, $code, CompilationContext $context): void
    {
        $code = $this->resolveValue($code, $context);
        if (!$variable->isDoublePointer()) {
            $context->symbolTable->mustGrownStack(true);
            $symbolVariable = $this->getVariableCode($variable);
            $context->codePrinter->output('ZVAL_COPY(' . $symbolVariable . ', ' . $code . ');');
        } else {
            $context->codePrinter->output($variable->getName() . ' = ' . $code . ';');
        }
    }

    public function callDynamicFunction(
        $symbolVariable,
        Variable $variable,
        CompilationContext $context,
        $params = [],
        $cache = 'NULL',
        $cacheSlot = 0
    ): void {
        $paramStr = null != $params ? ', ' . implode(', ', $params) : '';
        if (!isset($symbolVariable)) {
            $context->codePrinter->output(
                'ZEPHIR_CALL_ZVAL_FUNCTION(NULL, ' . $this->getVariableCode(
                    $variable
                ) . ', ' . $cache . ', ' . $cacheSlot . $paramStr . ');'
            );
        } elseif ('return_value' == $symbolVariable->getName()) {
            $context->codePrinter->output(
                'ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(' . $this->getVariableCode(
                    $variable
                ) . ', ' . $cache . ', ' . $cacheSlot . $paramStr . ');'
            );
        } else {
            $context->codePrinter->output(
                'ZEPHIR_CALL_ZVAL_FUNCTION(' . $this->getVariableCode($symbolVariable) . ', ' . $this->getVariableCode(
                    $variable
                ) . ', ' . $cache . ', ' . $cacheSlot . $paramStr . ');'
            );
        }
    }

    public function callMethod(
        $symbolVariable,
        Variable $variable,
        $methodName,
        $cachePointer,
        $params,
        CompilationContext $context
    ): void {
        $paramStr = null != $params ? ', ' . implode(', ', $params) : '';
        $macro    = 'CALL_METHOD';
        if ($methodName instanceof Variable) {
            $macro      = 'CALL_METHOD_ZVAL';
            $methodName = $this->getVariableCode($methodName);
        } else {
            $methodName = '"' . $methodName . '"';
        }

        if (!isset($symbolVariable)) {
            $context->codePrinter->output(
                'ZEPHIR_' . $macro . '(NULL, ' . $this->getVariableCode(
                    $variable
                ) . ', ' . $methodName . ', ' . $cachePointer . $paramStr . ');'
            );
        } elseif ('return_value' == $symbolVariable->getName()) {
            $context->codePrinter->output(
                'ZEPHIR_RETURN_' . $macro . '(' . $this->getVariableCode(
                    $variable
                ) . ', ' . $methodName . ', ' . $cachePointer . $paramStr . ');'
            );
        } else {
            $symbol = $this->getVariableCode($symbolVariable);
            $context->codePrinter->output(
                'ZEPHIR_' . $macro . '(' . $symbol . ', ' . $this->getVariableCode(
                    $variable
                ) . ', ' . $methodName . ', ' . $cachePointer . $paramStr . ');'
            );
        }
    }

    public function checkConstructor(Variable $var, CompilationContext $context): void
    {
        $context->codePrinter->output('if (zephir_has_constructor(' . $this->getVariableCode($var) . ')) {');
    }

    public function checkStrictType($type, $var, CompilationContext $context): void
    {
        $codePrinter = $context->codePrinter;
        $conditions  = [];

        $inputParamVariable = $context->symbolTable->getVariableForWrite($var['name'], $context);
        $inputParamCode     = $this->getVariableCode($inputParamVariable);
        $cond               = 'Z_TYPE_P(' . $inputParamCode . ') != ';
        if ($context->symbolTable->hasVariable($var['name'] . '_param')) {
            $parameterVariable = $context->symbolTable->getVariableForWrite($var['name'] . '_param', $context);
            $parameterCode     = $context->backend->getVariableCode($parameterVariable);
            $cond              = 'Z_TYPE_P(' . $parameterCode . ') != ';
        }

        switch ($type) {
            case 'int':
            case 'uint':
            case 'long':
                $conditions[] = $cond . 'IS_LONG';
                break;
            case 'bool':
                $conditions[] = $cond . 'IS_TRUE';
                $conditions[] = $cond . 'IS_FALSE';
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
                $conditions[] = 'zephir_is_callable(' . $inputParamCode . ') != 1';
                break;
            default:
                throw new CompilerException('Unknown type ' . $type);
        }

        /* Generate verification code */
        if (count($conditions)) {
            $codePrinter->output('if (UNEXPECTED(' . implode(' && ', $conditions) . ')) {');
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
                $codePrinter->output($var['name'] . ' = Z_LVAL_P(' . $parameterCode . ');');
                break;
            case 'uint':
                $codePrinter->output($var['name'] . ' = ZEND_ABS(Z_LVAL_P(' . $parameterCode . '));');
                break;
            case 'bool':
                $codePrinter->output(
                    $var['name'] . ' = ' . $this->getBoolCode($parameterVariable, $context, false) . ';'
                );
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
                $targetVar = '&' . $targetVar;
                $codePrinter->output('zephir_get_strval(' . $targetVar . ', ' . $var['name'] . '_param);');
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
            case 'mixed':
                break;
            default:
                throw new CompilerException('Unknown type: ' . $type);
        }
    }

    public function concatSelf(Variable $variable, Variable $itemVariable, CompilationContext $context): void
    {
        $variable     = $this->getVariableCode($variable);
        $itemVariable = $this->getVariableCode($itemVariable);
        $context->codePrinter->output('zephir_concat_self(' . $variable . ', ' . $itemVariable . ');');
    }

    public function copyOnWrite(Variable $target, $var, CompilationContext $context): void
    {
        if ('EG(exception)' === $var) {
            $context->codePrinter->output('ZVAL_OBJ(' . $this->getVariableCode($target) . ', EG(exception));');
            $context->codePrinter->output('Z_ADDREF_P(' . $this->getVariableCode($target) . ');');

            return;
        }

        $globalsManager = new Globals();

        if ($globalsManager->isSuperGlobal($target->getName())) {
            $context->codePrinter->output(
                sprintf(
                    'ZEPHIR_HASH_COPY(%s, %s);',
                    $this->getVariableCode($target),
                    $this->resolveValue($var, $context)
                )
            );
        } else {
            $context->codePrinter->output(
                sprintf(
                    'ZEPHIR_CPY_WRT(%s, %s);',
                    $this->getVariableCode($target),
                    $this->resolveValue($var, $context)
                )
            );
        }
    }

    public function createClosure(Variable $variable, $classDefinition, CompilationContext $context): void
    {
        $symbol = $this->getVariableCode($variable);
        $context->codePrinter->output(
            'zephir_create_closure_ex(' . $symbol . ', NULL, ' . $classDefinition->getClassEntry(
            ) . ', SL("__invoke"));'
        );
    }

    public function declareConstant($type, $name, $value, CompilationContext $context): void
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
                    $value = '"' . Name::addSlashes($value) . '"';
                }
                $dType = 'string';
                break;
        }

        if (!isset($dType)) {
            $dType = $type;
        }

        if ('null' == $dType) {
            $context->codePrinter->output('zephir_declare_class_constant_null(' . $ce . ', SL("' . $name . '"));');
        } else {
            $context->codePrinter->output(
                'zephir_declare_class_constant_' . $dType . '(' . $ce . ', SL("' . $name . '"), ' . $value . ');'
            );
        }
    }

    public function declareVariables($method, $typeToVariables)
    {
        $varInitCode    = [];
        $additionalCode = $method ? $this->onPreInitVar($method) : '';

        foreach ($typeToVariables as $type => $variables) {
            [$pointer, $code] = $this->getTypeDefinition($type);
            $code           .= ' ';
            $groupVariables = [];

            /**
             * @var Variable[]
             */
            foreach ($variables as $variable) {
                $nextCode = $this->generateInitCode($groupVariables, $type, $pointer, $variable);
                if ($nextCode && $additionalCode) {
                    $additionalCode .= PHP_EOL . $nextCode;
                } else {
                    $additionalCode .= $nextCode;
                }
            }

            $varInitCode[] = $code . implode(', ', $groupVariables) . ';';
        }

        /* Keep order consistent with previous zephir versions (BC-only) */
        $varInitCode = array_reverse($varInitCode);
        if ($additionalCode) {
            $varInitCode[] = $additionalCode;
        }

        return $varInitCode;
    }

    public function destroyIterator(Variable $iteratorVariable, CompilationContext $context): void
    {
        $context->codePrinter->output('zend_iterator_dtor(' . $iteratorVariable->getName() . ');');
    }

    public function fetchClass(Variable $zendClassEntry, $className, $guarded, CompilationContext $context): void
    {
        $context->headersManager->add('kernel/object');
        if ($guarded) {
            $context->codePrinter->output('if (!' . $zendClassEntry->getName() . ') {');
        }
        $context->codePrinter->output(
            $zendClassEntry->getName() . ' = zephir_fetch_class_str_ex(' . $className . ', ZEND_FETCH_CLASS_AUTO);'
        );
        if ($guarded) {
            $context->codePrinter->output('}');
        }
    }

    public function fetchClassEntry(string $str): string
    {
        return 'zephir_get_internal_ce(SL("' . $str . '"))';
    }

    public function fetchGlobal(Variable $globalVar, CompilationContext $compilationContext, $useCodePrinter = true)
    {
        $name   = $globalVar->getName();
        $output = strtr('zephir_get_global(&:name, SL(":name"));', [':name' => $name]);

        if ($useCodePrinter) {
            $compilationContext->codePrinter->output($output);
        }

        return $output;
    }

    /**
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
    ): void {
        $flags = 'PH_NOISY_CC';
        if ($readOnly) {
            $flags .= ' | PH_READONLY';
        }

        $variableCode = $this->getVariableCode($variableVariable);
        $symbol       = $this->getVariableCode($symbolVariable);

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
     * @param Variable           $symbolVariable
     * @param Definition         $classDefinition
     * @param                    $property
     * @param bool               $readOnly
     * @param CompilationContext $context
     *
     * @throws Exception
     */
    public function fetchStaticProperty(
        Variable $symbolVariable,
        $classDefinition,
        $property,
        $readOnly,
        CompilationContext $context
    ): void {
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

    public function forStatement(
        Variable $exprVariable,
        $keyVariable,
        $variable,
        $duplicateKey,
        $duplicateHash,
        $statement,
        $statementBlock,
        CompilationContext $compilationContext
    ): void {
        /**
         * Create a temporary zval to fetch the items from the hash.
         */
        $compilationContext->headersManager->add('kernel/fcall');
        $compilationContext->symbolTable->mustGrownStack(true);
        if (!$compilationContext->symbolTable->hasVariable('ZEPHIR_LAST_CALL_STATUS')) {
            $callStatus = new Variable(
                'int',
                'ZEPHIR_LAST_CALL_STATUS',
                $compilationContext->branchManager->getCurrentBranch()
            );
            $callStatus->setIsInitialized(true, $compilationContext);
            $callStatus->increaseUses();
            $callStatus->setReadOnly(true);
            $compilationContext->symbolTable->addRawVariable($callStatus);
        }
        $tempVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
        $tempVariable->setIsDoublePointer(true);
        $tempValidVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
        $codePrinter       = $compilationContext->codePrinter;

        $codePrinter->output(
            'zephir_is_iterable(' . $this->getVariableCode(
                $exprVariable
            ) . ', ' . $duplicateHash . ', "' . Compiler::getShortUserPath(
                $statement['file']
            ) . '", ' . $statement['line'] . ');'
        );

        $codePrinter->output('if (Z_TYPE_P(' . $this->getVariableCode($exprVariable) . ') == IS_ARRAY) {');
        $codePrinter->increaseLevel();

        $reverse = $statement['reverse'] ? 'REVERSE_' : '';

        if (isset($keyVariable)) {
            $arrayNumKey = $compilationContext->symbolTable->addTemp('zend_ulong', $compilationContext);
            $arrayStrKey = $compilationContext->symbolTable->addTemp('zend_string', $compilationContext);
        }

        if (isset($keyVariable) && isset($variable)) {
            $macro = 'ZEND_HASH_' . $reverse . 'FOREACH_KEY_VAL';
            $codePrinter->output(
                $macro . '(Z_ARRVAL_P(' . $this->getVariableCode($exprVariable) . '), ' . $arrayNumKey->getName(
                ) . ', ' . $arrayStrKey->getName() . ', ' . $tempVariable->getName() . ')'
            );
        } elseif (isset($keyVariable)) {
            $macro = 'ZEND_HASH_' . $reverse . 'FOREACH_KEY';
            $codePrinter->output(
                $macro . '(Z_ARRVAL_P(' . $this->getVariableCode($exprVariable) . '), ' . $arrayNumKey->getName(
                ) . ', ' . $arrayStrKey->getName() . ')'
            );
        } else {
            $macro = 'ZEND_HASH_' . $reverse . 'FOREACH_VAL';
            $codePrinter->output(
                $macro . '(Z_ARRVAL_P(' . $this->getVariableCode($exprVariable) . '), ' . $tempVariable->getName() . ')'
            );
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
                $codePrinter->output(
                    'ZVAL_STR_COPY(' . $this->getVariableCode($keyVariable) . ', ' . $arrayStrKey->getName() . ');'
                );
            } else {
                $codePrinter->output(
                    'ZVAL_STR(' . $this->getVariableCode($keyVariable) . ', ' . $arrayStrKey->getName() . ');'
                );
            }
            $codePrinter->decreaseLevel();
            $codePrinter->output('} else {');
            $codePrinter->increaseLevel();
            $codePrinter->output(
                'ZVAL_LONG(' . $this->getVariableCode($keyVariable) . ', ' . $arrayNumKey->getName() . ');'
            );
            $codePrinter->decreaseLevel();
            $codePrinter->output('}');
            $codePrinter->decreaseLevel();
        }

        if (isset($variable)) {
            $compilationContext->symbolTable->mustGrownStack(true);
            $codePrinter->increaseLevel();
            $variable->initVariant($compilationContext);
            $codePrinter->output(
                'ZVAL_COPY(' . $this->getVariableCode($variable) . ', ' . $this->getVariableCode($tempVariable) . ');'
            );
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
        $codePrinter->decreaseLevel();

        $codePrinter->output('} else {');
        $codePrinter->increaseLevel();

        $codePrinter->output(
            'ZEPHIR_CALL_METHOD(NULL, ' . $this->getVariableCode($exprVariable) . ', "rewind", NULL, 0);'
        );
        $codePrinter->output('zephir_check_call_status();');

        $codePrinter->output('while (1) {');
        $codePrinter->increaseLevel();

        $codePrinter->output(
            'ZEPHIR_CALL_METHOD(&' . $tempValidVariable->getName() . ', ' . $this->getVariableCode(
                $exprVariable
            ) . ', "valid", NULL, 0);'
        );
        $codePrinter->output('zephir_check_call_status();');
        $codePrinter->output('if (!zend_is_true(&' . $tempValidVariable->getName() . ')) {');
        $codePrinter->increaseLevel();
        $codePrinter->output('break;');
        $codePrinter->decreaseLevel();
        $codePrinter->output('}');

        if (isset($keyVariable)) {
            $codePrinter->output(
                'ZEPHIR_CALL_METHOD(' . $this->getVariableCode($keyVariable) . ', ' . $this->getVariableCode(
                    $exprVariable
                ) . ', "key", NULL, 0);'
            );
            $codePrinter->output('zephir_check_call_status();');
        }

        if (isset($variable)) {
            $codePrinter->output(
                'ZEPHIR_CALL_METHOD(' . $this->getVariableCode($variable) . ', ' . $this->getVariableCode(
                    $exprVariable
                ) . ', "current", NULL, 0);'
            );
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

        $codePrinter->output(
            'ZEPHIR_CALL_METHOD(NULL, ' . $this->getVariableCode($exprVariable) . ', "next", NULL, 0);'
        );
        $codePrinter->output('zephir_check_call_status();');

        $codePrinter->decreaseLevel();
        $codePrinter->output('}');

        $codePrinter->decreaseLevel();
        $codePrinter->output('}');

        /* Since we do not observe, still do cleanup */
        $variable?->initVariant($compilationContext);
        $keyVariable?->initVariant($compilationContext);
    }

    public function generateInitCode(&$groupVariables, $type, $pointer, Variable $variable): ?string
    {
        $isComplex = in_array($type, ['variable', 'string', 'array', 'resource', 'callable', 'object', 'mixed'], true);

        if ($isComplex && !$variable->isDoublePointer()) {
            $groupVariables[] = $variable->getName();

            return match ($variable->getRealname()) {
                '__$null'  => "\t" . 'ZVAL_NULL(&' . $variable->getName() . ');',
                '__$true'  => "\t" . 'ZVAL_BOOL(&' . $variable->getName() . ', 1);',
                '__$false' => "\t" . 'ZVAL_BOOL(&' . $variable->getName() . ', 0);',
                default    => "\t" . 'ZVAL_UNDEF(&' . $variable->getName() . ');',
            };
        }

        if ($variable->isLocalOnly()) {
            $groupVariables[] = $variable->getName();

            return null;
        }

        if ($variable->isSuperGlobal()) {
            $groupVariables[] = $variable->getName();

            return null;
        }

        if ($variable->isDoublePointer()) {
            /* Double pointers for ZE3 are used as zval * */
            $ptr = $isComplex ? $pointer : $pointer . $pointer;
            if ($variable->mustInitNull()) {
                $groupVariables[] = $ptr . $variable->getName() . ' = NULL';
            } else {
                $groupVariables[] = $ptr . $variable->getName();
            }

            return null;
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
                case 'mixed':
                    $groupVariables[] = $pointer . $variable->getName();
                    break;

                case 'char':
                    $defaultValue = (string)$defaultValue;
                    if (strlen($defaultValue) > 4) {
                        if (strlen($defaultValue) > 10) {
                            throw new CompilerException(
                                "Invalid char literal: '" . substr($defaultValue, 0, 10) . "...'",
                                $variable->getOriginal()
                            );
                        } else {
                            throw new CompilerException(
                                "Invalid char literal: '" . $defaultValue . "'",
                                $variable->getOriginal()
                            );
                        }
                    }
                /* no break */

                default:
                    $groupVariables[] = $pointer . $variable->getName() . ' = ' . $defaultValue;
                    break;
            }

            return null;
        }

        if ($variable->mustInitNull() && $pointer) {
            $groupVariables[] = $pointer . $variable->getName() . ' = NULL';

            return null;
        }

        $groupVariables[] = $pointer . $variable->getName();

        return null;
    }

    public function getBoolCode(Variable $variable, CompilationContext $context, $useCodePrinter = true): string
    {
        $code = '(Z_TYPE_P(' . $this->getVariableCode($variable) . ') == IS_TRUE)';
        if ($useCodePrinter) {
            $context->codePrinter->output($code);
        }

        return $code;
    }

    public function getFcallManager(): FcallManagerInterface
    {
        return $this->fcallManager;
    }

    /**
     * Resolves the path to the source kernel files of the backend.
     */
    public function getInternalKernelPath(): string
    {
        return $this->kernelsPath;
    }

    /**
     * @param Method             $method
     * @param CompilationContext $context
     *
     * @return string
     */
    public function getInternalSignature(Method $method, CompilationContext $context): string
    {
        if ($method->isInitializer() && !$method->isStatic()) {
            return 'zend_object *' . $method->getName() . '(zend_class_entry *class_type)';
        }

        if ($method->isInitializer() && $method->isStatic()) {
            return 'void ' . $method->getName() . '()';
        }

        $signatureParameters = [];
        $parameters          = $method->getParameters();
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

        $signaturePattern =
            'int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used';

        if (count($signatureParameters)) {
            $signaturePattern = sprintf('%s, %s', $signaturePattern, implode(', ', $signatureParameters));
        }

        return "void {$method->getInternalName()}({$signaturePattern})";
    }

    /**
     * @param string             $type
     * @param CompilationContext $compilationContext
     *
     * @return Variable
     */
    public function getScalarTempVariable(
        string $type,
        CompilationContext $compilationContext,
    ): Variable {
        return $compilationContext->symbolTable->getTempNonTrackedVariable($type, $compilationContext);
    }

    /**
     * Resolves the path to the source template file of the backend.
     */
    public function getTemplateFileContents(string $filename): string
    {
        $templatePath = rtrim((string)$this->config->get('templatepath', 'backend'), '\\/');
        if (empty($templatePath)) {
            $templatePath = $this->templatesPath;
        }

        return file_get_contents("$templatePath/engine/$filename");
    }

    public function getTypeDefinition($type): array
    {
        if ($type === 'zend_ulong') {
            return ['', 'zend_ulong'];
        } elseif ($type === 'zend_string') {
            return ['*', 'zend_string'];
        }

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
            case 'mixed':
                $pointer = '*';
                $code    = 'zval';
                break;

            case 'HashTable':
                $pointer = '*';
                $code    = 'HashTable';
                break;

            case 'HashPosition':
                $code = 'HashPosition';
                break;

            case 'zend_class_entry':
            case 'static_zend_class_entry':
                $pointer = '*';
                $code    = 'zend_class_entry';
                break;

            case 'zend_function':
                $pointer = '*';
                $code    = 'zend_function';
                break;

            case 'zend_object_iterator':
                $pointer = '*';
                $code    = 'zend_object_iterator';
                break;

            case 'zend_property_info':
                $pointer = '*';
                $code    = 'zend_property_info';
                break;

            case 'zephir_fcall_cache_entry':
            case 'static_zephir_fcall_cache_entry':
                $pointer = '*';
                $code    = 'zephir_fcall_cache_entry';
                break;

            default:
                throw new CompilerException('Unsupported type in declare: ' . $type);
        }

        return [$pointer, $code];
    }

    /**
     * @param Variable $variableVariable
     * @param string   $operator
     * @param string   $value
     *
     * @return string
     *
     * @throws CompilerException
     */
    public function getTypeofCondition(
        Variable $variableVariable,
        string $operator,
        string $value,
    ): string {
        $variableName = $this->getVariableCode($variableVariable);

        return match ($value) {
            'array'                  => 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_ARRAY',
            'object'                 => 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_OBJECT',
            'null'                   => 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_NULL',
            'string'                 => 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_STRING',
            'int', 'long', 'integer' => 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_LONG',
            'double', 'float'        => 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_DOUBLE',
            'boolean', 'bool'        => '((Z_TYPE_P(' . $variableName . ') == IS_TRUE || Z_TYPE_P(' . $variableName . ') == IS_FALSE) ' . $operator . ' 1)',
            'resource'               => 'Z_TYPE_P(' . $variableName . ') ' . $operator . ' IS_RESOURCE',
            'callable'               => 'zephir_is_callable(' . $variableName . ') ' . $operator . ' 1',
            default                  => throw new CompilerException(
                sprintf('Unknown type: "%s" in typeof comparison', $value)
            ),
        };
    }

    public function getVariableCode(Variable $variable): string
    {
        if (
            $variable->isDoublePointer() ||
            in_array($variable->getName(), ['this_ptr', 'return_value']) ||
            in_array($variable->getType(), ['int', 'long'])
        ) {
            return $variable->getName();
        }

        return '&' . $variable->getName();
    }

    public function ifVariableValueUndefined(
        Variable $var,
        CompilationContext $context,
        $useBody = false,
        $useCodePrinter = true
    ): string {
        if ($var->isDoublePointer()) {
            return $this->ifVariableValueUndefined2($var, $context, $useBody, $useCodePrinter);
        }
        $body   = 'ZEPHIR_IS_EMPTY(' . $this->getVariableCode($var) . ')';
        $output = 'if (' . $body . ') {';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $useBody ? $body : $output;
    }

    public function ifVariableValueUndefined2(
        Variable $var,
        CompilationContext $context,
        $onlyBody = false,
        $useCodePrinter = true
    ): string {
        $body   = '!' . $var->getName();
        $output = 'if (' . $body . ') {';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $onlyBody ? $body : $output;
    }

    /**
     * Initialize array
     *
     * Init empty array or specific size array.
     *
     * @param Variable           $variable
     * @param CompilationContext $context
     * @param int|null           $size
     *
     * @return void
     */
    public function initArray(Variable $variable, CompilationContext $context, int $size = null): void
    {
        $code = $this->getVariableCode($variable);

        if (null === $size) {
            $output = "array_init({$code});";
        } else {
            $output = "zephir_create_array({$code}, {$size}, 0);";
        }

        $context->codePrinter->output($output);
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

    public function initVar(
        Variable $variable,
        CompilationContext $context,
        bool $useCodePrinter = true,
        bool $second = false
    ): string {
        $macro = !$second ? 'ZVAL_NULL' : 'ZEPHIR_INIT_NVAR';
        $code  = $macro . '(' . $this->getVariableCode($variable) . ');';
        if ($useCodePrinter) {
            $context->codePrinter->output($code);
        }

        return $code;
    }

    /**
     * @param Variable[]         $variables
     * @param CompilationContext $context
     *
     * @return string
     *
     * @throws CompilerException
     */
    public function initializeVariableDefaults(array $variables, CompilationContext $context): string
    {
        $codePrinter = new Printer();
        $codePrinter->increaseLevel();

        $oldCodePrinter       = $context->codePrinter;
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
            if (!is_array($value)) {
                continue;
            }

            $variablesManager->initializeDefaults($variable, $value, $context);
        }

        $context->codePrinter = $oldCodePrinter;

        return $codePrinter->getOutput();
    }

    public function onPostCompile(Method $method, CompilationContext $context): void
    {
        if (str_starts_with($method->getName(), 'zephir_init_properties')) {
            $context->codePrinter->increaseLevel();
            $context->codePrinter->output('return Z_OBJ_P(this_ptr);');
            $context->codePrinter->decreaseLevel();
            $context->codePrinter->output('}');
            $context->codePrinter->decreaseLevel();
        }
    }

    public function onPreCompile(Method $method, CompilationContext $context): void
    {
        /**
         * Initialize the properties within create_object, handler code
         */
        if (str_starts_with($method->getName(), 'zephir_init_properties')) {
            $context->codePrinter->increaseLevel();
            $context->codePrinter->output('{');
            $context->codePrinter->increaseLevel();
            $context->codePrinter->output('zval local_this_ptr, *this_ptr = &local_this_ptr;');
            $context->codePrinter->output('ZEPHIR_CREATE_OBJECT(this_ptr, class_type);');
            $context->codePrinter->decreaseLevel();
        }
    }

    public function onPreInitVar(Method $method): string
    {
        if (!$method instanceof FunctionDefinition && !$method->isInternal()) {
            return "zval *this_ptr = getThis();\n"; // TODO: think about a better way to solve this.
        }

        return '';
    }

    public function propertyIsset(Variable $var, $key)
    {
        return new CompiledExpression(
            'bool',
            'zephir_isset_property(' . $this->getVariableCode($var) . ', SL("' . $key . '"))',
            null
        );
    }

    /**
     * @param                    $value
     * @param CompilationContext $context
     *
     * @return bool|string|Variable
     *
     * @throws CompilerException
     */
    public function resolveValue($value, CompilationContext $context): Variable | bool | string
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
                    throw new CompilerException('Unknown constant ' . $value->getName());
            }
        }

        if ('null' == $value || 'true' == $value || 'false' == $value) {
            $varName = '__$' . $value;
            if (!$context->symbolTable->hasVariable($varName)) {
                $tempVariable = new Variable('variable', $varName, $context->branchManager->getCurrentBranch());
                $context->symbolTable->addRawVariable($tempVariable);
            }

            $tempVariable = $context->symbolTable->getVariableForWrite($varName, $context);
            $tempVariable->increaseUses();
            $tempVariable->setUsed(true);

            if ('null' == $value) {
                $tempVariable->setDynamicTypes('null');
            } else {
                $tempVariable->setDynamicTypes('bool');
            }

            $value = $this->getVariableCode($tempVariable);
        } else {
            if ($value instanceof CompiledExpression) {
                if (in_array($value->getType(), ['array', 'variable', 'mixed'])) {
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

    public function returnString($value, CompilationContext $context, $useCodePrinter = true)
    {
        return $this->returnHelper('RETURN_MM_STRING', $value, $context, $useCodePrinter);
    }

    public function subStaticProperty($classEntry, $property, $value, CompilationContext $context): void
    {
        $value = $this->resolveValue($value, $context);
        $context->codePrinter->output(
            'zephir_sub_static_property_ce(' . $classEntry . ', ZEND_STRL("' . $property . '"), ' . $value . ');'
        );
    }

    public function updateArray(
        Variable $symbolVariable,
        $key,
        $value,
        CompilationContext $compilationContext,
        $flags = null
    ): void {
        $value = $this->resolveValue($value, $compilationContext, true);
        if (!isset($flags)) {
            $flags = 'PH_COPY';
        }

        if ($key instanceof Variable) {
            switch ($key->getType()) {
                case 'string':
                case 'variable':
                    $compilationContext->codePrinter->output(
                        'zephir_array_update_zval(' . $this->getVariableCode(
                            $symbolVariable
                        ) . ', ' . $this->getVariableCode($key) . ', ' . $value . ', ' . $flags . ');'
                    );
                    break;

                case 'int':
                case 'uint':
                case 'long':
                    $compilationContext->codePrinter->output(
                        'zephir_array_update_long(' . $this->getVariableCode($symbolVariable) . ', ' . $key->getName(
                        ) . ', ' . $value . ', ' . $flags . ' ZEPHIR_DEBUG_PARAMS_DUMMY);'
                    );
                    break;

                default:
                    throw new CompilerException(
                        'updateArray: Found a variable with unsupported type ' . $key->getType()
                    );
            }
        } elseif ($key instanceof CompiledExpression) {
            switch ($key->getType()) {
                case 'string':
                    $compilationContext->codePrinter->output(
                        'zephir_array_update_string(' . $this->getVariableCode(
                            $symbolVariable
                        ) . ', SL("' . $key->getCode() . '"), ' . $value . ', ' . $flags . ');'
                    );
                    break;

                case 'int':
                    $compilationContext->codePrinter->output(
                        'zephir_array_update_long(' . $this->getVariableCode($symbolVariable) . ', ' . $key->getCode(
                        ) . ', ' . $value . ', ' . $flags . ' ZEPHIR_DEBUG_PARAMS_DUMMY);'
                    );
                    break;

                case 'variable':
                    $this->updateArray(
                        $symbolVariable,
                        $compilationContext->symbolTable->getVariableForRead($key->getCode()),
                        $value,
                        $compilationContext,
                        $flags
                    );
                    break;

                default:
                    throw new CompilerException(
                        'updateArray: Found an expression with unsupported type ' . $key->getType()
                    );
            }
        } else {
            throw new CompilerException('?');
        }
    }

    /**
     * @param Variable           $variable
     * @param string|Variable    $property
     * @param mixed              $value
     * @param CompilationContext $context
     *
     * @return void
     */
    public function updateProperty(Variable $variable, $property, $value, CompilationContext $context): void
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

    public function updateStaticProperty($classEntry, $property, $value, CompilationContext $context): void
    {
        // TODO(serghei): Sort out as well as above
        $value = $this->resolveValue($value, $context);
        $context->codePrinter->output(
            'zephir_update_static_property_ce(' . $classEntry . ', ZEND_STRL("' . $property . '"), ' . $value . ');'
        );
    }

    public function zvalOperator(
        $zvalOperator,
        Variable $expected,
        Variable $variableLeft,
        Variable $variableRight,
        CompilationContext $compilationContext
    ): void {
        $expected = $this->getVariableCode($expected);
        $op1      = $this->getVariableCode($variableLeft);
        $op2      = $this->getVariableCode($variableRight);

        $compilationContext->codePrinter->output($zvalOperator . '(' . $expected . ', ' . $op1 . ', ' . $op2 . ');');
    }

    /**
     * Assign value to variable helper.
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
            $value = 'ZVAL_STRING' === $macro ? '"' . $value . '"' : $value;
        }

        $output = $macro . '(' . $variableName . ', ' . $value . ');';
        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $output;
    }

    protected function returnHelper(
        string $macro,
        $value,
        CompilationContext $context,
        $useCodePrinter,
        $doCopy = null
    ): string {
        if ($value instanceof Variable) {
            $value = $value->getName();
        } elseif ($macro === 'RETURN_MM_STRING' && !str_starts_with($value, 'ZEPHIR_GLOBAL')) {
            $value = '"' . $value . '"';
        }

        $copyStr = '';
        if (true === $doCopy) {
            $copyStr = ', 1';
        } elseif (false === $doCopy) {
            $copyStr = ', 0';
        } elseif (isset($doCopy)) {
            $copyStr = ', ' . $doCopy;
        }

        $output = $macro . '(' . $value . $copyStr . ');';
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
     * @return array
     *
     * @throws CompilerException
     */
    private function resolveOffsetExprs(array $offsetExprs, CompilationContext $compilationContext): array
    {
        $keys         = '';
        $offsetItems  = [];
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
                    $keys          .= 'l';
                    $offsetItems[] = $offsetExpr->getCode();
                    ++$numberParams;
                    break;

                case 'string':
                    $keys          .= 's';
                    $offsetItems[] = 'SL("' . $offsetExpr->getCode() . '")';
                    $numberParams  += 2;
                    break;

                case 'variable':
                    $variableIndex = $compilationContext->symbolTable->getVariableForRead(
                        $offsetExpr->getCode(),
                        $compilationContext,
                    );

                    switch ($variableIndex->getType()) {
                        case 'int':
                        case 'uint':
                        case 'long':
                        case 'ulong':
                            $keys          .= 'l';
                            $offsetItems[] = $this->getVariableCode($variableIndex);
                            ++$numberParams;
                            break;
                        case 'string':
                        case 'variable':
                            $keys          .= 'z';
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
