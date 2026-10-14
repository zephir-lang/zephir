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
use Zephir\Class\Method\Method;
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
     * How to reach the enclosing object.
     *
     * A capturing closure binds its capture carrier as `$this`, and `__invoke`
     * re-points its `this_ptr` local at the enclosing object. getThis() does
     * not follow that rebind, so it must not be used there.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2652
     */
    private function thisAccessor(CompilationContext $compilationContext): string
    {
        if (true !== $compilationContext->currentMethod?->hasCaptures()) {
            return 'getThis()';
        }

        return $compilationContext->backend->getVariableCode(
            $compilationContext->symbolTable->getVariable('this')
        );
    }

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
                             * A typed method checks the property's value against its own
                             * return type. PHP's engine only verifies the return value of
                             * an internal method in debug builds, where a mismatch aborts,
                             * so without this `return this->prop` leaks a value its
                             * Reflection type rejects.
                             *
                             * @see https://github.com/zephir-lang/zephir/issues/1991
                             * @see https://github.com/zephir-lang/zephir/issues/2391
                             */
                            $macro = $this->verifiesReturnType($currentMethod)
                                ? 'RETURN_MM_MEMBER_VERIFIED'
                                : 'RETURN_MM_MEMBER';
                            $codePrinter->output(
                                $macro . '(' . $this->thisAccessor($compilationContext) . ', "' . $property . '");'
                            );

                            return;
                        }
                    }
                }
            }

            /**
             * `return self::prop` in a typed method gets the same check as
             * `return this->prop` above.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2391
             */
            if (
                'static-property-access' === $statement['expr']['type'] &&
                in_array($statement['expr']['left']['value'] ?? null, ['self', 'static'], true) &&
                $this->verifiesReturnType($currentMethod)
            ) {
                $expr = new Expression($statement['expr']);
                $expr->setReadOnly(true);
                $resolvedExpr = $expr->compile($compilationContext);

                $variable = $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $statement['expr']
                );
                $codePrinter->output(
                    'ZVAL_COPY(return_value, ' . $compilationContext->backend->getVariableCode($variable) . ');'
                );
                $codePrinter->output('zephir_verify_return_value(return_value);');
                $codePrinter->output(self::RETURN_RETURN);

                return;
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
                    // The code is the bare character, so quote it as a C char
                    // literal — same as every other char-literal consumer.
                    $codePrinter->output('RETURN_MM_LONG((unsigned char) (' . $resolvedExpr->getCharCode() . '));');
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
                                /*
                                 * RETURN_THIS() reads getThis(), which in a
                                 * capturing closure is the capture carrier.
                                 * @see https://github.com/zephir-lang/zephir/issues/2652
                                 */
                                if (true === $compilationContext->currentMethod?->hasCaptures()) {
                                    $codePrinter->output('RETURN_THIS_ZVAL(this_ptr);');
                                } else {
                                    $codePrinter->output('RETURN_THIS();');
                                }
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
     * Whether a returned property must be checked against the method's
     * return type: any declared type except `mixed`, which accepts anything.
     */
    private function verifiesReturnType(Method $currentMethod): bool
    {
        return $currentMethod->hasReturnTypes() && !$currentMethod->isMixed();
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
