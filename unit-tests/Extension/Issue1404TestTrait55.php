<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

/**
 * Test trait for Zephir function is_php_version(id)
 *
 * @package  Extension
 * @author   DanHunsaker <danhunsaker@gmail.com>
 * @license  MIT http://zephir-lang.com/license.html
 * @link     https://github.com/phalcon/zephir/issues/1404
 */
trait Issue1404TestTrait55
{
    protected function onNotSuccessfulTest(\Exception $error)
    {
        $phpVer = "PHP_VERSION_ID:" . PHP_VERSION_ID . " (".PHP_MAJOR_VERSION .'.'.PHP_MINOR_VERSION.'.'.PHP_RELEASE_VERSION.')';
        fwrite(STDOUT, "{$phpVer}\nError: {$error}");
        throw $error;
    }
}
