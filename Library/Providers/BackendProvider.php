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
use Zephir\CommandArgumentParser;
use Zephir\Config;
use Zephir\Di\ServiceProviderInterface;
use Zephir\Exception\IllegalStateException;

/**
 * Zephir\Providers\BackendProvider
 *
 * @package Zephir\Providers
 */
final class BackendProvider implements ServiceProviderInterface
{
    /**
     * {@inheritdoc}
     *
     * @param  ContainerInterface|Container $container
     * @return void
     *
     * @throws IllegalStateException
     */
    public function register(ContainerInterface $container)
    {
        $backendClassName = $this->resolveBackendClass($container);
        $service = function () use ($container, $backendClassName) {
            return new $backendClassName($container->get(Config::class));
        };

        $container
            ->add(BaseBackend::class, $service)
            ->setAlias('backend');
    }

    /**
     * Resolve backend class.
     *
     * @param  ContainerInterface $container
     * @return string
     *
     * @throws IllegalStateException
     */
    private function resolveBackendClass(ContainerInterface $container)
    {
        $params = [];

        if (count($_SERVER['argv']) > 2) {
            $args = array_slice($_SERVER['argv'], 2);

            /** @var CommandArgumentParser $parser */
            $parser = $container->get(CommandArgumentParser::class);
            $params = $parser->parseArgs(array_merge(['command'], $args));
        }

        // Do not use this feature for typical use case.
        // Overriding backend using env var provided only for
        // testing purposes and may be removed in future.
        if ($backend = getenv('ZEPHIR_BACKEND')) {
            $params['backend'] = $backend;
        } elseif ($container->has('ZEPHIR_BACKEND')) {
            $params['backend'] = $container->get('ZEPHIR_BACKEND');
        }

        if (isset($params['backend'])) {
            $backend = $params['backend'];
        } else {
            $backend = BaseBackend::getActiveBackend();
        }

        $className = "Zephir\\Backends\\{$backend}\\Backend";

        if (!class_exists($className)) {
            throw new IllegalStateException(sprintf('Backend class "%s" doesn\'t exist.', $backend));
        }

        return $className;
    }
}
