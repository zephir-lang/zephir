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

use ReflectionException;
use Zephir\Class\Definition\GeneratorTransformer;
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Exception\InvalidTypeException;
use Zephir\Expression;
use Zephir\Name;
use Zephir\Traits\VariablesTrait;
use Zephir\Types\Types;

use function sprintf;

/**
 * Return statement is used to assign variables
 */
final class ReturnStatement extends StatementAbstract
{
    use VariablesTrait;

    private const RETURN_RETURN = 'RETURN_MM();';

    /**
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $statement = $this->statement;

        $codePrinter = $compilationContext->codePrinter;

        /**
         * Inside a generator step body, `return` finishes the generator: the
         * value feeds getReturn() and the step function simply exits (its C
         * return_value is unused). See issue #1849.
         */
        if ($compilationContext->currentMethod?->isGeneratorStep()) {
            $this->compileGeneratorFinish($compilationContext);

            return;
        }

        if (isset($statement['expr'])) {
            $currentMethod = $compilationContext->currentMethod;

            if ($currentMethod->isConstructor()) {
                throw new CompilerException('Constructors cannot return values', $statement['expr']);
            }

            if ($currentMethod->isVoid()) {
                throw new CompilerException(
                    "Method is marked as 'void' and it must not return any value",
                    $statement['expr']
                );
            }

            /**
             * Use return member for properties on this
             */
            if ('property-access' == $statement['expr']['type']) {
                if ('variable' == $statement['expr']['left']['type']) {
                    if ('this' == $statement['expr']['left']['value']) {
                        if ('variable' == $statement['expr']['right']['type']) {
                            /**
                             * If the property is accessed on 'this', we check if the property does exist.
                             */
                            $property        = $statement['expr']['right']['value'];
                            $classDefinition = $compilationContext->classDefinition;

                            /**
                             * If this is a closure class with an enclosing class, resolve
                             * property access on `this` against the enclosing class definition.
                             * @see https://github.com/zephir-lang/zephir/issues/2497
                             */
                            $enclosingClassDefinition = $classDefinition->getEnclosingClassDefinition();
                            if ($enclosingClassDefinition !== null) {
                                $classDefinition = $enclosingClassDefinition;
                            }

                            $this->checkClassHasProperty(
                                $classDefinition,
                                $property,
                                $statement['expr']['right']
                            );

                            $compilationContext->headersManager->add('kernel/object');

                            /**
                             * If the method declares a strict scalar return type, emit a
                             * runtime type check after reading the property. PHP's engine
                             * does not verify return types of internal/extension methods in
                             * release builds, so without this `return this->prop` could leak
                             * a NULL (or other mismatching type) past a `-> string` hint.
                             *
                             * @see https://github.com/zephir-lang/zephir/issues/1991
                             */
                            $expectedType = $this->resolveStrictScalarReturnTypeConst($currentMethod);
                            if ($expectedType !== null) {
                                $codePrinter->output(
                                    'RETURN_MM_MEMBER_TYPED(getThis(), "' . $property . '", ' . $expectedType . ');'
                                );
                            } else {
                                $codePrinter->output('RETURN_MM_MEMBER(getThis(), "' . $property . '");');
                            }

                            return;
                        }
                    }
                }
            }

            /**
             * Fetches return_value and tries to return the value directly there.
             */
            $variable = $compilationContext->symbolTable->getVariable('return_value');

            $expr = new Expression($statement['expr']);
            $expr->setExpectReturn(true, $variable);
            $expr->setReadOnly(true);
            $resolvedExpr = $expr->compile($compilationContext);

            /**
             * Here we check if the variable returns a compatible type according to its type hints
             */
            if ($currentMethod->hasReturnTypes()) {
                switch ($resolvedExpr->getType()) {
                    case Types::T_NULL:
                        if (!$currentMethod->areReturnTypesNullCompatible() && !$currentMethod->isMixed()) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_INT:
                    case Types::T_UINT:
                    case Types::T_LONG:
                    case Types::T_ULONG:
                    case Types::T_CHAR:
                    case Types::T_UCHAR:
                        if (!$currentMethod->areReturnTypesIntCompatible() && !$currentMethod->isMixed()) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_BOOL:
                        if (
                            !$currentMethod->areReturnTypesBoolCompatible() &&
                            !$currentMethod->isMixed() &&
                            !$currentMethod->areReturnTypesFalseCompatible()
                        ) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_FALSE:
                        if (!$currentMethod->areReturnTypesFalseCompatible() && !$currentMethod->isMixed()) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_DOUBLE:
                        if (!$currentMethod->areReturnTypesDoubleCompatible() && !$currentMethod->isMixed()) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_STRING:
                    case Types::T_ISTRING:
                        if (!$currentMethod->areReturnTypesStringCompatible() && !$currentMethod->isMixed()) {
                            throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                        }
                        break;

                    case Types::T_VARIABLE:
                    case Types::T_MIXED:
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolvedExpr->getCode(),
                            $compilationContext,
                            $statement['expr']
                        );

                        switch ($symbolVariable->getType()) {
                            case Types::T_INT:
                            case Types::T_UINT:
                            case Types::T_LONG:
                            case Types::T_ULONG:
                            case Types::T_CHAR:
                            case Types::T_UCHAR:
                                if (!$currentMethod->areReturnTypesIntCompatible() && !$currentMethod->isMixed()) {
                                    throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                                }
                                break;

                            case Types::T_DOUBLE:
                                if (!$currentMethod->areReturnTypesDoubleCompatible() && !$currentMethod->isMixed()) {
                                    throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                                }
                                break;

                            case Types::T_STRING:
                            case Types::T_ISTRING:
                                if (!$currentMethod->areReturnTypesStringCompatible() && !$currentMethod->isMixed()) {
                                    throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                                }
                                break;

                            case Types::T_BOOL:
                                if (!$currentMethod->areReturnTypesBoolCompatible() && !$currentMethod->isMixed()) {
                                    throw new InvalidTypeException($resolvedExpr->getType(), $statement['expr']);
                                }
                                break;

                            case Types::T_VARIABLE:
                            case Types::T_MIXED:
                                break;
                        }
                        break;
                }
            }

