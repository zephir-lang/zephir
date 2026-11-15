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

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Traits\VariablesTrait;
use Zephir\Variable\Variable as ZephirVariable;

use function in_array;

/**
 * Compiles a compound assignment on an object property: `o->x += v`,
 * `o->{n} .= v`, `o->{"x"} <<= v` and every other combination.
 *
 * PHP compiles all of them to ZEND_ASSIGN_OBJ_OP: the property is fetched
 * once, read-write, and the operator applies to it in place, or, for an object
 * with no slot to give, reads and writes it back. Here that is one call to the
 * kernel's zephir_property_assign_op(), handed the engine's own operator
 * function. The assignment handlers check the object and the name first.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2808
 */
final class PropertyOperator
{
    use VariablesTrait;

    private const ASSIGN_TYPES = [
        'object-property',
        'variable-dynamic-object-property',
        'string-dynamic-object-property',
    ];

    /**
     * Whether the assignment is a compound operator on an object property.
     */
    public static function handles(array $assignment): bool
    {
        return isset(ArrayIndexOperator::OPERATOR_FUNCTIONS[$assignment['operator'] ?? 'assign'])
            && in_array($assignment['assign-type'], self::ASSIGN_TYPES, true);
    }

    /**
     * `$property` is the name written in the code, or the local holding a
     * runtime name.
     *
     * @throws CompilerException
     */
    public function assign(
        ZephirVariable $object,
        string | ZephirVariable $property,
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement
    ): void {
        $operator = $statement['operator'];
        $value    = $this->operand($resolvedExpr, 'concat-assign' === $operator, $compilationContext);

        $compilationContext->headersManager->add('kernel/object');
        $compilationContext->backend->assignPropertyOperator(
            $object,
            $property,
            $value,
            ArrayIndexOperator::OPERATOR_FUNCTIONS[$operator],
            $compilationContext
        );

        if ($value instanceof ZephirVariable) {
            $this->checkVariableTemporal($value);
        }
    }

    /**
     * The right-hand side as a zval. A `char` is the character it holds for
     * `.=`, as a local's `.=` appends it, and its code otherwise.
     *
     * @throws CompilerException
     */
    private function operand(
        CompiledExpression $resolvedExpr,
        bool $concat,
        CompilationContext $compilationContext
    ): ZephirVariable | string {
        $backend  = $compilationContext->backend;
        $original = $resolvedExpr->getOriginal();

        if ('variable' === $resolvedExpr->getType()) {
            $variable = $compilationContext->symbolTable->getVariableForRead(
                $resolvedExpr->getCode(),
                $compilationContext,
                $original
            );

            switch ($variable->getType()) {
                case 'variable':
                case 'mixed':
                case 'string':
                case 'array':
                    return $variable;

                case 'int':
                case 'uint':
                case 'long':
                case 'ulong':
                    $temp = $this->temp($compilationContext);
                    $backend->assignLong($temp, $variable, $compilationContext);

                    return $temp;

                case 'double':
                    $temp = $this->temp($compilationContext);
                    $backend->assignDouble($temp, $variable, $compilationContext);

                    return $temp;

                case 'bool':
                    $temp = $this->temp($compilationContext);
                    $backend->assignBool($temp, $variable, $compilationContext);

                    return $temp;

                case 'char':
                case 'uchar':
                    $temp = $this->temp($compilationContext);
                    if ($concat) {
                        $backend->assignChar($temp, $variable, $compilationContext);
                    } else {
                        $backend->assignLong($temp, $variable, $compilationContext);
                    }

                    return $temp;

                default:
                    throw new CompilerException(
                        'Variable type: ' . $variable->getType() . ' cannot be used in a compound assignment',
                        $original
                    );
            }
        }

        switch ($resolvedExpr->getType()) {
            case 'null':
                return 'null';

            case 'bool':
                $code = $resolvedExpr->getBooleanCode();
                if ('1' === $code || 'true' === $code) {
                    return 'true';
                }

                if ('0' === $code || 'false' === $code) {
                    return 'false';
                }

                $temp = $this->temp($compilationContext);
                $backend->assignBool($temp, $code, $compilationContext);

                return $temp;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                $temp = $this->temp($compilationContext);
                $backend->assignLong($temp, $resolvedExpr->getCode(), $compilationContext);

                return $temp;

            case 'double':
                $temp = $this->temp($compilationContext);
                $backend->assignDouble($temp, $resolvedExpr->getCode(), $compilationContext);

                return $temp;

            case 'char':
            case 'uchar':
                $temp = $this->temp($compilationContext);
                if (!$concat) {
                    $backend->assignLong($temp, $resolvedExpr->getCharCode(), $compilationContext);

                    return $temp;
                }

                $byte = $compilationContext->symbolTable->getTempVariableForWrite('char', $compilationContext);
                $compilationContext->codePrinter->output(
                    $byte->getName() . ' = ' . $resolvedExpr->getCharCode() . ';'
                );
                $backend->assignChar($temp, $byte, $compilationContext);

                return $temp;

            case 'string':
                $temp = $this->temp($compilationContext);
                $backend->assignString($temp, $resolvedExpr->getCode(), $compilationContext);

                return $temp;

            case 'array':
                return $compilationContext->symbolTable->getVariableForRead(
                    $resolvedExpr->getCode(),
                    $compilationContext,
                    $original
                );

            default:
                throw new CompilerException(
                    'Expression: ' . $resolvedExpr->getType() . ' cannot be used in a compound assignment',
                    $original
                );
        }
    }

    private function temp(CompilationContext $compilationContext): ZephirVariable
    {
        return $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
    }
}
