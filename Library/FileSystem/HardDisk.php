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

namespace Zephir\FileSystem;

use League\Flysystem;
use League\Flysystem\Filesystem;
use League\Flysystem\Local\LocalFilesystemAdapter;
use Zephir\Exception\InvalidArgumentException;
use Zephir\Exception\RuntimeException;
use Zephir\Zephir;

/**
 * Local Hard Disk
 *
 * Uses the standard hard-disk as filesystem for temporary operations.
 */
class HardDisk implements FileSystemInterface
{
    private ?Filesystem $filesystem = null;

    /** @var string */
    private string $localPath;

    /**
     * Initialize checker
     *
     * @var bool
     */
    private bool $initialized = false;

    /**
     * Root or base path
     *
     * Path to where all cached files and folders are collected.
     *
     * @var string
     */
    private string $basePath;

    /**
     * HardDisk constructor.
     *
     * @param string $basePath
     * @param string $localPath
     *
     * @throws InvalidArgumentException
     */
    public function __construct(string $basePath, string $localPath = Zephir::VERSION)
    {
        $this->basePath = $this->rightTrimPath($basePath);
        $this->localPath = $this->rightTrimPath($localPath);

        if (empty($this->localPath)) {
            throw new InvalidArgumentException('The temporary container can not be empty.');
        }
    }

    /**
     * @param string $path
     *
     * @return string
     */
    private function rightTrimPath(string $path): string
    {
        return rtrim($path, '\\/');
    }

    /**
     * @return bool
     */
    public function isInitialized(): bool
    {
        return $this->initialized;
    }

    /**
     * Start File System
     *
     * @throws RuntimeException
     */
    public function initialize()
    {
        $this->initialized = true;
        $this->filesystem = new Filesystem(new LocalFilesystemAdapter($this->basePath));
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     *
     * @return bool
     *
     * @throws Flysystem\FilesystemException
     */
    public function exists(string $path): bool
    {
        if ('.' === $path || empty($path)) {
            $path = $this->localPath;
        } else {
            $path = "{$this->localPath}/{$path}";
        }

        return $this->filesystem->fileExists($path);
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     *
     * @return bool
     *
     * @throws Flysystem\FilesystemException
     */
    public function makeDirectory(string $path): bool
    {
        if ('.' === $path || empty($path)) {
            $path = $this->localPath;
        } else {
            $path = "{$this->localPath}/{$path}";
        }

        $this->filesystem->createDirectory($path);

        return is_dir($path);
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     *
     * @return array
     *
     * @throws Flysystem\FilesystemException
     */
    public function file(string $path): array
    {
        $contents = $this->filesystem->read($this->localPath."/{$path}");

        return preg_split("/\r\n|\n|\r/", $contents);
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     *
     * @return int
     *
     * @throws Flysystem\FilesystemException
     */
    public function modificationTime(string $path): int
    {
        return $this->filesystem->lastModified($this->localPath."/{$path}");
    }

    /**
     * Writes data from a temporary entry.
     *
     * @param string $path
     *
     * @return string
     *
     * @throws Flysystem\FilesystemException
     */
    public function read(string $path): string
    {
        return $this->filesystem->read($this->localPath."/{$path}");
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     *
     * @throws Flysystem\FilesystemException
     */
    public function delete(string $path)
    {
        $this->filesystem->delete($this->localPath."/{$path}");
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     * @param string $data
     *
     * @throws Flysystem\FilesystemException
     */
    public function write(string $path, string $data)
    {
        $this->filesystem->write($this->localPath."/{$path}", $data);
    }

    /**
     * {@inheritdoc}
     *
     * @param string $command
     * @param string $descriptor
     * @param string $destination
     */
    public function system(string $command, string $descriptor, string $destination)
    {
        // fallback
        $redirect = "{$this->localPath}/{$destination}";
        if (!empty($this->basePath)) {
            $redirect = "{$this->basePath}/{$this->localPath}/{$destination}";
        }

        switch ($descriptor) {
            default:
            case 'stdout':
                system("{$command} > ".escapeshellarg($redirect));
                break;
            case 'stderr':
                system("{$command} 2> ".escapeshellarg($redirect));
                break;
        }
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     *
     * @return mixed
     *
     * @throws Flysystem\FilesystemException
     */
    public function requireFile(string $path)
    {
        if (!empty($this->basePath)) {
            return require "{$this->basePath}/{$this->localPath}/{$path}";
        }

        $code = $this->filesystem->read($this->localPath."/{$path}");

        return eval(str_replace('<?php ', '', $code));
    }

    /**
     * {@inheritdoc}
     *
     * @throws Flysystem\FilesystemException
     */
    public function clean()
    {
        $this->filesystem->deleteDirectory($this->localPath);
    }

    /**
     * {@inheritdoc}
     *
     * This function does not perform operations in the temporary
     * directory but it caches the results to avoid reprocessing.
     *
     * @param string $algorithm
     * @param string $sourceFile
     * @param bool   $useCache
     *
     * @return string
     *
     * @throws Flysystem\FilesystemException
     */
    public function getHashFile(string $algorithm, string $sourceFile, $useCache = false): string
    {
        if (false === $useCache) {
            return hash_file($algorithm, $sourceFile);
        }

        $cacheFile = sprintf(
            '%s/%s.%s',
            $this->localPath,
            $this->normalizePath($sourceFile),
            $algorithm
        );

        if (false === $this->filesystem->fileExists($cacheFile)) {
            $contents = hash_file($algorithm, $sourceFile);
            $this->filesystem->write($cacheFile, $contents);

            return $contents;
        }

        if (filemtime($sourceFile) > $this->filesystem->lastModified($cacheFile)) {
            $contents = hash_file($algorithm, $sourceFile);
            $this->filesystem->write($cacheFile, $contents);

            return $contents;
        }

        return $this->filesystem->read($cacheFile);
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     *
     * @return string
     */
    public function normalizePath(string $path): string
    {
        return str_replace(['\\', ':', '/'], '_', $path);
    }
}
