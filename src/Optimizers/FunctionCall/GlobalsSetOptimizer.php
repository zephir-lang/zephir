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
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Exception\InvalidArgumentException;
use Zephir\Optimizers\OptimizerAbstract;
use Zephir\Types\Types;

use function count;
use function explode;
use function is_string;
use function strtr;

/**
 * `globals_set()` internal function.
 *
 * Writes values from extensions globals.
 */
class GlobalsSetOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return CompiledExpression
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context): CompiledExpression
    {
        if (!isset($expression['parameters'])) {
            throw new CompilerException("'globals_set' requires two parameters", $expression);
        }

        if (2 !== count($expression['parameters'])) {
            throw new CompilerException("'globals_set' only accepts two parameters", $expression);
        }

        if ('string' !== $expression['parameters'][0]['parameter']['type']) {
            throw new CompilerException("A string parameter is required for 'globals_set'", $expression);
        }

        $globalName = $expression['parameters'][0]['parameter']['value'];

        if (!$context->compiler->isExtensionGlobal($globalName)) {
            throw new CompilerException(
                "Global variable '{$globalName}' cannot be written because it wasn't defined",
                $expression
            );
        }

        unset($expression['parameters'][0]);

        try {
            $globalDefinition = $context->compiler->getExtensionGlobal($globalName);
            $resolvedParams   = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

            if (!isset($resolvedParams[0]) || empty($resolvedParams[0]) || !is_string($resolvedParams[0])) {
                throw new CompilerException(
                    "Unable to reslove value for '{$globalName}' global variable.",
                    $expression
                );
            }

            if (!isset($globalDefinition['type'])) {
                throw new CompilerException(
                    "Unable to determine type for '{$globalName}' global variable.",
                    $expression
                );
            }

            $this->write($context, $expression, $globalName, $globalDefinition, $resolvedParams[0]);

            return new CompiledExpression('null', null, $expression);
        } catch (Exception | InvalidArgumentException $e) {
            throw new CompilerException($e->getMessage(), $expression);
        }
    }

    private function resolveInternalAccessor(string $globalName): string
    {
        $parts = explode('.', $globalName);

        if (isset($parts[1])) {
            return strtr('ZEPHIR_GLOBAL(:v).:parts', [':v' => $parts[0], ':parts' => $parts[1]]);
        }

        return strtr('ZEPHIR_GLOBAL(:v)', [':v' => $parts[0]]);
    }

    /**
     * Emits the write of `$value` (a `zval *`) to the global.
     *
     * Every conversion but the boolean one can throw: `(string)` on an object
     * without __toString(), and any of them through an error handler that
     * turns the "could not be converted" warning into an exception. PHP's cast
     * produces no value then, so the global is left alone and execution stops.
     *
     * TODO:  Add 'hash' support
     *
     * @throws CompilerException
     * @throws InvalidArgumentException
     */
    private function write(
        CompilationContext $context,
        array $expression,
        string $name,
        array $definition,
        string $value
    ): void {
        $type     = $definition['type'];
        $accessor = $this->resolveInternalAccessor($name);

        switch ($type) {
            case Types::T_BOOLEAN:
            case Types::T_BOOL:
                $context->codePrinter->output($accessor . ' = zend_is_true(' . $value . ');');
                break;

            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_INTEGER:
            case Types::T_LONG:
            case Types::T_ULONG:
                $this->writeConverted($context, $accessor, 'long', 'zval_get_long(' . $value . ')');
                break;

            case Types::T_STRING:
                /**
                 * The member may point into the ini entry, which the engine
                 * owns, so the kernel writes a copy the extension owns instead.
                 */
                $owned = (new ExtensionGlobal($name, $definition))->ownedMember();
                $context->codePrinter->output(
                    'zephir_globals_set_string(&' . $accessor . ', &'
                    . $this->resolveInternalAccessor($owned) . ', ' . $value . ');'
                );
                $context->emitExceptionCheck();
                break;

            case Types::T_CHAR:
            case Types::T_UCHAR:
                $this->writeConverted(
                    $context,
                    $accessor,
                    'char',
                    strtr(
                        '(Z_TYPE_P(:v) == IS_STRING ? (Z_STRLEN_P(:v) ? Z_STRVAL_P(:v)[0] : (char)0) : (char) zval_get_long(:v))',
                        [':v' => $value]
                    )
                );
                break;

            case Types::T_DOUBLE:
            case Types::T_FLOAT:
                $this->writeConverted($context, $accessor, 'double', 'zval_get_double(' . $value . ')');
                break;

            default:
                throw new CompilerException(
                    "Unknown type '$type' to setting global variable '$name'.",
                    $expression
                );
        }
    }

    /**
     * Converts into a temp first, so a conversion that threw never reaches the
     * global.
     */
    private function writeConverted(CompilationContext $context, string $accessor, string $tempType, string $code): void
    {
        $temp = $context->symbolTable->getTempVariableForWrite($tempType, $context);

        $context->codePrinter->output($temp->getName() . ' = ' . $code . ';');
        $context->emitExceptionCheck();
        $context->codePrinter->output($accessor . ' = ' . $temp->getName() . ';');
    }
}
