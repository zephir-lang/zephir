<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Backend;

use Zephir\CompilationContext as Context;
use Zephir\Exception\CompilerException as Exception;
use Zephir\Types;
use Zephir\Variable;
use function Zephir\Backend\ZendEngine3\add_slashes;

class VariablesManager
{
    public const RESERVED_NAMES = [
        'this_ptr',
        'return_value',
        'return_value_ptr',
    ];

    /**
     * Initialize variable defaults.
     *
     * Meant for Backend::initializeVariableDefaults.
     * Shouldn't called directly.
     *
     * TODO: Figure out why it never reached during generation.
     *
     * @param Variable $variable
     * @param array    $value
     * @param Context  $context
     *
     * @return void
     */
    public function initializeDefaults(Variable $variable, array $value, Context $context): void
    {
        switch ($variable->getType()) {
            case Types::T_VARIABLE:
                $this->initDynamicVar($variable, $value, $context);
                break;
            case Types::T_STRING:
                $this->initStringVar($variable, $value, $context);
                break;
            case Types::T_ARRAY:
                $this->initArrayVar($variable, $value, $context);
                break;
        }
    }

    /**
     * Initialize 'dynamic' variables with default values.
     * Meant for VariablesManager::initializeDefaults.
     *
     * @param Variable $variable
     * @param array    $value
     * @param Context  $context
     *
     * @return void
     */
    private function initDynamicVar(Variable $variable, array $value, Context $context): void
    {
        /* These are system variables, do not add default values.
           Also see: https://github.com/zephir-lang/zephir/issues/1660 */
        if (\in_array($variable->getName(), self::RESERVED_NAMES, true)) {
            return;
        }

        $context->symbolTable->mustGrownStack(true);
        $context->backend->initVar($variable, $context);

        switch ($value['type']) {
            case Types::T_INT:
            case Types::T_UINT:
            case Types::T_LONG:
            case Types::T_ULONG:
                $context->backend->assignLong($variable, $value['value'], $context);
                break;

            case Types::T_BOOL:
                $context->backend->assignBool($variable, $value['value'], $context);
                break;

            case Types::T_CHAR:
            case Types::T_UCHAR:
                $this->validateCharValue($value);
                $context->backend->assignLong($variable, "'".$value['value']."'", $context);
                break;

            case Types::T_NULL:
                $context->backend->assignNull($variable, $context);
                break;

            case Types::T_DOUBLE:
                $context->backend->assignDouble($variable, $value['value'], $context);
                break;

            case Types::T_STRING:
                $string = add_slashes($value['value']);
                $context->backend->assignString($variable, $string, $context);
                break;

            case Types::T_ARRAY:
            case 'empty-array':
                $context->backend->initArray($variable, $context);
                break;

            default:
                throw $this->invalidDefaultTypeException($variable, $value);
        }
    }

    /**
     * Initialize 'string' variables with default values.
     * Meant for VariablesManager::initializeDefaults.
     *
     * @param Variable $variable
     * @param array    $value
     * @param Context  $context
     *
     * @return void
     */
    private function initStringVar(Variable $variable, array $value, Context $context): void
    {
        $context->symbolTable->mustGrownStack(true);
        $context->backend->initVar($variable, $context);

        switch ($value['type']) {
            case Types::T_STRING:
                $string = add_slashes($value['value']);
                $context->backend->assignString($variable, $string, $context);
                break;

            case Types::T_NULL:
                $context->backend->assignString($variable, null, $context);
                break;

            default:
                throw $this->invalidDefaultTypeException($variable, $value);
        }
    }

    /**
     * Initialize 'array' variables with default values.
     * Meant for VariablesManager::initializeDefaults.
     *
     * @param Variable $variable
     * @param array    $value
     * @param Context  $context
     *
     * @return void
     */
    private function initArrayVar(Variable $variable, array $value, Context $context): void
    {
        $context->symbolTable->mustGrownStack(true);
        $context->backend->initVar($variable, $context);

        switch ($value['type']) {
            case Types::T_NULL:
                $context->backend->assignNull($variable, $context);
                break;

            case Types::T_ARRAY:
            case 'empty-array':
                $context->backend->initArray($variable, $context);
                break;

            default:
                throw $this->invalidDefaultTypeException($variable, $value);
        }
    }

    /**
     * Create a compiler exception with 'Invalid default type' message.
     *
     * @param Variable $variable
     * @param array    $value
     *
     * @return Exception
     */
    private function invalidDefaultTypeException(Variable $variable, array $value): Exception
    {
        new Exception(
            sprintf(
                'Invalid default type: %s for data type: %s',
                $value['type'],
                $variable->getType()
            ),
            $variable->getOriginal()
        );
    }

    /**
     * Validate 'char' value type.
     *
     * @param array $value
     *
     * @throws Exception
     *
     * @return void
     */
    private function validateCharValue(array $value): void
    {
        if (\strlen($value['value']) > 2) {
            throw new Exception(
                sprintf(
                    "Invalid char literal: '%s%s'",
                    substr($value['value'], 0, 10),
                    \strlen($value['value']) > 10 ? '...' : ''
                ),
                $value
            );
        }
    }
}
