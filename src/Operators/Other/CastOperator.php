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

namespace Zephir\Operators\Other;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;
use Zephir\Statements\Let\Variable as LetVariable;
use Zephir\Types\Types;

/**
 * Converts a value into another of a different type
 */
class CastOperator extends AbstractOperator
{
    /**
     * Compiles a type cast operation.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws ReflectionException
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        try {
            $expr     = new Expression($expression['right']);
            $resolved = $expr->compile($compilationContext);
        } catch (Exception $e) {
            throw new CompilerException($e->getMessage(), $expression, $e->getCode(), $e);
        }

        switch ($expression['left']) {
            case Types::T_INT:
                switch ($resolved->getType()) {
                    case Types::T_NULL:
                        return new CompiledExpression('int', '0', $expression);

                    case Types::T_CHAR:
                    case Types::T_UCHAR:
                        return new CompiledExpression('int', "'{$resolved->getCode()}'", $expression);

                    case Types::T_INT:
                        return new CompiledExpression('int', $resolved->getCode(), $expression);

                    case Types::T_DOUBLE:
                        return new CompiledExpression('int', '(int) ' . $resolved->getCode(), $expression);

                    case Types::T_BOOL:
                        return new CompiledExpression('int', $resolved->getBooleanCode(), $expression);

                    case Types::T_STRING:
                        $compilationContext->headersManager->add('kernel/operators');

                        /**
                         * zephir_get_intval_ex use zval variable
                         * before use with it we create a new variable and assign value of literal.
                         *
                         * TODO: Optimize by creating native function for string without zval using
                         */
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                            'string',
                            $compilationContext
                        );

                        $original             = $resolved->getOriginal();
                        $original['operator'] = 'assign';

                        $let = new LetVariable();
                        $let->assign(
                            $symbolVariable->getName(),
                            $symbolVariable,
                            $resolved,
                            new ReadDetector(),
                            $compilationContext,
                            $original
                        );
                        $symbol = $compilationContext->backend->getVariableCode($symbolVariable);

                        return new CompiledExpression(
                            'int',
                            'zephir_get_intval_ex(' . $symbol . ')',
                            $expression
                        );

                    case Types::T_ARRAY:
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext,
                            $expression
                        );
                        $symbol         = $compilationContext->backend->getVariableCode($symbolVariable);

                        return new CompiledExpression('int', 'zephir_get_intval(' . $symbol . ')', $expression);

                    case Types::T_VARIABLE:
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext,
                            $expression
                        );
                        switch ($symbolVariable->getType()) {
                            case Types::T_INT:
                            case Types::T_CHAR:
                            case Types::T_UCHAR:
                                return new CompiledExpression('int', $symbolVariable->getName(), $expression);

                            case Types::T_DOUBLE:
                            case Types::T_BOOL:
                                return new CompiledExpression(
                                    'int',
                                    '(int) (' . $symbolVariable->getName() . ')',
                                    $expression
                                );

                            case Types::T_ARRAY:
                            case Types::T_VARIABLE:
                            case Types::T_MIXED:
                            case Types::T_STRING:
                                $symbol = $compilationContext->backend->getVariableCode($symbolVariable);

                                return new CompiledExpression(
                                    'int',
                                    'zephir_get_intval(' . $symbol . ')',
                                    $expression
                                );

                            default:
                                throw new CompilerException(
                                    sprintf(
                                        'Cannot cast: %s(%s) to %s',
                                        $resolved->getType(),
                                        $symbolVariable->getType(),
                                        $expression['left']
                                    ),
                                    $expression
                                );
                        }
                        break;

                    default:
                        throw new CompilerException(
                            sprintf('Cannot cast: %s to %s', $resolved->getType(), $expression['left']),
                            $expression
                        );
                }
                break;

            case Types::T_LONG:
                switch ($resolved->getType()) {
                    case Types::T_INT:
                        return new CompiledExpression('long', $resolved->getCode(), $expression);

                    case Types::T_CHAR:
                    case Types::T_UCHAR:
                        return new CompiledExpression(
                            'long',
                            "(long) '{$resolved->getCode()}'",
                            $expression
                        );

                    case Types::T_DOUBLE:
                        return new CompiledExpression('long', '(long) ' . $resolved->getCode(), $expression);

                    case Types::T_BOOL:
                        return new CompiledExpression('long', $resolved->getBooleanCode(), $expression);

                    case Types::T_ARRAY:
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext,
                            $expression
                        );

                        return new CompiledExpression(
                            'long',
                            'zephir_get_intval(' . $symbolVariable->getName() . ')',
                            $expression
                        );

                    case Types::T_VARIABLE:
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext,
                            $expression
                        );
                        switch ($symbolVariable->getType()) {
                            case Types::T_INT:
                            case Types::T_CHAR:
                            case Types::T_UCHAR:
                                return new CompiledExpression('long', $symbolVariable->getName(), $expression);

                            case Types::T_DOUBLE:
                                return new CompiledExpression(
                                    'long',
                                    '(long) (' . $symbolVariable->getName() . ')',
                                    $expression
                                );

                            case Types::T_VARIABLE:
                            case Types::T_MIXED:
                                $symbol = $compilationContext->backend->getVariableCode($symbolVariable);

                                return new CompiledExpression(
                                    'long',
                                    'zephir_get_intval(' . $symbol . ')',
                                    $expression
                                );

                            default:
                                throw new CompilerException(
                                    sprintf(
                                        'Cannot cast: %s(%s) to %s',
                                        $resolved->getType(),
                                        $symbolVariable->getType(),
                                        $expression['left']
                                    ),
                                    $expression
                                );
                        }
                        break;

                    default:
                        throw new CompilerException(
                            sprintf('Cannot cast: %s to %s', $resolved->getType(), $expression['left']),
                            $expression
                        );
                }
                break;

            case Types::T_DOUBLE:
                switch ($resolved->getType()) {
                    case Types::T_NULL:
                        return new CompiledExpression('double', '0', $expression);

                    case Types::T_BOOL:
                        return new CompiledExpression('double', $resolved->getBooleanCode(), $expression);

                    case Types::T_CHAR:
                    case Types::T_UCHAR:
                        return new CompiledExpression(
                            'double',
                            "(double) '{$resolved->getCode()}'",
                            $expression
                        );

                    case Types::T_INT:
                    case Types::T_LONG:
                    case Types::T_DOUBLE:
                        return new CompiledExpression('double', $resolved->getCode(), $expression);

                    case Types::T_ARRAY:
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext,
                            $expression
                        );
                        $symbol         = $compilationContext->backend->getVariableCode($symbolVariable);

                        return new CompiledExpression(
                            'double',
                            'zephir_get_doubleval(' . $symbol . ')',
                            $expression
                        );

                    case Types::T_VARIABLE:
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext,
                            $expression
                        );

                        switch ($symbolVariable->getType()) {
                            case Types::T_INT:
                            case Types::T_CHAR:
                            case Types::T_UCHAR:
                                return new CompiledExpression(
                                    'double',
                                    "(double) {$symbolVariable->getName()}",
                                    $expression
                                );

                            case Types::T_DOUBLE:
                            case Types::T_BOOL:
                                return new CompiledExpression(
                                    'double',
                                    sprintf('(double) (%s)', $symbolVariable->getName()),
                                    $expression
                                );

                            case Types::T_ARRAY:
                            case Types::T_VARIABLE:
                            case Types::T_MIXED:
                                $symbol = $compilationContext->backend->getVariableCode($symbolVariable);

                                return new CompiledExpression(
                                    'double',
                                    sprintf('zephir_get_doubleval(%s)', $symbol),
                                    $expression
                                );

                            default:
                                throw new CompilerException(
                                    sprintf(
                                        'Cannot cast: %s(%s) to %s',
                                        $resolved->getType(),
                                        $symbolVariable->getType(),
                                        $expression['left']
                                    ),
                                    $expression
                                );
                        }
                        break;

                    default:
                        throw new CompilerException(
                            sprintf('Cannot cast: %s to %s', $resolved->getType(), $expression['left']),
                            $expression
                        );
                }
                break;

            case Types::T_BOOL:
                switch ($resolved->getType()) {
                    case Types::T_INT:
                        return new CompiledExpression(
                            'bool',
                            '(zend_bool) ' . $resolved->getCode(),
                            $expression
                        );

                    case Types::T_CHAR:
                    case Types::T_UCHAR:
                        return new CompiledExpression(
                            'bool',
                            "(zend_bool) '{$resolved->getCode()}'",
                            $expression
                        );

                    case Types::T_BOOL:
                        return new CompiledExpression('bool', $resolved->getCode(), $expression);

                    case Types::T_VARIABLE:
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext,
                            $expression
                        );
                        $symbol         = $compilationContext->backend->getVariableCode($symbolVariable);

                        if ($symbolVariable->isTemporal()) {
                            $symbolVariable->setIdle(true);
                        }
                        return match ($symbolVariable->getType()) {
                            Types::T_INT,
                            Types::T_CHAR,
                            Types::T_UCHAR => new CompiledExpression(
                                'bool',
                                sprintf('(zend_bool) %s', $symbolVariable->getName()),
                                $expression
                            ),
                            Types::T_VARIABLE,
                            Types::T_MIXED => new CompiledExpression(
                                'bool',
                                sprintf('zephir_get_boolval(%s)', $symbol),
                                $expression
                            ),
                            default        => throw new CompilerException(
                                sprintf(
                                    'Cannot cast: %s(%s) to %s',
                                    $resolved->getType(),
                                    $symbolVariable->getType(),
                                    $expression['left']
                                ),
                                $expression
                            ),
                        };
                        break;

                    default:
                        throw new CompilerException(
                            sprintf('Cannot cast: %s to %s', $resolved->getType(), $expression['left']),
                            $expression
                        );
                }
                break;

            case Types::T_CHAR:
                switch ($resolved->getType()) {
                    case Types::T_UCHAR:
                        return new CompiledExpression('uchar', "'{$resolved->getCode()}'", $expression);

                    case Types::T_CHAR:
                        return new CompiledExpression('char', "'{$resolved->getCode()}'", $expression);

                    case Types::T_VARIABLE:
                    case Types::T_MIXED:
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext,
                            $expression
                        );

                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                            'char',
                            $compilationContext
                        );

                        switch ($symbolVariable->getType()) {
                            // TODO: uchar
                            case Types::T_CHAR:
                                $compilationContext->codePrinter->output(
                                    sprintf('%s = %s;', $tempVariable->getName(), $symbolVariable->getName())
                                );
                                break;
                            default:
                                $variableCode = $compilationContext->backend->getVariableCode($symbolVariable);
                                $compilationContext->codePrinter->output(
                                    sprintf(
                                        '%s = (char) zephir_get_intval(%s);',
                                        $tempVariable->getName(),
                                        $variableCode
                                    )
                                );
                        }

                        return new CompiledExpression('variable', $tempVariable->getName(), $expression);
                    default:
                        throw new CompilerException(
                            sprintf('Cannot cast: %s to %s', $resolved->getType(), $expression['left']),
                            $expression
                        );
                }
                break;

            case Types::T_STRING:
                switch ($resolved->getType()) {
                    case Types::T_UCHAR:
                    case Types::T_CHAR:
                        return new CompiledExpression('string', $resolved->getCode(), $expression);

                    case Types::T_VARIABLE:
                    case Types::T_MIXED:
                        $compilationContext->headersManager->add('kernel/operators');
                        $compilationContext->symbolTable->mustGrownStack(true);

                        $resolvedVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext,
                            $expression
                        );

                        switch ($resolvedVariable->getType()) {
                            case Types::T_CHAR:
                                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                                    'string',
                                    $compilationContext
                                );

                                $symbol       = $compilationContext->backend->getVariableCode($symbolVariable);
                                $resolvedCode = $compilationContext->backend->getVariableCode($resolvedVariable);

                                $compilationContext->codePrinter->output(
                                    sprintf('ZVAL_STRINGL(%s, %s, 1);', $symbol, $resolvedCode)
                                );

                                return new CompiledExpression(
                                    'variable',
                                    $symbolVariable->getName(),
                                    $expression
                                );
                            default:
                                // TODO: I'm not a pretty sure this branch works
                                // This is old code I just moved to "default"
                                // See: https://github.com/zephir-lang/zephir/issues/1988
                                $symbolVariable = $compilationContext->symbolTable->getTempVariable(
                                    'string',
                                    $compilationContext
                                );

                                $symbolVariable->setMustInitNull(true);
                                $symbolVariable->setIsInitialized(true, $compilationContext);
                                $symbolVariable->increaseUses();
                                $symbol       = $compilationContext->backend->getVariableCode($symbolVariable);
                                $resolvedCode = $compilationContext->backend->getVariableCode($resolvedVariable);

                                $compilationContext->codePrinter->output(
                                    sprintf('zephir_cast_to_string(%s, %s);', $symbol, $resolvedCode)
                                );

                                if ($symbolVariable->isTemporal()) {
                                    $symbolVariable->setIdle(true);
                                }

                                return new CompiledExpression(
                                    'variable',
                                    $symbolVariable->getName(),
                                    $expression
                                );
                        }

                        break;
                    default:
                        throw new CompilerException(
                            sprintf('Cannot cast: %s to %s', $resolved->getType(), $expression['left']),
                            $expression
                        );
                }
                break;

            case Types::T_ARRAY:
                switch ($resolved->getType()) {
                    case Types::T_VARIABLE:
                    case Types::T_MIXED:
                        $compilationContext->headersManager->add('kernel/operators');
                        $compilationContext->symbolTable->mustGrownStack(true);

                        $symbolVariable = $compilationContext->symbolTable->getTempVariable(
                            'array',
                            $compilationContext
                        );

                        $symbolVariable->setMustInitNull(true);
                        $symbolVariable->setIsInitialized(true, $compilationContext);
                        $symbolVariable->increaseUses();
                        $symbol           = $compilationContext->backend->getVariableCode($symbolVariable);
                        $resolvedVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext
                        );
                        $resolvedCode     = $compilationContext->backend->getVariableCode($resolvedVariable);

                        $compilationContext->codePrinter->output(
                            sprintf('zephir_get_arrval(%s, %s);', $symbol, $resolvedCode)
                        );

                        if ($symbolVariable->isTemporal()) {
                            $symbolVariable->setIdle(true);
                        }

                        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);

                    default:
                        throw new CompilerException(
                            sprintf('Cannot cast: %s to %s', $resolved->getType(), $expression['left']),
                            $expression
                        );
                }
                break;

            case Types::T_OBJECT:
                switch ($resolved->getType()) {
                    case Types::T_INT:
                    case Types::T_DOUBLE:
                    case Types::T_BOOL:
                    case Types::T_NULL:
                    case Types::T_STRING:
                    case Types::T_ARRAY:
                        $compilationContext->headersManager->add('kernel/operators');
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $symbolVariable = $compilationContext->symbolTable->getTempVariable(
                            'variable',
                            $compilationContext
                        );
                        $symbol         = $compilationContext->backend->getVariableCode($symbolVariable);

                        /**
                         * zephir_convert_to_object use zval variable
                         * before use with it we create a new variable and assign value of literal.
                         */
                        $let                  = new LetVariable();
                        $original             = $resolved->getOriginal();
                        $original['operator'] = 'assign';
                        $let->assign(
                            $symbolVariable->getName(),
                            $symbolVariable,
                            $resolved,
                            new ReadDetector(),
                            $compilationContext,
                            $original
                        );

                        $compilationContext->codePrinter->output('zephir_convert_to_object(' . $symbol . ');');

                        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);

                    case Types::T_VARIABLE:
                    case Types::T_MIXED:
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolved->getCode(),
                            $compilationContext,
                            $expression
                        );
                        $symbol         = $compilationContext->backend->getVariableCode($symbolVariable);
                        if ($symbolVariable->isTemporal()) {
                            $symbolVariable->setIdle(true);
                        }

                        $compilationContext->codePrinter->output('zephir_convert_to_object(' . $symbol . ');');

                        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);

                    default:
                        throw new CompilerException(
                            sprintf('Cannot cast: %s to %s', $resolved->getType(), $expression['left']),
                            $expression
                        );
                }
                break;

            default:
                throw new CompilerException(
                    sprintf('Cannot cast: %s to %s', $resolved->getType(), $expression['left']),
                    $expression
                );
        }
    }
}
