<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

require_once __DIR__.'/../Library/autoload.php';

// TODO: Bellow code will be removed after dropping support of PHP 5.x

if (!class_exists('\PHPUnit\TextUI\Command') && class_exists('PHPUnit_TextUI_Command')) {
    /* @noinspection PhpIgnoredClassAliasDeclaration */
    class_alias('PHPUnit_TextUI_Command', '\PHPUnit\TextUI\Command');
}

if (!class_exists('\PHPUnit\Framework\SkippedTestError') && class_exists('PHPUnit_Framework_SkippedTestError')) {
    /* @noinspection PhpIgnoredClassAliasDeclaration */
    class_alias('PHPUnit_Framework_SkippedTestError', '\PHPUnit\Framework\SkippedTestError');
}

if (!class_exists('\PHPUnit\Framework\MockObject\MockObject') && class_exists('PHPUnit_Framework_MockObject_MockObject')) {
    /* @noinspection PhpIgnoredClassAliasDeclaration */
    class_alias('PHPUnit_Framework_MockObject_MockObject', '\PHPUnit\Framework\MockObject\MockObject');
}

if (!class_exists('\PHPUnit\Framework\Error\Warning') && class_exists('PHPUnit_Framework_Error_Warning')) {
    /* @noinspection PhpIgnoredClassAliasDeclaration */
    class_alias('PHPUnit_Framework_Error_Warning', '\PHPUnit\Framework\Error\Warning');
}
