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

namespace Zephir;

use ReflectionException;
use Zephir\Exception\CompilerException;
use Zephir\Expression\Closure;
use Zephir\Expression\ClosureArrow;
use Zephir\Expression\Constants;
use Zephir\Expression\NativeArray;
use Zephir\Expression\NativeArrayAccess;
use Zephir\Expression\PropertyAccess;
use Zephir\Expression\PropertyDynamicAccess;
use Zephir\Expression\Reference;
use Zephir\Expression\StaticConstantAccess;
use Zephir\Expression\StaticPropertyAccess;
use Zephir\Operators\Arithmetical\AddOperator;
use Zephir\Operators\Arithmetical\DivOperator;
use Zephir\Operators\Arithmetical\ModOperator;
use Zephir\Operators\Arithmetical\MulOperator;
use Zephir\Operators\Arithmetical\SubOperator;
use Zephir\Operators\Bitwise\BitwiseAndOperator;
use Zephir\Operators\Bitwise\BitwiseNotOperator;
use Zephir\Operators\Bitwise\BitwiseOrOperator;
use Zephir\Operators\Bitwise\BitwiseXorOperator;
use Zephir\Operators\Bitwise\ShiftLeftOperator;
use Zephir\Operators\Bitwise\ShiftRightOperator;
use Zephir\Operators\Comparison\EqualsOperator;
use Zephir\Operators\Comparison\GreaterEqualOperator;
use Zephir\Operators\Comparison\GreaterOperator;
use Zephir\Operators\Comparison\IdenticalOperator;
use Zephir\Operators\Comparison\LessEqualOperator;
use Zephir\Operators\Comparison\LessOperator;
use Zephir\Operators\Comparison\NotEqualsOperator;
use Zephir\Operators\Comparison\NotIdenticalOperator;
use Zephir\Operators\Logical\AndOperator;
use Zephir\Operators\Logical\OrOperator;
use Zephir\Operators\Other\CastOperator;
use Zephir\Operators\Other\CloneOperator;
use Zephir\Operators\Other\ConcatOperator;
use Zephir\Operators\Other\EmptyOperator;
use Zephir\Operators\Other\FetchOperator;
use Zephir\Operators\Other\InstanceOfOperator;
use Zephir\Operators\Other\IssetOperator;
use Zephir\Operators\Other\LikelyOperator;
use Zephir\Operators\Other\NewInstanceOperator;
use Zephir\Operators\Other\NewInstanceTypeOperator;
use Zephir\Operators\Other\RangeExclusiveOperator;
use Zephir\Operators\Other\RangeInclusiveOperator;
use Zephir\Operators\Other\RequireOnceOperator;
use Zephir\Operators\Other\RequireOperator;
use Zephir\Operators\Other\ShortTernaryOperator;
use Zephir\Operators\Other\TernaryOperator;
use Zephir\Operators\Other\TypeHintOperator;
use Zephir\Operators\Other\TypeOfOperator;
use Zephir\Operators\Other\UnlikelyOperator;
use Zephir\Operators\Unary\MinusOperator;
use Zephir\Operators\Unary\NotOperator;
use Zephir\Variable\Variable;

use function str_replace;
use function strlen;
use function substr;

use const PHP_EOL;

/**
 * Represents an expression.
 * Most language constructs in a language are expressions.
 */
class Expression
{
    protected bool      $evalMode          = false;
    protected bool      $expecting         = true;
    protected ?Variable $expectingVariable = null;
    protected bool      $noisy             = true;
    protected bool      $readOnly          = false;

    public function __construct(protected array $expression)
    {
    }

