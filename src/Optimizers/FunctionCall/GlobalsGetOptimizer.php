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

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;
use Zephir\Types\Types;

use function count;
use function explode;
use function str_contains;

/**
 * `globals_get()` internal function.
 *
 * Reads values from extensions globals
 */
class GlobalsGetOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return CompiledExpression|null
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context): ?CompiledExpression
    {
        if (!isset($expression['parameters'])) {
            return null;
        }

        if (1 !== count($expression['parameters'])) {
            throw new CompilerException("'globals_get' only accepts one parameter", $expression);
        }

        if ('string' !== $expression['parameters'][0]['parameter']['type']) {
            throw new CompilerException("A string parameter is required for 'globals_get'", $expression);
        }

        $globalName = $expression['parameters'][0]['parameter']['value'];

        if (!$context->compiler->isExtensionGlobal($globalName)) {
            throw new CompilerException(
                "Global '" . $globalName . "' cannot be read because it isn't defined",
                $expression
            );
        }

        $type     = $context->compiler->getExtensionGlobal($globalName)['type'];
        $accessor = $this->resolveInternalAccessor($globalName);

        if (Types::T_STRING !== $type) {
            return new CompiledExpression($type, $accessor, $expression);
        }

        /**
         * Everywhere else in the compiler a `string` compiled expression holds
         * literal text, so the member's name was emitted as the value: a
         * `let`, a concatenation or an array element got "ZEPHIR_GLOBAL(x)".
         * Copy the member into a zval instead (#2725).
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);
        $this->checkInitSymbolVariable($call, $symbolVariable, $context);

        $symbolVariable->setDynamicTypes('string');

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output('ZVAL_STRING(' . $symbol . ', ' . $accessor . ');');

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    private function resolveInternalAccessor(string $globalName): string
    {
        if (str_contains($globalName, '.')) {
            $parts = explode('.', $globalName);

            return 'ZEPHIR_GLOBAL(' . $parts[0] . ').' . $parts[1];
        }

        return 'ZEPHIR_GLOBAL(' . $globalName . ')';
    }
}
