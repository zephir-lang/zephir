<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\DependencyInjection;

use Oneup\FlysystemBundle\OneupFlysystemBundle;
use Symfony\Bundle\MonologBundle\MonologBundle;
use Symfony\Component\Config\Loader\LoaderInterface;
use Symfony\Component\DependencyInjection\ContainerBuilder;
use Symfony\Component\HttpKernel\Bundle\BundleInterface;
use Symfony\Component\HttpKernel\Kernel;
use Zephir\DependencyInjection\CompilerPass\CollectCommandsToApplicationCompilerPass;
use Zephir\Zephir;

final class ZephirKernel extends Kernel
{
    private $extraConfigFiles = [];
    private $startedDirectory;

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
        $this->startedDirectory = getcwd();

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

        return $this->startedDirectory.'/.zephir/'.substr(md5($hash), 0, 16).'/cache';
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getLogDir()
    {
        // allows container rebuild when config or version changes
        $hash = Zephir::VERSION.$this->environment.serialize($this->extraConfigFiles);

        return $this->startedDirectory.'/.zephir/'.substr(md5($hash), 0, 16).'/logs';
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
     * {@inheritdoc}
     *
     * @param ContainerBuilder $containerBuilder
     */
    protected function build(ContainerBuilder $containerBuilder)
    {
        $containerBuilder->addCompilerPass(new CollectCommandsToApplicationCompilerPass());
    }
}
