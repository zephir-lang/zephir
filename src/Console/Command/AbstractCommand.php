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

namespace Zephir\Console\Command;

use Symfony\Component\Console\Command\Command;

abstract class AbstractCommand extends Command
{
    use RemoveOptionsTrait;

    public function mergeApplicationDefinition(bool $mergeArgs = true): void
    {
        parent::mergeApplicationDefinition($mergeArgs);

        $this->removeOptions(['dumpversion', 'version', 'vernum']);
    }
}
