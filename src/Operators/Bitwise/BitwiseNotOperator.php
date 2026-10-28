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

namespace Zephir\Operators\Bitwise;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;
use Zephir\Types\Types;

/**
 * Generates PHP's `~`.
 *
 * An integer stays a C expression and a double is converted the way the
 * engine converts it, with its deprecation. Anything else goes to PHP's
 * bitwise_not_function(): it inverts a string byte by byte, lets an
 * overloaded object such as GMP answer and throws the TypeError for a bool,
 * null or an array.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2753
 */
class BitwiseNotOperator extends AbstractOperator
{
    /**
     * bitwise_not_function() yields an int, a string from a string, or an
     * object from an overloaded operand.
     */
    private const RESULT_TYPES = [Types::T_LONG, Types::T_STRING, Types::T_OBJECT];

    protected string $operator = '~';

    /**
     * @param                    $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile($expression, CompilationContext $compilationContext)
    {
        $this->checkLeft($expression, CompilerException::class, $expression);

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->readOnly);
        $left = $leftExpr->compile($compilationContext);

        $operand = $this->operand($left, $expression, $compilationContext);
        if (self::KIND_ZVAL === $operand['kind'] || isset($operand['bool'])) {
            return $this->zvalNot($this->boxedOperand($operand, $compilationContext), $expression, $compilationContext);
        }

        return new CompiledExpression(
            'int',
            '~(' . $this->integerOperandCode($operand, $compilationContext) . ')',
            $expression
        );
    }

    /**
     * @throws CompilerException
     */
    private function zvalNot(array $operand, array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $expected     = $this->getExpected($compilationContext, $expression);
        $expectedCode = $compilationContext->backend->getVariableCode($expected);

        $compilationContext->codePrinter->output(
            'bitwise_not_function(' . $expectedCode . ', ' . $operand['code'] . ');'
        );
        $compilationContext->emitExceptionCheck();

        if (null !== $operand['variable']) {
            $this->checkVariableTemporal($operand['variable']);
        }

        $expected->setDynamicTypes(self::RESULT_TYPES);

        return new CompiledExpression('variable', $expected->getName(), $expression);
    }
}
