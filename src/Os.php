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

namespace Zephir;

use function preg_match;
use function stripos;

use const PHP_OS;

class Os
{
    /**
     * Checks if currently running under MS Windows.
     */
    public static function isWindows(): bool
    {
        return 0 === stripos(PHP_OS, 'WIN');
    }

    /**
     * Checks if a path is absolute, on any platform: a leading slash or
     * backslash, a Windows drive (`C:\` or `C:/`), or a stream wrapper such as
     * `phar://`. A bare `C:out` is relative to the drive's current directory.
     *
     * Only the syntax is inspected, the path does not have to exist.
     */
    public static function isAbsolutePath(string $path): bool
    {
        return 1 === preg_match('#^(?:[/\\\\]|[A-Za-z]:[/\\\\]|[A-Za-z][A-Za-z0-9+.-]*://)#', $path);
    }
}
