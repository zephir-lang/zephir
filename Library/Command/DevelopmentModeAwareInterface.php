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

use Symfony\Component\Console\Input\InputInterface;

interface DevelopmentModeAwareInterface
{
    /**
     * Returns the development mode for the command.
     *
     * @return string
     */
    public function getDevelopmentModeHelp();

    /**
     * Checks if the development mode is enabled.
     *
     * @param  InputInterface $input
     * @return bool
     */
    public function isDevelopmentModeEnabled(InputInterface $input);
}
