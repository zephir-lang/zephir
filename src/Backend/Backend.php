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

use Zephir\Cache\PropertyCacheSlots;
use Zephir\Class\Method\Method;
use Zephir\Code\Printer;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Compiler;
use Zephir\Config;
use Zephir\Detectors\YieldDetector;
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

    public function arrayIsset(Variable $var, $resolvedExpr, $expression): CompiledExpression
    {
        /**
         * isset() must match PHP semantics: key exists AND value is not null.
         * The *_isset_value_* helpers do both checks; *_isset_* (key-only) is
         * kept for callers that legitimately want existence semantics.
         * See https://github.com/zephir-lang/zephir/issues/2385.
         */
        if (!($resolvedExpr instanceof Variable)) {
            if ('string' == $resolvedExpr->getType()) {
                return new CompiledExpression(
                    'bool',
                    'zephir_array_isset_value_string('
                    . $this->getVariableCode($var)
                    . ', SL("'
                    . $resolvedExpr->getCode()
                    . '"))',
                    $expression
                );
            }
        }

        if (!($resolvedExpr instanceof Variable)) {
            if ('string' == $resolvedExpr->getType()) {
                return new CompiledExpression(
                    'bool',
                    'zephir_array_isset_value_string('
                    . $this->getVariableCode($var)
                    . ', SS("'
                    . $resolvedExpr->getCode()
                    . '"))',
                    $expression
                );
            }

            return new CompiledExpression(
                'bool',
                'zephir_array_isset_value_long('
                . $this->getVariableCode($var)
                . ', '
                . $resolvedExpr->getCode() . ')',
                $expression
            );
        }

        if ('int' == $resolvedExpr->getType() || 'long' == $resolvedExpr->getType()) {
            return new CompiledExpression(
                'bool',
                'zephir_array_isset_value_long('
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
                'zephir_array_isset_value(' . $this->getVariableCode($var) . ', ' . $this->getVariableCode(
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

        $symbol  = $this->resolveValue($symbolVariable, $compilationContext);
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

    /**
     * Boxes a native C byte (a `char`/`uchar` variable, or a raw C char
     * expression) into a 1-character PHP string.
     *
     * This cannot go through assignHelper(): it unwraps a Variable to its bare
     * name, which would emit `ZVAL_STRING(&s, ch)` — passing a `char` where a
     * `const char *` is expected. The byte's address is what ZVAL_STRINGL needs.
     */
    public function assignChar(
        Variable $variable,
        $value,
        CompilationContext $context,
        bool $useCodePrinter = true
    ): string {
        $output = sprintf(
            'ZVAL_STRINGL(%s, &%s, 1);',
            $this->getVariableCode($variable),
            $value instanceof Variable ? $value->getName() : $value
        );

        if ($useCodePrinter) {
            $context->codePrinter->output($output);
        }

        return $output;
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
        $valueVariable = $this->resolveValue($valueVariable, $compilationContext);
        $variable->setUsed(true);

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
        $valueVariable = $this->resolveValue($valueVariable, $compilationContext);

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

    public function assignString(
        Variable $variable,
        $value,
        CompilationContext $context,
        bool $useCodePrinter = true
    ): string {
        // A native zend_string * value must be boxed with ZVAL_STR; ZVAL_STRING
        // expects a const char * and fails to compile otherwise. See #2562.
        $macro = ($value instanceof Variable && $value->isNativeString())
            ? 'ZVAL_STR'
            : 'ZVAL_STRING';

        return $this->assignHelper(
            $macro,
            $this->getVariableCode($variable),
            $value,
            $context,
            $useCodePrinter
        );
    }

    /**
     * Assigns a zval to another.
     */
    public function assignZval(Variable $variable, $code, CompilationContext $context): void
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

    public function createClosure(Variable $variable, $classDefinition, CompilationContext $context, bool $bindThis = false): void
    {
        $symbol = $this->getVariableCode($variable);
        $thisArg = $bindThis ? 'this_ptr' : 'NULL';
        $context->codePrinter->output(
            'zephir_create_closure_ex(' . $symbol . ', ' . $thisArg . ', ' . $classDefinition->getClassEntry(
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

    /**
     * Declares an array class constant by building its value in the class
     * initializer and handing it to zephir_declare_class_constant().
     *
     * @see https://github.com/zephir-lang/zephir/issues/2533
     */
    public function declareArrayConstant(string $name, array $node, CompilationContext $context): void
    {
        $ce      = $context->classDefinition->getClassEntry($context);
        $printer = $context->codePrinter;
        $counter = 0;
        $lines   = [];

        $rootVar = $this->buildConstantArray($node, $lines, $counter);

        $printer->output('{');
        $printer->increaseLevel();
        foreach ($lines as $line) {
            $printer->output($line);
        }
        $printer->output(
            sprintf('zephir_declare_class_constant_array(%s, SL("%s"), &%s);', $ce, $name, $rootVar)
        );
        $printer->decreaseLevel();
        $printer->output('}');
    }

    /**
     * Emits a class property whose default value is an array literal, built into
     * a persistent immutable array on the class entry (see
     * zephir_declare_property_array). Used for trait properties so PHP's native
     * trait binding carries the default into userland classes that `use` them.
     *
     * @param string $visibility ZEND_ACC_* accessor string for the property
     *
     * @see https://github.com/zephir-lang/zephir/issues/2607
     */
    public function declareArrayProperty(string $name, array $node, string $visibility, CompilationContext $context): void
    {
        $ce      = $context->classDefinition->getClassEntry($context);
        $printer = $context->codePrinter;
        $counter = 0;
        $lines   = [];

        $rootVar = $this->buildConstantArray($node, $lines, $counter);

        $printer->output('{');
        $printer->increaseLevel();
        foreach ($lines as $line) {
            $printer->output($line);
        }
        $printer->output(
            sprintf('zephir_declare_property_array(%s, SL("%s"), &%s, %s);', $ce, $name, $rootVar, $visibility)
        );
        $printer->decreaseLevel();
        $printer->output('}');
    }

    /**
     * Emits a typed class property declaration (issue #2608).
     *
     * Builds the default value into a fresh local zval — array via
     * {@see buildConstantArray()}, scalars via ZVAL_*, and a missing default as
     * ZVAL_UNDEF (uninitialized) — then hands it to the kernel wrapper which
     * makes it persistent and calls the engine's zend_declare_typed_property.
     *
     * @param string      $name       property name
     * @param array|null  $default    default AST node, or null for uninitialized
     * @param string      $visibility ZEND_ACC_* accessor string
     * @param string      $typeMask   MAY_BE_* expression (e.g. "MAY_BE_STRING|MAY_BE_NULL")
     * @param string|null $className  escaped FQCN for a class type, or null
     */
    public function declareTypedProperty(string $name, ?array $default, string $visibility, string $typeMask, ?string $className, CompilationContext $context): void
    {
        $ce      = $context->classDefinition->getClassEntry($context);
        $printer = $context->codePrinter;
        $counter = 0;
        $lines   = [];

        if (null === $default) {
            $rootVar = '_zc0';
            $lines[] = 'zval _zc0;';
            $lines[] = 'ZVAL_UNDEF(&_zc0);';
        } elseif (in_array($default['type'], ['array', 'empty-array'], true)) {
            $rootVar = $this->buildConstantArray($default, $lines, $counter);
        } else {
            $rootVar = '_zc0';
            $lines[] = 'zval _zc0;';
            $lines[] = $this->typedScalarInit('_zc0', $default);
        }

        $classArg = null === $className
            ? 'NULL, 0'
            : sprintf('SL("%s")', $className);

        $printer->output('{');
        $printer->increaseLevel();
        foreach ($lines as $line) {
            $printer->output($line);
        }
        $printer->output(
            sprintf(
                'zephir_declare_typed_property(%s, SL("%s"), &%s, %s, %s, %s);',
                $ce,
                $name,
                $rootVar,
                $visibility,
                $typeMask,
                $classArg
            )
        );
        $printer->decreaseLevel();
        $printer->output('}');
    }

    /**
     * Declare a union-typed class property that includes at least one class
     * member (issue #2613), e.g. `<A> | <B>` or `string | <Foo>`. The scalar
     * and null members are collapsed into `$typeMask`; the class members are
     * emitted as a C string array so the engine builds the object part of the
     * union (single class, or a zend_type_list for two or more).
     */
    public function declareTypedPropertyUnion(string $name, ?array $default, string $visibility, string $typeMask, array $classNames, CompilationContext $context): void
    {
        $ce      = $context->classDefinition->getClassEntry($context);
        $printer = $context->codePrinter;
        $counter = 0;
        $lines   = [];

        if (null === $default) {
            $rootVar = '_zc0';
            $lines[] = 'zval _zc0;';
            $lines[] = 'ZVAL_UNDEF(&_zc0);';
        } elseif (in_array($default['type'], ['array', 'empty-array'], true)) {
            $rootVar = $this->buildConstantArray($default, $lines, $counter);
        } else {
            $rootVar = '_zc0';
            $lines[] = 'zval _zc0;';
            $lines[] = $this->typedScalarInit('_zc0', $default);
        }

        $literals = array_map(static fn (string $cn): string => '"' . $cn . '"', $classNames);

        $printer->output('{');
        $printer->increaseLevel();
        foreach ($lines as $line) {
            $printer->output($line);
        }
        $printer->output('const char *_zut[] = { ' . implode(', ', $literals) . ' };');
        $printer->output(
            sprintf(
                'zephir_declare_typed_property_union(%s, SL("%s"), &%s, %s, %s, _zut, %d);',
                $ce,
                $name,
                $rootVar,
                $visibility,
                '' === $typeMask ? '0' : $typeMask,
                count($classNames)
            )
        );
        $printer->decreaseLevel();
        $printer->output('}');
    }

    /**
     * Renders the ZVAL_* initializer for a scalar typed-property default.
     */
    private function typedScalarInit(string $var, array $default): string
    {
        return match ($default['type']) {
            'int', 'long', 'uint', 'ulong' => sprintf('ZVAL_LONG(&%s, %s);', $var, $default['value']),
            'double', 'float'              => sprintf('ZVAL_DOUBLE(&%s, %s);', $var, $default['value']),
            'bool'                         => sprintf('ZVAL_BOOL(&%s, %s);', $var, 'true' === $default['value'] ? '1' : '0'),
            'null'                         => sprintf('ZVAL_NULL(&%s);', $var),
            'char', 'string', 'istring'    => $this->zvalStringInit($var, (string) $default['value']),
            default => throw new CompilerException('Unsupported typed property default: ' . $default['type']),
        };
    }

    /**
     * Renders a `ZVAL_STRINGL` initializer whose length is measured by the C
     * compiler via `sizeof(literal) - 1` (exactly what the `SL()` macro expands
     * to). `strlen()` on the PHP-side value over-counts escape sequences — a
     * source `"\\"` is two PHP chars but one emitted byte — which corrupted the
     * runtime string. `sizeof()` counts the bytes the compiler actually emits,
     * correct for every escape (and any embedded NUL). `SL()` cannot be nested
     * inside the `ZVAL_STRINGL` macro (the preprocessor counts the macro's args
     * before expanding `SL`, so its hidden comma yields "one arg given"), so the
     * length is spelled out. See #2617.
     */
    private function zvalStringInit(string $var, string $value): string
    {
        $literal = Name::addSlashes($value);

        return sprintf('ZVAL_STRINGL(&%s, "%s", sizeof("%s") - 1);', $var, $literal, $literal);
    }

    /**
     * Recursively emits C that builds an array literal AST node into a fresh
     * local zval, returning the name of that zval. Used for array class
     * constants which must be materialized in the class initializer.
     *
     * @param array    $node    array/empty-array AST node
     * @param string[] $lines   accumulator for the emitted C lines
     * @param int      $counter unique-variable counter (by reference)
     */
    private function buildConstantArray(array $node, array &$lines, int &$counter): string
    {
        $var     = '_zc' . $counter++;
        $items   = $node['left'] ?? [];
        $lines[] = sprintf('zval %s;', $var);
        $lines[] = sprintf('array_init_size(&%s, %d);', $var, count($items) + 1);

        foreach ($items as $item) {
            $value = $item['value'];
            $key   = $item['key'] ?? null;

            if (in_array($value['type'], ['array', 'empty-array'], true)) {
                $child   = $this->buildConstantArray($value, $lines, $counter);
                $lines[] = $this->arrayAddLine($var, $key, 'zval', '&' . $child);
                continue;
            }

            [$func, $arg] = $this->scalarConstantValue($value);
            $lines[]      = $this->arrayAddLine($var, $key, $func, $arg);
        }

        return $var;
    }

    /**
     * Maps a scalar literal AST node to the add_*_index helper suffix and the
     * already-rendered C argument(s).
     *
     * @return array{0: string, 1: string} [helper-suffix, argument-string]
     */
    private function scalarConstantValue(array $value): array
    {
        return match ($value['type']) {
            'int', 'long', 'uint', 'ulong'   => ['long', (string) ($value['value'])],
            'double', 'float'                => ['double', (string) ($value['value'])],
            'bool'                           => ['bool', 'true' === $value['value'] ? '1' : '0'],
            'null'                           => ['null', ''],
            'char', 'string', 'istring'      => [
                // SL() delegates the byte count to the C compiler; see #2617.
                'stringl',
                sprintf('SL("%s")', Name::addSlashes((string) $value['value'])),
            ],
            default                          => throw new CompilerException(
                'Unsupported array constant element type: ' . $value['type']
            ),
        };
    }

    /**
     * Builds a single add_(next_index|index|assoc)_<func> C call line for an
     * array constant element, honoring an optional integer/string key.
     */
    private function arrayAddLine(string $var, ?array $key, string $func, string $arg): string
    {
        $tail = '' === $arg ? '' : ', ' . $arg;

        if ($key === null) {
            return sprintf('add_next_index_%s(&%s%s);', $func, $var, $tail);
        }

        if ('int' === $key['type'] || 'long' === $key['type']) {
            return sprintf('add_index_%s(&%s, %d%s);', $func, $var, (int) $key['value'], $tail);
        }

        $keyValue = (string) $key['value'];

        return sprintf(
            // SL() delegates the key byte count to the C compiler; see #2617.
            'add_assoc_%s_ex(&%s, SL("%s")%s);',
            $func,
            $var,
            Name::addSlashes($keyValue),
            $tail
        );
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
             * @var Variable[] $variables
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

        /* Are we going to init default object property value? */
        if ($context->currentMethod && $context->currentMethod->isInitializer()) {
            $context->codePrinter->output(
                sprintf('zephir_read_property_ex(%s, %s, ZEND_STRL("%s"), %s);', $symbol, $variableCode, $property, $flags)
            );

            return;
        }

        /*
         * Cached read: pass a method-scope interned zend_string for the name
         * (no per-call alloc). cache_slot 0 = uncached in Stage 1; Stage 2
         * threads a real inline-cache slot here.
         */
        $nameSlot  = $this->internedPropertyName((string) $property, $context);
        $cacheSlot = $this->propertyCacheSlot($variableVariable, (string) $property, $context);
        $context->codePrinter->output(
            sprintf('zephir_read_property_cached(%s, %s, %s, %d, %s);', $symbol, $variableCode, $nameSlot, $cacheSlot, $flags)
        );
    }

    /**
     * Registers (once per property name per method) a method-scope interned
     * zend_string slot for a compile-time-known object-property name and
     * returns its C variable. The `static` declaration and lazy init are
     * emitted at function scope by Method::compile() so the reference is valid
     * from any nested block. Mirrors the isset `_fast` slot, hoisted to
     * function scope. See property-access optimization (issue #1884 follow-up).
     */
    private function internedPropertyName(string $key, CompilationContext $context): string
    {
        if (!isset($context->propertyNameCache[$key])) {
            $context->propertyNameCache[$key] = '_zephir_prop_' . $context->propertyNameCacheCounter++;
        }

        return $context->propertyNameCache[$key];
    }

    /**
     * Returns the project-global inline-cache slot index for an object
     * property access, or 0 (uncached) when it should not be cached.
     *
     * Only `this->prop` is cached: it is near-monomorphic (this_ptr is the
     * compiling class or a subclass, whose ce the engine re-validates), so a
     * slot keyed by (compiling-class, property) stays warm and correct.
     * Accesses on other objects stay uncached to avoid mis-keyed sharing.
     * See https://github.com/zephir-lang/zephir/issues/1902.
     */
    private function propertyCacheSlot(Variable $objectVariable, string $key, CompilationContext $context): int
    {
        if ('this' !== $objectVariable->getRealName()) {
            return 0;
        }

        $className = $context->classDefinition?->getCompleteName();
        if (empty($className)) {
            return 0;
        }

        return PropertyCacheSlots::getSlot($className, $key);
    }

    /**
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
        $codePrinter = $compilationContext->codePrinter;

        /**
         * Use type inference to skip branches that cannot be reached at runtime.
         * - 'array' / 'object' in dynamicTypes => that branch is reachable
         * - 'unknown' / 'undefined' => we don't know, keep both branches
         * Scalar-only types (null, bool, string, long, double) get both branches as a
         * fallback: zephir_is_iterable() emits a runtime warning but execution then
         * falls through to the object branch (current behavior).
         */
        $dynamicTypes = $exprVariable->getDynamicTypes();
        $mayBeArray   = isset($dynamicTypes['unknown'])
            || isset($dynamicTypes['undefined'])
            || isset($dynamicTypes['array']);
        $mayBeObject  = isset($dynamicTypes['unknown'])
            || isset($dynamicTypes['undefined'])
            || isset($dynamicTypes['object']);

        if (!$mayBeArray && !$mayBeObject) {
            $mayBeArray  = true;
            $mayBeObject = true;
        }

        $emitTypeWrapper = $mayBeArray && $mayBeObject;

        $codePrinter->output(
            'zephir_is_iterable(' . $this->getVariableCode(
                $exprVariable
            ) . ', ' . $duplicateHash . ', "' . Compiler::getShortUserPath(
                $statement['file']
            ) . '", ' . $statement['line'] . ');'
        );

        /**
         * ZEND_HASH_FOREACH macros keep their cursor in block-scoped locals,
         * which a generator resume `goto` would jump past (undefined
         * behavior). When the loop body of a generator step contains a
         * yield, iterate position-based over a snapshot instead: the
         * snapshot zval and the HashPosition are plain function-scope
         * locals the suspension machinery can save/restore, and the
         * snapshot matches PHP's by-value foreach semantics. Issue #1849.
         */
        if (
            $mayBeArray
            && $compilationContext->currentMethod?->isGeneratorStep()
            && isset($statement['statements'])
            && (new YieldDetector())->detect($statement['statements'])
        ) {
            $this->forStatementSuspendableArray(
                $exprVariable,
                $keyVariable,
                $variable,
                $emitTypeWrapper,
                $statement,
                $statementBlock,
                $compilationContext
            );
        } elseif ($mayBeArray) {
            $tempVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
            $tempVariable->setIsDoublePointer(true);

            if ($emitTypeWrapper) {
                $codePrinter->output('if (Z_TYPE_P(' . $this->getVariableCode($exprVariable) . ') == IS_ARRAY) {');
                $codePrinter->increaseLevel();
            }

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

            if ($emitTypeWrapper) {
                $codePrinter->decreaseLevel();
            }
        }

        if ($mayBeObject) {
            $tempValidVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
            /*
             * A first-iteration flag is used to move next() to the TOP of the while(1)
             * body so that a C `continue` in user code correctly advances the iterator.
             *
             * Without this, `continue` jumps to the while(1) condition, skipping the
             * next() call that was at the bottom of the loop body, producing an infinite
             * loop on the first element that triggered continue.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2546
             */
            $firstIterFlag = $compilationContext->symbolTable->addTemp('bool', $compilationContext);

            if ($emitTypeWrapper) {
                $codePrinter->output('} else {');
                $codePrinter->increaseLevel();
            }

            $codePrinter->output(
                'ZEPHIR_CALL_METHOD(NULL, ' . $this->getVariableCode($exprVariable) . ', "rewind", NULL, 0);'
            );
            $codePrinter->output('zephir_check_call_status();');
            $codePrinter->output($firstIterFlag->getName() . ' = 1;');

            $codePrinter->output('while (1) {');
            $codePrinter->increaseLevel();

            // next() lives here so that `continue` in the user body reaches it.
            $codePrinter->output('if (' . $firstIterFlag->getName() . ') {');
            $codePrinter->increaseLevel();
            $codePrinter->output($firstIterFlag->getName() . ' = 0;');
            $codePrinter->decreaseLevel();
            $codePrinter->output('} else {');
            $codePrinter->increaseLevel();
            $codePrinter->output(
                'ZEPHIR_CALL_METHOD(NULL, ' . $this->getVariableCode($exprVariable) . ', "next", NULL, 0);'
            );
            $codePrinter->output('zephir_check_call_status();');
            $codePrinter->decreaseLevel();
            $codePrinter->output('}');

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

            $codePrinter->decreaseLevel();
            $codePrinter->output('}');

            if ($emitTypeWrapper) {
                $codePrinter->decreaseLevel();
                $codePrinter->output('}');
            }
        }

        /* Since we do not observe, still do cleanup */
        $variable?->initVariant($compilationContext);
        $keyVariable?->initVariant($compilationContext);
    }

    /**
     * Suspension-safe `for k, v in <array>` used inside generator step
     * bodies whose loop contains a yield (issue #1849): a HashPosition
     * cursor over a snapshot zval. Both live at function scope and are
     * saved/restored by the generator slot machinery; the current entry is
     * copied into the user variables and the cursor advanced *before* the
     * body runs, so `continue`, `break` and suspension all behave.
     */
    private function forStatementSuspendableArray(
        Variable $exprVariable,
        ?Variable $keyVariable,
        ?Variable $variable,
        bool $emitTypeWrapper,
        $statement,
        $statementBlock,
        CompilationContext $compilationContext
    ): void {
        $codePrinter = $compilationContext->codePrinter;
        $symbolTable = $compilationContext->symbolTable;

        $symbolTable->mustGrownStack(true);

        if ($emitTypeWrapper) {
            $codePrinter->output('if (Z_TYPE_P(' . $this->getVariableCode($exprVariable) . ') == IS_ARRAY) {');
            $codePrinter->increaseLevel();
        }

        /* getTempVariableForWrite() already emits the (N)VAR init. */
        $snapVariable = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
        $snapCode = $this->getVariableCode($snapVariable);
        $codePrinter->output('ZVAL_COPY(' . $snapCode . ', ' . $this->getVariableCode($exprVariable) . ');');

        $positionVariable = $symbolTable->addTemp('HashPosition', $compilationContext);
        $position         = '&' . $positionVariable->getName();

        $currentVariable = $symbolTable->addTemp('variable', $compilationContext);
        $currentVariable->setIsDoublePointer(true);
        $current = $currentVariable->getName();

        $hash    = 'Z_ARRVAL_P(' . $snapCode . ')';
        $reverse = !empty($statement['reverse']);

        $codePrinter->output(
            'zend_hash_internal_pointer_' . ($reverse ? 'end' : 'reset') . '_ex(' . $hash . ', ' . $position . ');'
        );
        $codePrinter->output('while (1) {');
        $codePrinter->increaseLevel();
        $codePrinter->output($current . ' = zend_hash_get_current_data_ex(' . $hash . ', ' . $position . ');');
        $codePrinter->output('if (' . $current . ' == NULL) {');
        $codePrinter->output("\t" . 'break;');
        $codePrinter->output('}');

        if ($keyVariable instanceof Variable) {
            $keyVariable->initVariant($compilationContext);
            $codePrinter->output(
                'zend_hash_get_current_key_zval_ex(' . $hash . ', '
                . $this->getVariableCode($keyVariable) . ', ' . $position . ');'
            );
        }

        if ($variable instanceof Variable) {
            $variable->initVariant($compilationContext);
            $codePrinter->output(
                'ZVAL_COPY(' . $this->getVariableCode($variable) . ', ' . $current . ');'
            );
        }

        $codePrinter->output(
            'zend_hash_move_' . ($reverse ? 'backwards' : 'forward') . '_ex(' . $hash . ', ' . $position . ');'
        );

        if (isset($statement['statements'])) {
            $statementBlock->isLoop(true);
            if (isset($statement['key'])) {
                $statementBlock->getMutateGatherer()->increaseMutations($statement['key']);
            }
            $statementBlock->getMutateGatherer()->increaseMutations($statement['value']);
            $statementBlock->compile($compilationContext);
        }

        $codePrinter->decreaseLevel();
        $codePrinter->output('}');

        if ($emitTypeWrapper) {
            /* Leave the `if (Z_TYPE... == IS_ARRAY) {` open: the object
             * branch that follows emits its own `} else {` and final `}`. */
            $codePrinter->decreaseLevel();
        }
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

            case 'zephir_method_globals':
                $pointer = '*';
                $code = 'zephir_method_globals';
                break;

            default:
                throw new CompilerException('Unsupported type in declare: ' . $type);
        }

        return [$pointer, $code];
    }

    /**
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
        $variable->setUsed(true);

        if (
            $variable->isDoublePointer() ||
            in_array($variable->getName(), ['this_ptr', 'return_value']) ||
            in_array($variable->getType(), ['int', 'long'])
        ) {
            return $variable->getName();
        }

        if ($variable->isNativeString()) {
            return '&' . $variable->getName() . '_zv';
        }

        return '&' . $variable->getName();
    }

    /**
     * Wraps a zend_string * variable into a temp zval for use in zval-expecting operations.
     * Returns the temp Variable (type 'variable') with ZVAL_STR already emitted.
     */
    public function wrapZendStringToZval(Variable $variable, CompilationContext $context): Variable
    {
        $tempVar = $context->symbolTable->getTempLocalVariableForWrite('variable', $context);
        $context->codePrinter->output(
            sprintf('ZVAL_STR(&%s, %s);', $tempVar->getName(), $variable->getName())
        );

        return $tempVar;
    }

    public function ifVariableValueUndefined(
        Variable $var,
        CompilationContext $context,
        $useBody = false,
        $useCodePrinter = true
    ): string {
        /**
         * Native zend_string * optional params: initialized to NULL,
         * Z_PARAM_STR only sets the pointer when the argument is provided.
         */
        if ($var->isNativeString()) {
            $body   = '!' . $var->getName();
            $output = 'if (' . $body . ') {';
            if ($useCodePrinter) {
                $context->codePrinter->output($output);
            }

            return $useBody ? $body : $output;
        }

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
    public function initArray(Variable $variable, CompilationContext $context, ?int $size = null): void
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
        $macro = !$second ? 'ZEPHIR_INIT_VAR' : 'ZEPHIR_INIT_NVAR';
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

    public function propertyIsset(Variable $var, $key, ?CompilationContext $context = null): CompiledExpression
    {
        /*
         * PHP isset() semantics — see https://github.com/zephir-lang/zephir/issues/2385.
         *
         * Static-name property isset goes through has_property, which needs
         * a zend_string. Allocating one per call would mean a heap alloc per
         * isset; instead emit a method-static zend_string * slot that the
         * generated method initializes lazily on first call and reuses for
         * the lifetime of the worker. Repeated isset() of the same property
         * within a method shares one slot.
         */
        if ($context !== null) {
            if (!isset($context->issetPropertyCache[$key])) {
                $cacheVar = '_zephir_isset_' . $context->issetPropertyCacheCounter++;
                $context->issetPropertyCache[$key] = $cacheVar;

                $escapedKey = addslashes($key);
                $context->codePrinter->output('static zend_string *' . $cacheVar . ' = NULL;');
                $context->codePrinter->output('if (UNEXPECTED(!' . $cacheVar . ')) {');
                $context->codePrinter->output(
                    "\t" . $cacheVar . ' = zend_string_init("' . $escapedKey . '", '
                    . strlen($key) . ', 1);'
                );
                $context->codePrinter->output('}');
            }

            return new CompiledExpression(
                'bool',
                'zephir_isset_property_value_fast(' . $this->getVariableCode($var) . ', '
                . $context->issetPropertyCache[$key] . ')',
                null
            );
        }

        /* Fallback for callers that don't pass a CompilationContext. */
        return new CompiledExpression(
            'bool',
            'zephir_isset_property_value(' . $this->getVariableCode($var) . ', SL("' . $key . '"))',
            null
        );
    }

    /**
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
            throw new CompilerException(
                'updateArray: Unsupported array key. Expected a variable or a '
                . 'compiled expression but got ' . get_debug_type($key)
            );
        }
    }

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

        /* Are we going to init default object property value? */
        if ($context->currentMethod && $context->currentMethod->isInitializer()) {
            $context->codePrinter->output(
                sprintf('zephir_update_property_zval_ex(%s, ZEND_STRL("%s"), %s);', $this->getVariableCode($variable), $property, $value)
            );

            return;
        }

        /*
         * Cached write: method-scope interned zend_string name, cache_slot 0
         * (uncached) in Stage 1; Stage 2 threads a real inline-cache slot.
         */
        $nameSlot  = $this->internedPropertyName((string) $property, $context);
        $cacheSlot = $this->propertyCacheSlot($variable, (string) $property, $context);
        $context->codePrinter->output(
            sprintf('zephir_update_property_zval_cached(%s, %s, %d, %s);', $this->getVariableCode($variable), $nameSlot, $cacheSlot, $value)
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
