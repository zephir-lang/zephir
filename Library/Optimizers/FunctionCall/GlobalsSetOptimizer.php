<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * Zephir\Optimizers\FunctionCall\GlobalsSetOptimizer.
 *
 * Writes values from extensions globals
 */
class GlobalsSetOptimizer extends OptimizerAbstract
{
    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @throws CompilerException
     *
     * @return CompiledExpression
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            throw new CompilerException("'globals_set' requires two parameters", $expression);
        }

        if (2 != \count($expression['parameters'])) {
            throw new CompilerException("'globals_set' only accepts two parameters", $expression);
        }

        if ('string' != $expression['parameters'][0]['parameter']['type']) {
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
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

            if (!isset($resolvedParams[0]) || empty($resolvedParams[0]) || !\is_string($resolvedParams[0])) {
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

            $internalAccessor = $this->resolveInternalAccessor($globalName);
            $internalValue = $this->resolveInternalValue($globalDefinition, $expression, $globalName, $resolvedParams[0]);

            $context->codePrinter->output("{$internalAccessor} = {$internalValue};");

            return new CompiledExpression('null', null, $expression);
        } catch (Exception $e) {
            throw new CompilerException($e->getMessage(), $expression);
        }
    }

    private function resolveInternalAccessor($globalName)
    {
        $parts = explode('.', $globalName);

        if (isset($parts[1])) {
            return strtr('ZEPHIR_GLOBAL(:v).:parts', [':v' => $parts[0], ':parts' => $parts[1]]);
        }

        return strtr('ZEPHIR_GLOBAL(:v)', [':v' => $parts[0]]);
    }

    /**
     * TODO:  Add 'hash' support
     * TODO:  Use zval_get_string, zval_get_long, zval_get_double for ZE3
     *
     * @param array  $definition
     * @param array  $expression
     * @param string $name
     * @param string $value
     *
     * @return string
     */
    private function resolveInternalValue(array $definition, array $expression, $name, $value)
    {
        $type = $definition['type'];

        switch ($type) {
            case 'boolean':
            case 'bool':
                return strtr('zend_is_true(:v)', [':v' => $value]);
            case 'int':
            case 'uint':
            case 'integer':
            case 'long':
            case 'ulong':
                // TODO: Use zval_get_long when we'll drop Zend Engine 2
                return strtr('Z_LVAL_P(:v)', [':v' => $value]);
            case 'string':
                // TODO: Use zval_get_string when we'll drop Zend Engine 2
                return strtr('Z_STRVAL_P(:v)', [':v' => $value]);
            case 'char':
            case 'uchar':
                // TODO: Use zval_get_string and zval_get_long when we'll drop Zend Engine 2
                return strtr(
                    '(Z_TYPE_P(:v) == IS_STRING ? (Z_STRLEN_P(:v) ? Z_STRVAL_P(:v)[0] : NULL) : Z_LVAL_P(:v))',
                    [':v' => $value]
                );
            case 'double':
            case 'float':
                // TODO: Use zval_get_double when we'll drop Zend Engine 2
                return strtr('Z_DVAL_P(:v)', [':v' => $value]);
            default:
                throw new CompilerException(
                    "Unknown type '{$type}' to setting global variable '{$name}'.",
                    $expression
                );
        }
    }
}
