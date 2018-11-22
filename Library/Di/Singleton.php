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
 * Zephir\Di\Singleton
 */
final class Singleton
{
    /**
     * Latest container build.
     *
     * @var ContainerInterface|null
     */
    private static $default;

    /**
     * Set a default dependency injection container to be obtained into static methods.
     *
     * @param Container|ContainerInterface $container
     * @return void
     */
    public static function setDefault(ContainerInterface $container)
    {
        self::$default = $container;
    }

    /**
     * Return the latest container created.
     *
     * @return Container|ContainerInterface|null
     */
    public static function getDefault()
    {
        return self::$default;
    }

    /**
     * Resets the internal default container.
     *
     * @return void
     */
    public static function reset()
    {
        self::$default = null;
    }
}
