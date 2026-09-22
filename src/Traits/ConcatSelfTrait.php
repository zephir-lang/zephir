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

namespace Zephir\Traits;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\IllegalOperationException;

use function sprintf;

/**
 * Emits the kernel call that appends one operand onto a zval, for the
 * left-hand sides whose `.=` is a read-modify-write on a stored value
 * (an object property, a static property).
 *
 * PHP concatenates the string form of any scalar, so every operand type has a
 * helper here. A `double` is deliberately not rendered by the compiler: PHP
 * converts a float with `zend_double_to_str()`, which reads `EG(precision)` at
 * run time, so the value is boxed and handed to the engine instead.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2664
 */
trait ConcatSelfTrait
{
    /**
     * Appends `$expression` onto the zval named by `$targetCode`.
     *
     * @param string $targetCode C code naming the zval to append to, e.g. `&_0`
     */
    protected function concatSelfOntoTarget(
        string $targetCode,
        CompiledExpression $expression,
        CompilationContext $context,
        array $statement
    ): void {
        $context->headersManager->add('kernel/operators');
        $context->codePrinter->output(
            $this->concatSelfCall($targetCode, $expression, $context, $statement)
        );
    }

    private function concatSelfCall(
        string $targetCode,
        CompiledExpression $expression,
        CompilationContext $context,
        array $statement
    ): string {
        switch ($expression->getType()) {
            case 'null':
                /**
                 * PHP appends nothing for `null`, but the left operand still
                 * becomes a string, so the call is not dropped.
                 */
                return sprintf('zephir_concat_self_str(%s, SL(""));', $targetCode);

            case 'string':
                return sprintf('zephir_concat_self_str(%s, SL("%s"));', $targetCode, $expression->getCode());

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return sprintf('zephir_concat_self_long(%s, %s);', $targetCode, $expression->getCode());

            case 'char':
            case 'uchar':
                return sprintf('zephir_concat_self_char(%s, %s);', $targetCode, $expression->getCharCode());

            case 'double':
                return sprintf('zephir_concat_self_double(%s, %s);', $targetCode, $expression->getCode());

            case 'bool':
                return sprintf('zephir_concat_self_bool(%s, %s);', $targetCode, $expression->getBooleanCode());

            case 'array':
            case 'empty-array':
                /**
                 * PHP raises "Array to string conversion" and appends "Array".
                 * `zephir_concat_self()` reaches the same conversion through
                 * `zephir_make_printable_zval()`, warning included.
                 */
                return sprintf(
                    'zephir_concat_self(%s, %s);',
                    $targetCode,
                    $context->backend->resolveValue($expression, $context)
                );

            case 'variable':
            case 'mixed':
                return $this->concatSelfVariableCall($targetCode, $expression, $context, $statement);

            default:
                throw new IllegalOperationException($statement, $expression);
        }
    }

    private function concatSelfVariableCall(
        string $targetCode,
        CompiledExpression $expression,
        CompilationContext $context,
        array $statement
    ): string {
        $itemVariable = $context->symbolTable->getVariableForRead(
            $expression->getCode(),
            $context,
            $statement
        );

        switch ($itemVariable->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return sprintf('zephir_concat_self_long(%s, %s);', $targetCode, $itemVariable->getName());

            case 'char':
            case 'uchar':
                return sprintf('zephir_concat_self_char(%s, %s);', $targetCode, $itemVariable->getName());

            case 'double':
                return sprintf('zephir_concat_self_double(%s, %s);', $targetCode, $itemVariable->getName());

            case 'bool':
                return sprintf('zephir_concat_self_bool(%s, %s);', $targetCode, $itemVariable->getName());

            default:
                /* Already a zval: the engine performs the conversion. */
                return sprintf(
                    'zephir_concat_self(%s, %s);',
                    $targetCode,
                    $context->backend->getVariableCode($itemVariable)
                );
        }
    }
}
