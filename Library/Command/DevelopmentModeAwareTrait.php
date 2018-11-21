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

/**
 * Zephir\Command\DevelopmentModeAwareTrait
 */
trait DevelopmentModeAwareTrait
{
    /**
     * {@inheritdoc}
     *
     * @param  InputInterface $input
     * @return bool
     */
    public function isDevelopmentModeEnabled(InputInterface $input)
    {
        if ($input->getOption('no-dev') == false) {
            return $input->getOption('dev') || PHP_DEBUG;
        }

        return false;
    }
}
