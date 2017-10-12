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

namespace Zephir\Optimizers;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Compiler\CompilerException;

abstract class IsTypeOptimizerAbstract extends OptimizerAbstract
{
    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return bool|CompiledExpression|mixed
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        if (count($expression['parameters']) != 1) {
            return false;
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if ($context->backend->isZE3()) {
            if ($this->getType() == 'IS_BOOL') {
                $condition = '(Z_TYPE_P(' . $resolvedParams[0] . ') == IS_TRUE || Z_TYPE_P(' . $resolvedParams[0] . ') == IS_FALSE)';
            } else {
                $condition = 'Z_TYPE_P(' . $resolvedParams[0] . ') == ' . $this->getType();
            }
        } else {
            $condition = 'Z_TYPE_P(' . $resolvedParams[0] . ') == ' . $this->getType();
        }

        return new CompiledExpression('bool', $condition, $expression);
    }

    abstract protected function getType();
}
