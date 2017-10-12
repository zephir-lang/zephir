<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

use Zephir\Compiler\CompilerException;
use Zephir\Operators\Arithmetical\AddOperator;
use Zephir\Operators\Arithmetical\SubOperator;
use Zephir\Operators\Arithmetical\MulOperator;
use Zephir\Operators\Arithmetical\DivOperator;
use Zephir\Operators\Arithmetical\ModOperator;
use Zephir\Operators\Other\ShortTernaryOperator;
use Zephir\Operators\Unary\MinusOperator;
use Zephir\Operators\Unary\NotOperator;
use Zephir\Operators\Logical\AndOperator;
use Zephir\Operators\Logical\OrOperator;
use Zephir\Operators\Comparison\EqualsOperator;
use Zephir\Operators\Comparison\NotEqualsOperator;
use Zephir\Operators\Comparison\IdenticalOperator;
use Zephir\Operators\Comparison\NotIdenticalOperator;
use Zephir\Operators\Comparison\LessEqualOperator;
use Zephir\Operators\Comparison\LessOperator;
use Zephir\Operators\Comparison\GreaterOperator;
use Zephir\Operators\Comparison\GreaterEqualOperator;
use Zephir\Operators\Bitwise\BitwiseNotOperator;
use Zephir\Operators\Bitwise\BitwiseAndOperator;
use Zephir\Operators\Bitwise\BitwiseOrOperator;
use Zephir\Operators\Bitwise\BitwiseXorOperator;
use Zephir\Operators\Bitwise\ShiftLeftOperator;
use Zephir\Operators\Bitwise\ShiftRightOperator;
use Zephir\Operators\Other\NewInstanceOperator;
use Zephir\Operators\Other\NewInstanceTypeOperator;
use Zephir\Operators\Other\CloneOperator;
use Zephir\Operators\Other\ConcatOperator;
use Zephir\Operators\Other\EmptyOperator;
use Zephir\Operators\Other\IssetOperator;
use Zephir\Operators\Other\FetchOperator;
use Zephir\Operators\Other\LikelyOperator;
use Zephir\Operators\Other\UnlikelyOperator;
use Zephir\Operators\Other\TernaryOperator;
use Zephir\Operators\Other\InstanceOfOperator;
use Zephir\Operators\Other\RequireOperator;
use Zephir\Operators\Other\TypeOfOperator;
use Zephir\Operators\Other\CastOperator;
use Zephir\Operators\Other\RangeInclusiveOperator;
use Zephir\Operators\Other\RangeExclusiveOperator;
use Zephir\Operators\Other\TypeHintOperator;
use Zephir\Expression\Closure;
use Zephir\Expression\ClosureArrow;
use Zephir\Expression\Constants;
use Zephir\Expression\Reference;
use Zephir\Expression\NativeArray;
use Zephir\Expression\NativeArrayAccess;
use Zephir\Expression\PropertyAccess;
use Zephir\Expression\PropertyDynamicAccess;
use Zephir\Expression\StaticConstantAccess;
use Zephir\Expression\StaticPropertyAccess;

/**
 * Expressions
 *
 * Represents an expression. Most language constructs in a language are expressions
 */
class Expression
{
    protected $_expression;

    protected $_expecting = true;

    protected $_readOnly = false;

    protected $_noisy = true;

    protected $_stringOperation = false;

    /**
     * @var Variable
     */
    protected $_expectingVariable;

    protected $_evalMode = false;

    /**
     * Expression constructor
     *
     * @param array $expression
     */
    public function __construct(array $expression)
    {
        $this->_expression = $expression;
    }

    /**
     * Returns the original expression
     *
     * @return array
     */
    public function getExpression()
    {
        return $this->_expression;
    }

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value
     *
     * @param boolean $expecting
     * @param Variable $expectingVariable
     */
    public function setExpectReturn($expecting, Variable $expectingVariable = null)
    {
        $this->_expecting = $expecting;
        $this->_expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only
     *
     * @param boolean $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->_readOnly = $readOnly;
    }

    /**
     * Checks if the result of the evaluated expression is read only
     *
     * @return boolean
     */
    public function isReadOnly()
    {
        return $this->_readOnly;
    }

    /**
     * Checks if the returned value by the expression
     * is expected to be assigned to an external symbol
     *
     * @return boolean
     */
    public function isExpectingReturn()
    {
        return $this->_expecting;
    }

    /**
     * Returns the variable which is expected to return the
     * result of the expression evaluation
     *
     * @return Variable
     */
    public function getExpectingVariable()
    {
        return $this->_expectingVariable;
    }

    /**
     * Sets whether the expression must be resolved in "noisy" mode
     *
     * @param boolean $noisy
     */
    public function setNoisy($noisy)
    {
        $this->_noisy = $noisy;
    }

    /**
     * Checks whether the expression must be resolved in "noisy" mode
     *
     * @return boolean
     */
    public function isNoisy()
    {
        return $this->_noisy;
    }

