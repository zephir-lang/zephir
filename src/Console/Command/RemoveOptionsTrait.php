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

use Symfony\Component\Console\Input\InputOption;

use function array_filter;
use function in_array;

trait RemoveOptionsTrait
{
    protected function removeOptions(array $names): void
    {
        $definition = $this->getDefinition();

        $filtered = array_filter(
            $definition->getOptions(),
            fn(InputOption $option) => !in_array($option->getName(), $names, true)
        );

        $definition->setOptions($filtered);
    }
}
