<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Command;

use Psr\Container\ContainerInterface;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Exception\LogicException;
use Zephir\Di\ContainerAwareTrait;
use Zephir\Di\InjectionAwareInterface;

/**
 * Zephir\Command\ContainerAwareCommand
 */
abstract class ContainerAwareCommand extends Command implements InjectionAwareInterface
{
    use ContainerAwareTrait {
        ContainerAwareTrait::__construct as protected __DiInject;
    }

    /**
     * {@inheritdoc}
     *
     * @param ContainerInterface|null $container
     * @param string|null $name
     *
     * @throws LogicException
     */
    public function __construct(ContainerInterface $container = null, $name = null)
    {
        if ($container != null) {
            $this->__DiInject($container);
        }

        parent::__construct($name);
    }
}
