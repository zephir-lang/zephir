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
 * Apc
 *
 * Uses APC as filesystem for temporary operations
 * APC creates a new memory pool when a CLI process is created so this
 * adapter is useless
 */
class Apc
{
    protected $basePrefix;

    protected $initialized = false;

    /**
     * Apc constructor
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
        return apc_exists($this->basePrefix . $path);
    }

    /**
     * Creates a directory inside the temporary container
     *
     * @param string $path
     * @return boolean
     */
    public function makeDirectory($path)
    {
        return apc_store($this->basePrefix . $path);
    }

    /**
     * Returns a temporary entry as an array
     *
     * @param string $path
     * @return array
     */
    public function file($path)
    {
        return explode("\n", apc_fetch($this->basePrefix . $path));
    }

    /**
     * Returns the modification time of a temporary  entry
     *
     * @param string $path
     * @return boolean
     */
    public function modificationTime($path)
    {
        return apc_fetch($this->basePrefix . $path . '-mtime');
    }

    /**
     * Writes data from a temporary entry
     *
     * @param string $path
     */
    public function read($path)
    {
        return apc_fetch($this->basePrefix . $path);
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
        $status = apc_store($this->basePrefix . $path, $data);
        apc_store($this->basePrefix . $path . '-mtime', time());
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
        apc_store($this->basePrefix . $destination, file_get_contents($tempDestination));
        apc_store($this->basePrefix . $destination . '-mtime', time());
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
        $code = apc_fetch($this->basePrefix . $path);
        return eval(str_replace('<?php ', '', $code));
    }

    /**
     * Deletes the temporary directory
     */
    public function clean()
    {
        foreach (new \APCIterator('user') as $counter) {
            apc_delete($counter['key']);
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
            if (!apc_exists($cacheFile)) {
                $hash = hash_file($algorithm, $path);
                apc_store($cacheFile, $hash);
                apc_store($cacheFile . '-mtime', $hash);
                $changed = true;
            } else {
                if (filemtime($path) > filemtime($cacheFile)) {
                    $hash = hash_file($algorithm, $path);
                    apc_store($cacheFile, $hash);
                    apc_store($cacheFile . '-mtime', $hash);
                    $changed = true;
                }
            }

            if (!$changed) {
                return apc_fetch($cacheFile);
            }

            return $hash;
        }
    }
}
