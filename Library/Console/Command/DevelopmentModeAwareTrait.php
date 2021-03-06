<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Console\Command;

use Symfony\Component\Console\Input\InputInterface;

/**
 * Zephir\Console\Command\DevelopmentModeAwareTrait.
 */
trait DevelopmentModeAwareTrait
{
    /**
     * Checks if the development mode is enabled.
     *
     * @param InputInterface $input
     *
     * @return bool
     */
    protected function isDevelopmentModeEnabled(InputInterface $input): bool
    {
        if (false === $input->getOption('no-dev')) {
            return $input->getOption('dev') || PHP_DEBUG;
        }

        return false;
    }
}
