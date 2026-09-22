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

namespace Zephir\Statements\Let;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Traits\VariablesTrait;
use Zephir\Variable\Variable;

/**
 * Updates static properties
 */
class StaticProperty extends StaticPropertySub
{
    use VariablesTrait;

    protected string $methodName = 'updateStaticProperty';

    /**
     * @throws ReflectionException
     */
    protected function processDefaultType(
        CompilationContext $compilationContext,
        array $statement,
        string $classEntry,
        string $property,
        Variable $variableVariable
    ): void {
        switch ($variableVariable->getType()) {
            case 'string':
                switch ($statement['operator']) {
                    case 'assign':
                        $compilationContext->backend->updateStaticProperty(
                            $classEntry,
                            $property,
                            $variableVariable,
                            $compilationContext
                        );
                        $this->checkVariableTemporal($variableVariable);
                        break;
                    default:
                        throw CompilerException::illegalOperationTypeOnStaticVariable(
                            $statement['operator'],
                            $variableVariable->getType(),
                            $statement
                        );
                }
                break;
            case 'variable':
            case 'array':
                $compilationContext->backend->updateStaticProperty(
                    $classEntry,
                    $property,
                    $variableVariable,
                    $compilationContext
                );
                $this->checkVariableTemporal($variableVariable);
                break;

            default:
                throw new CompilerException(
                    'Unknown type ' . $variableVariable->getType(),
                    $statement
                );
        }
    }

    protected function processStringType(
        CompiledExpression $resolvedExpr,
        CompilationContext $compilationContext,
        array $statement
    ): Variable {
        // TODO: add-assign
        if ('assign' !== $statement['operator']) {
            throw CompilerException::illegalOperationTypeOnStaticVariable(
                $statement['operator'],
                $resolvedExpr->getType(),
                $statement
            );
        }

        return $compilationContext->symbolTable->getTempNonTrackedVariable(
            'variable',
            $compilationContext,
            true
        );
    }

    protected function processVariableDoubleType(
        array $statement,
        Variable $variableVariable
    ): void {
        // TODO: mul-assign, div-assign, sub-assign, add-assign
        if ('assign' !== $statement['operator']) {
            throw CompilerException::illegalOperationTypeOnStaticVariable(
                $statement['operator'],
                $variableVariable->getType(),
                $statement
            );
        }
    }

    protected function processVariableIntType(
        array $statement,
        Variable $variableVariable
    ): void {
        // TODO: mul-assign, div-assign, sub-assign, add-assign
        if ('assign' !== $statement['operator']) {
            throw CompilerException::illegalOperationTypeOnStaticVariable(
                $statement['operator'],
                $variableVariable->getType(),
                $statement
            );
        }
    }
}
