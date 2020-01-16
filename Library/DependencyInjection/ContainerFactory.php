<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\DependencyInjection;

final class ContainerFactory
{
    public function createWithConfigs(array $configs)
    {
        $kernel = new ZephirKernel('dev', true, $configs);
        $kernel->boot();

        return $kernel->getContainer();
    }

    public function create()
    {
        $kernel = new ZephirKernel('dev', true);
        $kernel->boot();

        return $kernel->getContainer();
    }
}
