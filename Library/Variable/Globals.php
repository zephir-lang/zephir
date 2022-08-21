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

/**
 * Zephir\Variable\Globals.
 *
 * Represents a central knowledge base on global variables.
 */
class Globals
{
    /**
     * The variables passed to the current script via the URL parameters.
     */
    const GET = '_GET';

    /**
     * The variables passed to the current script via the HTTP POST method.
     */
    const POST = '_POST';

    /**
     * An array in which the environment variable names are element keys,
     * and the environment variable value strings are element values.
     */
    const ENV = '_ENV';

    /**
     * The variables passed to the current script via HTTP Cookies.
     */
    const COOKIE = '_COOKIE';

    /**
     * Server and execution environment information, such as headers, paths, and script locations.
     */
    const SERVER = '_SERVER';

    /**
     * The session variables available to the current script.
     */
    const SESSION = '_SESSION';

    /**
     * By default contains the contents of $_COOKIE, $_GET, and $_POST.
     */
    const REQUEST = '_REQUEST';

    /**
     * The items uploaded to the current script via the HTTP POST method.
     */
    const FILES = '_FILES';

    /**
     * Names of superglobal variables.
     *
     * @var array
     */
    protected $superglobals = [
        self::GET => 1,
        self::POST => 1,
        self::ENV => 1,
        self::COOKIE => 1,
        self::SERVER => 1,
        self::SESSION => 1,
        self::REQUEST => 1,
        self::FILES => 1,
    ];

    /**
     * Checks if a variable is a superglobal.
     *
     * @param string $name
     *
     * @return bool
     */
    public function isSuperGlobal($name)
    {
        return isset($this->superglobals[$name]);
    }
}
