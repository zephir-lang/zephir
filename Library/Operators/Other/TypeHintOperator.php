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

namespace Zephir\Operators\Other;

use Zephir\Expression;
use Zephir\CompiledExpression;
use Zephir\CompilationContext;
use Zephir\Operators\BaseOperator;
use Zephir\Compiler\CompilerException;

/**
 * Zephir\Operators\Other\TypeHintOperator
 *
 * @package Zephir\Operators\Other
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
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $expr = new Expression($expression['right']);
        $expr->setReadOnly(true);
        $resolved = $expr->compile($compilationContext);

        if ($resolved->getType() != 'variable') {
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
