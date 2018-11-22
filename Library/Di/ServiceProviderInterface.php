<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Di;

use League\Container\Container;
use Psr\Container\ContainerInterface;

/**
 * Zephir\Di\ServiceProviderInterface
 */
interface ServiceProviderInterface
{
    /**
     * Registers a service provider.
     *
     * @param Container|ContainerInterface $container
     * @return void
     */
    public function register(ContainerInterface $container);
}
