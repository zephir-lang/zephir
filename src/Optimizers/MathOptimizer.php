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

namespace Zephir\Optimizers;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;

use function count;

abstract class MathOptimizer extends OptimizerAbstract
{
    protected string $zephirMethod = '';

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

        if (count($expression['parameters']) > 1) {
            return false;
        }

        /**
         * Resolve parameters as vars
         */
        $call->getResolvedParams($expression['parameters'], $context, $expression);
        /**
         * Get CompiledExpression(s) for resolved var(s).
         */
        $resolvedParams     = $call->getResolvedParamsAsExpr(
            $expression['parameters'],
            $context,
            $expression
        );
        $compiledExpression = $resolvedParams[0];

        switch ($compiledExpression->getType()) {
            case 'int':
            case 'float':
            case 'long':
            case 'ulong':
            case 'double':
                $context->headersManager->add('math');

                return $this->passNativeFCall($compiledExpression, $expression);
                break;
            case 'variable':
                $variable = $context->symbolTable->getVariable($compiledExpression->getCode());
                switch ($variable->getType()) {
                    case 'int':
                    case 'float':
                    case 'long':
                    case 'ulong':
                    case 'double':
                        $context->headersManager->add('math');

                        return $this->passNativeFCall($compiledExpression, $expression);
                        break;
                    case 'variable':
                        $context->headersManager->add('kernel/math');

                        return new CompiledExpression(
                            'double',
                            'zephir_' . $this->zephirMethod
                            . '(' . $context->backend->getVariableCode($variable) . ')',
                            $expression
                        );
                }
        }

        return false;
    }

    /**
     * @param CompiledExpression $compiledExpression
     * @param array              $expression
     *
     * @return CompiledExpression
     */
    protected function passNativeFCall($compiledExpression, $expression)
    {
        return new CompiledExpression(
            'double',
            $this->zephirMethod . '(' . $compiledExpression->getCode() . ')',
            $expression
        );
    }
}
