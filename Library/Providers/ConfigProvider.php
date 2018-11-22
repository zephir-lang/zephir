<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Providers;

use League\Container\Container;
use Psr\Container\ContainerInterface;
use Zephir\Config;
use Zephir\Di\ServiceProviderInterface;

/**
 * Zephir\Providers\ConfigProvider
 */
final class ConfigProvider implements ServiceProviderInterface
{
    /**
     * {@inheritdoc}
     *
     * @param Container|ContainerInterface $container
     * @throws \Zephir\Exception
     * @return void
     */
    public function register(ContainerInterface $container)
    {
        $container->share('config', Config::fromServer());
    }
}
