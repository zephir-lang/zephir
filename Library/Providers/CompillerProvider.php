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
 * Zephir\Providers\CompillerProvider
 *
 * @package Zephir\Providers
 */
final class CompillerProvider implements ServiceProviderInterface
{
    /**
     * {@inheritdoc}
     *
     * @param  ContainerInterface|Container $container
     * @return void
     */
    public function register(ContainerInterface $container)
    {
        $container->add(
            Compiler::class,
            function () use ($container) {
                $compiller = new Compiler(
                    $container->get(Config::class),
                    $container->get(Logger::class),
                    $container->get(BaseBackend::class),
                    $container->get(Manager::class)
                );

                $compiller->setContainer($container);

                return $compiller;
            }
        );
    }
}
