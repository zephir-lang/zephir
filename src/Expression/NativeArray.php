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

namespace Zephir\Expression;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\GlobalConstant;
use Zephir\Traits\VariablesTrait;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

use function count;
use function function_exists;
use function gmp_nextprime;
use function gmp_strval;
use function is_float;
use function is_int;
use function is_numeric;
use function var_export;

/**
 * Resolves expressions that create arrays
 */
class NativeArray
{
    use VariablesTrait;

    protected bool      $expecting         = true;
    protected ?Variable $expectingVariable = null;
    protected bool      $readOnly          = false;

    /**
     * Compiles an array initialization.
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        /**
         * Resolves the symbol that expects the value
         */
        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
                if ('variable' != $symbolVariable->getType() && 'array' != $symbolVariable->getType()) {
                    throw CompilerException::cannotUseVariableTypeAs(
                        $symbolVariable,
                        'as an array',
                        $expression
                    );
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'array',
                    $compilationContext,
                    $expression
                );
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'array',
                $compilationContext,
                $expression
            );
        }

        if (!isset($expression['left'])) {
            if ($this->expectingVariable) {
                $symbolVariable->initVariant($compilationContext);
            }

            /**
             * Mark the variable as an array
             */
            $symbolVariable->setDynamicTypes('array');

            return new CompiledExpression('array', $symbolVariable->getRealName(), $expression);
        }

        $codePrinter = $compilationContext->codePrinter;
        $compilationContext->headersManager->add('kernel/array');

        /**
         * This calculates a prime number bigger than the current array size to possibly
         * reduce hash collisions when adding new members to the array.
         */
        $arrayLength = count($expression['left']);
        if ($arrayLength >= 33 && function_exists('gmp_nextprime')) {
            $arrayLength = (int)gmp_strval(gmp_nextprime($arrayLength - 1));
        }

        if ($this->expectingVariable && $symbolVariable->getVariantInits() >= 1) {
            $symbolVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
            $symbolVariable->initVariant($compilationContext);
            $compilationContext->backend->initArray(
                $symbolVariable,
                $compilationContext,
                $arrayLength > 0 ? $arrayLength : null
            );
            $symbolVariable->setDynamicTypes('array');
        } else {
            if ($this->expectingVariable) {
                $symbolVariable->initVariant($compilationContext);
            }

            /**
             * Mark the variable as an array
             */
            $symbolVariable->setDynamicTypes('array');
            $compilationContext->backend->initArray(
                $symbolVariable,
                $compilationContext,
                $arrayLength > 0 ? $arrayLength : null
            );
        }
        foreach ($expression['left'] as $item) {
            if (!isset($item['key'])) {
                $expr         = new Expression($item['value']);
                $resolvedExpr = $expr->compile($compilationContext);
                $itemVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                $symbol       = $compilationContext->backend->getVariableCode($symbolVariable);
                $item         = $compilationContext->backend->resolveValue($itemVariable, $compilationContext);
                $codePrinter->output('zephir_array_fast_append(' . $symbol . ', ' . $item . ');');
                $this->checkVariableTemporal($itemVariable);

                continue;
            }

            $exprKey         = new Expression($item['key']);
            $resolvedExprKey = $exprKey->compile($compilationContext);
            $resolvedExprKey = $this->resolveDeclarationKey($resolvedExprKey, $item['key'], $compilationContext)
                ?? $compilationContext->backend->boxArrayKey($resolvedExprKey, $compilationContext);

            switch ($resolvedExprKey->getType()) {
                case Types::T_STRING:
                    $expr         = new Expression($item['value']);
                    $resolvedExpr = $expr->compile($compilationContext);
                    switch ($resolvedExpr->getType()) {
                        case Types::T_INT:
                        case Types::T_UINT:
                        case Types::T_LONG:
                        case Types::T_ULONG:
                        case Types::T_DOUBLE:
                        case Types::T_STRING:
                            $compilationContext->backend->addArrayEntry(
                                $symbolVariable,
                                $resolvedExprKey,
                                $resolvedExpr,
                                $compilationContext
                            );
                            break;

                        case Types::T_NULL:
                            $compilationContext->backend->updateArray(
                                $symbolVariable,
                                $resolvedExprKey,
                                'null',
                                $compilationContext,
                                'PH_COPY | PH_SEPARATE'
                            );
                            break;

                        case Types::T_ARRAY:
                        case Types::T_VARIABLE:
                        // A char is boxed by getArrayValue(); see #1629.
                        case Types::T_CHAR:
                        case Types::T_UCHAR:
                        // A bool may be any C condition, not only `true` or `false`; see #2771.
                        case Types::T_BOOL:
                            $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                            $compilationContext->backend->updateArray(
                                $symbolVariable,
                                $resolvedExprKey,
                                $valueVariable,
                                $compilationContext,
                                'PH_COPY | PH_SEPARATE'
                            );
                            $this->checkVariableTemporal($valueVariable);
                            break;

                        default:
                            throw new CompilerException(
                                'Invalid value type: ' . $resolvedExpr->getType(),
                                $item['value']
                            );
                    }
                    break;

                case Types::T_INT:
                case Types::T_UINT:
                case Types::T_LONG:
                case Types::T_ULONG:
                    $expr         = new Expression($item['value']);
                    $resolvedExpr = $expr->compile($compilationContext);
                    switch ($resolvedExpr->getType()) {
                        case Types::T_INT:
                        case Types::T_UINT:
                        case Types::T_LONG:
                        case Types::T_ULONG:
                        case Types::T_DOUBLE:
                        case Types::T_STRING:
                            $compilationContext->backend->addArrayEntry(
                                $symbolVariable,
                                $resolvedExprKey,
                                $resolvedExpr,
                                $compilationContext
                            );
                            break;

                        case Types::T_NULL:
                            $compilationContext->backend->updateArray(
                                $symbolVariable,
                                $resolvedExprKey,
                                'null',
                                $compilationContext,
                                'PH_COPY'
                            );
                            break;

                        case Types::T_ARRAY:
                        case Types::T_VARIABLE:
                        // A char is boxed by getArrayValue(); see #1629.
                        case Types::T_CHAR:
                        case Types::T_UCHAR:
                        // A bool may be any C condition, not only `true` or `false`; see #2771.
                        case Types::T_BOOL:
                            $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                            $compilationContext->backend->updateArray(
                                $symbolVariable,
                                $resolvedExprKey,
                                $valueVariable,
                                $compilationContext,
                                'PH_COPY'
                            );
                            $this->checkVariableTemporal($valueVariable);
                            break;

                        default:
                            throw new CompilerException(
                                'Invalid value type: ' . $item['value']['type'],
                                $item['value']
                            );
                    }
                    break;

                case Types::T_VARIABLE:
                    $variableVariable = $compilationContext->symbolTable->getVariableForRead(
                        $resolvedExprKey->getCode(),
                        $compilationContext,
                        $item['key']
                    );
                    switch ($variableVariable->getType()) {
                        case Types::T_INT:
                        case Types::T_UINT:
                        case Types::T_LONG:
                        case Types::T_ULONG:
                            $expr         = new Expression($item['value']);
                            $resolvedExpr = $expr->compile($compilationContext);
                            switch ($resolvedExpr->getType()) {
                                case Types::T_INT:
                                case Types::T_UINT:
                                case Types::T_LONG:
                                case Types::T_ULONG:
                                case Types::T_DOUBLE:
                                case Types::T_STRING:
                                    $compilationContext->backend->addArrayEntry(
                                        $symbolVariable,
                                        $resolvedExprKey,
                                        $resolvedExpr,
                                        $compilationContext
                                    );
                                    break;

                                default:
                                    $this->updateArrayWithValue(
                                        $symbolVariable,
                                        $variableVariable,
                                        $resolvedExpr,
                                        $compilationContext
                                    );
                                    break;
                            }
                            break;

                        case Types::T_STRING:
                        case Types::T_VARIABLE:
                        case Types::T_MIXED:
                            $expr         = new Expression($item['value']);
                            $resolvedExpr = $expr->compile($compilationContext);
                            $this->updateArrayWithValue(
                                $symbolVariable,
                                $variableVariable,
                                $resolvedExpr,
                                $compilationContext
                            );
                            // A dynamic key can be illegal
                            if (Types::T_STRING !== $variableVariable->getType()) {
                                $compilationContext->emitExceptionCheck();
                            }
                            break;

                        default:
                            throw CompilerException::cannotUseVariableTypeAs(
                                $variableVariable,
                                'as an array index',
                                $expression
                            );
                    }
                    break;

                default:
                    throw new CompilerException(
                        'Invalid key type: '
                        . $resolvedExprKey->getType(),
                        $item['key']
                    );
            }
        }

        return new CompiledExpression('array', $symbolVariable->getRealName(), $expression);
    }

    /**
     * Resolves an item to be added in an array.
     *
     * @param CompiledExpression $exprCompiled
     * @param CompilationContext $compilationContext
     *
     * @return GlobalConstant|Variable
     */
    /**
     * A property default is built by an initializer, which runs on every
     * `new` (or once at startup for a static one), while PHP converts a
     * declaration's keys once. A float, bool or null key whose value is known
     * at compile time, written as a literal or a constant, is therefore
     * folded here into the key PHP gives it, instead of being converted, and
     * deprecated, at run time. Returns null for any other key, which
     * boxArrayKey() then converts at run time.
     */
    private function resolveDeclarationKey(
        CompiledExpression $resolvedKey,
        array $keyAst,
        CompilationContext $compilationContext
    ): ?CompiledExpression {
        $method = $compilationContext->currentMethod;
        if (null === $method || !$method->isInitializer()) {
            return null;
        }

        $literalKey = $this->literalKeyNode($resolvedKey, $keyAst);
        if (null === $literalKey) {
            return null;
        }

        $compilationContext->backend->warnLossyFloatKey($literalKey, $compilationContext);
        $key = $compilationContext->backend->constantArrayKey($literalKey);

        return is_int($key)
            ? new CompiledExpression(Types::T_INT, (string) $key, $keyAst)
            : new CompiledExpression(Types::T_STRING, $key, $keyAst);
    }

    /**
     * The literal node of a compiled float, bool or null key, so a constant
     * such as `self::ON` folds like the literal it stands for. Returns null
     * when the key is C code, such as `1.5 + n` or a comparison, whose value
     * only exists at run time (#2771).
     */
    private function literalKeyNode(CompiledExpression $resolvedKey, array $keyAst): ?array
    {
        switch ($resolvedKey->getType()) {
            case Types::T_NULL:
                return ['type' => 'null', 'value' => null] + $keyAst;

            case Types::T_BOOL:
                $code = $resolvedKey->getBooleanCode();
                if ('1' !== $code && '0' !== $code) {
                    return null;
                }

                return ['type' => 'bool', 'value' => '1' === $code ? 'true' : 'false'] + $keyAst;

            case Types::T_DOUBLE:
                $code = $resolvedKey->getCode();
                if (!is_numeric($code)) {
                    return null;
                }

                // A PHP constant holds a float; var_export() keeps all its digits.
                $value = is_float($code) ? var_export($code, true) : (string) $code;

                return ['type' => 'double', 'value' => $value] + $keyAst;

            default:
                return null;
        }
    }

    /**
     * Writes any value under a key held in a variable: the value is boxed by
     * getArrayValue(), and zephir_array_update_*() converts the key as PHP
     * does, a numeric string included.
     */
    private function updateArrayWithValue(
        Variable $symbolVariable,
        Variable $keyVariable,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext
    ): void {
        $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
        $compilationContext->backend->updateArray(
            $symbolVariable,
            $keyVariable,
            $valueVariable,
            $compilationContext,
            'PH_COPY'
        );
        $this->checkVariableTemporal($valueVariable);
    }

    public function getArrayValue(
        CompiledExpression $exprCompiled,
        CompilationContext $compilationContext
    ) {
        switch ($exprCompiled->getType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignLong($tempVar, $exprCompiled->getCode(), $compilationContext);

                return $tempVar;

            case Types::T_CHAR:
            case Types::T_UCHAR:
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignLong(
                    $tempVar,
                    $exprCompiled->getCharCode(),
                    $compilationContext
                );

                return $tempVar;

            case Types::T_DOUBLE:
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignDouble($tempVar, $exprCompiled->getCode(), $compilationContext);

                return $tempVar;

            case Types::T_BOOL:
                if ('true' === $exprCompiled->getCode()) {
                    return new GlobalConstant('ZEPHIR_GLOBAL(global_true)');
                }

                if ('false' === $exprCompiled->getCode()) {
                    return new GlobalConstant('ZEPHIR_GLOBAL(global_false)');
                }

                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignBool($tempVar, $exprCompiled->getCode(), $compilationContext);

                return $tempVar;

            case Types::T_NULL:
                return new GlobalConstant('ZEPHIR_GLOBAL(global_null)');

            case Types::T_STRING:
            case Types::T_ULONG:
                $tempVar = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignString($tempVar, $exprCompiled->getCode(), $compilationContext);

                return $tempVar;

            case Types::T_ARRAY:
                return $compilationContext->symbolTable->getVariableForRead(
                    $exprCompiled->getCode(),
                    $compilationContext,
                    $exprCompiled->getOriginal()
                );

            case Types::T_VARIABLE:
                $itemVariable = $compilationContext->symbolTable->getVariableForRead(
                    $exprCompiled->getCode(),
                    $compilationContext,
                    $exprCompiled->getOriginal()
                );
                switch ($itemVariable->getType()) {
                    case Types::T_INT:
                    case Types::T_UINT:
                    case Types::T_LONG:
                    case Types::T_ULONG:
                    case Types::T_CHAR:
                    case Types::T_UCHAR:
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite(
                            'variable',
                            $compilationContext
                        );
                        $compilationContext->backend->assignLong($tempVar, $itemVariable, $compilationContext);

                        return $tempVar;

                    case Types::T_DOUBLE:
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite(
                            'variable',
                            $compilationContext
                        );
                        $compilationContext->backend->assignDouble($tempVar, $itemVariable, $compilationContext);

                        return $tempVar;

                    case Types::T_BOOL:
                        $tempVar = $compilationContext->symbolTable->getTempVariableForWrite(
                            'variable',
                            $compilationContext
                        );
                        $compilationContext->backend->assignBool($tempVar, $itemVariable, $compilationContext);

                        return $tempVar;

                    case Types::T_STRING:
                    case Types::T_VARIABLE:
                    case Types::T_ARRAY:
                    case Types::T_MIXED:
                        return $itemVariable;


                    default:
                        throw new CompilerException(
                            'Unknown ' . $itemVariable->getType(),
                            $exprCompiled->getOriginal() ?? []
                        );
                }
                break;

            default:
                throw new CompilerException('Unknown', $exprCompiled);
        }
    }

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     */
    public function setExpectReturn(bool $expecting, ?Variable $expectingVariable = null): void
    {
        $this->expecting         = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     */
    public function setReadOnly(bool $readOnly): void
    {
        $this->readOnly = $readOnly;
    }
}
