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

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\MathOptimizer;

/**
 * LdexpOptimizer
 *
 * Optimizes calls to 'ldexp' using internal function
 */
class LdexpOptimizer extends MathOptimizer
{
    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getFunctionName()
    {
        return 'ldexp';
    }

    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return bool|CompiledExpression|mixed
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters']) || count($expression['parameters']) != 2) {
            throw new CompilerException(sprintf("'%s' requires two parameters", $this->getFunctionName()), $expression);
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $context->headersManager->add('kernel/math');
        return new CompiledExpression(
            'double',
            'zephir_' . $this->getFunctionName() . '(' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ' TSRMLS_CC)',
            $expression
        );
    }
}
