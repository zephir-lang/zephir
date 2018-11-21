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

use Psr\Container\ContainerInterface;

/**
 * Zephir\Di\InjectionAwareInterface
 *
 * @package Zephir\Di
 */
interface InjectionAwareInterface
{
    /**
     * Set a container
     *
     * @param  ContainerInterface $container
     * @return void
     */
    public function setContainer(ContainerInterface $container);

    /**
     * Get the container
     *
     * @return ContainerInterface
     */
    public function getContainer();
}
