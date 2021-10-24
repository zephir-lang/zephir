<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Backends;

use Symfony\Component\Console\Input\ArgvInput;
use Zephir\BaseBackend;
use Zephir\Config;
use Zephir\Exception\IllegalStateException;

final class BackendFactory
{
    private Config $config;
    private string $kernelsPath;
    private string $templatesPath;

    public function __construct(Config $config, string $kernelsPath, string $templatesPath)
    {
        $this->config = $config;
        $this->kernelsPath = $kernelsPath;
        $this->templatesPath = $templatesPath;
    }

    public function createBackend()
    {
        $backendClassName = $this->resolveBackendClass();

        return new $backendClassName(
            $this->config,
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
    private function resolveBackendClass(): string
    {
        $parser = new ArgvInput();
        $backend = $parser->getParameterOption('--backend', null);

        if (null === $backend) {
            // Do not use this feature for typical use case.
            // Overriding backend using env var provided only for
            // testing purposes and may be removed in the future.
            // You SHOULD NOT rely on this possibility.
            if (getenv('ZEPHIR_BACKEND')) {
                $backend = getenv('ZEPHIR_BACKEND');
            }
        }

        if (null === $backend) {
            $backend = BaseBackend::getActiveBackend();
        }

        $className = "Zephir\\Backends\\{$backend}\\Backend";

        if (!class_exists($className) || 'ZendEngine2' === $backend) {
            throw new IllegalStateException(sprintf('Backend class "%s" does not exist.', $backend));
        }

        return $className;
    }
}