            switch ($resolvedExpr->getType()) {
                case Types::T_NULL:
                    $codePrinter->output('RETURN_MM_NULL();');
                    break;

                case Types::T_INT:
                case Types::T_UINT:
                case Types::T_LONG:
                case Types::T_ULONG:
                    $codePrinter->output('RETURN_MM_LONG(' . $resolvedExpr->getCode() . ');');
                    break;

                case Types::T_CHAR:
                case Types::T_UCHAR:
                    // Issue #1291: a char holds a single byte. Cast to unsigned
                    // char to avoid sign extension when the value exceeds 127.
                    $codePrinter->output('RETURN_MM_LONG((unsigned char) (' . $resolvedExpr->getCode() . '));');
                    break;

                case Types::T_BOOL:
                    $codePrinter->output('RETURN_MM_BOOL(' . $resolvedExpr->getBooleanCode() . ');');
                    break;

                case Types::T_DOUBLE:
                    $codePrinter->output('RETURN_MM_DOUBLE(' . $resolvedExpr->getCode() . ');');
                    break;

                case Types::T_STRING:
                case Types::T_ISTRING:
                    $compilationContext->backend->returnString(
                        Name::addSlashes($resolvedExpr->getCode()),
                        $compilationContext
                    );
                    break;

                case Types::T_ARRAY:
                    if ('return_value' != $resolvedExpr->getCode()) {
                        $codePrinter->output('RETURN_CTOR(' . $resolvedExpr->getCode() . ');');
                    } else {
                        $codePrinter->output(self::RETURN_RETURN);
                    }
                    break;

                case Types::T_VARIABLE:
                case Types::T_MIXED:
                    if (!isset($symbolVariable)) {
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
                            $resolvedExpr->getCode(),
                            $compilationContext,
                            $statement['expr']
                        );
                    }

