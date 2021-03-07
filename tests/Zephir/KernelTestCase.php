<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test;

use Symfony\Bundle\FrameworkBundle\Test\KernelTestCase as BaseTestCase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\DependencyInjection\Exception\ServiceCircularReferenceException;
use Symfony\Component\DependencyInjection\Exception\ServiceNotFoundException;
use Symfony\Component\HttpKernel\KernelInterface;
use Zephir\Config;
use Zephir\DependencyInjection\ZephirKernel;

class KernelTestCase extends BaseTestCase
{
    /**
     * Creates a Kernel.
     *
     * Available options:
     *
     *  * environment
     *  * debug
     *  * config_files
     *
     * @param array $options Kernel configuration
     *
     * @return KernelInterface A KernelInterface instance
     *
     * @throws \RuntimeException
     */
    protected static function createKernel(array $options = []): KernelInterface
    {
        if (null === static::$class) {
            static::$class = static::getKernelClass();
        }

        if (isset($options['environment'])) {
            $env = $options['environment'];
        } elseif (isset($_ENV['APP_ENV'])) {
            $env = $_ENV['APP_ENV'];
        } elseif (isset($_SERVER['APP_ENV'])) {
            $env = $_SERVER['APP_ENV'];
        } else {
            $env = 'test';
        }

        if (isset($options['debug'])) {
            $debug = $options['debug'];
        } elseif (isset($_ENV['APP_DEBUG'])) {
            $debug = $_ENV['APP_DEBUG'];
        } elseif (isset($_SERVER['APP_DEBUG'])) {
            $debug = $_SERVER['APP_DEBUG'];
        } else {
            $debug = true;
        }

        if (ZephirKernel::class === static::$class) {
            $configFiles = [];
            if (isset($options['config_files']) && \is_array($options['config_files'])) {
                $configFiles = $options['config_files'];
            }

            return new static::$class($env, $debug, $configFiles);
        } else {
            return new static::$class($env, $debug);
        }
    }

    /**
     * Do not output messages from the Zephir compiler.
     *
     * @param ContainerInterface $container
     */
    protected function muteOutput(ContainerInterface $container): void
    {
        try {
            $container->get(Config::class)->set('silent', true);
        } catch (ServiceCircularReferenceException $e) {
            $this->fail($e->getMessage());
        } catch (ServiceNotFoundException $e) {
            $this->fail($e->getMessage());
        }
    }
}
