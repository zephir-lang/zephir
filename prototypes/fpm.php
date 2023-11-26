<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */
if (function_exists('fastcgi_finish_request')) {
    return false;
}

/**
 * This function flushes all response data
 * to the client and finishes the request.
 *
 * @return bool
 */
function fastcgi_finish_request()
{
}
