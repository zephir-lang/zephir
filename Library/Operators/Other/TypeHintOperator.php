<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Operators\Other;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\BaseOperator;

/**
 * Zephir\Operators\Other\TypeHintOperator.
 */
class TypeHintOperator extends BaseOperator
{
    private $strict = false;

    /**
     * Sets if the type hint is strict or not.
     *
     * @param $strict
     */
    public function setStrict($strict)
    {
        $this->strict = (bool) $strict;
    }

    /**
     * Performs type-hint compilation.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     *
     * @return CompiledExpression
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $expr = new Expression($expression['right']);
        $expr->setReadOnly(true);
        $resolved = $expr->compile($compilationContext);

        if ('variable' != $resolved->getType()) {
            throw new CompilerException('Type-Hints only can be applied to dynamic variables.', $expression);
        }

        $symbolVariable = $compilationContext->symbolTable->getVariableForRead(
            $resolved->getCode(),
            $compilationContext,
            $expression
        );

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException('Type-Hints only can be applied to dynamic variables.', $expression);
        }

        $symbolVariable->setDynamicTypes('object');
        $symbolVariable->setClassTypes($compilationContext->getFullName($expression['left']['value']));

        return $resolved;
    }
}
