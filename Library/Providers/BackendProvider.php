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
use Symfony\Component\Console\Input\ArgvInput;
use Zephir\BaseBackend;
use Zephir\Di\ServiceProviderInterface;
use Zephir\Exception\IllegalStateException;

/**
 * Zephir\Providers\BackendProvider
 */
final class BackendProvider implements ServiceProviderInterface
{
    /**
     * {@inheritdoc}
     *
     * @param  ContainerInterface|Container $container
     * @throws IllegalStateException
     * @return void
     */
    public function register(ContainerInterface $container)
    {
        $backendClassName = $this->resolveBackendClass($container);
        $service = function () use ($container, $backendClassName) {
            return new $backendClassName($container->get('config'));
        };

        /**
         * TODO: Does not work. Why???
         * ->setAlias('backend');
         */
        $container->add(BaseBackend::class, $service);
    }

    /**
     * Resolve backend class.
     *
     * @param  ContainerInterface    $container
     * @throws IllegalStateException
     * @return string
     */
    private function resolveBackendClass(ContainerInterface $container)
    {
        $parser = new ArgvInput();
        $backend = $backend = $parser->getParameterOption('--backend', null);

        // Do not use this feature for typical use case.
        // Overriding backend using env var provided only for
        // testing purposes and may be removed in the future.
        // You SHOULD NOT rely on this possibility.
        if (getenv('ZEPHIR_BACKEND')) {
            $backend = $backend = getenv('ZEPHIR_BACKEND');
        } elseif ($container->has('ZEPHIR_BACKEND')) {
            $backend = $container->get('ZEPHIR_BACKEND');
        }

        if ($backend == null) {
            $backend = BaseBackend::getActiveBackend();
        }

        $className = "Zephir\\Backends\\{$backend}\\Backend";

        if (!class_exists($className)) {
            throw new IllegalStateException(sprintf('Backend class "%s" doesn\'t exist.', $backend));
        }

        return $className;
    }
}
