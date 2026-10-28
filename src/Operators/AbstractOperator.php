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

namespace Zephir\Operators;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Name;
use Zephir\Traits\VariablesTrait;
use Zephir\Types\TypeRegistry;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

use function is_finite;
use function is_int;
use function is_numeric;
use function is_object;

abstract class AbstractOperator
{
    use VariablesTrait;

    /**
     * The C kind an operand is read as by the operators that classify their
     * operands (`/`, `%`, the arithmetic and the bitwise operators).
     */
    protected const KIND_LONG   = 'long';
    protected const KIND_DOUBLE = 'double';
    protected const KIND_ZVAL   = 'zval';

    protected bool      $expecting         = true;
    protected ?Variable $expectingVariable = null;
    protected bool      $literalOnly       = true;
    protected string    $operator;
    protected bool      $readOnly          = false;

    /**
     * Rewrites a `char`/`uchar` expression so its code is a valid C expression
     * (see CompiledExpression::getCharCode()), letting it take part in integer
     * arithmetic and comparisons. Any other expression is returned untouched.
     */
    protected function quoteCharLiteral(CompiledExpression $expression, ?array $original = null): CompiledExpression
    {
        if (!TypeRegistry::isChar($expression->getType())) {
            return $expression;
        }

        return new CompiledExpression(
            $expression->getType(),
            $expression->getCharCode(),
            $original ?? $expression->getOriginal()
        );
    }

    /**
     * Returns the expected variable for assignment or creates a temporary variable to
     * store the result.
     *
     * @param CompilationContext $compilationContext
     * @param array              $expression
     * @param bool               $init
     *
     * @return Variable|null
     */
    public function getExpected(CompilationContext $compilationContext, array $expression, bool $init = true): ?Variable
    {
        if (!$this->expecting) {
            return $this->expectingVariable;
        }

        $symbolVariable = $this->expectingVariable;
        if (is_object($symbolVariable)) {
            if ('variable' === $symbolVariable->getType()) {
                if (!$init) {
                    return $symbolVariable;
                }
                $symbolVariable->initVariant($compilationContext);
            } else {
                if (!$this->readOnly) {
                    if (!$this->literalOnly) {
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                            'variable',
                            $compilationContext,
                            $expression
                        );
                    } else {
                        $symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite(
                            'variable',
                            $compilationContext
                        );
                    }
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite(
                        'variable',
                        $compilationContext
                    );
                }
            }
        } else {
            if (!$this->readOnly) {
                if (!$this->literalOnly) {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                        'variable',
                        $compilationContext,
                        $expression
                    );
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite(
                        'variable',
                        $compilationContext
                    );
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite(
                    'variable',
                    $compilationContext
                );
            }
        }

