<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\FileSystem;

use League\Flysystem;
use Zephir\Exception\InvalidArgumentException;
use Zephir\Exception\RuntimeException;
use Zephir\Zephir;

/**
 * Zephir\FileSystem\HardDisk
 *
 * Uses the standard hard-disk as filesystem for temporary operations.
 *
 * @package Zephir\FileSystem
 */
final class HardDisk implements FileSystemInterface
{
    /** @var Flysystem\FilesystemInterface */
    private $filesystem;

    /** @var string */
    private $localPath;

    /** @var bool */
    private $initialized = false;

    /**
     * HardDisk constructor
     *
     * @param  Flysystem\FilesystemInterface $filesystem
     * @param  string                        $localPath
     *
     * @throws InvalidArgumentException
     */
    public function __construct(Flysystem\FilesystemInterface $filesystem, $localPath = '.temp')
    {
        $this->filesystem = $filesystem;
        $this->localPath = trim($localPath, '\\/');

        if (empty($this->localPath)) {
            throw new InvalidArgumentException('The temporary container can not be empty.');
        }
    }

    /**
     * {@inheritdoc}
     *
     * @return bool
     */
    public function isInitialized()
    {
        return $this->initialized;
    }

    /**
     * {@inheritdoc}
     *
     * @return void
     *
     * @throws RuntimeException
     */
    public function initialize()
    {
        if ($this->exists($this->localPath) == false) {
            if ($this->filesystem->createDir($this->localPath) == false) {
                throw new RuntimeException(
                    'Unable to create a local storage for temporary filesystem operations.'
                );
            }
        }

        $this->initialized = true;
    }

    /**
     * {@inheritdoc}
     *
     * @param  string $path
     * @return bool
     */
    public function exists($path)
    {
        return $this->filesystem->has($this->localPath . "/{$path}");
    }

    /**
     * {@inheritdoc}
     *
     * @param  string $path
     * @return bool
     */
    public function makeDirectory($path)
    {
        return $this->filesystem->createDir($this->localPath . "/{$path}");
    }

    /**
     * {@inheritdoc}
     *
     * @param  string $path
     * @return array
     *
     * @throws Flysystem\FileNotFoundException
     */
    public function file($path)
    {
        $contents = $this->filesystem->read($this->localPath . "/{$path}");

        return preg_split("/\r\n|\n|\r/", $contents);
    }

    /**
     * {@inheritdoc}
     *
     * @param  string $path
     *
     * @throws Flysystem\FileNotFoundException
     */
    public function modificationTime($path)
    {
        return (int) $this->filesystem->getTimestamp($this->localPath . "/{$path}");
    }

    /**
     * Writes data from a temporary entry.
     *
     * @param  string $path
     * @return string
     *
     * @throws Flysystem\FileNotFoundException
     */
    public function read($path)
    {
        return (string) $this->filesystem->read($this->localPath . "/{$path}");
    }

    /**
     * {@inheritdoc}
     *
     * @param  string $path
     * @return void
     *
     * @throws Flysystem\FileNotFoundException
     */
    public function delete($path)
    {
        $this->filesystem->delete($this->localPath . "/{$path}");
    }

    /**
     * {@inheritdoc}
     *
     * @param  string $path
     * @param  string $contents
     * @return void
     *
     * @throws Flysystem\FileExistsException
     */
    public function write($path, $contents)
    {
        $this->filesystem->write($this->localPath . "/{$path}", $contents);
    }

    /**
     * {@inheritdoc}
     *
     * @param  string $command
     * @param  string $descriptor
     * @param  string $destination
     *
     * @return void
     */
    public function system($command, $descriptor, $destination)
    {
        switch ($descriptor) {
            default:
            case 'stdout':
                system($command . ' > ' . realpath($this->localPath) . "/{$destination}");
                break;
            case 'stderr':
                system($command . ' 2> ' . realpath($this->localPath) . "/{$destination}");
                break;
        }
    }

    /**
     * {@inheritdoc}
     *
     * @param  string $path
     * @return mixed
     */
    public function requireFile($path)
    {
        // Another possible approach:
        //
        // $code = $this->filesystem->read($this->localPath . "/{$path}");
        // return eval(str_replace('<?php ', '', $code));

        return require $this->localPath . "/{$path}";
    }

    /**
     * {@inheritdoc}
     *
     * @return void
     *
     * @throws Flysystem\RootViolationException
     */
    public function clean()
    {
        $this->filesystem->deleteDir($this->localPath);
    }

    /**
     * {@inheritdoc}
     *
     * This function does not perform operations in the temporary
     * directory but it caches the results to avoid reprocessing.
     *
     * @param  string  $algorithm
     * @param  string  $sourceFile
     * @param  boolean $useCache
     * @return string
     *
     * @throws Flysystem\FileExistsException
     * @throws Flysystem\FileNotFoundException
     */
    public function getHashFile($algorithm, $sourceFile, $useCache = false)
    {
        if ($useCache == false) {
            return hash_file($algorithm, $sourceFile);
        }

        $cacheFile = sprintf(
            '%s/%s/%s.%s',
            $this->localPath,
            Zephir::VERSION,
            $this->normalizePath($sourceFile),
            $algorithm
        );

        if ($this->filesystem->has($cacheFile) == false) {
            $contents = hash_file($algorithm, $sourceFile);
            $this->filesystem->write($cacheFile, $contents);
            return $contents;
        } elseif ($this->filesystem->getTimestamp($sourceFile) > $this->filesystem->getTimestamp($cacheFile)) {
            $contents = hash_file($algorithm, $sourceFile);
            $this->filesystem->update($cacheFile, $contents);
            return $contents;
        } else {
            return $this->filesystem->read($cacheFile);
        }
    }

    /**
     * {@inheritdoc}
     *
     * @param  string $path
     * @return string
     */
    public function normalizePath($path)
    {
        return str_replace(['\\', ':', '/'], '_', $path);
    }
}
