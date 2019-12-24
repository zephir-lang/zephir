<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Backends;

use Psr\Container\ContainerInterface;
use Symfony\Component\Console\Input\ArgvInput;
use Zephir\BaseBackend;
use Zephir\Config;
use Zephir\Exception\IllegalStateException;

final class BackendFactory
{
    private $container;
    private $kernelsPath;
    private $templatesPath;

    public function __construct(ContainerInterface $container, $kernelsPath, $templatesPath)
    {
        $this->container = $container;
        $this->kernelsPath = $kernelsPath;
        $this->templatesPath = $templatesPath;
    }

    public function createBackend()
    {
        $backendClassName = $this->resolveBackendClass();

        return new $backendClassName(
            $this->container->get(Config::class),
            $this->kernelsPath,
            $this->templatesPath
        );
    }

    /**
     * Resolve backend class.
     *
     * @throws IllegalStateException
     *
     * @return string
     */
    private function resolveBackendClass()
    {
        $parser = new ArgvInput();
        $backend = $parser->getParameterOption('--backend', null);

        if (null === $backend) {
            // Do not use this feature for typical use case.
            // Overriding backend using env var provided only for
            // testing purposes and may be removed in the future.
            // You SHOULD NOT rely on this possibility.
            if (getenv('ZEPHIR_BACKEND')) {
                $backend = $backend = getenv('ZEPHIR_BACKEND');
            } elseif ($this->container->has('ZEPHIR_BACKEND')) {
                $backend = $this->container->get('ZEPHIR_BACKEND');
            }
        }

        if (null === $backend) {
            $backend = BaseBackend::getActiveBackend();
        }

        $className = "Zephir\\Backends\\{$backend}\\Backend";

        if (!class_exists($className) || 'ZendEngine2' == $backend) {
            throw new IllegalStateException(sprintf('Backend class "%s" doesn\'t exist.', $backend));
        }

        return $className;
    }
}