        return $symbolVariable;
    }

    /**
     * Returns the expected variable for assignment or creates a temporary variable to
     * store the result, if a temporary variable is created it use whose body is only freed
     * on every iteration.
     *
     * @param CompilationContext $compilationContext
     * @param string             $type
     *
     * @return Variable|null
     */
    public function getExpectedComplexLiteral(
        CompilationContext $compilationContext,
        string $type = 'variable'
    ): ?Variable {
        $symbolVariable = $this->expectingVariable;

        if ($this->expecting) {
            if (is_object($symbolVariable)) {
                if ($symbolVariable->getType() === $type || 'return_value' === $symbolVariable->getName()) {
                    $symbolVariable->initVariant($compilationContext);
                } else {
                    if (!$this->readOnly) {
                        $symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite(
                            $type,
                            $compilationContext
                        );
                    } else {
                        $symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite(
                            $type,
                            $compilationContext
                        );
                    }
                }
            } else {
                if (!$this->readOnly) {
                    $symbolVariable = $compilationContext->symbolTable->getTempComplexLiteralVariableForWrite(
                        $type,
                        $compilationContext
                    );
                } else {
                    $symbolVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite(
                        $type,
                        $compilationContext
                    );
                }
            }
        }

        return $symbolVariable;
    }

    /**
     * Returns the expected variable for assignment or creates a temporary variable to
     * store the result. This method returns a variable that is always stored in the heap.
     *
     * @param CompilationContext $compilationContext
     * @param array              $expression
     * @param bool               $init
     *
     * @return Variable|null
     */
    public function getExpectedNonLiteral(
        CompilationContext $compilationContext,
        array $expression,
        bool $init = true
    ): ?Variable {
        $symbolVariable = $this->expectingVariable;

        if (!$this->expecting) {
            return $symbolVariable;
        }

        if ($symbolVariable !== null) {
            if ('variable' === $symbolVariable->getType() && !$symbolVariable->isLocalOnly()) {
                if (!$init) {
                    return $symbolVariable;
                }

                $symbolVariable->initVariant($compilationContext);
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext,
                    $expression
                );
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'variable',
                $compilationContext,
                $expression
            );
        }

        return $symbolVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     *
     * @return bool
     */
    public function isExpecting(): bool
    {
        return $this->expecting;
    }

    /**
     * Checks if the result of the evaluated expression is read only.
     *
     * @return bool
     */
    public function isReadOnly(): bool
    {
        return $this->readOnly;
    }

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     *
     * @param bool          $expecting
     * @param Variable|null $expectingVariable
     */
    public function setExpectReturn(bool $expecting, ?Variable $expectingVariable = null): void
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

    /**
     * Resolves a numeric literal value (as stored verbatim by the parser) to an
     * actual int/float so it can be used in constant folding.
     *
     * Hexadecimal literals such as `0xffffffff` are kept as their raw string by
     * the parser; performing arithmetic on that string directly raises
     * "A non-numeric value encountered". They are decoded explicitly here, while
     * plain decimal/double literals keep their normal numeric semantics (e.g.
     * a leading-zero "052" stays decimal 52, not octal). See #2014.
     *
     * Static and public so declaration-time folding
     * ({@see \Zephir\Expression\ConstantExpressionEvaluator}) decodes literals
     * exactly the same way, from one copy of the rule.
     *
     * @return int|float
     */
    public static function literalToNumber(string $value): int|float
    {
        if (preg_match('/^-?0x[0-9a-fA-F]+$/', $value)) {
            return intval($value, 16);
        }

        return $value + 0;
    }

    /**
     * Whether PHP reads the number as an integer without losing anything, as
     * zend_is_long_compatible() does: a float must be finite, in range and
     * have no fractional part.
     */
    protected static function isLongCompatible(int|float $value): bool
    {
        if (is_int($value)) {
            return true;
        }

        return is_finite($value)
            && $value >= (float) PHP_INT_MIN
            && $value < (float) PHP_INT_MAX
            && (float) (int) $value === $value;
    }

    /**
     * Both operands classified as a C integer, a C double or a zval. Against
     * a zval a bool stays a bool, so a TypeError names it as PHP does.
     *
     * @return array{0: array, 1: array}
     *
     * @throws CompilerException
     */
    protected function classifiedOperands(
        CompiledExpression $left,
        CompiledExpression $right,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        $leftOperand  = $this->operand($left, $expression, $compilationContext);
        $rightOperand = $this->operand($right, $expression, $compilationContext);
        if (self::KIND_ZVAL === $leftOperand['kind'] && isset($rightOperand['bool'])) {
            $rightOperand = $this->boxedBool($rightOperand['bool'], $compilationContext);
        }
        if (self::KIND_ZVAL === $rightOperand['kind'] && isset($leftOperand['bool'])) {
            $leftOperand = $this->boxedBool($leftOperand['bool'], $compilationContext);
        }

        return [$leftOperand, $rightOperand];
    }

    /**
     * Classifies one compiled operand as a C integer, a C double or a zval,
     * and returns the C code that reads it as that kind. A string or null
     * literal is boxed, since PHP coerces it or throws for it at runtime.
     *
     * @return array{kind: string, code: string, variable: ?Variable}
     *
     * @throws CompilerException
     */
    protected function operand(
        CompiledExpression $operand,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        switch ($operand->getType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
                return ['kind' => self::KIND_LONG, 'code' => $operand->getCode(), 'variable' => null];

            case Types::T_CHAR:
            case Types::T_UCHAR:
                return ['kind' => self::KIND_LONG, 'code' => $operand->getCharCode(), 'variable' => null];

            case Types::T_BOOL:
                return $this->boolOperand($operand->getBooleanCode());

            case Types::T_DOUBLE:
                return ['kind' => self::KIND_DOUBLE, 'code' => $operand->getCode(), 'variable' => null];

            case Types::T_STRING:
                $boxed = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignString(
                    $boxed,
                    Name::addSlashes($operand->getCode()),
                    $compilationContext
                );

                return $this->zvalOperand($boxed, $compilationContext);

            case Types::T_NULL:
                $boxed = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext);
                $compilationContext->backend->assignNull($boxed, $compilationContext);

                return $this->zvalOperand($boxed, $compilationContext);

            case Types::T_ARRAY:
            case Types::T_VARIABLE:
                $variable = $compilationContext->symbolTable->getVariableForRead(
                    $operand->getCode(),
                    $compilationContext,
                    $expression
                );

                return $this->variableOperand($variable, $expression, $compilationContext);

            default:
                throw new CompilerException(
                    'Cannot operate ' . $operand->getType() . ' with the ' . $this->operator . ' operator',
                    $expression
                );
        }
    }

    /**
     * A typed string or array local is already a zval; a char is its byte.
     *
     * @return array{kind: string, code: string, variable: ?Variable}
     *
     * @throws CompilerException
     */
    protected function variableOperand(
        Variable $variable,
        array $expression,
        CompilationContext $compilationContext
    ): array {
        switch ($variable->getType()) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
            case Types::T_CHAR:
            case Types::T_UCHAR:
                return ['kind' => self::KIND_LONG, 'code' => $variable->getName(), 'variable' => null];

            case Types::T_BOOL:
                return $this->boolOperand($variable->getName());

            case Types::T_DOUBLE:
                return ['kind' => self::KIND_DOUBLE, 'code' => $variable->getName(), 'variable' => null];

            case Types::T_VARIABLE:
            case Types::T_MIXED:
            case Types::T_STRING:
            case Types::T_ARRAY:
                return $this->zvalOperand($variable, $compilationContext);

            default:
                throw new CompilerException(
                    'Cannot operate ' . $variable->getType() . ' variables with the ' . $this->operator . ' operator',
                    $expression
                );
        }
    }

    /**
     * @return array{kind: string, code: string, variable: Variable}
     */
    protected function zvalOperand(Variable $variable, CompilationContext $compilationContext): array
    {
        return [
            'kind'     => self::KIND_ZVAL,
            'code'     => $compilationContext->backend->getVariableCode($variable),
            'variable' => $variable,
        ];
    }

    /**
     * A bool is the integer 0 or 1; `bool` keeps its C code for boxing.
     *
     * @return array{kind: string, code: string, variable: ?Variable, bool: string}
     */
    protected function boolOperand(string $code): array
    {
        return ['kind' => self::KIND_LONG, 'code' => '(zend_long) ' . $code, 'variable' => null, 'bool' => $code];
    }

    protected function boxedBool(string $code, CompilationContext $compilationContext): array
    {
        $boxed = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext);
        $compilationContext->backend->assignBool($boxed, $code, $compilationContext);

        return $this->zvalOperand($boxed, $compilationContext);
    }

    /**
     * A native operand copied into a temporary zval for PHP's own operator.
     *
     * @return array{kind: string, code: string, variable: Variable}
     */
    protected function boxedOperand(array $operand, CompilationContext $compilationContext): array
    {
        if (self::KIND_ZVAL === $operand['kind']) {
            return $operand;
        }

        if (isset($operand['bool'])) {
            return $this->boxedBool($operand['bool'], $compilationContext);
        }

        $boxed = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext);
        if (self::KIND_DOUBLE === $operand['kind']) {
            $compilationContext->backend->assignDouble($boxed, $operand['code'], $compilationContext);
        } else {
            $compilationContext->backend->assignLong($boxed, $operand['code'], $compilationContext);
        }

        return $this->zvalOperand($boxed, $compilationContext);
    }

    /**
     * Reads a native operand as a C `zend_long`, the way PHP's bitwise
     * operators read theirs (zendi_try_get_long() in Zend/zend_operators.c).
     *
     * A double goes through ZEPHIR_DVAL_TO_LVAL, which raises the float to int
     * deprecation of PHP 8.1 and the out of range warning of PHP 8.5 exactly
     * as the engine does. An error handler may turn either into an exception,
     * so the conversion is checked before the operation runs. A double literal
     * PHP reads as an integer without loss is written as that integer.
     *
     * @param array{kind: string, code: int|float|string} $operand a long or a double operand
     *
     * @see https://github.com/zephir-lang/zephir/issues/2753
     */
    protected function integerOperandCode(array $operand, CompilationContext $compilationContext): string
    {
        $code = (string) $operand['code'];
        if (self::KIND_DOUBLE !== $operand['kind']) {
            return $code;
        }

        if (is_numeric($code) && self::isLongCompatible((float) $code)) {
            return (string) (int) (float) $code;
        }

        $compilationContext->headersManager->add('kernel/main');
        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('long', $compilationContext);
        $compilationContext->codePrinter->output(
            $tempVariable->getName() . ' = ZEPHIR_DVAL_TO_LVAL(' . $code . ');'
        );
        $compilationContext->emitExceptionCheck();

        return $tempVariable->getName();
    }

    /**
     * Writes the result of a zval-producing helper into the expected variable.
     *
     * @throws CompilerException
     */
    protected function zvalResult(
        string $helper,
        array $left,
        array $right,
        array $expression,
        CompilationContext $compilationContext,
        array $dynamicTypes
    ): CompiledExpression {
        $expected     = $this->getExpected($compilationContext, $expression);
        $expectedCode = $compilationContext->backend->getVariableCode($expected);

        $compilationContext->codePrinter->output(
            $helper . '(' . $expectedCode . ', ' . $left['code'] . ', ' . $right['code'] . ');'
        );
        $compilationContext->emitExceptionCheck();

        foreach ([$left['variable'], $right['variable']] as $operandVariable) {
            if (null !== $operandVariable) {
                $this->checkVariableTemporal($operandVariable);
            }
        }

        $expected->setDynamicTypes($dynamicTypes);

        return new CompiledExpression('variable', $expected->getName(), $expression);
    }
}
