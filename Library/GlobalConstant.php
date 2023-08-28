<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

/**
 * Creates a new global constant.
 *
 * Global constants are allocated one time at extension initialization
 * and are referenced across the C code saving memory
 */
class GlobalConstant
{
    public function __construct(protected string $name)
    {
    }

    /**
     * Returns global constant name.
     *
     * @return string
     */
    public function getName(): string
    {
        return $this->name;
    }

    /**
     * Check if the global constant is temporal.
     *
     * @return bool
     */
    public function isTemporal(): bool
    {
        return false;
    }
}
