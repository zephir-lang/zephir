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

    public function __construct($basePath = '.temp/')
    {
        $this->basePath = $basePath;
    }

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

    public function read($path)
    {
        return file_get_contents($this->basePath . $path);
    }

    public function write($path, $data)
    {
        return file_put_contents($this->basePath . $path, $data);
    }

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

    public function requireFile($path)
    {
        return require $this->basePath . $path;
    }

    public function clean()
    {
        system('rm -fr ' . $this->basePath);
    }
}
