<?php

/**
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
use Zephir\Commands\Manager;
use Zephir\Di\ServiceProviderInterface;

/**
 * Zephir\Providers\CommandsManagerProvider
 *
 * @package Zephir\Providers
 */
final class CommandsManagerProvider implements ServiceProviderInterface
{
    /**
     * {@inheritdoc}
     *
     * @param  ContainerInterface|Container $container
     * @return void
     */
    public function register(ContainerInterface $container)
    {
        $container->share(Manager::class, function () use ($container) {
            $manager = new Manager($container);
            $manager->registerBuiltinCommands();

            return $manager;
        });
    }
}
