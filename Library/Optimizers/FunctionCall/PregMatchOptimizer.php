<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
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
use Zephir\Variable;

/**
 * Zephir\Optimizers\FunctionCall\PregMatchOptimizer.
 *
 * Optimizes calls to 'preg_match' using internal function.
 */
class PregMatchOptimizer extends OptimizerAbstract
{
    const GLOBAL_MATCH = 0;

    /**
     * {@inheritdoc}
     *
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
        if (!isset($expression['parameters']) || \count($expression['parameters']) < 2) {
            throw new CompilerException(
                sprintf(
                    'preg_match() expects at least 2 parameters, %d given',
                    isset($expression['parameters']) ? \count($expression['parameters']) : 0
                ),
                $expression
            );
        }

        if (\count($expression['parameters']) > 5) {
            throw new CompilerException(
                sprintf(
                    'preg_match() expects at most 5 parameters, %d given',
                    \count($expression['parameters'])
                ),
                $expression
            );
        }

        $matches = $this->createMatches($expression, $context);
        list($flags, $offset) = $this->processOptionals($expression, $call, $context);

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        if (!$symbolVariable->isVariable()) {
            throw new CompilerException(
                'Returned values by functions can only be assigned to variant variables',
                $expression
            );
        }

        // zephir_preg_match
        $context->headersManager->add('kernel/string');

        try {
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

            if ($call->mustInitSymbolVariable()) {
                $symbolVariable->initVariant($context);
            }

            $symbol = $context->backend->getVariableCode($symbolVariable);
            $matchesSymbol = $context->backend->getVariableCode($matches);

            $context->codePrinter->output(
                sprintf(
                    'zephir_preg_match(%s, %s, %s, %s, %d, %s, %s TSRMLS_CC);',
                    $symbol,
                    $resolvedParams[0],
                    $resolvedParams[1],
                    $matchesSymbol,
                    static::GLOBAL_MATCH,
                    $flags,
                    $offset
                )
            );

            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        } catch (Exception $e) {
            throw new CompilerException($e->getMessage(), $expression, $e->getCode(), $e);
        }
    }

    /**
     * Process optional parameters.
     *
     *     preg_match(pattern, subject, matches, flags, offset)
     *
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @throws CompilerException
     *
     * @return array
     */
    private function processOptionals(array &$expression, Call $call, CompilationContext $context)
    {
        $flags = null;
        $offset = null;

        $offsetParamOffset = 4;
        if (isset($expression['parameters'][4]) && 'int' === $expression['parameters'][4]['parameter']['type']) {
            $offset = $expression['parameters'][4]['parameter']['value'].' ';
            unset($expression['parameters'][4]);
        }

        if (isset($expression['parameters'][3]) && 'int' === $expression['parameters'][3]['parameter']['type']) {
            $flags = $expression['parameters'][3]['parameter']['value'].' ';
            $offsetParamOffset = 3;
            unset($expression['parameters'][3]);
        }

        try {
            $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

            if (null === $offset && isset($resolvedParams[$offsetParamOffset])) {
                $context->headersManager->add('kernel/operators');
                $offset = 'zephir_get_intval('.$resolvedParams[$offsetParamOffset].') ';
            }

            if (null === $flags && isset($resolvedParams[3])) {
                $context->headersManager->add('kernel/operators');
                $flags = 'zephir_get_intval('.$resolvedParams[3].') ';
            }
        } catch (Exception $e) {
            throw new CompilerException($e->getMessage(), $expression, $e->getCode(), $e);
        }

        if (null === $flags) {
            $flags = '0 ';
        }

        if (null === $offset) {
            $offset = '0 ';
        }

        return [$flags, $offset];
    }

    /**
     * Process the matches result.
     *
     *     preg_match(pattern, subject, matches)
     *
     * @param array              $expression
     * @param CompilationContext $context
     *
     * @return Variable
     */
    private function createMatches(array $expression, CompilationContext $context)
    {
        if (isset($expression['parameters'][2])) {
            $type = $expression['parameters'][2]['parameter']['type'];

            if ('variable' !== $type) {
                throw new CompilerException('Only variables can be passed by reference', $expression);
            }

            $name = $expression['parameters'][2]['parameter']['value'];

            if (!$variable = $context->symbolTable->getVariable($name)) {
                throw new CompilerException(
                    sprintf("Cannot mutate variable '%s' because it wasn't defined", $name),
                    $expression
                );
            }

            if (!\in_array($variable->getType(), ['variable', 'array'], true)) {
                throw new CompilerException(
                    sprintf(
                        "The '%s' variable must be either a variable or an array, got %s",
                        $name,
                        $variable->getType()
                    ),
                    $expression
                );
            }

            if (false == $variable->isInitialized()) {
                $variable->initVariant($context);
                $variable->setIsInitialized(true, $context);
            }
        } else {
            $variable = $context->symbolTable->addTemp('variable', $context);
            $variable->initVariant($context);
        }

        $variable->setDynamicTypes('array');

        return $variable;
    }
}
