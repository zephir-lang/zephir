<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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
 * Redis
 *
 * Uses Redis as filesystem for temporary operations
 */
class Redis
{
    protected $basePrefix;

    protected $initialized = false;

    /**
     * Redis constructor
     *
     * @param string $basePrefix
     */
    public function __construct($basePrefix = 'zephir-')
    {
        $this->basePrefix = $basePrefix;
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

    /**
     * Initialize the filesystem
     */
    public function initialize()
    {
        $this->redis = new \Redis;
        $this->redis->connect('127.0.0.1', 6379);
        $this->initialized = true;
    }

    /**
     * Checks whether a temporary entry does exist
     *
     * @param string $path
     * @return boolean
     */
    public function exists($path)
    {
        return $this->redis->exists($this->basePrefix . $path);
    }

    /**
     * Creates a directory inside the temporary container
     *
     * @param string $path
     * @return boolean
     */
    public function makeDirectory($path)
    {
        $this->redis->set($this->basePrefix . $path, true);
    }

    /**
     * Returns a temporary entry as an array
     *
     * @param string $path
     * @return array
     */
    public function file($path)
    {
        return explode("\n", $this->redis->get($this->basePrefix . $path));
    }

    /**
     * Returns the modification time of a temporary  entry
     *
     * @param string $path
     * @return boolean
     */
    public function modificationTime($path)
    {
        return $this->redis->get($this->basePrefix . $path . '-mtime');
    }

    /**
     * Writes data from a temporary entry
     *
     * @param string $path
     */
    public function read($path)
    {
        return $this->redis->get($this->basePrefix . $path);
    }

    /**
     * Writes data into a temporary entry
     *
     * @param string $path
     * @param string $data
     * @return boolean
     */
    public function write($path, $data)
    {
        $status = $this->redis->set($this->basePrefix . $path, $data);
        $this->redis->set($this->basePrefix . $path . '-mtime', time());
        return $status;
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
        $tempDestination = '.temp-cmd';
        switch ($descriptor) {
            case 'stdout':
                system($command . ' > ' . $tempDestination);
                break;
            case 'stderr':
                system($command . ' 2> ' . $tempDestination);
                break;
        }
        $this->redis->set($this->basePrefix . $destination, file_get_contents($tempDestination));
        $this->redis->set($this->basePrefix . $destination . '-mtime', time());
        @unlink('.temp-cmd');
    }

    /**
     * Requires a file from the temporary directory
     *
     * @param string $path
     * @return array
     */
    public function requireFile($path)
    {
        $code = $this->redis->get($this->basePrefix . $path);
        return eval(str_replace('<?php ', '', $code));
    }

    /**
     * Deletes the temporary directory
     */
    public function clean()
    {
        if (!$this->initialized) {
            $this->initialize();
        }
        foreach ($this->redis->keys('zephir-*') as $key) {
            $this->redis->delete($key);
        }
    }

    /**
     * This function does not perform operations in the temporary
     * directory but it caches the results to avoid reprocessing
     *
     * @param string $algorithm
     * @param string $path
     * @param boolean $cache
     * @return string
     */
    public function getHashFile($algorithm, $path, $cache = false)
    {
        if ($cache == false) {
            return hash_file($algorithm, $path);
        } else {
            $changed = false;
            $cacheFile = $this->basePrefix . str_replace('/', '_', $path) . '.md5';
            if (!$this->redis->exists($cacheFile)) {
                $hash = hash_file($algorithm, $path);
                $this->redis->set($cacheFile, $hash);
                $this->redis->set($cacheFile . '-mtime', time());
                $changed = true;
            } else {
                if (filemtime($path) < $this->redis->get($cacheFile . '-mtime')) {
                    $hash = hash_file($algorithm, $path);
                    $this->redis->set($cacheFile, $hash);
                    $this->redis->set($cacheFile . '-mtime', time());
                    $changed = true;
                }
            }

            if (!$changed) {
                return $this->redis->get($cacheFile);
            }

            return $hash;
        }
    }
}
