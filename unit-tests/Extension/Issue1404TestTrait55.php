<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

trait Issue1404TestTrait55
{
    protected function onNotSuccessfulTest(\Exception $error)
    {
        $phpVer = "PHP_VERSION_ID:" . PHP_VERSION_ID . " (".PHP_MAJOR_VERSION .'.'.PHP_MINOR_VERSION.'.'.PHP_RELEASE_VERSION.')';
        fwrite(STDOUT, "{$phpVer}\nError: {$error}");
        throw $error;
    }
}
