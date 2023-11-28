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

namespace Zephir\Traits;

use Zephir\Exception\CompilerException;
use Zephir\Variable\Variable;

use function session_cache_limiter;

trait VariablesTrait
{
    protected string $returnValueMessage = 'Returned values by functions can only be assigned to variant variables';

    /**
     * @param Variable|null $variable
     * @param array         $expression
     *
     * @return void
     */
    protected function checkNotVariable(
        ?Variable $variable,
        array $expression
    ): void {
        if (null !== $variable && $variable->isNotVariable()) {
            throw new CompilerException($this->returnValueMessage, $expression);
        }
    }

    /**
     * @param Variable|null $variable
     * @param array         $expression
     *
     * @return void
     */
    protected function checkNotVariableString(
        ?Variable $variable,
        array $expression
    ): void {
        if (null !== $variable && $variable->isNotVariableAndString()) {
            throw new CompilerException($this->returnValueMessage, $expression);
        }
    }
}
