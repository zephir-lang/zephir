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
use League\Flysystem\Adapter\Local;
use League\Flysystem\Filesystem;
use Psr\Container\ContainerInterface;
use Zephir\Di\ServiceProviderInterface;
use Zephir\FileSystem\HardDisk;

/**
 * Zephir\Providers\FileSystemProvider
 */
final class FileSystemProvider implements ServiceProviderInterface
{
    /**
     * {@inheritdoc}
     *
     * @param Container|ContainerInterface $container
     * @return void
     */
    public function register(ContainerInterface $container)
    {
        $container->add('filesystem', $this->createService($container));
        // TODO: Use when drop PHP 5.x
        // ->share(\Zephir\FileSystem\FileSystemInterface::class, $this->createService($container))
        // ->setAlias('filesystem');
    }

    protected function createService(ContainerInterface $container)
    {
        return function () use ($container) {
            $adapter = new Local(getcwd());
            return new HardDisk(
                new Filesystem($adapter, ['visibility' => 'public'])
            );
        };
    }
}
