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
use Zephir\Variable\Variable;

use function count;

/**
 * CamelizeOptimizer.
 *
 * Optimizes calls to 'camelize' using internal function
 */
class CamelizeOptimizer extends OptimizerAbstract
{
    protected string $trimWhere    = '';
    protected string $warningName  = 'camelize';
    protected string $zephirMethod = 'zephir_camelize';

    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return bool|CompiledExpression|mixed
     *
     * @throws CompilerException
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            return false;
        }

        $this->checkParameters($expression['parameters']);

        $charlist = 'NULL ';
        if (2 == count($expression['parameters'])) {
            if ('null' == $expression['parameters'][1]['parameter']['type']) {
                unset($expression['parameters'][1]);
            }
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $this->symbolVariablePre($call, $symbolVariable, $context);

        $context->headersManager->add('kernel/string');

        $symbolVariable->setDynamicTypes('string');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        if (isset($resolvedParams[1])) {
            $charlist = $resolvedParams[1];
        }

        $this->symbolVariablePost($call, $symbolVariable, $context);

        $symbol = $context->backend->getVariableCode($symbolVariable);
        $context->codePrinter->output(
            $this->zephirMethod
            . 'zephir_fast_trim(' . $symbol . ', '
            . $resolvedParams[0] . ', ' . $charlist . $this->trimWhere . ');'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * @param $parameters
     *
     * @return void
     */
    protected function checkParameters($parameters): void
    {
        if (count($parameters) < 1 || count($parameters) > 2) {
            throw new CompilerException(
                "'" . $this->warningName . "' only accepts one or two parameters"
            );
        }
    }

    /**
     * @param Call               $call
     * @param Variable|null      $symbolVariable
     * @param CompilationContext $context
     *
     * @return void
     */
    protected function symbolVariablePost(
        Call $call,
        ?Variable $symbolVariable,
        CompilationContext $context
    ): void {
        // empty
    }

    /**
     * @param Call               $call
     * @param Variable|null      $symbolVariable
     * @param CompilationContext $context
     *
     * @return void
     */
    protected function symbolVariablePre(
        Call $call,
        ?Variable $symbolVariable,
        CompilationContext $context
    ): void {
        if ($call->mustInitSymbolVariable()) {
            $symbolVariable->initVariant($context);
        }
    }
}
