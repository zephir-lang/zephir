<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

use Zephir\Operators\Arithmetical\AddOperator;
use Zephir\Operators\Arithmetical\SubOperator;
use Zephir\Operators\Arithmetical\MulOperator;
use Zephir\Operators\Arithmetical\DivOperator;
use Zephir\Operators\Arithmetical\ModOperator;
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

        $compilationContext->codePrinter->output('array_init(' . $symbolVariable->getName() . ');');

        return new CompiledExpression('array', $symbolVariable->getRealName(), $expression);
    }

    /**
     *
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     */
    public function compileTypeHint($expression, CompilationContext $compilationContext)
    {
        $expr = new Expression($expression['right']);
        $expr->setReadOnly(true);
        $resolved = $expr->compile($compilationContext);

        if ($resolved->getType() != 'variable') {
            throw new CompilerException("Type-Hints only can be applied to dynamic variables", $expression);
        }

        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException("Type-Hints only can be applied to dynamic variables", $expression);
        }

        $symbolVariable->setDynamicTypes('object');
        $symbolVariable->setClassTypes($compilationContext->getFullName($expression['left']['value']));

        return $resolved;
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

        switch ($type) {

            case 'null':
                return new LiteralCompiledExpression('null', null, $expression);

            case 'int':
            case 'integer':
                return new LiteralCompiledExpression('int', $expression['value'], $expression);

            case 'double':
                return new LiteralCompiledExpression('double', $expression['value'], $expression);

            case 'bool':
                return new LiteralCompiledExpression('bool', $expression['value'], $expression);

            case 'string':
                if (!$this->_stringOperation) {
                    if (ctype_digit($expression['value'])) {
                        return new CompiledExpression('int', $expression['value'], $expression);
                    }
                }
                return new LiteralCompiledExpression('string', str_replace(PHP_EOL, '\\n', $expression['value']), $expression);
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
                return new CompiledExpression('variable', $expression['value'], $expression);

            case 'constant':
                $constant = new Constants();
                $constant->setReadOnly($this->isReadOnly());
                $constant->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $constant->compile($expression, $compilationContext);

            case 'empty-array':
                return $this->emptyArray($expression, $compilationContext);

            case 'array-access':
                $arrayAccess = new NativeArrayAccess();
                $arrayAccess->setReadOnly($this->isReadOnly());
                $arrayAccess->setNoisy($this->isNoisy());
                $arrayAccess->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $arrayAccess->compile($expression, $compilationContext);

            case 'property-access':
                $propertyAccess = new PropertyAccess();
                $propertyAccess->setReadOnly($this->isReadOnly());
                $propertyAccess->setNoisy($this->isNoisy());
                $propertyAccess->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $propertyAccess->compile($expression, $compilationContext);

            case 'property-string-access':
            case 'property-dynamic-access':
                $propertyAccess = new PropertyDynamicAccess();
                $propertyAccess->setReadOnly($this->isReadOnly());
                $propertyAccess->setNoisy($this->isNoisy());
                $propertyAccess->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $propertyAccess->compile($expression, $compilationContext);

            case 'static-constant-access':
                $staticConstantAccess = new StaticConstantAccess();
                $staticConstantAccess->setReadOnly($this->isReadOnly());
                $staticConstantAccess->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $staticConstantAccess->compile($expression, $compilationContext);

            case 'static-property-access':
                $staticPropertyAccess = new StaticPropertyAccess();
                $staticPropertyAccess->setReadOnly($this->isReadOnly());
                $staticPropertyAccess->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $staticPropertyAccess->compile($expression, $compilationContext);

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
                $expr = new IssetOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'fetch':
                $expr = new FetchOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'empty':
                $expr = new EmptyOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'array':
                $array = new NativeArray();
                $array->setReadOnly($this->isReadOnly());
                $array->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $array->compile($expression, $compilationContext);

            case 'new':
                $expr = new NewInstanceOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'new-type':
                $expr = new NewInstanceTypeOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'not':
                $expr = new NotOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'equals':
                $expr = new EqualsOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'not-equals':
                $expr = new NotEqualsOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'identical':
                $expr = new IdenticalOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'not-identical':
                $expr = new NotIdenticalOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'greater':
                $expr = new GreaterOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'less':
                $expr = new LessOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'less-equal':
                $expr = new LessEqualOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'greater-equal':
                $expr = new GreaterEqualOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'add':
                $expr = new AddOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'minus':
                $expr = new MinusOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'sub':
                $expr = new SubOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'mul':
                $expr = new MulOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'div':
                $expr = new DivOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'mod':
                $expr = new ModOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'and':
                $expr = new AndOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'or':
                $expr = new OrOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'bitwise_and':
                $expr = new BitwiseAndOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'bitwise_or':
                $expr = new BitwiseOrOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'bitwise_xor':
                $expr = new BitwiseXorOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'bitwise_shiftleft':
                $expr = new ShiftLeftOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'bitwise_shiftright':
                $expr = new ShiftRightOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'concat':
                $expr = new ConcatOperator();
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'irange':
                $expr = new RangeInclusiveOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'erange':
                $expr = new RangeExclusiveOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

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
                $expr = new CastOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'type-hint':
                return $this->compileTypeHint($expression, $compilationContext);

            case 'instanceof':
                $expr = new InstanceOfOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'clone':
                $expr = new CloneOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'ternary':
                $expr = new TernaryOperator();
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
                $expr = new TypeOfOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'require':
                $expr = new RequireOperator();
                $expr->setReadOnly($this->isReadOnly());
                $expr->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $expr->compile($expression, $compilationContext);

            case 'closure':
                $closure = new Closure();
                $closure->setReadOnly($this->isReadOnly());
                $closure->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $closure->compile($expression, $compilationContext);

            case 'closure-arrow':
                $closure = new ClosureArrow();
                $closure->setReadOnly($this->isReadOnly());
                $closure->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $closure->compile($expression, $compilationContext);

            case 'reference':
                $reference = new Reference();
                $reference->setReadOnly($this->isReadOnly());
                $reference->setExpectReturn($this->_expecting, $this->_expectingVariable);
                return $reference->compile($expression, $compilationContext);

            default:
                throw new CompilerException("Unknown expression: " . $type, $expression);
        }
    }
}
