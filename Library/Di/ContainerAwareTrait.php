<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Di;

use League\Container\Container;
use League\Container\ReflectionContainer;
use Psr\Container\ContainerInterface;
use Zephir\Exception\ContainerException;
use Zephir\Exception\InvalidCallException;
use Zephir\Exception\UnknownPropertyException;
use Zephir\Support\PropertyAccessor;

/**
 * Zephir\Di\ContainerAwareTrait
 *
 * @package Zephir\Di
 *
 * @property-read \Zephir\Environment $environment
 * @property-read \Zephir\Logger $logger
 * @property-read \Zephir\Config $config
 * @property-read \Zephir\BaseBackend $backend
 */
trait ContainerAwareTrait
{
    use PropertyAccessor {
        PropertyAccessor::__get as protected missingMethod;
    }

    /**
     * The Dependency Injection container.
     *
     * @var ContainerInterface|Container
     */
    private $container;

    /**
     * Latest container build.
     *
     * @var ContainerInterface|null
     */
    private static $default;

    /**
     * ContainerAwareTrait constructor.
     *
     * @param ContainerInterface|Container|null $container
     */
    public function __construct(ContainerInterface $container = null)
    {
        if ($container == null) {
            $container = Singleton::getDefault() ?: $this->createContainer();
        }

        Singleton::setDefault($container);
        $this->setContainer($container);
    }

    /**
     * {@inheritdoc}
     *
     * @see    InjectionAwareInterface
     * @return Container
     *
     * @throws ContainerException
     */
    public function getContainer()
    {
        if ($this->container instanceof ContainerInterface) {
            return $this->container;
        }

        throw new ContainerException('No container implementation has been set.');
    }

    /**
     * {@inheritdoc}
     *
     * @see    InjectionAwareInterface
     * @param  ContainerInterface|Container $container
     * @return void
     */
    public function setContainer(ContainerInterface $container)
    {
        $this->container = $container;
    }

    protected function createContainer(Container $container = null)
    {
        $container = $container ?: new Container();

        // Required to enable auto wiring
        $container->delegate(new ReflectionContainer());

        return $container;
    }

    /**
     * Magic method to get services using internal container.
     *
     * @param string $name
     *
     * @return mixed
     *
     * @throws InvalidCallException
     * @throws UnknownPropertyException
     */
    public function __get($name)
    {
        $container = $this->getContainer();

        if ($container && $container->has($name)) {
            return $container->get($name);
        }

        return $this->missingMethod($name);
    }
}
