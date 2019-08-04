<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Console\Command;

use Psr\Log\LoggerInterface;
use Symfony\Component\Console\Exception\RuntimeException;
use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Input\InputDefinition;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Zephir\BaseBackend;
use Zephir\Compiler;
use Zephir\Config;

/**
 * Zephir\Console\Command\InitCommand.
 *
 * Initializes a Zephir extension.
 */
final class InitCommand extends Command
{
    private $compiler;
    private $backend;
    private $config;
    private $logger;

    public function __construct(Compiler $compiler, BaseBackend $backend, Config $config, LoggerInterface $logger)
    {
        $this->compiler = $compiler;
        $this->backend = $backend;
        $this->config = $config;
        $this->logger = $logger;

        parent::__construct();
    }

    protected function configure()
    {
        $this
            ->setName('init')
            ->setDescription('Initializes a Zephir extension')
            ->setDefinition($this->createDefinition())
            ->setHelp(sprintf('%s.', $this->getDescription()));
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $namespace = $this->sanitizeNamespace($input->getArgument('namespace'));

        // Tell the user the name could be reserved by another extension
        if (\extension_loaded($namespace)) {
            $this->logger->error('This extension can have conflicts with an existing loaded extension');
        }

        $this->config->set('namespace', $namespace);
        $this->config->set('name', $namespace);

        if (!is_dir("{$namespace}/{$namespace}")) {
            mkdir("{$namespace}/{$namespace}", 0755, true);
        }

        chdir($namespace);

        // Create 'kernel'
        if (!is_dir('ext/kernel')) {
            mkdir('ext/kernel', 0755, true);
        }

        // Copy the latest kernel files
        $this->recursiveProcess($this->backend->getInternalKernelPath(), 'ext/kernel');

        return 0;
    }

    protected function createDefinition()
    {
        return new InputDefinition(
            [
                new InputArgument(
                    'namespace',
                    InputArgument::REQUIRED,
                    'The extension namespace'
                ),
                new InputOption(
                    'backend',
                    null,
                    InputOption::VALUE_REQUIRED,
                    'Used backend to create extension',
                    'ZendEngine3'
                ),
            ]
        );
    }

    private function sanitizeNamespace($namespace)
    {
        // Prevent "" namespace
        if (empty($namespace)) {
            throw new RuntimeException('Not enough arguments (missing: "namespace").');
        }

        $namespace = strtolower(preg_replace('/[^0-9a-zA-Z]/', '', $namespace));

        // If sanitizing returns an empty string
        if (empty($namespace)) {
            throw new RuntimeException('Cannot obtain a valid initial namespace for the project.');
        }

        return $namespace;
    }

    /**
     * Copies the base kernel to the extension destination.
     *
     * @param $src
     * @param $dst
     * @param string $pattern
     * @param mixed  $callback
     *
     * @return bool
     */
    private function recursiveProcess($src, $dst, $pattern = null, $callback = 'copy')
    {
        $success = true;
        $iterator = new \DirectoryIterator($src);

        foreach ($iterator as $item) {
            $pathName = $item->getPathname();
            if (!is_readable($pathName)) {
                $this->logger->error('File is not readable :'.$pathName);
                continue;
            }

            $fileName = $item->getFileName();

            if ($item->isDir()) {
                if ('.' != $fileName && '..' != $fileName && '.libs' != $fileName) {
                    if (!is_dir($dst.\DIRECTORY_SEPARATOR.$fileName)) {
                        mkdir($dst.\DIRECTORY_SEPARATOR.$fileName, 0755, true);
                    }
                    $this->recursiveProcess($pathName, $dst.\DIRECTORY_SEPARATOR.$fileName, $pattern, $callback);
                }
            } elseif (!$pattern || ($pattern && 1 === preg_match($pattern, $fileName))) {
                $path = $dst.\DIRECTORY_SEPARATOR.$fileName;
                $success = $success && \call_user_func($callback, $pathName, $path);
            }
        }

        return $success;
    }
}