                    switch ($symbolVariable->getType()) {
                        case Types::T_INT:
                        case Types::T_UINT:
                        case Types::T_LONG:
                        case Types::T_ULONG:
                            $codePrinter->output('RETURN_MM_LONG(' . $symbolVariable->getName() . ');');
                            break;

                        case Types::T_CHAR:
                        case Types::T_UCHAR:
                            // Issue #1291: a char holds a single byte. Cast to
                            // unsigned char to avoid sign extension when the
                            // value exceeds 127.
                            $codePrinter->output('RETURN_MM_LONG((unsigned char) ' . $symbolVariable->getName() . ');');
                            break;

                        case Types::T_DOUBLE:
                            $codePrinter->output('RETURN_MM_DOUBLE(' . $symbolVariable->getName() . ');');
                            break;

                        case Types::T_STRING:
                        case Types::T_ISTRING:
                        case Types::T_ARRAY:
                            if ($symbolVariable->isNativeString()) {
                                $name = $symbolVariable->getName();
                                $mustGrow = $compilationContext->symbolTable->getMustGrownStack();

                                /**
                                 * Nullable native strings (null-default) can be
                                 * NULL at runtime. Guard with a null check to
                                 * prevent zend_string_copy(NULL) segfault.
                                 */
                                if ($symbolVariable->isNullableNativeString()) {
                                    $codePrinter->output('if (' . $name . ') {');
                                    if ($mustGrow) {
                                        $codePrinter->output("\t" . 'RETURN_MM_STR(zend_string_copy(' . $name . '));');
                                    } else {
                                        $codePrinter->output("\t" . 'RETURN_STR(zend_string_copy(' . $name . '));');
                                    }
                                    $codePrinter->output('}');
                                    if ($mustGrow) {
                                        $codePrinter->output('RETURN_MM_NULL();');
                                    } else {
                                        $codePrinter->output('RETURN_NULL();');
                                    }
                                } else {
                                    /**
                                     * zend_string_copy() increments the refcount so that both
                                     * the return_value and the caller's argument zval can safely
                                     * coexist.  Plain RETURN_STR() would transfer ownership
                                     * without addref, causing use-after-free when the caller
                                     * releases the return value.
                                     *
                                     * Methods with memory-grow need RETURN_MM_STR to restore
                                     * the memory frame (which frees the companion zval).
                                     */
                                    if ($mustGrow) {
                                        $codePrinter->output(
                                            'RETURN_MM_STR(zend_string_copy(' . $name . '));'
                                        );
                                    } else {
                                        $codePrinter->output(
                                            'RETURN_STR(zend_string_copy(' . $name . '));'
                                        );
                                    }
                                }
                            } else {
                                $codePrinter->output(
                                    'RETURN_CTOR(' . $compilationContext->backend->getVariableCode($symbolVariable) . ');'
                                );
                            }
                            break;


                        case Types::T_BOOL:
                            $codePrinter->output('RETURN_MM_BOOL(' . $symbolVariable->getName() . ');');
                            break;

                        case Types::T_VARIABLE:
                        case Types::T_MIXED:
                            if ('this_ptr' == $symbolVariable->getName()) {
                                $codePrinter->output('RETURN_THIS();');
                            } else {
                                if ('return_value' != $symbolVariable->getName()) {
                                    if (!$symbolVariable->isExternal()) {
                                        if ($symbolVariable->isLocalOnly()) {
                                            $codePrinter->output(
                                                sprintf(
                                                    'RETURN_LCTOR(%s);',
                                                    $compilationContext->backend->getVariableCode($symbolVariable)
                                                )
                                            );
                                        } else {
                                            if (!$symbolVariable->isMemoryTracked()) {
                                                $codePrinter->output(
                                                    sprintf(
                                                        'RETURN_CTOR(%s);',
                                                        $compilationContext->backend->getVariableCode($symbolVariable)
                                                    )
                                                );
                                            } else {
                                                $codePrinter->output(
                                                    sprintf(
                                                        'RETURN_CCTOR(%s);',
                                                        $compilationContext->backend->getVariableCode($symbolVariable)
                                                    )
                                                );
                                            }
                                        }
                                    } else {
                                        $codePrinter->output(
                                            sprintf(
                                                'RETVAL_ZVAL(%s, 1, 0);',
                                                $compilationContext->backend->getVariableCode($symbolVariable)
                                            )
                                        );
                                        $codePrinter->output(self::RETURN_RETURN);
                                    }
                                } else {
                                    $codePrinter->output(self::RETURN_RETURN);
                                }
                            }

                            $this->checkVariableTemporal($symbolVariable);
                            break;

                        default:
                            throw new CompilerException(
                                sprintf("Cannot return variable '%s'", $symbolVariable->getType()),
                                $statement['expr']
                            );
                    }
                    break;

                default:
                    throw new CompilerException("Cannot return '" . $resolvedExpr->getType() . "'", $statement['expr']);
            }

            return;
        }

        /**
         * Return without an expression
         */
        $codePrinter->output('RETURN_MM_NULL();');
    }

    /**
     * Returns the IS_* type constant for a method declared with a single,
     * strict, non-nullable scalar return type that we can enforce at runtime
     * via `RETURN_MM_MEMBER_TYPED`. Returns null for any case we don't yet
     * verify (nullable, union types, mixed, object, bool, void).
     *
     * `bool` is intentionally not enforced here because PHP uses two distinct
     * zval types (IS_TRUE / IS_FALSE) which would require a more elaborate
     * macro. `object` would need class-hierarchy checks. `mixed` accepts
     * anything by definition. These are left to a future change.
     */
    private function resolveStrictScalarReturnTypeConst($currentMethod): ?string
    {
        if (!$currentMethod->hasReturnTypes()) {
            return null;
        }

        if ($currentMethod->isMixed() || $currentMethod->areReturnTypesNullCompatible()) {
            return null;
        }

        $returnTypes = $currentMethod->getReturnTypes();
        if (count($returnTypes) !== 1) {
            return null;
        }

        $type = array_key_first($returnTypes);

        return match ($type) {
            'string', 'istring' => 'IS_STRING',
            'int', 'uint', 'long', 'ulong', 'char', 'uchar' => 'IS_LONG',
            'double' => 'IS_DOUBLE',
            'array' => 'IS_ARRAY',
            default => null,
        };
    }

    /**
     * `return [expr];` inside a generator step: store the getReturn() payload
     * on the generator object, mark it finished, release the frame and exit.
     */
    private function compileGeneratorFinish(CompilationContext $compilationContext): void
    {
        $codePrinter = $compilationContext->codePrinter;

        $compilationContext->headersManager->add('kernel/generator');

        $genVariable = $compilationContext->symbolTable->getVariableForRead(
            GeneratorTransformer::GEN_PARAM,
            $compilationContext,
            $this->statement
        );
        $gen = $compilationContext->backend->getVariableCode($genVariable);

        $valueCode = isset($this->statement['expr'])
            ? GeneratorZvalResolver::resolve($this->statement['expr'], $compilationContext)
            : 'NULL';

        $codePrinter->output('zephir_generator_finish(' . $gen . ', ' . $valueCode . ');');
        $codePrinter->output('ZEPHIR_MM_RESTORE();');
        $codePrinter->output('return;');
    }
}
