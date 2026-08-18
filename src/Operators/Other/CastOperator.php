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
use Zephir\LiteralCompiledExpression;
use Zephir\Operators\AbstractOperator;
use Zephir\Statements\Let\Variable as LetVariable;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

use function in_array;
use function sprintf;

/**
 * Converts a value into another of a different type.
 *
 * Every target resolves the same way: a fast path emits a plain C cast when the
 * operand's type is known at compile time, and anything else is normalized into
 * a zval (see toZval()) and handed to the matching kernel conversion. A cast
 * must never depend on the operand happening to be one of an enumerated set of
 * source types — the sparse matrix that used to do exactly that is why
 * `(string) 5`, `(array) 5`, `(bool) []`, `(uint) x` and `(var) x` all failed
 * with "Cannot cast: X to Y".
 *
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
class CastOperator extends AbstractOperator
{
    /**
     * C type used to narrow each integer cast target.
     *
     * `int` and `long` keep their Zephir spelling: it is what the generated code
     * has always used and both are valid C here.
     */
    private const INT_C_TYPES = [
        Types::T_INT   => 'int',
        Types::T_UINT  => 'zend_ulong',
        Types::T_LONG  => 'long',
        Types::T_ULONG => 'unsigned long',
    ];

    /**
     * Expression types whose code is the name of a zval-backed variable, so the
     * value can be passed to a kernel conversion without being boxed first.
     *
     * Note this is not the same set as ZVAL_VARIABLES: a `string` *expression*
     * is a literal carrying the text itself, while a `string` *variable* is a
     * zval.
     */
    private const ZVAL_EXPRESSIONS = [
        Types::T_ARRAY,
        Types::T_MIXED,
        Types::T_VARIABLE,
    ];

    /**
     * Variable types that are stored as a zval. Everything else (int, uint,
     * long, ulong, char, uchar, bool, double) is a bare C scalar.
     */
    private const ZVAL_VARIABLES = [
        Types::T_ARRAY,
        Types::T_MIXED,
        Types::T_STRING,
        Types::T_VARIABLE,
    ];

    /**
     * Compiles a type cast operation.
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

        $target   = $expression['left'];
        $resolved = $this->unwrapNative($resolved, $compilationContext, $expression);

        return match ($target) {
            Types::T_INT,
            Types::T_UINT,
            Types::T_LONG,
            Types::T_ULONG   => $this->castToInteger($target, $resolved, $compilationContext, $expression),
            Types::T_DOUBLE,
            Types::T_FLOAT   => $this->castToDouble($resolved, $compilationContext, $expression),
            Types::T_BOOL,
            Types::T_BOOLEAN => $this->castToBool($resolved, $compilationContext, $expression),
            Types::T_CHAR,
            Types::T_UCHAR   => $this->castToChar($target, $resolved, $compilationContext, $expression),
            Types::T_STRING,
            Types::T_ISTRING => $this->castToString($resolved, $compilationContext, $expression),
            Types::T_ARRAY   => $this->castToArray($resolved, $compilationContext, $expression),
            Types::T_OBJECT  => $this->castToObject($resolved, $compilationContext, $expression),
            /**
             * `var` and `mixed` accept any value, so there is nothing to
             * convert. The operand still goes through unwrapNative() above, so
             * `(var) someInt` yields the int it holds rather than a `variable`
             * expression that consumers would try to read as a zval.
             */
            Types::T_VARIABLE,
            Types::T_MIXED   => $resolved,
            default          => throw new CompilerException(
                sprintf(
                    "Cannot cast to '%s'. Supported cast targets are: int, uint, long, ulong, char, uchar, "
                    . 'double, float, bool, boolean, string, array, object, var.',
                    $target
                ),
                $expression
            ),
        };
    }

    /**
     * Casts to `array`, mirroring PHP's `(array)`.
     */
    private function castToArray(
        CompiledExpression $resolved,
        CompilationContext $compilationContext,
        array $expression
    ): CompiledExpression {
        if (Types::T_ARRAY === $resolved->getType()) {
            return $resolved;
        }

        $compilationContext->headersManager->add('kernel/operators');
        $compilationContext->symbolTable->mustGrownStack(true);

        /**
         * zephir_get_arrval() converts its source in place, so the source must
         * be a value nobody else owns — otherwise `let b = (array) a` would turn
         * `a` itself into an array.
         */
        $source = $compilationContext->backend->getVariableCode(
            $this->toZval($resolved, $compilationContext, $expression, true)
        );

        $result = $this->newZvalTemp(Types::T_ARRAY, $compilationContext);
        $compilationContext->codePrinter->output(
            sprintf(
                'zephir_get_arrval(%s, %s);',
                $compilationContext->backend->getVariableCode($result),
                $source
            )
        );
        $this->checkVariableTemporal($result);

        return new CompiledExpression(Types::T_VARIABLE, $result->getName(), $expression);
    }

    /**
     * Casts to `bool`/`boolean`, mirroring PHP's truthiness rules.
     */
    private function castToBool(
        CompiledExpression $resolved,
        CompilationContext $compilationContext,
        array $expression
    ): CompiledExpression {
        switch ($resolved->getType()) {
            case Types::T_NULL:
                return new CompiledExpression(Types::T_BOOL, '0', $expression);

            case Types::T_BOOL:
                return new CompiledExpression(Types::T_BOOL, $resolved->getBooleanCode(), $expression);

            case Types::T_CHAR:
            case Types::T_UCHAR:
                return new CompiledExpression(
                    Types::T_BOOL,
                    sprintf('(zend_bool) %s', $resolved->getCharCode()),
                    $expression
                );

            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
                return new CompiledExpression(
                    Types::T_BOOL,
                    sprintf('(zend_bool) %s', $resolved->getCode()),
                    $expression
                );

            case Types::T_DOUBLE:
                /**
                 * A C cast truncates, which would make `(bool) 0.4` false. PHP's
                 * rule is "any non-zero double is true", hence the comparison.
                 */
                return new CompiledExpression(
                    Types::T_BOOL,
                    sprintf('(zend_bool) (0 != (%s))', $resolved->getCode()),
                    $expression
                );
        }

        $compilationContext->headersManager->add('kernel/operators');
        $source = $this->toZval($resolved, $compilationContext, $expression);
        $this->checkVariableTemporal($source);

        return new CompiledExpression(
            Types::T_BOOL,
            sprintf('zephir_get_boolval(%s)', $compilationContext->backend->getVariableCode($source)),
            $expression
        );
    }

    /**
     * Casts to `char`/`uchar`.
     *
     * Zephir's `char` is a byte, not PHP's non-existent character type, so this
     * follows `(int)` semantics and boxes as the integer byte value.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1629
     * @see https://github.com/zephir-lang/zephir/issues/2069
     */
    private function castToChar(
        string $target,
        CompiledExpression $resolved,
        CompilationContext $compilationContext,
        array $expression
    ): CompiledExpression {
        $cType = Types::T_UCHAR === $target ? 'unsigned char' : 'char';

        switch ($resolved->getType()) {
            case Types::T_CHAR:
            case Types::T_UCHAR:
                /**
                 * Char to char is a relabel. getCharCode() yields a valid C
                 * expression for both a literal character and something like
                 * ZEPHIR_GLOBAL(x), so the result no longer needs quoting
                 * downstream — which is what used to produce
                 * `ZVAL_LONG(&x, ''a'')`.
                 */
                return new CompiledExpression($target, $resolved->getCharCode(), $expression);

            case Types::T_NULL:
                return new CompiledExpression($target, '0', $expression);

            case Types::T_BOOL:
                return new CompiledExpression(
                    $target,
                    sprintf('(%s) %s', $cType, $resolved->getBooleanCode()),
                    $expression
                );

            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
            case Types::T_DOUBLE:
                return new CompiledExpression(
                    $target,
                    sprintf('(%s) %s', $cType, $resolved->getCode()),
                    $expression
                );
        }

        $compilationContext->headersManager->add('kernel/operators');
        $source = $compilationContext->backend->getVariableCode(
            $this->toZval($resolved, $compilationContext, $expression)
        );

        $result = $compilationContext->symbolTable->getTempVariableForWrite($target, $compilationContext);
        $compilationContext->codePrinter->output(
            sprintf('%s = (%s) zephir_get_intval(%s);', $result->getName(), $cType, $source)
        );

        return new CompiledExpression(Types::T_VARIABLE, $result->getName(), $expression);
    }

    /**
     * Casts to `double`/`float`.
     */
    private function castToDouble(
        CompiledExpression $resolved,
        CompilationContext $compilationContext,
        array $expression
    ): CompiledExpression {
        switch ($resolved->getType()) {
            case Types::T_NULL:
                return new CompiledExpression(Types::T_DOUBLE, '0', $expression);

            case Types::T_BOOL:
                return new CompiledExpression(Types::T_DOUBLE, $resolved->getBooleanCode(), $expression);

            case Types::T_CHAR:
            case Types::T_UCHAR:
                return new CompiledExpression(
                    Types::T_DOUBLE,
                    sprintf('(double) %s', $resolved->getCharCode()),
                    $expression
                );

            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
                return new CompiledExpression(
                    Types::T_DOUBLE,
                    sprintf('(double) %s', $resolved->getCode()),
                    $expression
                );

            case Types::T_DOUBLE:
                return new CompiledExpression(Types::T_DOUBLE, (string) $resolved->getCode(), $expression);
        }

        $compilationContext->headersManager->add('kernel/operators');
        $source = $compilationContext->backend->getVariableCode(
            $this->toZval($resolved, $compilationContext, $expression)
        );

        return new CompiledExpression(
            Types::T_DOUBLE,
            sprintf('zephir_get_doubleval(%s)', $source),
            $expression
        );
    }

    /**
     * Casts to any of the integer targets (`int`, `uint`, `long`, `ulong`).
     */
    private function castToInteger(
        string $target,
        CompiledExpression $resolved,
        CompilationContext $compilationContext,
        array $expression
    ): CompiledExpression {
        /**
         * The C narrowing lives in the emitted code, so the expression itself is
         * reported as a plain `int`. `uint`/`ulong` are not a type every consumer
         * of a CompiledExpression knows about, and a narrowed value still boxes
         * as ZVAL_LONG — labelling it `uint`/`ulong` only made those consumers
         * fall through and emit the C source text as a PHP string.
         */
        $resultType = Types::T_LONG === $target ? Types::T_LONG : Types::T_INT;

        switch ($resolved->getType()) {
            case Types::T_NULL:
                return new CompiledExpression($resultType, '0', $expression);

            case Types::T_BOOL:
                return new CompiledExpression($resultType, $resolved->getBooleanCode(), $expression);

            case Types::T_CHAR:
            case Types::T_UCHAR:
                return new CompiledExpression(
                    $resultType,
                    $this->narrowInteger($target, (string) $resolved->getCharCode()),
                    $expression
                );

            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
                return new CompiledExpression(
                    $resultType,
                    $this->narrowInteger($target, (string) $resolved->getCode()),
                    $expression
                );

            case Types::T_DOUBLE:
                // Truncating a double is lossy, so the cast is always explicit.
                return new CompiledExpression(
                    $resultType,
                    sprintf('(%s) %s', self::INT_C_TYPES[$target], $resolved->getCode()),
                    $expression
                );
        }

        $compilationContext->headersManager->add('kernel/operators');
        $source = $compilationContext->backend->getVariableCode(
            $this->toZval($resolved, $compilationContext, $expression)
        );

        return new CompiledExpression(
            $resultType,
            $this->narrowInteger($target, sprintf('zephir_get_intval(%s)', $source)),
            $expression
        );
    }

    /**
     * Casts to `object`, mirroring PHP's `(object)`.
     */
    private function castToObject(
        CompiledExpression $resolved,
        CompilationContext $compilationContext,
        array $expression
    ): CompiledExpression {
        $compilationContext->headersManager->add('kernel/operators');
        $compilationContext->symbolTable->mustGrownStack(true);

        /**
         * zephir_convert_to_object() converts in place, so it must be handed a
         * value nobody else owns — otherwise `let b = (object) a` would turn `a`
         * itself into an object.
         */
        $result = $this->toZval($resolved, $compilationContext, $expression, true);
        $compilationContext->codePrinter->output(
            sprintf('zephir_convert_to_object(%s);', $compilationContext->backend->getVariableCode($result))
        );

        return new CompiledExpression(Types::T_VARIABLE, $result->getName(), $expression);
    }

    /**
     * Casts to `string`/`istring`, mirroring PHP's `(string)`.
     */
    private function castToString(
        CompiledExpression $resolved,
        CompilationContext $compilationContext,
        array $expression
    ): CompiledExpression {
        switch ($resolved->getType()) {
            case Types::T_STRING:
            case Types::T_ISTRING:
                return $resolved;

            case Types::T_CHAR:
            case Types::T_UCHAR:
                /**
                 * A char *variable* holds a byte, whose string form is the
                 * 1-character string it stands for. A char *literal* (and a C
                 * expression such as a char global) already carries the
                 * character, so it becomes the string value directly.
                 */
                if (
                    !$resolved instanceof LiteralCompiledExpression
                    && $compilationContext->symbolTable->hasVariable((string) $resolved->getCode())
                ) {
                    $compilationContext->symbolTable->mustGrownStack(true);
                    $result = $compilationContext->symbolTable->getTempVariableForWrite(
                        Types::T_STRING,
                        $compilationContext
                    );
                    $compilationContext->backend->assignChar(
                        $result,
                        $compilationContext->symbolTable->getVariableForRead(
                            (string) $resolved->getCode(),
                            $compilationContext,
                            $expression
                        ),
                        $compilationContext
                    );

                    return new CompiledExpression(Types::T_VARIABLE, $result->getName(), $expression);
                }

                return new CompiledExpression(Types::T_STRING, $resolved->getCode(), $expression);
        }

        $compilationContext->headersManager->add('kernel/operators');
        $compilationContext->symbolTable->mustGrownStack(true);

        /**
         * zephir_cast_to_string() goes through zephir_make_printable_zval(), so
         * every source formats exactly as PHP formats it — including `5.0`
         * rendering as "5" and an array rendering as "Array" with a warning.
         */
        $source = $compilationContext->backend->getVariableCode(
            $this->toZval($resolved, $compilationContext, $expression)
        );

        $result = $this->newZvalTemp(Types::T_STRING, $compilationContext);
        $compilationContext->codePrinter->output(
            sprintf(
                'zephir_cast_to_string(%s, %s);',
                $compilationContext->backend->getVariableCode($result),
                $source
            )
        );
        $this->checkVariableTemporal($result);

        return new CompiledExpression(Types::T_VARIABLE, $result->getName(), $expression);
    }

    /**
     * Prefixes a C cast only when the target is not the type the source code
     * already produces. `int`/`long` need no narrowing from another integer or
     * from zephir_get_intval(); the unsigned targets always do.
     */
    private function narrowInteger(string $target, string $code): string
    {
        if (Types::T_INT === $target || Types::T_LONG === $target) {
            return $code;
        }

        return sprintf('(%s) %s', self::INT_C_TYPES[$target], $code);
    }

    /**
     * Creates a tracked temporary zval of the given variable type.
     */
    private function newZvalTemp(string $type, CompilationContext $compilationContext): Variable
    {
        $temp = $compilationContext->symbolTable->getTempVariable($type, $compilationContext);
        $temp->setMustInitNull(true);
        $temp->setIsInitialized(true, $compilationContext);
        $temp->increaseUses();

        return $temp;
    }

    /**
     * Materializes any resolved expression as a zval-backed variable.
     *
     * This is what makes the cast matrix total: a literal, a native-typed local
     * and a dynamic variable all reduce to something the kernel conversions can
     * accept, so no target needs to enumerate its source types.
     *
     * @param bool $isolate Pass true when the caller converts the zval in place
     *                      and must therefore not touch a variable the rest of
     *                      the method still reads.
     */
    private function toZval(
        CompiledExpression $resolved,
        CompilationContext $compilationContext,
        array $expression,
        bool $isolate = false
    ): Variable {
        if (in_array($resolved->getType(), self::ZVAL_EXPRESSIONS, true)) {
            $symbol = $compilationContext->symbolTable->getVariableForRead(
                (string) $resolved->getCode(),
                $compilationContext,
                $expression
            );

            if (!$isolate || $symbol->isTemporal()) {
                return $symbol;
            }

            // Fall through to copy the variable into a temp we own.
            $resolved = new CompiledExpression(
                Types::T_VARIABLE,
                $symbol->getName(),
                $resolved->getOriginal()
            );
        }

        $compilationContext->symbolTable->mustGrownStack(true);
        $temp = $this->newZvalTemp(Types::T_VARIABLE, $compilationContext);

        $original             = $resolved->getOriginal() ?? $expression['right'];
        $original['operator'] = 'assign';

        (new LetVariable())->assign(
            $temp->getName(),
            $temp,
            $resolved,
            new ReadDetector(),
            $compilationContext,
            $original
        );

        return $temp;
    }

    /**
     * Re-labels a `variable`/`mixed` expression that points at a native-typed
     * local with that local's own type.
     *
     * A native local (int, double, bool, char, …) carries a bare C scalar, not a
     * zval, so treating it as dynamic is what produced calls such as
     * `zephir_cast_to_string(&_0, someLong)`. Normalizing here also means every
     * cast below sees one flat set of source types instead of repeating an inner
     * switch per target.
     */
    private function unwrapNative(
        CompiledExpression $resolved,
        CompilationContext $compilationContext,
        array $expression
    ): CompiledExpression {
        if (!in_array($resolved->getType(), [Types::T_VARIABLE, Types::T_MIXED], true)) {
            return $resolved;
        }

        $code = (string) $resolved->getCode();
        if (!$compilationContext->symbolTable->hasVariable($code)) {
            return $resolved;
        }

        $symbol = $compilationContext->symbolTable->getVariableForRead($code, $compilationContext, $expression);
        if (in_array($symbol->getType(), self::ZVAL_VARIABLES, true)) {
            return $resolved;
        }

        return new CompiledExpression($symbol->getType(), $symbol->getName(), $resolved->getOriginal());
    }
}
