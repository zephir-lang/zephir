<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

/**
 * GlobalConstant.
 *
 * Global constants are allocated one time at extension initialization
 * and are referenced across the C code saving memory
 */
class GlobalConstant
{
    protected $name;

    /**
     * Creates a new global constant.
     *
     * @param string $name
     */
    public function __construct($name)
    {
        $this->name = $name;
    }

    /**
     * Returns global constant name.
     *
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * Check if the global constant is temporal.
     *
     * @return bool
     */
    public function isTemporal()
    {
        return false;
    }
}
