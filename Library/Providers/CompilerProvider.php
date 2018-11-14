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
use Zephir\BaseBackend;
use Zephir\Compiler;
use Zephir\Config;
use Zephir\Di\ServiceProviderInterface;
use Zephir\Logger;
use Zephir\Parser\Manager;

/**
 * Zephir\Providers\CompilerProvider
 *
 * @package Zephir\Providers
 */
final class CompilerProvider implements ServiceProviderInterface
{
    /**
     * {@inheritdoc}
     *
     * @param  ContainerInterface|Container $container
     * @return void
     */
    public function register(ContainerInterface $container)
    {
        $service = function () use ($container) {
            $compiler = new Compiler(
                $container->get(Config::class),
                $container->get(Logger::class),
                $container->get(BaseBackend::class),
                $container->get(Manager::class)
            );

            $compiler->setContainer($container);

            return $compiler;
        };

        $container
            ->share(Compiler::class, $service)
            ->setAlias('compiler');
    }
}
