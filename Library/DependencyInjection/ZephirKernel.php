<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\DependencyInjection;

use const DIRECTORY_SEPARATOR;
use Oneup\FlysystemBundle\OneupFlysystemBundle;
use Symfony\Bundle\MonologBundle\MonologBundle;
use Symfony\Component\Config\Loader\LoaderInterface;
use Symfony\Component\DependencyInjection\ContainerBuilder;
use Symfony\Component\HttpKernel\Bundle\BundleInterface;
use Symfony\Component\HttpKernel\Kernel;
use Zephir\DependencyInjection\CompilerPass\CollectCommandsToApplicationCompilerPass;
use function Zephir\is_macos;
use function Zephir\is_windows;
use Zephir\Zephir;

final class ZephirKernel extends Kernel
{
    private $extraConfigFiles;
    private $startedDir;

    /**
     * AppKernel constructor.
     *
     * @param string   $environment The environment
     * @param bool     $debug       Whether to enable debugging or not
     * @param string[] $configFiles Optional Zephir configuration files
     */
    public function __construct($environment, $debug, array $configFiles = [])
    {
        $this->extraConfigFiles = $configFiles;
        $this->startedDir = getcwd();

        parent::__construct($environment, $debug);
    }

    /**
     * {@inheritdoc}
     *
     * @return BundleInterface[]|iterable
     */
    public function registerBundles()
    {
        return [
            new MonologBundle(),
            new OneupFlysystemBundle(),
        ];
    }

    /**
     * {@inheritdoc}
     *
     * @param LoaderInterface $loader
     *
     * @throws \Exception|\Throwable
     */
    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load(__DIR__.'/../config/config.yml');

        foreach ($this->extraConfigFiles as $configFile) {
            $loader->load($configFile);
        }
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getCacheDir()
    {
        // allows container rebuild when config or version changes
        $hash = Zephir::VERSION.$this->environment.serialize($this->extraConfigFiles);
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

        $path = $cacheDir.DIRECTORY_SEPARATOR.substr(md5($hash), 0, 16);
        if (!is_dir($path) && !mkdir($path, 0777, true) && !is_dir($path)) {
            throw new \RuntimeException(sprintf('Directory "%s" was not created', $path));
        }

        return $path;
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getLogDir()
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

        $path = $stateDir.DIRECTORY_SEPARATOR.Zephir::VERSION;
        if (!is_dir($path) && !mkdir($path, 0777, true) && !is_dir($path)) {
            throw new \RuntimeException(sprintf('Directory "%s" was not created', $path));
        }

        return $path;
    }

    /**
     * {@inheritdoc}
     *
     * @return string The project root dir
     */
    public function getProjectDir()
    {
        return \dirname(\dirname(__DIR__));
    }

    /**
     * {@inheritdoc}
     */
    public function getRootDir()
    {
        return \dirname(__DIR__);
    }

    /**
     * Gets the local cache directory used internally by zephir.
     *
     * @return string The local cache dir
     */
    public function getStartedDir()
    {
        return $this->startedDir.'/.zephir';
    }

    /**
     * {@inheritdoc}
     *
     * @param ContainerBuilder $containerBuilder
     */
    protected function build(ContainerBuilder $containerBuilder)
    {
        $containerBuilder->addCompilerPass(new CollectCommandsToApplicationCompilerPass());
    }

    /**
     * {@inheritdoc}
     *
     * @return array An array of kernel parameters
     */
    protected function getKernelParameters()
    {
        $parameters = parent::getKernelParameters();
        $parameters['kernel.local_cache_dir'] = $this->getStartedDir();

        return $parameters;
    }
}
