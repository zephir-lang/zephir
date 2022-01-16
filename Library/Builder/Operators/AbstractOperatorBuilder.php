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

namespace Zephir\Builder\Operators;

/**
 * Abstract class for builders
 */
abstract class AbstractOperatorBuilder
{
    protected ?string $file = null;

    protected int $line = 0;

    protected int $char = 0;

    /**
     * Returns a builder definition.
     *
     * @return array
     */
    abstract public function get(): array;
}
