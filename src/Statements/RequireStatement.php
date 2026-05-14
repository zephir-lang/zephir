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

namespace Zephir\Statements;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Expression;

/**
 * Require statement is used to execute PHP scripts in a given path
 */
class RequireStatement extends StatementAbstract
{
    protected string $methodName = 'require';

    /**
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $expr = new Expression([
            'type' => $this->methodName,
            'left' => $this->statement['expr'],
            'file' => $this->statement['file'],
            'line' => $this->statement['line'],
            'char' => $this->statement['char'],
        ]);
        $expr->setExpectReturn(false);
        $expr->compile($compilationContext);
    }
}
