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

namespace Zephir\Compiler;

use Psr\Log\LoggerInterface;
use Zephir\AliasManager;
use Zephir\CompilerFile;
use Zephir\Config;
use Zephir\FileSystem\FileSystemInterface;

final class CompilerFileFactory
{
    public function __construct(private Config $config, private FileSystemInterface $filesystem, private LoggerInterface $logger)
    {
    }

    /**
     * Creates CompilerFile instance.
     *
     * NOTE: Each instance of CompilerFile must have its own AliasManager instance.
     *
     * @param string $className
     * @param string $filePath
     *
     * @return FileInterface
     */
    public function create(string $className, string $filePath): FileInterface
    {
        $compiler = new CompilerFile($this->config, new AliasManager(), $this->filesystem);

        $compiler->setClassName($className);
        $compiler->setFilePath($filePath);
        $compiler->setLogger($this->logger);

        return $compiler;
    }
}
