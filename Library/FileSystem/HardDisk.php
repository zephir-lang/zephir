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

use FilesystemIterator;
use Generator;
use RecursiveDirectoryIterator;
use RecursiveIteratorIterator;
use SplFileInfo;
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
    }

    /**
     * @param string $path
     *
     * @return bool
     */
    public function exists(string $path): bool
    {
        if ('.' === $path || empty($path)) {
            $path = $this->localPath;
        } else {
            $path = "{$this->localPath}/{$path}";
        }

        return is_file($this->basePath.DIRECTORY_SEPARATOR.$path);
    }

    /**
     * @param string $path
     *
     * @return bool
     */
    public function makeDirectory(string $path): bool
    {
        if ('.' === $path || empty($path)) {
            $path = $this->localPath;
        } else {
            $path = "{$this->localPath}/{$path}";
        }

        $dir = $this->basePath.DIRECTORY_SEPARATOR.$path;

        if (is_dir($dir)) {
            chmod($dir, 0755);

            return true;
        }

        mkdir($this->basePath.DIRECTORY_SEPARATOR.$path, 0755, true);

        return is_dir($path);
    }

    /**
     * @param string $path
     *
     * @return array
     */
    public function file(string $path): array
    {
        $contents = file_get_contents($this->basePath.DIRECTORY_SEPARATOR.$this->localPath."/{$path}");

        return preg_split("/\r\n|\n|\r/", $contents);
    }

    /**
     * @param string $path
     *
     * @return int
     */
    public function modificationTime(string $path): int
    {
        return filemtime($this->basePath.DIRECTORY_SEPARATOR.$this->localPath."/{$path}");
    }

    /**
     * @param string $path
     *
     * @return string
     */
    public function read(string $path): string
    {
        return file_get_contents($this->basePath.DIRECTORY_SEPARATOR.$this->localPath."/{$path}");
    }

    /**
     * @param string $path
     */
    public function delete(string $path)
    {
        unlink($this->basePath.DIRECTORY_SEPARATOR.$this->localPath."/{$path}");
    }

    /**
     * @param string $path
     * @param string $data
     */
    public function write(string $path, string $data)
    {
        file_put_contents($this->basePath.DIRECTORY_SEPARATOR.$this->localPath."/{$path}", $data);
    }

    /**
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
     * @param string $path
     *
     * @return mixed
     */
    public function requireFile(string $path)
    {
        if (!empty($this->basePath)) {
            return require "{$this->basePath}/{$this->localPath}/{$path}";
        }

        $code = file_get_contents($this->basePath.DIRECTORY_SEPARATOR.$this->localPath."/{$path}");

        return eval(str_replace('<?php ', '', $code));
    }

    /**
     * Recursive directory clean
     */
    public function clean(): void
    {
        if (!is_dir($this->basePath.DIRECTORY_SEPARATOR.$this->localPath)) {
            return;
        }

        $contents = $this->listDirectoryRecursively($this->basePath.DIRECTORY_SEPARATOR.$this->localPath, RecursiveIteratorIterator::CHILD_FIRST);

        /** @var SplFileInfo $file */
        foreach ($contents as $file) {
            $this->deleteFileInfoObject($file);
        }

        unset($contents);

        rmdir($this->basePath.DIRECTORY_SEPARATOR.$this->localPath);
    }

    /**
     * This function does not perform operations in the temporary
     * directory, but it caches the results to avoid reprocessing.
     *
     * @param string $algorithm
     * @param string $sourceFile
     * @param bool   $useCache
     *
     * @return string
     */
    public function getHashFile(string $algorithm, string $sourceFile, bool $useCache = false): string
    {
        if (false === $useCache) {
            return hash_file($algorithm, $sourceFile);
        }

        $cacheFile = sprintf(
            '%s/%s/%s.%s',
            $this->basePath,
            $this->localPath,
            $this->normalizePath($sourceFile),
            $algorithm
        );

        if (!is_file($cacheFile)) {
            $contents = hash_file($algorithm, $sourceFile);
            file_put_contents($cacheFile, $contents);

            return $contents;
        }

        if (filemtime($sourceFile) > filemtime($cacheFile)) {
            $contents = hash_file($algorithm, $sourceFile);
            file_put_contents($cacheFile, $contents);

            return $contents;
        }

        return file_get_contents($cacheFile);
    }

    /**
     * @param string $path
     *
     * @return string
     */
    public function normalizePath(string $path): string
    {
        return str_replace(['\\', ':', '/'], '_', $path);
    }

    protected function deleteFileInfoObject(SplFileInfo $file): bool
    {
        switch ($file->getType()) {
            case 'dir':
                return @rmdir((string) $file->getRealPath());
            case 'link':
                return @unlink($file->getPathname());
            default:
                return @unlink((string) $file->getRealPath());
        }
    }

    private function listDirectoryRecursively(
        string $path,
        int $mode = RecursiveIteratorIterator::SELF_FIRST
    ): Generator {
        yield from new RecursiveIteratorIterator(
            new RecursiveDirectoryIterator($path, FilesystemIterator::SKIP_DOTS),
            $mode
        );
    }
}
