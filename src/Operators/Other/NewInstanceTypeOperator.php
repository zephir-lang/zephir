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

use Zephir\Builder\FunctionCallBuilder;
use Zephir\Builder\Operators\CastOperatorBuilder;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Operators\AbstractOperator;

/**
 * Creates a value of the specified type with parameters
 */
class NewInstanceTypeOperator extends AbstractOperator
{
    protected bool $literalOnly = false;

    /**
     * Executes the operator.
     *
     * @param array              $expression
     * @param CompilationContext $compilationContext
     *
     * @return CompiledExpression
     *
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        if (!isset($expression['parameters'])) {
            throw new CompilerException("Invalid 'parameters' for new-type", $expression);
        }

        switch ($expression['internal-type']) {
            case 'array':
                $compilationContext->headersManager->add('kernel/array');
                $functionName = 'create_array';
                break;

            case 'string':
                $compilationContext->headersManager->add('kernel/string');
                $functionName = 'create_string';
                break;

            default:
                throw new CompilerException('Cannot build instance of type', $expression);
        }

        $builder = new FunctionCallBuilder(
            $functionName,
            $expression['parameters'],
            1,
            $expression['file'],
            $expression['line'],
            $expression['char']
        );

        /**
         * Implicit type coercing.
         */
        $castBuilder = new CastOperatorBuilder($expression['internal-type'], $builder);

        $expression = new Expression($castBuilder->get());
        $expression->setReadOnly($this->readOnly);

        try {
            return $expression->compile($compilationContext);
        } catch (Exception $e) {
            throw new CompilerException($e->getMessage(), $expression, $e->getCode(), $e);
        }
    }
}