    /**
     * Resolves an expression.
     *
     * @throws CompilerException|Exception
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(CompilationContext $compilationContext): CompiledExpression
    {
        $expression = $this->expression;
        $type       = $expression['type'];

        switch ($type) {
            case 'null':
                return new LiteralCompiledExpression('null', null, $expression);

            case 'int':
            case 'integer':
                return new LiteralCompiledExpression('int', $expression['value'], $expression);

            case 'long':
            case 'double':
            case 'bool':
                return new LiteralCompiledExpression($type, $expression['value'], $expression);

            case 'string':
                return new LiteralCompiledExpression(
                    'string',
                    str_replace(PHP_EOL, '\\n', $expression['value']),
                    $expression
                );
            case 'istring':
                return new LiteralCompiledExpression(
                    'istring',
                    str_replace(PHP_EOL, '\\n', $expression['value']),
                    $expression
                );

            case 'char':
                if (!strlen($expression['value'])) {
                    throw new CompilerException('Invalid empty char literal', $expression);
                }
                if (strlen($expression['value']) > 2) {
                    if (strlen($expression['value']) > 10) {
                        throw new CompilerException(
                            "Invalid char literal: '"
                            . substr($expression['value'], 0, 10)
                            . "...'",
                            $expression
                        );
                    } else {
                        throw new CompilerException(
                            "Invalid char literal: '"
                            . $expression['value']
                            . "'",
                            $expression
                        );
                    }
                }

                return new LiteralCompiledExpression('char', $expression['value'], $expression);

            case 'variable':
                $var = $compilationContext->symbolTable->getVariable($expression['value']);
                if ($var) {
                    if ('this' == $var->getRealName()) {
                        $var = 'this';
                    } else {
                        $var = $var->getName();
                    }
                } else {
                    $var = $expression['value'];
                }

                return new CompiledExpression('variable', $var, $expression);

            case 'constant':
                $compilableExpression = new Constants();
                break;

            case 'empty-array':
                return $this->emptyArray($expression, $compilationContext);

            case 'array-access':
                $compilableExpression = new NativeArrayAccess();
                $compilableExpression->setNoisy($this->isNoisy());
                break;

            case 'property-access':
                $compilableExpression = new PropertyAccess();
                $compilableExpression->setNoisy($this->isNoisy());
                break;

            case 'property-string-access':
            case 'property-dynamic-access':
                $compilableExpression = new PropertyDynamicAccess();
                $compilableExpression->setNoisy($this->isNoisy());
                break;

            case 'static-constant-access':
                $compilableExpression = new StaticConstantAccess();
                break;

            case 'static-property-access':
                $compilableExpression = new StaticPropertyAccess();
                break;

            case 'fcall':
                $functionCall = new FunctionCall();

                return $functionCall->compile($this, $compilationContext);

            case 'mcall':
                $methodCall = new MethodCall();

                return $methodCall->compile($this, $compilationContext);

            case 'scall':
                $staticCall = new StaticCall();

                return $staticCall->compile($this, $compilationContext);

            case 'isset':
                $compilableExpression = new IssetOperator();
                break;

            case 'fetch':
                $compilableExpression = new FetchOperator();
                break;

            case 'empty':
                $compilableExpression = new EmptyOperator();
                break;

            case 'array':
                $compilableExpression = new NativeArray();
                break;

            case 'new':
                $compilableExpression = new NewInstanceOperator();
                break;

            case 'new-type':
                $compilableExpression = new NewInstanceTypeOperator();
                break;

            case 'not':
                $compilableExpression = new NotOperator();
                break;

            case 'bitwise_not':
                $compilableExpression = new BitwiseNotOperator();
                break;

            case 'equals':
                $compilableExpression = new EqualsOperator();
                break;

            case 'not-equals':
                $compilableExpression = new NotEqualsOperator();
                break;

            case 'identical':
                $compilableExpression = new IdenticalOperator();
                break;

            case 'not-identical':
                $compilableExpression = new NotIdenticalOperator();
                break;

            case 'greater':
                $compilableExpression = new GreaterOperator();
                break;

            case 'less':
                $compilableExpression = new LessOperator();
                break;

            case 'less-equal':
                $compilableExpression = new LessEqualOperator();
                break;

            case 'greater-equal':
                $compilableExpression = new GreaterEqualOperator();
                break;

            case 'add':
                $compilableExpression = new AddOperator();
                break;

            case 'minus':
                $compilableExpression = new MinusOperator();
                break;

            case 'sub':
                $compilableExpression = new SubOperator();
                break;

            case 'mul':
                $compilableExpression = new MulOperator();
                break;

            case 'div':
                $compilableExpression = new DivOperator();
                break;

            case 'mod':
                $compilableExpression = new ModOperator();
                break;

            case 'and':
                $compilableExpression = new AndOperator();
                break;

            case 'or':
                $compilableExpression = new OrOperator();
                break;

            case 'bitwise_and':
                $compilableExpression = new BitwiseAndOperator();
                break;

            case 'bitwise_or':
                $compilableExpression = new BitwiseOrOperator();
                break;

            case 'bitwise_xor':
                $compilableExpression = new BitwiseXorOperator();
                break;

            case 'bitwise_shiftleft':
                $compilableExpression = new ShiftLeftOperator();
                break;

            case 'bitwise_shiftright':
                $compilableExpression = new ShiftRightOperator();
                break;

            case 'concat':
                $expr = new ConcatOperator();
                $expr->setExpectReturn($this->expecting, $this->expectingVariable);

                return $expr->compile($expression, $compilationContext);

            case 'irange':
                $compilableExpression = new RangeInclusiveOperator();
                break;

            case 'erange':
                $compilableExpression = new RangeExclusiveOperator();
                break;

            case 'list':
                if ('list' == $expression['left']['type']) {
                    $compilationContext->logger->warning(
                        'Unnecessary extra parentheses',
                        ['extra-parentheses', $expression]
                    );
                }
                $numberPrints = $compilationContext->codePrinter->getNumberPrints();
                $expr         = new self($expression['left']);
                $expr->setExpectReturn($this->expecting, $this->expectingVariable);
                $resolved = $expr->compile($compilationContext);
                if (($compilationContext->codePrinter->getNumberPrints() - $numberPrints) <= 1) {
                    if (str_contains($resolved->getCode(), ' ')) {
                        return new CompiledExpression(
                            $resolved->getType(),
                            '(' . $resolved->getCode() . ')',
                            $expression
                        );
                    }
                }

                return $resolved;

            case 'cast':
                $compilableExpression = new CastOperator();
                break;

            case 'type-hint':
                $expr = new TypeHintOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->expecting, $this->expectingVariable);

                return $expr->compile($expression, $compilationContext);

            case 'instanceof':
                $compilableExpression = new InstanceOfOperator();
                break;

            case 'clone':
                $compilableExpression = new CloneOperator();
                break;

            case 'ternary':
                $compilableExpression = new TernaryOperator();
                break;

            case 'short-ternary':
                $expr = new ShortTernaryOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->expecting, $this->expectingVariable);

                return $expr->compile($expression, $compilationContext);

            case 'likely':
                if (!$this->evalMode) {
                    throw new CompilerException(
                        "'likely' operator can only be used in evaluation expressions",
                        $expression
                    );
                }
                $expr = new LikelyOperator();
                $expr->setReadOnly($this->isReadOnly());

                return $expr->compile($expression, $compilationContext);

            case 'unlikely':
                if (!$this->evalMode) {
                    throw new CompilerException(
                        "'unlikely' operator can only be used in evaluation expressions",
                        $expression
                    );
                }
                $expr = new UnlikelyOperator();
                $expr->setReadOnly($this->isReadOnly());

                return $expr->compile($expression, $compilationContext);

            case 'typeof':
                $compilableExpression = new TypeOfOperator();
                break;

            case 'require':
                $compilableExpression = new RequireOperator();
                break;

            case 'require_once':
                $compilableExpression = new RequireOnceOperator();
                break;

            case 'closure':
                $compilableExpression = new Closure();
                break;

            case 'closure-arrow':
                $compilableExpression = new ClosureArrow();
                break;

            case 'reference':
                $compilableExpression = new Reference();
                break;

            default:
                throw new CompilerException('Unknown expression: ' . $type, $expression);
        }

        $compilableExpression->setReadOnly($this->isReadOnly());
        $compilableExpression->setExpectReturn($this->expecting, $this->expectingVariable);

        return $compilableExpression->compile($expression, $compilationContext);
    }

    /**
     * Compiles foo = [].
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     */
    public function emptyArray(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        /**
         * Resolves the symbol that expects the value
         */
        if ($this->expecting) {
            if ($this->expectingVariable) {
                $symbolVariable = &$this->expectingVariable;
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

        /**
         * Variable that receives property accesses must be polymorphic
         */
        if (!$symbolVariable->isVariable() && 'array' != $symbolVariable->getType()) {
            throw new CompilerException(
                'Cannot use variable: '
                . $symbolVariable->getName()
                . '('
                . $symbolVariable->getType()
                . ') to create empty array',
                $expression
            );
        }

        /**
         * Mark the variable as an 'array'
         */
        $symbolVariable->setDynamicTypes('array');
        $compilationContext->backend->initArray($symbolVariable, $compilationContext);

        return new CompiledExpression('array', $symbolVariable->getRealName(), $expression);
    }

    /**
     * Returns the variable which is expected to return the
     * result of the expression evaluation.
     *
     * @return Variable|null
     */
    public function getExpectingVariable(): ?Variable
    {
        return $this->expectingVariable;
    }

    /**
     * Returns the original expression.
     *
     * @return array
     */
    public function getExpression(): array
    {
        return $this->expression;
    }

    /**
     * Checks if the returned value by the expression
     * is expected to be assigned to an external symbol.
     *
     * @return bool
     */
    public function isExpectingReturn(): bool
    {
        return $this->expecting;
    }

    /**
     * Checks whether the expression must be resolved in "noisy" mode.
     *
     * @return bool
     */
    public function isNoisy(): bool
    {
        return $this->noisy;
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
     * Sets if the expression is being evaluated in an evaluation like the ones in 'if' and 'while' statements.
     *
     * @param bool $evalMode
     */
    public function setEvalMode(bool $evalMode): void
    {
        $this->evalMode = $evalMode;
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
     * Sets whether the expression must be resolved in "noisy" mode.
     *
     * @param bool $noisy
     */
    public function setNoisy(bool $noisy): void
    {
        $this->noisy = $noisy;
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
