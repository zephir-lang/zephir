<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Console\Command;

/**
 * Zephir\Console\Command\ZflagsAwareTrait.
 */
trait ZflagsAwareTrait
{
    /**
     * Returns ZFLAGS help.
     *
     * @return string
     */
    protected function getZflagsHelp(): string
    {
        return <<<EOT
Common flags are:
    <info>-f([a-z0-9\-]+)</info>    Enables compiler optimizations
    <info>-fno-([a-z0-9\-]+)</info> Disables compiler optimizations
    <info>-w([a-z0-9\-]+)</info>    Turns a warning on
    <info>-W([a-z0-9\-]+)</info>    Turns a warning off
EOT;
    }
}
