<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Console\Command;

use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;
use Zephir\FileSystem\FileSystemInterface;
use function Zephir\is_windows;

/**
 * Zephir\Console\Command\CleanCommand
 *
 * Cleans any object files created by the extension.
 */
final class CleanCommand extends Command
{
    private $filesystem;

    public function __construct(FileSystemInterface $filesystem)
    {
        $this->filesystem = $filesystem;

        parent::__construct();
    }

    protected function configure()
    {
        $this
            ->setName('clean')
            ->setDescription('Cleans any object files created by the extension');
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $this->filesystem->clean();

        if (is_windows()) {
            \system('cd ext && nmake clean-all');
        } else {
            \system('cd ext && make clean > /dev/null');
        }

        return 0;
    }
}
