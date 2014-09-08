<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\FileSystem;

/**
 * HardDisk
 *
 * Uses the standard hard-disk as filesystem for temporary operations
 */
class HardDisk
{
    protected $basePath;

    protected $initialized = false;

    /**
     * HardDisk constructor
     *
     * @param string $basePath
     */
    public function __construct($basePath = '.temp/')
    {
        $this->basePath = $basePath;
    }

    /**
     * Checks if the filesystem is initialized
     *
     * @return boolean
     */
    public function isInitialized()
    {
        return $this->initialized;
    }

    public function initialize()
    {
        if (!is_dir($this->basePath)) {
            mkdir($this->basePath);
        }
        $this->basePath = realpath($this->basePath) . DIRECTORY_SEPARATOR;
        $this->initialized = true;
    }

    public function exists($path)
    {
        return file_exists($this->basePath . $path);
    }

    public function makeDirectory($path)
    {
        return mkdir($this->basePath . $path);
    }

    public function file($path)
    {
        return file($this->basePath . $path);
    }

    public function modificationTime($path)
    {
        return filemtime($this->basePath . $path);
    }

    /**
     * Writes data from a temporary entry
     *
     * @param string $path
     */
    public function read($path)
    {
        return file_get_contents($this->basePath . $path);
    }

    /**
     * Writes data into a temporary entry
     *
     * @param string $path
     * @param string $data
     */
    public function write($path, $data)
    {
        return file_put_contents($this->basePath . $path, $data);
    }

    /**
     * Executes a command and saves the result into a temporary entry
     *
     * @param string $command
     * @param string $descriptor
     * @param string $destination
     */
    public function system($command, $descriptor, $destination)
    {
        switch ($descriptor) {
            case 'stdout':
                system($command . ' > ' . $this->basePath . $destination);
                break;
            case 'stderr':
                system($command . ' 2> ' . $this->basePath . $destination);
                break;
        }
    }

    /**
     * Requires a file from the temporary directory
     *
     * @param string $path
     */
    public function requireFile($path)
    {
        return require $this->basePath . $path;
    }

    /**
     * Deletes the temporary directory
     */
    public function clean()
    {
        system('rm -fr ' . $this->basePath);
    }

    /**
     * This function does not perform operations in the temporary
     * directory but it caches the results to avoid reprocessing
     *
     * @param string $algorithm
     * @param string $path
     * @param boolean $cache
     */
    public function getHashFile($algorithm, $path, $cache = false)
    {
        if ($cache == false) {
            return hash_file($algorithm, $path);
        } else {

            $changed = false;
            $cacheFile = $this->basePath . str_replace('/', '_', $path) . '.md5';
            if (!file_exists($cacheFile)) {
                $hash = hash_file($algorithm, $path);
                file_put_contents($cacheFile, $hash);
                $changed = true;
            } else {
                if (filemtime($path) < filemtime($cacheFile)) {
                    $hash = hash_file($algorithm, $path);
                    file_put_contents($cacheFile, $hash);
                    $changed = true;
                }
            }

            if (!$changed) {
                return file_get_contents($cacheFile);
            }

            return $hash;
        }
    }
}
