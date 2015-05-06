<?php

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
