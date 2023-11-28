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

namespace Zephir\Operators\Logical;

use Exception;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Statements\LetStatement;
use Zephir\Variable\Variable;

class OrOperator extends LogicalBaseOperator
{
    protected string $bitOperator = '||';
    protected string $operator    = '||';


    /**
     * @param Variable $variable
     *
     * @return string
     */
    protected function getOutput(Variable $variable): string
    {
        return 'if (!(' . $variable->getName() . ')) {';
    }
}
