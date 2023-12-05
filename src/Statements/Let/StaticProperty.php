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
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Traits\VariablesTrait;
use \Zephir\Variable\Variable;

use function sprintf;

/**
 * Updates static properties
 */
class StaticProperty extends StaticPropertySub
{
    use VariablesTrait;

    protected string $methodName = 'updateStaticProperty';

    /**
     * @param CompilationContext $compilationContext
     * @param array              $statement
     * @param string             $classEntry
     * @param string             $property
     * @param Variable           $variableVariable
     *
     * @return void
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
                    /* @noinspection PhpMissingBreakStatementInspection */
                    case 'concat-assign':
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify(
                            'variable',
                            $compilationContext
                        );
                        $expression   = new Expression([
                            'type'  => 'static-property-access',
                            'left'  => [
                                'value' => $statement['variable'],
                            ],
                            'right' => [
                                'value' => $statement['property'],
                            ],
                        ]);
                        $expression->setExpectReturn(true, $tempVariable);

                        try {
                            $expression->compile($compilationContext);
                        } catch (Exception $e) {
                            throw new CompilerException($e->getMessage(), $statement, $e->getCode(), $e);
                        }

                        $variableVariableCode = $compilationContext->backend->getVariableCode(
                            $variableVariable
                        );
                        $tempVariableCode     = $compilationContext->backend->getVariableCode($tempVariable);
                        $compilationContext->codePrinter->output(
                            'SEPARATE_ZVAL(' . $variableVariableCode . ');'
                        );
                        $compilationContext->codePrinter->output(
                            'zephir_concat_function(' . $variableVariableCode . ', ' . $tempVariableCode . ', ' . $variableVariableCode . ');'
                        );
                    // no break
                    case 'assign':
                        $compilationContext->backend->updateStaticProperty(
                            $classEntry,
                            $property,
                            $variableVariable,
                            $compilationContext
                        );
                        if ($variableVariable->isTemporal()) {
                            $variableVariable->setIdle(true);
                        }
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
                if ($variableVariable->isTemporal()) {
                    $variableVariable->setIdle(true);
                }
                break;

            default:
                throw new CompilerException(
                    'Unknown type ' . $variableVariable->getType(),
                    $statement
                );
        }
    }


    /**
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext
     * @param array              $statement
     *
     * @return Variable
     */
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

    /**
     * @param array    $statement
     * @param Variable $variableVariable
     *
     * @return void
     */
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

    /**
     * @param array    $statement
     * @param Variable $variableVariable
     *
     * @return void
     */
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
