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
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;

/**
 * Checks if a variable is empty string or null
 */
class EmptyOperator extends AbstractOperator
{
    /**
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $compilationContext->headersManager->add('kernel/operators');

        if (!isset($expression['left'])) {
            throw new CompilerException("Invalid 'left' operand for 'empty' expression", $expression['left']);
        }

        /**
         * A subscript operand is not `empty(read($c, $i))` in PHP: it has its
         * own handler, `zend_isempty_dim_slow`, which reports nothing for a
         * missing key, an out-of-range string offset or an illegal offset
         * type, and calls all of them empty. Composing it out of a read would
         * emit diagnostics PHP does not and answer differently for an offset
         * the read path rejects.
         */
        if ('array-access' === $expression['left']['type']) {
            return $this->compileDimension($expression['left'], $compilationContext);
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly(true);
        $leftExpr->setNoisy(false);

        $left = $leftExpr->compile($compilationContext);

        if ('variable' != $left->getType() && 'array' != $left->getType()) {
            throw new CompilerException("'empty' operand only can be a variable", $expression['left']);
        }

        $variableLeft = $compilationContext->symbolTable->getVariableForRead(
            $left->getCode(),
            $compilationContext,
            $expression['left']
        );
        if (!$variableLeft->isVariable() && !$variableLeft->isString() && !$variableLeft->isArray()) {
            throw new CompilerException(
                "Only dynamic/string variables can be used in 'empty' operators",
                $expression['left']
            );
        }

        return new CompiledExpression(
            'bool',
            'ZEPHIR_IS_EMPTY(' . $compilationContext->backend->getVariableCode($variableLeft) . ')',
            $expression
        );
    }

    /**
     * Compiles `empty(container[offset])` onto PHP's dedicated silent handler.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function compileDimension(
        array $arrayAccess,
        CompilationContext $compilationContext
    ): CompiledExpression {
        $compilationContext->headersManager->add('kernel/array');

        $containerExpr = new Expression($arrayAccess['left']);
        $containerExpr->setReadOnly(true);
        $containerExpr->setNoisy(false);
        $container = $containerExpr->compile($compilationContext);

        if ('variable' !== $container->getType()) {
            throw new CompilerException(
                "'empty' operand only can be a variable",
                $arrayAccess['left']
            );
        }

        $containerVariable = $compilationContext->symbolTable->getVariableForRead(
            $container->getCode(),
            $compilationContext,
            $arrayAccess['left']
        );

        if (
            !$containerVariable->isVariable()
            && !$containerVariable->isString()
            && !$containerVariable->isArray()
        ) {
            throw new CompilerException(
                "Only dynamic/string variables can be used in 'empty' operators",
                $arrayAccess['left']
            );
        }

        $containerCode = $compilationContext->backend->getVariableCode($containerVariable);

        $offsetExpr = new Expression($arrayAccess['right']);
        $offsetExpr->setReadOnly(true);
        $offsetExpr->setNoisy(false);
        $offset = $offsetExpr->compile($compilationContext);

        switch ($offset->getType()) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                $code = sprintf('zephir_isempty_dim_long(%s, %s)', $containerCode, $offset->getCode());
                break;

            case 'string':
            case 'istring':
                $code = sprintf(
                    'zephir_isempty_dim_string(%s, SL("%s"))',
                    $containerCode,
                    $offset->getCode()
                );
                break;

            case 'variable':
            case 'mixed':
                $offsetVariable = $compilationContext->symbolTable->getVariableForRead(
                    $offset->getCode(),
                    $compilationContext,
                    $arrayAccess['right']
                );

                $code = in_array($offsetVariable->getType(), ['int', 'uint', 'long', 'ulong'], true)
                    ? sprintf('zephir_isempty_dim_long(%s, %s)', $containerCode, $offsetVariable->getName())
                    : sprintf(
                        'zephir_isempty_dim(%s, %s)',
                        $containerCode,
                        $compilationContext->backend->getVariableCode($offsetVariable)
                    );
                break;

            default:
                throw new CompilerException(
                    'Cannot use index type ' . $offset->getType() . " in 'empty'",
                    $arrayAccess['right']
                );
        }

        return new CompiledExpression('bool', $code, $arrayAccess);
    }
}
