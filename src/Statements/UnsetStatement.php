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
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Name;
use Zephir\Types\Types;

use function in_array;

/**
 * unset() statement
 */
class UnsetStatement extends StatementAbstract
{
    /**
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $flags = 'PH_SEPARATE';

        $compilationContext->headersManager->add('kernel/array');
        $expression = $this->statement['expr'];

        if ('list' === $expression['type']) {
            $expression = $expression['left'];
        }

        switch ($expression['type']) {
            case 'array-access':
                /**
                 * Unset of object property.
                 *
                 * zephir_unset_property_array() reads the property, separates
                 * it, unsets and writes it back, so it is the whole statement.
                 * Falling through to the generic array-access path below
                 * emitted it a second time, which for an ArrayAccess property
                 * meant two offsetUnset() calls where PHP makes one.
                 *
                 * @see https://github.com/zephir-lang/zephir/issues/2702
                 */
                if (isset($expression['left']['type']) && $expression['left']['type'] === 'property-access') {
                    $this->generateUnsetPropertyFromObject($expression, $compilationContext);

                    return;
                }

                $expr = new Expression($expression['left']);
                $expr->setReadOnly(true);
                $exprVar  = $expr->compile($compilationContext);
                $variable = $compilationContext->symbolTable->getVariableForWrite(
                    $exprVar->getCode(),
                    $compilationContext,
                    $this->statement
                );

                $expr = new Expression($expression['right']);
                $expr->setReadOnly(true);
                $exprIndex = $expr->compile($compilationContext);
                break;

            case 'property-access':
            case 'property-string-access':
                $expr = new Expression($expression['left']);
                $expr->setReadOnly(true);
                $exprVar      = $expr->compile($compilationContext);
                $variable     = $compilationContext->symbolTable->getVariableForWrite(
                    $exprVar->getCode(),
                    $compilationContext,
                    $this->statement
                );
                $variableCode = $compilationContext->backend->getVariableCode($variable);

                $compilationContext->headersManager->add('kernel/object');
                $compilationContext->codePrinter->output(
                    'zephir_unset_property(' . $variableCode . ', "' . $expression['right']['value'] . '");'
                );

                return;

            case 'property-dynamic-access':
                $expr = new Expression($expression['left']);
                $expr->setReadOnly(true);
                $exprVar      = $expr->compile($compilationContext);
                $variable     = $compilationContext->symbolTable->getVariableForWrite(
                    $exprVar->getCode(),
                    $compilationContext,
                    $this->statement
                );
                $variableCode = $compilationContext->backend->getVariableCode($variable);

                $compilationContext->headersManager->add('kernel/object');

                if ('string' === $expression['right']['type']) {
                    $compilationContext->codePrinter->output(
                        'zephir_unset_property(' . $variableCode . ', "' . $expression['right']['value'] . '");'
                    );
                } else {
                    $propExpr     = new Expression($expression['right']);
                    $propExpr->setReadOnly(true);
                    $propCompiled = $propExpr->compile($compilationContext);
                    $propVariable = $compilationContext->symbolTable->getVariableForRead(
                        $propCompiled->getCode(),
                        $compilationContext,
                        $this->statement
                    );
                    $propCode = $compilationContext->backend->getVariableCode($propVariable);
                    $compilationContext->codePrinter->output(
                        'zephir_unset_property_zval(' . $variableCode . ', ' . $propCode . ');'
                    );
                }

                return;

            default:
                throw new CompilerException(
                    'Cannot use expression type: ' . $expression['type'] . ' in "unset"',
                    $expression
                );
        }

        if (!in_array($variable->getType(), ['variable', 'array'])) {
            throw CompilerException::cannotUseVariableTypeAs(
                $variable,
                'in "unset"',
                $expression['left']
            );
        }

        /**
         * Only a dynamic variable can carry a wrong dynamic type. A native
         * `array` passes the check above and has no dynamic types at all, so
         * testing it here reported every `unset arr[key]` on a declared array
         * as suspicious. The sibling sites all narrow first; this one did not,
         * and the key was unregistered so the noise never surfaced (#2727).
         */
        if (
            'variable' === $variable->getType()
            && $variable->hasDifferentDynamicType(['undefined', 'array', 'object', 'null'])
        ) {
            $compilationContext->logger->warning(
                'Possible attempt to use non array/object in unset operator',
                ['non-valid-unset', $expression['left']]
            );
        }

        $compilationContext->backend->arrayUnset($variable, $exprIndex, $flags, $compilationContext);
    }

    /**
     * Emits `unset obj->property[offset]`.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function generateUnsetPropertyFromObject(
        array $expression,
        CompilationContext $compilationContext
    ): void {
        $offsetCode = $this->resolveOffsetAsZval($expression['right'], $compilationContext);

        $expr = new Expression($expression['left']['left']);
        $expr->setReadOnly(true);
        $exprVar      = $expr->compile($compilationContext);
        $variable     = $compilationContext->symbolTable->getVariableForWrite(
            $exprVar->getCode(),
            $compilationContext,
            $this->statement
        );
        $variableCode = $compilationContext->backend->getVariableCode($variable);

        $compilationContext->headersManager->add('kernel/object');
        $compilationContext->codePrinter->output(
            'zephir_unset_property_array(' . $variableCode . ', ZEND_STRL("'
            . $expression['left']['right']['value'] . '"), ' . $offsetCode . ');'
        );
    }

    /**
     * Resolves an offset expression to C code denoting a `zval *`.
     *
     * zephir_unset_property_array() takes the offset as a zval, so a literal
     * and a native `int` local both have to be boxed. The box is a temporary
     * the memory manager observes, which is what releases the zend_string a
     * string offset owns; the old code reused the temp a read-only property
     * fetch had filled, and nothing released that.
     *
     * The accepted types are those Backend::arrayUnset() accepts, so
     * `unset obj->prop[x]` and `unset arr[x]` reject the same expressions.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2702
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function resolveOffsetAsZval(array $offsetAst, CompilationContext $compilationContext): string
    {
        $expr = new Expression($offsetAst);
        $expr->setReadOnly(true);
        $resolved = $expr->compile($compilationContext);

        $backend     = $compilationContext->backend;
        $symbolTable = $compilationContext->symbolTable;

        switch ($resolved->getType()) {
            case Types::T_STRING:
                $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $backend->assignString($temp, Name::addSlashes($resolved->getCode()), $compilationContext);

                return $backend->getVariableCode($temp);

            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
                $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $backend->assignLong($temp, $resolved->getCode(), $compilationContext);

                return $backend->getVariableCode($temp);

            case Types::T_VARIABLE:
                $variable = $symbolTable->getVariableForRead(
                    $resolved->getCode(),
                    $compilationContext,
                    $offsetAst
                );

                switch ($variable->getType()) {
                    case Types::T_INT:
                    case Types::T_UINT:
                    case Types::T_LONG:
                        $temp = $symbolTable->getTempVariableForWrite('variable', $compilationContext);
                        $backend->assignLong($temp, $variable->getName(), $compilationContext);

                        return $backend->getVariableCode($temp);

                    case Types::T_STRING:
                    case Types::T_VARIABLE:
                    case Types::T_MIXED:
                        return $backend->getVariableCode($variable);

                    default:
                        throw new CompilerException(
                            'Variable type: ' . $variable->getType()
                            . ' cannot be used as array index without cast',
                            $offsetAst
                        );
                }

                // no break (all paths return or throw)
            default:
                throw new CompilerException(
                    'Cannot use expression: ' . $resolved->getType() . ' as array index without cast',
                    $offsetAst
                );
        }
    }
}
