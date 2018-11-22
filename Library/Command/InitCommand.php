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

use Symfony\Component\Console\Exception\RuntimeException;
use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Zephir\BaseBackend;

/**
 * Zephir\Command\InitCommand
 *
 * Initializes a Zephir extension.
 */
class InitCommand extends ContainerAwareCommand
{
    protected function configure()
    {
        $this
            ->setName('init')
            ->setDescription('Initializes a Zephir extension')
            ->addArgument(
                'namespace',
                InputArgument::REQUIRED,
                'The extension namespace'
            )
            ->addOption(
                'backend',
                null,
                InputOption::VALUE_REQUIRED,
                'Used backend to create extension',
                'ZendEngine3'
            );
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $namespace = $this->sanitizeNamespace($input->getArgument('namespace'));

        // Tell the user the name could be reserved by another extension
        if (\extension_loaded($namespace)) {
            // TODO: Replace by $output->writeln()
            $this->logger->output('This extension can have conflicts with an existing loaded extension');
        }

        $this->config->set('namespace', $namespace);
        $this->config->set('name', $namespace);

        if (!\is_dir($namespace)) {
            \mkdir($namespace, 0755);
        }

        chdir($namespace);
        if (!\is_dir($namespace)) {
            \mkdir($namespace, 0755);
        }

        // Create 'kernel'
        if (!\is_dir('ext/kernel')) {
            \mkdir('ext/kernel', 0755, true);
        }

        // Copy the latest kernel files
        $this->recursiveProcess($this->getContainer()->get(BaseBackend::class)->getInternalKernelPath(), 'ext/kernel');

        return 0;
    }

    private function sanitizeNamespace($namespace)
    {
        // Prevent "" namespace
        if (empty($namespace)) {
            throw new RuntimeException('Not enough arguments (missing: "namespace").');
        }

        $namespace = \strtolower(\preg_replace('/[^0-9a-zA-Z]/', '', $namespace));

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
     * @param mixed $callback
     *
     * @return bool
     */
    private function recursiveProcess($src, $dst, $pattern = null, $callback = 'copy')
    {
        $success = true;
        $iterator = new \DirectoryIterator($src);

        foreach ($iterator as $item) {
            $pathName = $item->getPathname();
            if (!\is_readable($pathName)) {
                $this->logger->output('File is not readable :' . $pathName);
                continue;
            }

            $fileName = $item->getFileName();

            if ($item->isDir()) {
                if ($fileName != '.' && $fileName != '..' && $fileName != '.libs') {
                    if (!\is_dir($dst . DIRECTORY_SEPARATOR . $fileName)) {
                        \mkdir($dst . DIRECTORY_SEPARATOR . $fileName, 0755, true);
                    }
                    $this->recursiveProcess($pathName, $dst . DIRECTORY_SEPARATOR . $fileName, $pattern, $callback);
                }
            } elseif (!$pattern || ($pattern && \preg_match($pattern, $fileName) === 1)) {
                $path = $dst . DIRECTORY_SEPARATOR . $fileName;
                $success = $success && \call_user_func($callback, $pathName, $path);
            }
        }

        return $success;
    }
}
