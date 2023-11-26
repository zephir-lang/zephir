<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */
if (!function_exists('apache_child_terminate')) {
    /**
     * Terminate apache process after this request.
     *
     * @see https://www.php.net/manual/en/function.apache-child-terminate.php
     *
     * @return bool
     */
    function apache_child_terminate()
    {
    }
}

if (!function_exists('apache_get_modules')) {
    /**
     * Get a list of loaded Apache modules.
     *
     * @see https://www.php.net/manual/en/function.apache-get-modules.php
     *
     * @return array
     */
    function apache_get_modules()
    {
    }
}

if (!function_exists('apache_get_version')) {
    /**
     * Fetch Apache version.
     *
     * @see https://www.php.net/manual/en/function.apache-get-version.php
     *
     * @return string
     */
    function apache_get_version()
    {
    }
}

if (!function_exists('apache_getenv')) {
    /**
     * Get an Apache subprocess_env variable.
     *
     * @see https://www.php.net/manual/en/function.apache-getenv.php
     *
     * @param string $variable
     * @param bool   $walk_to_top
     *
     * @return string
     */
    function apache_getenv($variable, $walk_to_top = false)
    {
    }
}

if (!function_exists('apache_lookup_uri')) {
    /**
     * Perform a partial request for the specified URI and return all info about it.
     *
     * @see https://www.php.net/manual/en/function.apache-lookup-uri.php
     *
     * @param string $filename
     *
     * @return object
     */
    function apache_lookup_uri($filename)
    {
    }
}

if (!function_exists('apache_note')) {
    /**
     * Get and set apache request notes.
     *
     * @see https://www.php.net/manual/en/function.apache-note.php
     *
     * @param string $note_name
     * @param string $note_value
     *
     * @return object
     */
    function apache_note($note_name, $note_value = '')
    {
    }
}

if (!function_exists('apache_request_headers')) {
    /**
     * Fetch all HTTP request headers.
     *
     * @see https://www.php.net/manual/en/function.apache-request-headers.php
     *
     * @return array|bool
     */
    function apache_request_headers()
    {
    }
}

if (!function_exists('apache_reset_timeout')) {
    /**
     * Reset the Apache write timer.
     *
     * @see https://www.php.net/manual/en/function.apache-reset-timeout.php
     *
     * @return bool
     */
    function apache_reset_timeout()
    {
    }
}

if (!function_exists('apache_response_headers')) {
    /**
     * Fetch all HTTP response headers.
     *
     * @see https://www.php.net/manual/en/function.apache-response-headers.php
     *
     * @return array|bool
     */
    function apache_response_headers()
    {
    }
}

if (!function_exists('apache_setenv')) {
    /**
     * Set an Apache subprocess_env variable.
     *
     * @see https://www.php.net/manual/en/function.apache-setenv.php
     *
     * @param string $variable
     * @param string $value
     * @param bool   $walk_to_top
     *
     * @return bool
     */
    function apache_setenv($variable, $value, $walk_to_top = false)
    {
    }
}

if (!function_exists('getallheaders')) {
    /**
     * Fetch all HTTP request headers.
     *
     * @see https://www.php.net/manual/en/function.getallheaders.php
     *
     * @return array|bool
     */
    function getallheaders()
    {
    }
}

if (!function_exists('virtual')) {
    /**
     * Perform an Apache sub-request.
     *
     * @see https://www.php.net/manual/en/function.virtual.php
     *
     * @param string $filename
     *
     * @return bool
     */
    function virtual($filename)
    {
    }
}
