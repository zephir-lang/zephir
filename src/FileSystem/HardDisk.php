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
use Zephir\Zephir;

use function chmod;
use function escapeshellarg;
use function file_exists;
use function file_get_contents;
use function file_put_contents;
use function filemtime;
use function hash_file;
use function is_dir;
use function is_file;
use function md5;
use function md5_file;
use function mkdir;
use function preg_split;
use function rmdir;
use function rtrim;
use function sprintf;
use function str_replace;
use function system;
use function unlink;

use const DIRECTORY_SEPARATOR;

/**
 * Local Hard Disk
 *
 * Uses the standard hard-disk as filesystem for temporary operations.
 */
class HardDisk implements FileSystemInterface
{
    /**
     * Root or base path
     *
     * Path to where all cached files and folders are collected.
     */
    private string $basePath;
    /**
     * Initialize checker
     */
    private bool $initialized = false;

    /**
     * @throws InvalidArgumentException
     */
    public function __construct(string $basePath, private string $localPath = Zephir::VERSION)
    {
        $this->basePath  = $this->rightTrimPath($basePath);
        $this->localPath = $this->rightTrimPath($localPath);

        if (empty($this->localPath)) {
            throw new InvalidArgumentException('The temporary container can not be empty.');
        }
    }

    /**
     * Recursive directory clean
     */
    public function clean(): void
    {
        if (!is_dir($this->basePath . DIRECTORY_SEPARATOR . $this->localPath)) {
            return;
        }

        $contents = $this->listDirectoryRecursively(
            $this->basePath . DIRECTORY_SEPARATOR . $this->localPath,
            RecursiveIteratorIterator::CHILD_FIRST,
        );

        /** @var SplFileInfo $file */
        foreach ($contents as $file) {
            $this->deleteFileInfoObject($file);
        }

        unset($contents);

        rmdir($this->basePath . DIRECTORY_SEPARATOR . $this->localPath);
    }

    public function delete(string $path): void
    {
        unlink($this->basePath . DIRECTORY_SEPARATOR . $this->localPath . "/{$path}");
    }

    public function exists(string $path): bool
    {
        if ('.' === $path || empty($path)) {
            $path = $this->localPath;
        } else {
            $path = "{$this->localPath}/{$path}";
        }

        return is_file($this->basePath . DIRECTORY_SEPARATOR . $path);
    }

    public function file(string $path): array
    {
        $contents = file_get_contents($this->basePath . DIRECTORY_SEPARATOR . $this->localPath . "/{$path}");

        return preg_split("/\r\n|\n|\r/", $contents);
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
     * Start File System
     */
    public function initialize(): void
    {
        $this->initialized = true;
    }

    public function isInitialized(): bool
    {
        return $this->initialized;
    }

    public function makeDirectory(string $path): bool
    {
        if ('.' === $path || empty($path)) {
            $path = $this->localPath;
        } else {
            $path = "{$this->localPath}/{$path}";
        }

        $dir = $this->basePath . DIRECTORY_SEPARATOR . $path;

        if (is_dir($dir)) {
            chmod($dir, 0755);

            return true;
        }

        mkdir($this->basePath . DIRECTORY_SEPARATOR . $path, 0755, true);

        return is_dir($path);
    }

    public function modificationTime(string $path): int
    {
        return filemtime($this->basePath . DIRECTORY_SEPARATOR . $this->localPath . "/{$path}");
    }

    public function normalizePath(string $path): string
    {
        return str_replace(['\\', ':', '/'], '_', $path);
    }

    /**
     * Checks if the content of the file on the disk is the same as the content.
     */
    public static function persistByHash(string $data, string $path): int | bool
    {
        if (!file_exists($path)) {
            return file_put_contents($path, $data);
        }

        if (md5($data) !== md5_file($path)) {
            return file_put_contents($path, $data);
        }

        return false;
    }

    public function read(string $path): string
    {
        return file_get_contents($this->basePath . DIRECTORY_SEPARATOR . $this->localPath . "/{$path}");
    }

    public function requireFile(string $path): mixed
    {
        if (!empty($this->basePath)) {
            return require "{$this->basePath}/{$this->localPath}/{$path}";
        }

        $code = file_get_contents($this->basePath . DIRECTORY_SEPARATOR . $this->localPath . "/{$path}");

        return eval(str_replace('<?php ', '', $code));
    }

    public function system(string $command, string $descriptor, string $destination): void
    {
        // fallback
        $redirect = "{$this->localPath}/{$destination}";
        if (!empty($this->basePath)) {
            $redirect = "{$this->basePath}/{$this->localPath}/{$destination}";
        }

        switch ($descriptor) {
            default:
            case 'stdout':
                system("{$command} > " . escapeshellarg($redirect));
                break;
            case 'stderr':
                system("{$command} 2> " . escapeshellarg($redirect));
                break;
        }
    }

    public function write(string $path, string $data): void
    {
        file_put_contents($this->basePath . DIRECTORY_SEPARATOR . $this->localPath . "/{$path}", $data);
    }

    protected function deleteFileInfoObject(SplFileInfo $file): bool
    {
        return match ($file->getType()) {
            'dir'   => @rmdir((string)$file->getRealPath()),
            'link'  => @unlink($file->getPathname()),
            default => @unlink((string)$file->getRealPath()),
        };
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

    private function rightTrimPath(string $path): string
    {
        return rtrim($path, '\\/');
    }
}
