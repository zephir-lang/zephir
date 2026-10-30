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
use Zephir\Name;
use Zephir\Optimizers\OptimizerAbstract;

use function count;

/**
 * Optimizes calls to 'explode' using internal function
 */
class ExplodeOptimizer extends OptimizerAbstract
{
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
        if (count($expression['parameters']) < 2) {
            throw new CompilerException("'explode' require two parameter");
        }

        /*
         * Process the expected symbol to be returned
         */
        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $limitOffset = 2;
        if ('string' == $expression['parameters'][0]['parameter']['type']) {
            $str = Name::addSlashes($expression['parameters'][0]['parameter']['value']);
            unset($expression['parameters'][0]);
            $limitOffset = 1;
        }

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        /*
         * The kernel parses the limit after the separator and the subject, as
         * PHP's Z_PARAM_LONG does, so it is passed as a zval; NULL means none.
         */
        $limit = $resolvedParams[$limitOffset] ?? 'NULL';

        $context->headersManager->add('kernel/string');
        $symbolVariable->setDynamicTypes('array');
        $this->checkInitSymbolVariable($call, $symbolVariable, $context);

        $symbol = $context->backend->getVariableCode($symbolVariable);
        if (isset($str)) {
            $context->codePrinter->output(
                'zephir_fast_explode_str(' . $symbol . ', SL("' . $str . '"), ' . $resolvedParams[0] . ', ' . $limit . ');'
            );
        } else {
            $context->codePrinter->output(
                'zephir_fast_explode(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $limit . ');'
            );
        }

        $context->emitExceptionCheck();

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
