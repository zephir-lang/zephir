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

use Symfony\Component\Console\Command\Command as BaseCommand;

abstract class Command extends BaseCommand
{
    use RemoveOptionsTrait;

    /**
     * {@inheritdoc}
     *
     * @param bool $mergeArgs
     */
    public function mergeApplicationDefinition($mergeArgs = true)
    {
        parent::mergeApplicationDefinition($mergeArgs);

        $this->removeOptions(['dumpversion', 'version', 'vernum']);
    }
}
