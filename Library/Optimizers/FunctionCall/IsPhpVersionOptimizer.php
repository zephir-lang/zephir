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
use Zephir\Optimizers\OptimizerAbstract;

/**
 * IsPhpVersionOptimizer
 *
 * Checks if PHP has a specific version
 */
class IsPhpVersionOptimizer extends OptimizerAbstract
{
    /**
     *
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            throw new CompilerException("This function requires parameters", $expression);
        }

        if (count($expression['parameters']) != 1) {
            throw new CompilerException("This function only requires one parameter", $expression);
        }

        if ($expression['parameters'][0]['parameter']['type'] != 'string') {
            throw new CompilerException("This function requires a constant string parameter", $expression);
        }

        preg_match('/^([0-9]+)\.?([0-9]+)?\.?([0-9]+)?$/', $expression['parameters'][0]['parameter']['value'], $matches);
        if (!count($matches)) {
            throw new CompilerException("Could not parse PHP version", $expression);
        }

        $major = $matches[1] * 10000;
        if (isset($matches[2])) {
            $minor = $matches[2] * 100;
        } else {
            $minor = 0;
        }

        $version = $major + $minor;

        return new CompiledExpression('bool', 'zephir_is_php_version(' . $version . ')', $expression);
    }
}
