<?php

/**
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
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
