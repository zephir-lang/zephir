<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Operators\Other;

use Zephir\Utils;
use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Builder\FunctionCallBuilder;
use Zephir\Builder\Operators\CastOperatorBuilder;

/**
 * NewTypeInstance
 *
 * Creates a value of the specified type with parameters
 */
class NewInstanceTypeOperator extends BaseOperator
{
    protected $_literalOnly = false;

    /**
     * Executes the operator
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
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
                throw new CompilerException("Cannot build instance of type", $expression);
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
         * Implicit type coercing
         */
        $castBuilder = new CastOperatorBuilder($expression['internal-type'], $builder);

        $expression = new Expression($castBuilder->get());
        $expression->setReadOnly($this->_readOnly);

        return $expression->compile($compilationContext);
    }
}
