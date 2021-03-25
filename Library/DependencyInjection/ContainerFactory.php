<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\DependencyInjection;

final class ContainerFactory
{
    private $environment = '$kernel-environment$';
    private $debug = false;

    public function __construct()
    {
        /*
         * Please do not concatenate the string bellow.
         * The final version of this string is used to prepare release PHAR file.
         * For more see box.json.dist file at the project root.
         */
        if ('$'.'kernel-environment'.'$' === $this->environment) {
            $this->environment = 'dev';
        }

        if ('dev' === $this->environment) {
            $this->debug = true;
        }
    }

    public function createWithConfigs(array $configs)
    {
        $kernel = new ZephirKernel($this->environment, $this->debug, $configs);
        $kernel->boot();

        return $kernel->getContainer();
    }

    public function create()
    {
        $kernel = new ZephirKernel($this->environment, $this->debug);
        $kernel->boot();

        return $kernel->getContainer();
    }
}
