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
 * GlobalsSetOptimizer
 *
 * Writes values from extensions globals
 */
class GlobalsSetOptimizer extends OptimizerAbstract
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
            throw new CompilerException("'globals_set' requires two parameters", $expression);
        }

        if (count($expression['parameters']) != 2) {
            throw new CompilerException("'globals_set' only accepts two parameters", $expression);
        }

        if ($expression['parameters'][0]['parameter']['type'] != 'string') {
            throw new CompilerException("A string parameter is required for 'globals_set'", $expression);
        }

        $globalName = $expression['parameters'][0]['parameter']['value'];

        if (!$context->compiler->isExtensionGlobal($globalName)) {
            throw new CompilerException("Global '" . $globalName . "' cannot be written because it wasn't defined", $expression);
        }

        unset($expression['parameters'][0]);

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $globalDefinition = $context->compiler->getExtensionGlobal($globalName);

        if (strpos($globalName, '.') !== false) {
            $parts = explode('.', $globalName);
            $context->codePrinter->output('ZEPHIR_GLOBAL(' . $parts[0] . ').' . $parts[1] . ' = zend_is_true(' . $resolvedParams[0] . ');');
        }

        return new CompiledExpression('null', null, $expression);
    }
}