    /**
     * Sets if current operation is a string operation like "concat"
     * thus avoiding promote numeric strings to longs
     *
     * @param boolean $stringOperation
     */
    public function setStringOperation($stringOperation)
    {
        $this->_stringOperation = $stringOperation;
    }

    /**
     * Checks if the result of the evaluated expression is intended to be used
     * in a string operation like "concat"
     *
     * @return boolean
     */
    public function isStringOperation()
    {
        return $this->_stringOperation;
    }

    /**
     * Sets if the expression is being evaluated in an evaluation like the ones in 'if' and 'while' statements
     *
     * @param boolean $evalMode
     */
    public function setEvalMode($evalMode)
    {
        $this->_evalMode = $evalMode;
    }

    /**
     * Compiles foo = []
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     */
    public function emptyArray($expression, CompilationContext $compilationContext)
    {
        /**
         * Resolves the symbol that expects the value
         */
        if ($this->_expecting) {
            if ($this->_expectingVariable) {
                $symbolVariable = & $this->_expectingVariable;
                $symbolVariable->initVariant($compilationContext);
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
            }
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
        }

        /**
         * Variable that receives property accesses must be polymorphic
         */
        if (!$symbolVariable->isVariable() && $symbolVariable->getType() != 'array') {
            throw new CompilerException("Cannot use variable: " . $symbolVariable->getName() . '(' . $symbolVariable->getType() . ") to create empty array", $expression);
        }

        /**
         * Mark the variable as an 'array'
         */
        $symbolVariable->setDynamicTypes('array');

        $compilationContext->backend->initArray($symbolVariable, $compilationContext);

        return new CompiledExpression('array', $symbolVariable->getRealName(), $expression);
    }

    /**
     * Resolves an expression
     *
     * @param CompilationContext $compilationContext
     * @return bool|CompiledExpression|mixed
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        $expression = $this->_expression;
        $type = $expression['type'];
        $compilableExpression = null;

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
                $v = $expression['value'];
                if (!$this->_stringOperation) {
                    if (ctype_digit($v) && (strlen($v) == 1 || '0' != substr($v, 0, 1))) {
                        return new CompiledExpression('int', $v, $expression);
                    }
                }
                return new LiteralCompiledExpression('string', str_replace(PHP_EOL, '\\n', $v), $expression);
            case 'istring':
                return new LiteralCompiledExpression('istring', str_replace(PHP_EOL, '\\n', $expression['value']), $expression);

            case 'char':
                if (!strlen($expression['value'])) {
                    throw new CompilerException("Invalid empty char literal", $expression);
                }
                if (strlen($expression['value']) > 2) {
                    if (strlen($expression['value']) > 10) {
                        throw new CompilerException("Invalid char literal: '" . substr($expression['value'], 0, 10) . "...'", $expression);
                    } else {
                        throw new CompilerException("Invalid char literal: '" . $expression['value'] . "'", $expression);
                    }
                }
                return new LiteralCompiledExpression('char', $expression['value'], $expression);

            case 'variable':
                $var = $compilationContext->symbolTable->getVariable($expression['value']);
                if ($var) {
                    if ($var->getRealName() == 'this') {
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
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'irange':
                $compilableExpression = new RangeInclusiveOperator();
                break;

            case 'erange':
                $compilableExpression = new RangeExclusiveOperator();
                break;

            case 'list':
                if ($expression['left']['type'] == 'list') {
                    $compilationContext->logger->warning("Unnecessary extra parentheses", "extra-parentheses", $expression);
                }
                $numberPrints = $compilationContext->codePrinter->getNumberPrints();
                $expr = new Expression($expression['left']);
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                $resolved = $expr->compile($compilationContext);
                if (($compilationContext->codePrinter->getNumberPrints() - $numberPrints) <= 1) {
                    if (strpos($resolved->getCode(), ' ') !== false) {
                        return new CompiledExpression($resolved->getType(), '(' . $resolved->getCode() . ')', $expression);
                    }
                }
                return $resolved;

            case 'cast':
                $compilableExpression = new CastOperator();
                break;

            case 'type-hint':
                $expr = new TypeHintOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
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
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'likely':
                if (!$this->_evalMode) {
                    throw new CompilerException("'likely' operator can only be used in evaluation expressions", $expression);
                }
                $expr = new LikelyOperator();
                $expr->setReadOnly($this->isReadOnly());
                return $expr->compile($expression, $compilationContext);

            case 'unlikely':
                if (!$this->_evalMode) {
                    throw new CompilerException("'unlikely' operator can only be used in evaluation expressions", $expression);
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
                throw new CompilerException("Unknown expression: " . $type, $expression);
        }

        if (!$compilableExpression) {
            throw new CompilerException("Unknown expression passed as compilableExpression", $expression);
        }
        $compilableExpression->setReadOnly($this->isReadOnly());
        $compilableExpression->setExpectReturn($this->_expecting, $this->_expectingVariable);
        return $compilableExpression->compile($expression, $compilationContext);
    }
}
