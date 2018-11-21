<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Command;

/**
 * Zephir\Command\ZflagsAwareInterface
 *
 * @package Zephir\Command
 */
interface ZflagsAwareInterface
{
    /**
     * Returns ZFLAGS help.
     *
     * @return string
     */
    public function getZflagsHelp();
}
