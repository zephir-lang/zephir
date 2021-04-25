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

namespace Zephir\DependencyInjection;

use Exception;
use RuntimeException;
use Symfony\Bundle\MonologBundle\MonologBundle;
use Symfony\Component\Config\Loader\LoaderInterface;
use Symfony\Component\DependencyInjection\ContainerBuilder;
use Symfony\Component\HttpKernel\Kernel;
use Throwable;
use Zephir\DependencyInjection\CompilerPass\CollectCommandsToApplicationCompilerPass;
use Zephir\Zephir;

use function dirname;
use function Zephir\is_macos;
use function Zephir\is_windows;

use const DIRECTORY_SEPARATOR;

final class ZephirKernel extends Kernel
{
    private array $extraConfigFiles;
    private $startedDir;

    /**
     * AppKernel constructor.
     *
     * @param string   $environment The environment
     * @param bool     $debug       Whether to enable debugging or not
     * @param string[] $configFiles Optional Zephir configuration files
     */
    public function __construct(string $environment, bool $debug, array $configFiles = [])
    {
        $this->startedDir = getcwd();
        $this->extraConfigFiles = $configFiles;

        parent::__construct($environment, $debug);
    }

    /**
     * @return array
     */
    public function registerBundles(): array
    {
        return [
            new MonologBundle(),
        ];
    }

    /**
     * {@inheritdoc}
     *
     * @param LoaderInterface $loader
     *
     * @throws Exception|Throwable
     */
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/../config/config.yml');

        foreach ($this->extraConfigFiles as $configFile) {
            $loader->load($configFile);
        }
    }

    /**
     * @return string
     */
    public function getCacheDir(): string
    {
        $home = getenv('HOME') ?: (getenv('HOMEDRIVE').DIRECTORY_SEPARATOR.getenv('HOMEPATH'));

        if (is_macos()) {
            $cacheDir = getenv('XDG_CACHE_HOME') ?: $home.'/Library/Caches';
            $cacheDir .= '/Zephir';
        } elseif (is_windows()) {
            $cacheDir = getenv('LOCALAPPDATA') ?: $home;
            $cacheDir .= '\\Zephir\\Cache';
        } else {
            $cacheDir = getenv('XDG_CACHE_HOME') ?: $home.'/.cache';
            $cacheDir .= '/zephir';
        }

        $path = $cacheDir.DIRECTORY_SEPARATOR.$this->getPathSalt();
        if (!is_dir($path) && !mkdir($path, 0755, true) && !is_dir($path)) {
            throw new RuntimeException(
                sprintf('Unable to create cache directory: "%s"', $path)
            );
        }

        return $path;
    }

    /**
     * Allows container rebuild when config or version changes.
     * This also used to get unique path to kernel logs.
     *
     * @return string
     */
    private function getPathSalt(): string
    {
        $prefix =
            Zephir::VERSION.
            $this->environment.
            serialize($this->extraConfigFiles);

        $localConfig = $this->startedDir.DIRECTORY_SEPARATOR.'config.json';
        $suffix = $this->startedDir;
        if (file_exists($localConfig)) {
            $suffix = md5_file($localConfig);
        }

        return substr(md5("{$prefix}{$suffix}"), 0, 16);
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getLogDir(): string
    {
        $home = getenv('HOME') ?: (getenv('HOMEDRIVE').DIRECTORY_SEPARATOR.getenv('HOMEPATH'));

        if (is_macos()) {
            $stateDir = getenv('XDG_STATE_HOME') ?: $home.'/Library';
            $stateDir .= '/Zephir/State';
        } elseif (is_windows()) {
            $stateDir = getenv('LOCALAPPDATA') ?: $home;
            $stateDir .= '\\Zephir\\State';
        } else {
            // See: https://stackoverflow.com/a/27965014/1661465
            $stateDir = getenv('XDG_STATE_HOME') ?: $home.'/.local/state';
            $stateDir .= '/zephir';
        }

        $path = $stateDir.DIRECTORY_SEPARATOR.$this->getPathSalt();
        if (!is_dir($path) && !mkdir($path, 0755, true) && !is_dir($path)) {
            throw new RuntimeException(
                sprintf('Unable to create logs directory: "%s"', $path)
            );
        }

        return $path;
    }

    /**
     * {@inheritdoc}
     *
     * @return string The project root dir
     */
    public function getProjectDir(): string
    {
        return dirname(__DIR__, 2);
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getRootDir(): string
    {
        return dirname(__DIR__);
    }

    /**
     * Gets the local cache directory used internally by zephir.
     *
     * @return string The local cache dir
     */
    public function getStartedDir(): string
    {
        return $this->startedDir.'/.zephir';
    }

    /**
     * {@inheritdoc}
     *
     * @param ContainerBuilder $container
     */
    protected function build(ContainerBuilder $container)
    {
        $container->addCompilerPass(new CollectCommandsToApplicationCompilerPass());
    }

    /**
     * {@inheritdoc}
     *
     * @return array An array of kernel parameters
     */
    protected function getKernelParameters(): array
    {
        $parameters = parent::getKernelParameters();
        $parameters['kernel.local_cache_dir'] = $this->getStartedDir();

        return $parameters;
    }
}
