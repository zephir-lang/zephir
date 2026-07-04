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

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Name;
use Zephir\Types\Types;

/**
 * Resolves an arbitrary expression AST to C code denoting a `zval *`, boxing
 * scalar results into tracked temporaries. Used by the generator codegen
 * (yield values/keys and generator `return` payloads), whose kernel calls
 * take zval pointers only.
 */
final class GeneratorZvalResolver
{
    /**
     * @return string C expression evaluating to a zval * with the value
     *
     * @throws CompilerException
     */
    public static function resolve(array $exprAst, CompilationContext $compilationContext): string
    {
        $expr = new Expression($exprAst);
        $expr->setReadOnly(true);
        $resolved = $expr->compile($compilationContext);

        $backend     = $compilationContext->backend;
        $symbolTable = $compilationContext->symbolTable;

        switch ($resolved->getType()) {
            case Types::T_NULL:
                $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $backend->assignNull($temp, $compilationContext);

                return $backend->getVariableCode($temp);

            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
                $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $backend->assignLong($temp, $resolved->getCode(), $compilationContext);

                return $backend->getVariableCode($temp);

            case Types::T_CHAR:
            case Types::T_UCHAR:
                $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $backend->assignLong($temp, '(unsigned char) (' . $resolved->getCode() . ')', $compilationContext);

                return $backend->getVariableCode($temp);

            case Types::T_BOOL:
                $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $backend->assignBool($temp, $resolved->getBooleanCode(), $compilationContext);

                return $backend->getVariableCode($temp);

            case Types::T_DOUBLE:
                $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $backend->assignDouble($temp, $resolved->getCode(), $compilationContext);

                return $backend->getVariableCode($temp);

            case Types::T_STRING:
            case Types::T_ISTRING:
                $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $backend->assignString($temp, Name::addSlashes($resolved->getCode()), $compilationContext);

                return $backend->getVariableCode($temp);

            case Types::T_ARRAY:
            case Types::T_VARIABLE:
            case Types::T_MIXED:
                $variable = $symbolTable->getVariableForRead(
                    $resolved->getCode(),
                    $compilationContext,
                    $exprAst
                );

                switch ($variable->getType()) {
                    case Types::T_INT:
                    case Types::T_UINT:
                    case Types::T_LONG:
                    case Types::T_ULONG:
                        $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $backend->assignLong($temp, $variable->getName(), $compilationContext);

                        return $backend->getVariableCode($temp);

                    case Types::T_CHAR:
                    case Types::T_UCHAR:
                        $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $backend->assignLong(
                            $temp,
                            '(unsigned char) ' . $variable->getName(),
                            $compilationContext
                        );

                        return $backend->getVariableCode($temp);

                    case Types::T_DOUBLE:
                        $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $backend->assignDouble($temp, $variable->getName(), $compilationContext);

                        return $backend->getVariableCode($temp);

                    case Types::T_BOOL:
                        $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $backend->assignBool($temp, $variable->getName(), $compilationContext);

                        return $backend->getVariableCode($temp);

                    default:
                        if ($variable->isNativeString()) {
                            throw new CompilerException(
                                'Native zend_string locals cannot be yielded/returned from a generator yet',
                                $exprAst
                            );
                        }

                        return $backend->getVariableCode($variable);
                }

                // no break (all paths return)
            default:
                throw new CompilerException(
                    'Cannot use expression of type "' . $resolved->getType() . '" here',
                    $exprAst
                );
        }
    }
}
