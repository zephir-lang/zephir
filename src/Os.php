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

class Os
{
    /**
     * Checks if currently running under MS Windows.
     */
    public static function isWindows(): bool
    {
        return 0 === stripos(\PHP_OS, 'WIN');
    }
}
