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

namespace Zephir\Variable;

use function in_array;

/**
 * Represents a central knowledge base on global variables.
 */
final class Globals
{
    /**
     * Names of super global variables.
     */
    protected array $superGlobals = [
        '_GET',
        // The variables passed to the current script via the URL parameters.
        '_POST',
        // The variables passed to the current script via the HTTP POST method.
        '_ENV',
        // An array in which the environment variable names are element keys, and the environment variable value strings are element values.
        '_COOKIE',
        // The variables passed to the current script via HTTP Cookies.
        '_SERVER',
        // Server and execution environment information, such as headers, paths, and script locations.
        '_SESSION',
        // The session variables available to the current script.
        '_REQUEST',
        // By default, contains the contents of $_COOKIE, $_GET, and $_POST.
        '_FILES',
        // The items uploaded to the current script via the HTTP POST method.
    ];

    /**
     * Checks if a variable is a super global.
     */
    public function isSuperGlobal(string $name): bool
    {
        return in_array($name, $this->superGlobals, true);
    }
}
