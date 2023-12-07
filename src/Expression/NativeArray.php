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
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
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

                        case Types::T_BOOL:
                            $compilationContext->headersManager->add('kernel/array');
                            if ('true' == $resolvedExpr->getCode()) {
                                $compilationContext->backend->updateArray(
                                    $symbolVariable,
                                    $resolvedExprKey,
                                    'true',
                                    $compilationContext,
                                    'PH_COPY | PH_SEPARATE'
                                );
                            } else {
                                $compilationContext->backend->updateArray(
                                    $symbolVariable,
                                    $resolvedExprKey,
                                    'false',
                                    $compilationContext,
                                    'PH_COPY | PH_SEPARATE'
                                );
                            }
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

                        case Types::T_BOOL:
                            if ('true' == $resolvedExpr->getCode()) {
                                $compilationContext->backend->updateArray(
                                    $symbolVariable,
                                    $resolvedExprKey,
                                    'true',
                                    $compilationContext,
                                    'PH_COPY'
                                );
                            } else {
                                $compilationContext->backend->updateArray(
                                    $symbolVariable,
                                    $resolvedExprKey,
                                    'false',
                                    $compilationContext,
                                    'PH_COPY'
                                );
                            }
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
                                case Types::T_BOOL:
                                case Types::T_DOUBLE:
                                case Types::T_NULL:
                                case Types::T_STRING:
                                    $compilationContext->backend->addArrayEntry(
                                        $symbolVariable,
                                        $resolvedExprKey,
                                        $resolvedExpr,
                                        $compilationContext
                                    );
                                    break;

                                case Types::T_VARIABLE:
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

                        case Types::T_STRING:
                            $expr         = new Expression($item['value']);
                            $resolvedExpr = $expr->compile($compilationContext);
                            switch ($resolvedExpr->getType()) {
                                case Types::T_INT:
                                case Types::T_UINT:
                                case Types::T_LONG:
                                case Types::T_ULONG:
                                    $codePrinter->output(
                                        'add_assoc_long_ex(' . $symbolVariable->getName(
                                        ) . ', Z_STRVAL_P(' . $resolvedExprKey->getCode(
                                        ) . '), Z_STRLEN_P(' . $item['key']['value'] . '), ' . $resolvedExpr->getCode(
                                        ) . ');'
                                    );
                                    break;

                                case Types::T_DOUBLE:
                                    $codePrinter->output(
                                        'add_assoc_double_ex(' . $symbolVariable->getName(
                                        ) . ', Z_STRVAL_P(' . $resolvedExprKey->getCode(
                                        ) . '), Z_STRLEN_P(' . $item['key']['value'] . '), ' . $resolvedExpr->getCode(
                                        ) . ');'
                                    );
                                    break;

                                case Types::T_BOOL:
                                    $codePrinter->output(
                                        'add_assoc_bool_ex(' . $symbolVariable->getName(
                                        ) . ', Z_STRVAL_P(' . $resolvedExprKey->getCode(
                                        ) . '), Z_STRLEN_P(' . $item['key']['value'] . '), ' . $resolvedExpr->getBooleanCode(
                                        ) . ');'
                                    );
                                    break;

                                case Types::T_STRING:
                                    $compilationContext->backend->addArrayEntry(
                                        $symbolVariable,
                                        $resolvedExprKey,
                                        $resolvedExpr,
                                        $compilationContext
                                    );
                                    break;

                                case Types::T_NULL:
                                    $codePrinter->output(
                                        'add_assoc_null_ex(' . $symbolVariable->getName(
                                        ) . ', Z_STRVAL_P(' . $resolvedExprKey->getCode(
                                        ) . '), Z_STRLEN_P(' . $item['key']['value'] . ') + 1);'
                                    );
                                    break;

                                case Types::T_VARIABLE:
                                    $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                                    $compilationContext->backend->updateArray(
                                        $symbolVariable,
                                        $resolvedExprKey,
                                        $resolvedExpr,
                                        $compilationContext
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

                        case Types::T_VARIABLE:
                            $expr         = new Expression($item['value']);
                            $resolvedExpr = $expr->compile($compilationContext);
                            switch ($resolvedExpr->getType()) {
                                case Types::T_NULL:
                                case Types::T_VARIABLE:
                                case Types::T_BOOL:
                                    $valueVariable = $this->getArrayValue($resolvedExpr, $compilationContext);
                                    $compilationContext->backend->updateArray(
                                        $symbolVariable,
                                        $variableVariable,
                                        $valueVariable,
                                        $compilationContext
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
                        . $resolvedExprKey->getType(), $item['key']
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
                    '\'' . $exprCompiled->getCode() . '\'',
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
                        throw new CompilerException('Unknown ' . $itemVariable->getType(), $itemVariable);
                }
                break;

            default:
                throw new CompilerException('Unknown', $exprCompiled);
        }
    }

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     *
     * @param bool          $expecting
     * @param Variable|null $expectingVariable
     */
    public function setExpectReturn(bool $expecting, Variable $expectingVariable = null): void
    {
        $this->expecting         = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     *
     * @param bool $readOnly
     */
    public function setReadOnly(bool $readOnly): void
    {
        $this->readOnly = $readOnly;
    }
}
