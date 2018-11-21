<?php

/*
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

        $this->getContainer()->share(\get_class($this), $this);
        $this->setBasePath(empty($basePath) ? \dirname(__DIR__) : $basePath);
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
        $message = 'Unable to locate Zephir base directory. ' .
            'Try to reinstall Zephir to solve this problem. ' .
            'If the problem remains, ask for help on the forum ' .
            '(https://forum.zephir-lang.com) or file an issue at GitHub (https://github.com/phalcon/zephir).';

        if (!\is_string($basePath) || !\is_dir($basePath) || !\file_exists($basePath)) {
            throw new InvalidArgumentException($message);
        }

        $this->basePath = rtrim($basePath, '\\/');
        $this->registerPaths();
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

    /**
     * Checks if currently running under MS Windows.
     *
     * @return bool
     */
    public function isWindows()
    {
        return 'WIN' === \strtoupper(\substr(PHP_OS, 0, 3));
    }

    /**
     * Checks if currently running under macOs.
     *
     * @return bool
     */
    public function isMacOs()
    {
        return 'DARWIN' === \strtoupper(\substr(PHP_OS, 0, 6));
    }

    /**
     * Checks if currently running under BSD based OS.
     *
     * @link   https://en.wikipedia.org/wiki/List_of_BSD_operating_systems
     * @return bool
     */
    public function isBsd()
    {
        return false !== \stristr(\strtolower(PHP_OS), 'bsd');
    }

    /**
     * Checks if current PHP is thread safe.
     *
     * @return bool
     */
    public function isZts()
    {
        if (\defined('PHP_ZTS') && PHP_ZTS == 1) {
            return true;
        }

        \ob_start();
        \phpinfo(INFO_GENERAL);

        return (bool) \preg_match('/Thread\s*Safety\s*enabled/i', \strip_tags(\ob_get_clean()));
    }

    /**
     * Resolves Windows release folder.
     *
     * @return string
     */
    public function getWindowsReleaseDir()
    {
        if ($this->isZts()) {
            if (PHP_INT_SIZE === 4) {
                // 32-bit version of PHP
                return 'ext\\Release_TS';
            } elseif (PHP_INT_SIZE === 8) {
                // 64-bit version of PHP
                return 'ext\\x64\\Release_TS';
            } else {
                // fallback
                return 'ext\\Release_TS';
            }
        } else {
            if (PHP_INT_SIZE === 4) {
                // 32-bit version of PHP
                return 'ext\\Release';
            } elseif (PHP_INT_SIZE === 8) {
                // 64-bit version of PHP
                return 'ext\\x64\\Release';
            } else {
                // fallback
                return 'ext\\Release';
            }
        }
    }

    /**
     * Initialize all of the application paths in the DI container.
     *
     * @return void
     */
    protected function registerPaths()
    {
        $this->getContainer()->share('paths', (object) [
            'base' => $this->getPath(),
            'templates' => $this->getPath('templates'),
            'kernels' => $this->getPath('kernels'),
            'prototypes' => $this->getPath('prototypes'),
        ]);
    }
}
