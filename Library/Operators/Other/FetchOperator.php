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

namespace Zephir\Operators\Other;

use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\BaseOperator;

/**
 * FetchOperator.
 *
 * Fetch is a special operator that checks if an expression 'isset' and then obtain the value
 * without calculating the hash key twice
 */
class FetchOperator extends BaseOperator
{
    /**
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws Exception
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $compilationContext->headersManager->add('kernel/array');

        $variable = $compilationContext->symbolTable->getVariableForWrite($expression['left']['value'], $compilationContext, $expression['left']);
        if ('variable' != $variable->getType()) {
            throw new CompilerException('Cannot use variable type: '.$variable->gettype().' in "fetch" operator', $expression);
        }

        /**
         * return_value must not be observed
         */
        if ('return_value' !== $variable->getName()) {
            /**
             * TODO: use a read detector here
             */
            $readOnly = false;
            $line = max($compilationContext->symbolTable->getLastCallLine(), $compilationContext->symbolTable->getLastUnsetLine());
            if (false === $line || ($line > 0 && $line < $expression['line'])) {
                $numberMutations = $compilationContext->symbolTable->getExpectedMutations($variable->getName());
                if (1 == $numberMutations) {
                    if (1 == $variable->getNumberMutations()) {
                        $variable->setIsInitialized(true, $compilationContext);
                        $variable->setMemoryTracked(false);
                        $variable->setDynamicTypes('undefined');
                        $readOnly = true;
                    }
                }
            }

            if (!$readOnly || 'array-access' != $expression['right']['type']) {
                $variable->setIsInitialized(true, $compilationContext);
                $variable->observeVariant($compilationContext);
                $variable->setDynamicTypes('undefined');
                $variable->setPossibleValue(new CompiledExpression('undefined', '', $expression), $compilationContext);
            }
        } else {
            $variable = $compilationContext->symbolTable->getTempVariableForObserve('variable', $compilationContext);
        }

        $flags = $readOnly ? '1' : '0';

        switch ($expression['right']['type']) {
            case 'array-access':
                $exprVariable = new Expression($expression['right']['left']);
                $exprVariable->setReadOnly(true);
                $exprVariable->setNoisy(false);

                $exprCompiledVariable = $exprVariable->compile($compilationContext);
                if ('variable' != $exprCompiledVariable->getType()) {
                    throw new CompilerException('Expression type: '.$exprCompiledVariable->getType().' cannot be used as array', $expression['right']['left']);
                }

                $evalVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression['right']['left']);
                if ('variable' != $evalVariable->getType() && 'array' != $evalVariable->getType()) {
                    throw new CompilerException('Variable type: '.$variable->getType().' cannot be used as array', $expression['right']['left']);
                }

                if ('variable' == $evalVariable->getType()) {
                    if ($evalVariable->hasDifferentDynamicType(['undefined', 'array', 'null'])) {
                        $compilationContext->logger->warning(
                            'Possible attempt to use non array in fetch operator',
                            ['non-valid-fetch', $expression['right']]
                        );
                    }
                }

                $expr = new Expression($expression['right']['right']);
                $expr->setReadOnly(true);
                $expr->setNoisy(false);
                $resolvedExpr = $expr->compile($compilationContext);

                return $compilationContext->backend->arrayIssetFetch($variable, $evalVariable, $resolvedExpr, $flags, $expression, $compilationContext);
                break;

            case 'property-access':
                $exprVariable = new Expression($expression['right']['left']);
                $exprVariable->setReadOnly(true);
                $exprVariable->setNoisy(false);

                $exprCompiledVariable = $exprVariable->compile($compilationContext);
                if ('variable' != $exprCompiledVariable->getType()) {
                    throw new CompilerException('Expression type: '.$exprCompiledVariable->getType().' cannot be used as object', $expression['right']['left']);
                }

                $evalVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression['right']['left']);
                if ('variable' != $evalVariable->getType()) {
                    throw new CompilerException('Variable type: '.$variable->getType().' cannot be used as object', $expression['right']['left']);
                }

                if ($evalVariable->hasDifferentDynamicType(['undefined', 'object', 'null'])) {
                    $compilationContext->logger->warning(
                        'Possible attempt to use non object in fetch operator',
                        ['non-valid-fetch', $expression['right']]
                    );
                }

                $property = $expression['right']['right']['value'];

                $compilationContext->headersManager->add('kernel/object');
                $symbol = $compilationContext->backend->getVariableCodePointer($variable);
                $evalSymbol = $compilationContext->backend->getVariableCode($evalVariable);

                return new CompiledExpression('bool', 'zephir_fetch_property('.$symbol.', '.$evalSymbol.', SL("'.$property.'"), PH_SILENT_CC)', $expression);

            case 'property-dynamic-access':
                $exprVariable = new Expression($expression['right']['left']);
                $exprVariable->setReadOnly(true);
                $exprVariable->setNoisy(false);

                $exprCompiledVariable = $exprVariable->compile($compilationContext);
                if ('variable' != $exprCompiledVariable->getType()) {
                    throw new CompilerException('Expression type: '.$exprCompiledVariable->getType().' cannot be used as object', $expression['right']['left']);
                }

                $evalVariable = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariable->getCode(), $compilationContext, $expression['right']['left']);
                if ('variable' != $evalVariable->getType()) {
                    throw new CompilerException('Variable type: '.$evalVariable->getType().' cannot be used as object', $expression['right']['left']);
                }

                if ($evalVariable->hasDifferentDynamicType(['undefined', 'object', 'null'])) {
                    $compilationContext->logger->warning(
                        'Possible attempt to use non object in fetch operator',
                        ['non-valid-fetch', $expression['right']]
                    );
                }

                $exprVariableProperty = new Expression($expression['right']['right']);
                $exprVariableProperty->setReadOnly(true);

                $exprCompiledVariableProperty = $exprVariableProperty->compile($compilationContext);
                if ('variable' != $exprCompiledVariableProperty->getType()) {
                    throw new CompilerException('Expression type: '.$exprCompiledVariableProperty->getType().' cannot be used in property-dynamic-access', $expression['right']['right']);
                }

                $evalVariableProperty = $compilationContext->symbolTable->getVariableForRead($exprCompiledVariableProperty->getCode(), $compilationContext, $expression['right']['right']);
                if ('variable' != $evalVariableProperty->getType() && 'string' != $evalVariableProperty->getType()) {
                    throw new CompilerException('Variable type: '.$evalVariableProperty->getType().' cannot be used in property-dynamic-access', $expression['right']['right']);
                }

                $compilationContext->headersManager->add('kernel/object');

                $symbol = $compilationContext->backend->getVariableCodePointer($variable);
                $evalSymbol = $compilationContext->backend->getVariableCode($evalVariable);
                $evalPropertySymbol = $compilationContext->backend->getVariableCode($evalVariableProperty);

                return new CompiledExpression('bool', 'zephir_fetch_property_zval('.$symbol.', '.$evalSymbol.', '.$evalPropertySymbol.', PH_SILENT_CC)', $expression);

            default:
                throw new CompilerException('Cannot use this expression for "fetch" operators: '.$expression['right']['type'], $expression);
        }
    }
}
