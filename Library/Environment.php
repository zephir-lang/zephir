<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir;

use Psr\Container\ContainerInterface;
use Zephir\Di\ContainerAwareTrait;
use Zephir\Di\InjectionAwareInterface;
use Zephir\Exception\InvalidArgumentException;

/**
 * Zephir\Environment
 *
 * @package Zephir
 */
final class Environment implements InjectionAwareInterface
{
    use ContainerAwareTrait {
        ContainerAwareTrait::__construct as protected __DiInject;
    }

    /**
     * The base path for the application.
     *
     * @var string
     */
    protected $basePath;

    /**
     * ServiceRegistrator constructor.
     *
     * @param string|null             $basePath
     * @param ContainerInterface|null $container
     */
    public function __construct($basePath, ContainerInterface $container = null)
    {
        $this->__DiInject($container);

        $this->getContainer()->share(get_class($this), $this);
        $this->setBasePath(empty($basePath) ? dirname(__DIR__) : $basePath);
    }

    /**
     * Set the base path for the Zephir installation.
     *
     * @param  string $basePath
     * @return void
     *
     * @throws InvalidArgumentException
     */
    public function setBasePath($basePath)
    {
        $message = 'Unable to locate Zephir installation path.' . PHP_EOL . PHP_EOL .
            'Double check the Zephir installation. When installing from sources,' . PHP_EOL .
            'it is enough to specify ZEPHIRDIR environment variable to the proper' . PHP_EOL .
            'Zephir installation path.' . PHP_EOL . PHP_EOL .
            'Current ZEPHIRDIR value is: "%s"' . PHP_EOL .
            'The base path used by the Zephir compiler is: "%s"';

        if (!is_string($basePath) || !is_dir($basePath) || !file_exists($basePath)) {
            throw new InvalidArgumentException(
                sprintf($message, \getenv('ZEPHIRDIR'), \is_scalar($basePath) ? $basePath : \gettype($basePath))
            );
        }

        $this->basePath = rtrim($basePath, '\\/');
        $this->registerPaths();
    }

    /**
     * Initialize all of the application paths in the DI container.
     *
     * @return void
     */
    protected function registerPaths()
    {
        $this->getContainer()->share('paths', (object) [
            'base'       => $this->getPath(),
            'templates'  => $this->getPath('templates'),
            'kernels'    => $this->getPath('kernels'),
            'prototypes' => $this->getPath('prototypes'),
        ]);
    }

    /**
     * Get the path to the Zephir installation.
     *
     * @param  string $path
     * @return string
     */
    public function getPath($path = '')
    {
        return $this->basePath . ($path ? DIRECTORY_SEPARATOR . $path : $path);
    }

    /**
     * Get the path to the templates.
     *
     * @param  string $path
     * @return string
     */
    public function getTemplatesPath($path = '')
    {
        $basePath = $this->getContainer()->get('paths')->templates;

        return $basePath . ($path ? DIRECTORY_SEPARATOR . $path : $path);
    }

    /**
     * Get the path to the kernels.
     *
     * @param  string $path
     * @return string
     */
    public function getKernelsPath($path = '')
    {
        $basePath = $this->getContainer()->get('paths')->kernels;

        return $basePath . ($path ? DIRECTORY_SEPARATOR . $path : $path);
    }

    /**
     * Get the path to the prototypes.
     *
     * @param  string $path
     * @return string
     */
    public function getPrototypesPath($path = '')
    {
        $basePath = $this->getContainer()->get('paths')->prototypes;

        return $basePath . ($path ? DIRECTORY_SEPARATOR . $path : $path);
    }
}
