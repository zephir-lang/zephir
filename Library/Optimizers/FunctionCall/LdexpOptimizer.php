<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2016 Zephir Team and contributors                     |
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

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Expression;
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
