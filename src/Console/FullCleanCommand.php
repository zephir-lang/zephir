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

namespace Zephir\Console;

use Zephir\Exception\FileSystemException;
use Zephir\Os;

final class FullCleanCommand
{
    public static function execute(): void
    {
        /**
         * TODO: Do we need a batch file for Windows like "clean" as used below?
         * TODO: The 'clean' file contains duplicated commands
         */
        try {
            if (Os::isWindows()) {
                system('cd ext && phpize --clean');
            } else {
                system('cd ext && phpize --clean > /dev/null');
                system('cd ext && ./clean > /dev/null');
            }
        } catch (FileSystemException $e) {
            fwrite(
                STDERR,
                sprintf(
                    "For reasons beyond Zephir's control, a filesystem error has occurred. " .
                    'Please note: On Linux/Unix systems the current user must have the delete and execute ' .
                    'permissions on the internal cache directory,  For more information see chmod(1) and chown(1). ' .
                    'System error was: %s',
                    $e->getMessage()
                )
            );
        }
    }
}
