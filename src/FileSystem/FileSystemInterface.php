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

interface FileSystemInterface
{
    /**
     * Attempts to remove recursively the temporary directory with all subdirectories and files.
     */
    public function clean(): void;

    /**
     * Deletes a temporary entry.
     *
     * @param string $path
     */
    public function delete(string $path);

    /**
     * Checks whether a temporary entry does exist.
     *
     * @param string $path
     *
     * @return bool
     */
    public function exists(string $path): bool;

    /**
     * Returns a temporary entry as an array.
     *
     * @param string $path
     *
     * @return array
     */
    public function file(string $path): array;

    /**
     * Generate a hash value using the contents of a given file.
     *
     * @param string $algorithm
     * @param string $sourceFile
     * @param bool   $useCache
     *
     * @return string
     */
    public function getHashFile(string $algorithm, string $sourceFile, bool $useCache = false): string;

    /**
     * Initialize the filesystem.
     */
    public function initialize();

    /**
     * Checks if the filesystem is initialized.
     *
     * @return bool
     */
    public function isInitialized(): bool;

    /**
     * Creates a directory inside the temporary container.
     *
     * @param string $path
     *
     * @return bool
     */
    public function makeDirectory(string $path): bool;

    /**
     * Returns the modification time of a temporary entry.
     *
     * @param string $path
     *
     * @return int
     */
    public function modificationTime(string $path): int;

    /**
     * Normalizes path to be used as a temporary entry.
     *
     * @param string $path
     *
     * @return string
     */
    public function normalizePath(string $path): string;

    /**
     * Writes data from a temporary entry.
     *
     * @param string $path
     *
     * @return string
     */
    public function read(string $path): string;

    /**
     * Requires a file from the temporary directory.
     *
     * @param string $path
     *
     * @return mixed
     */
    public function requireFile(string $path);

    /**
     * Executes a command and saves the result into a temporary entry.
     *
     * @param string $command
     * @param string $descriptor
     * @param string $destination
     */
    public function system(string $command, string $descriptor, string $destination);

    /**
     * Writes data into a temporary entry.
     *
     * @param string $path
     * @param string $data
     */
    public function write(string $path, string $data);
}
