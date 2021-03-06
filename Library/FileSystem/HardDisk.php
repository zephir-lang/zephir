<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\FileSystem;

use ErrorException;
use League\Flysystem;
use Zephir\Exception\CompilerException;
use Zephir\Exception\FileSystemException;
use Zephir\Exception\InvalidArgumentException;
use Zephir\Exception\RuntimeException;
use Zephir\Zephir;

/**
 * Zephir\FileSystem\HardDisk.
 *
 * Uses the standard hard-disk as filesystem for temporary operations.
 */
class HardDisk implements FileSystemInterface
{
    /** @var Flysystem\FilesystemInterface */
    private $filesystem;

    /** @var string */
    private $localPath;

    /** @var bool */
    private $initialized = false;

    /** @var string|null */
    private $basePath;

    /**
     * HardDisk constructor.
     *
     * @param Flysystem\FilesystemInterface $filesystem
     * @param string                        $localPath
     *
     * @throws InvalidArgumentException
     */
    public function __construct(Flysystem\FilesystemInterface $filesystem, $localPath = Zephir::VERSION)
    {
        $this->filesystem = $filesystem;
        $this->localPath = trim($localPath, '\\/');

        if (empty($this->localPath)) {
            throw new InvalidArgumentException('The temporary container can not be empty.');
        }
    }

    /**
     * @param string|null $basePath
     */
    public function setBasePath($basePath)
    {
        $this->basePath = rtrim($basePath, '\\/');
    }

    /**
     * {@inheritdoc}
     *
     * @return bool
     */
    public function isInitialized(): bool
    {
        return $this->initialized;
    }

    /**
     * {@inheritdoc}
     *
     * @throws RuntimeException
     */
    public function initialize()
    {
        if ((false === $this->exists($this->localPath)) && false === $this->filesystem->createDir($this->localPath)) {
            throw new RuntimeException(
                'Unable to create a local storage for temporary filesystem operations.'
            );
        }

        $this->initialized = true;
    }

    /**
     * {@inheritdoc}
     *
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

        return $this->filesystem->has($path);
    }

    /**
     * {@inheritdoc}
     *
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

        return $this->filesystem->createDir($path);
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     *
     * @throws CompilerException
     *
     * @return array
     */
    public function file(string $path): array
    {
        try {
            $contents = $this->filesystem->read($this->localPath."/{$path}");

            return preg_split("/\r\n|\n|\r/", $contents);
        } catch (Flysystem\FileNotFoundException $e) {
            throw new CompilerException($e->getMessage(), null, $e->getCode(), $e);
        }
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     *
     * @return int
     *
     * @throws Flysystem\FileNotFoundException
     */
    public function modificationTime(string $path): int
    {
        return (int) $this->filesystem->getTimestamp($this->localPath."/{$path}");
    }

    /**
     * Writes data from a temporary entry.
     *
     * @param string $path
     *
     * @throws Flysystem\FileNotFoundException
     *
     * @return string
     */
    public function read(string $path): string
    {
        return (string) $this->filesystem->read($this->localPath."/{$path}");
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     *
     * @throws Flysystem\FileNotFoundException
     */
    public function delete(string $path)
    {
        $this->filesystem->delete($this->localPath."/{$path}");
    }

    /**
     * {@inheritdoc}
     *
     * @param string $path
     * @param string $contents
     *
     * @throws Flysystem\FileExistsException
     */
    public function write(string $path, string $contents)
    {
        $this->filesystem->write($this->localPath."/{$path}", $contents);
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
     * @throws Flysystem\FileNotFoundException
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
     * @throws FileSystemException
     * @throws Flysystem\RootViolationException
     */
    public function clean()
    {
        try {
            $this->filesystem->deleteDir($this->localPath);
        } catch (ErrorException $e) {
            // For reasons beyond our control, the actual owner of the directory
            // contents may not be the same as the current user. Therefore we need
            // to catch ErrorException and throw an expected exception with informing
            // the current user.
            throw new FileSystemException($e->getMessage(), $e->getCode(), $e);
        }
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
     * @throws Flysystem\FileExistsException
     * @throws Flysystem\FileNotFoundException
     *
     * @return string
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

        if (false === $this->filesystem->has($cacheFile)) {
            $contents = hash_file($algorithm, $sourceFile);
            $this->filesystem->write($cacheFile, $contents);

            return $contents;
        }

        if (filemtime($sourceFile) > $this->filesystem->getTimestamp($cacheFile)) {
            $contents = hash_file($algorithm, $sourceFile);
            $this->filesystem->update($cacheFile, $contents);

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
