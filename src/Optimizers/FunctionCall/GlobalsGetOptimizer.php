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
use Zephir\Code\Builder\ExtensionGlobal;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Exception\InvalidArgumentException;
use Zephir\Optimizers\OptimizerAbstract;
use Zephir\Types\Types;

use function count;

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

        $definition = $context->compiler->getExtensionGlobal($globalName);
        $type       = $definition['type'];
        $accessor   = ExtensionGlobal::accessorOf($globalName);

        if (Types::T_HASH === $type) {
            return $this->readHash($globalName, $definition, $accessor, $expression, $call, $context);
        }

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

    /**
     * A `hash` compiled expression could not be boxed by anything, so a hash
     * global was readable from C only. It is read into a zval instead: null
     * while the member is NULL, otherwise the table, shared copy-on-write.
     * Being nullable, it can only be received by a variant (#2726).
     *
     * @throws CompilerException
     */
    private function readHash(
        string $globalName,
        array $definition,
        string $accessor,
        array $expression,
        Call $call,
        CompilationContext $context
    ): CompiledExpression {
        try {
            $global = new ExtensionGlobal($globalName, $definition);
        } catch (InvalidArgumentException $e) {
            /* The classes are compiled first, so this is where a bad definition shows up. */
            throw new CompilerException($e->getMessage(), $expression, $e->getCode(), $e);
        }

        if ($global->isModule()) {
            throw CompilerException::moduleHashGlobal($globalName, $expression);
        }

        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariable($symbolVariable, $expression);
        $this->checkInitSymbolVariable($call, $symbolVariable, $context);

        $symbolVariable->setDynamicTypes(['array', 'null']);

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output('zephir_globals_get_hash(' . $symbol . ', ' . $accessor . ');');

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
