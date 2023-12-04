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

namespace Zephir\Console\Command;

use DirectoryIterator;
use Psr\Log\LoggerInterface;
use Symfony\Component\Console\Exception\RuntimeException;
use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Input\InputDefinition;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;
use Zephir\Backend\Backend;
use Zephir\Config;

use function chdir;
use function extension_loaded;
use function is_dir;
use function is_readable;
use function mkdir;
use function preg_match;
use function preg_replace;
use function sprintf;
use function strtolower;

use const DIRECTORY_SEPARATOR;

/**
 * Init Command
 *
 * Initializes a Zephir extension.
 */
final class InitCommand extends AbstractCommand
{
    private Backend         $backend;
    private Config          $config;
    private LoggerInterface $logger;

    public function __construct(Backend $backend, Config $config, LoggerInterface $logger)
    {
        $this->backend = $backend;
        $this->config  = $config;
        $this->logger  = $logger;

        parent::__construct();
    }

    protected function configure(): void
    {
        $this
            ->setName('init')
            ->setDescription('Initializes a Zephir extension')
            ->setDefinition($this->createDefinition())
            ->setHelp(sprintf('%s.', $this->getDescription()))
        ;
    }

    protected function createDefinition(): InputDefinition
    {
        return new InputDefinition(
            [
                new InputArgument(
                    'namespace',
                    InputArgument::REQUIRED,
                    'The extension namespace'
                ),
            ]
        );
    }

    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        $namespace = $this->sanitizeNamespace($input->getArgument('namespace'));

        // Tell the user the name could be reserved by another extension
        if (extension_loaded($namespace)) {
            $this->logger->warning('This extension can have conflicts with an existing loaded extension');
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
        // Dump initial configuration on project creation
        $this->config->dumpToFile();

        return 0;
    }

    /**
     * Copies the base kernel to the extension destination.
     *
     * @param string      $src
     * @param string      $dst
     * @param string|null $pattern
     * @param string      $callback
     *
     * @return void
     */
    private function recursiveProcess(string $src, string $dst, string $pattern = null, string $callback = 'copy'): void
    {
        $success  = true;
        $iterator = new DirectoryIterator($src);

        foreach ($iterator as $item) {
            $pathName = $item->getPathname();
            if (!is_readable($pathName)) {
                $this->logger->error('File is not readable :' . $pathName);
                continue;
            }

            $fileName = $item->getFileName();

            if ($item->isDir()) {
                if ('.' != $fileName && '..' != $fileName && '.libs' != $fileName) {
                    if (!is_dir($dst . DIRECTORY_SEPARATOR . $fileName)) {
                        mkdir($dst . DIRECTORY_SEPARATOR . $fileName, 0755, true);
                    }
                    $this->recursiveProcess($pathName, $dst . DIRECTORY_SEPARATOR . $fileName, $pattern, $callback);
                }
            } elseif ($pattern === null || 1 === preg_match($pattern, $fileName)) {
                $path    = $dst . DIRECTORY_SEPARATOR . $fileName;
                $success = $success && $callback($pathName, $path);
            }
        }
    }

    private function sanitizeNamespace(string $namespace): string
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
}
