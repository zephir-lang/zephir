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
 * GlobalsGetOptimizer
 *
 * Reads values from extensions globals
 */
class GlobalsGetOptimizer extends OptimizerAbstract
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
            throw new CompilerException("'globals_get' only accepts one parameter", $expression);
        }

        if ($expression['parameters'][0]['parameter']['type'] != 'string') {
            throw new CompilerException("A string parameter is required for 'globals_get'", $expression);
        }

        $globalName = $expression['parameters'][0]['parameter']['value'];

        if (!$context->compiler->isExtensionGlobal($globalName)) {
            throw new CompilerException("Global '" . $globalName . "' cannot be read because it isn't defined", $expression);
        }

        $globalDefinition = $context->compiler->getExtensionGlobal($globalName);

        if (strpos($globalName, '.') !== false) {
            $parts = explode('.', $globalName);
            return new CompiledExpression($globalDefinition['type'], 'ZEPHIR_GLOBAL(' . $parts[0] . ').' . $parts[1], $expression);
        }

        return new CompiledExpression($globalDefinition['type'], 'ZEPHIR_GLOBAL(' . $globalName . ')', $expression);
    }
}
