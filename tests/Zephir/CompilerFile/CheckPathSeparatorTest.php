<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test\CompilerFile;

use PHPUnit\Framework\TestCase;

final class CheckPathSeparatorTest extends TestCase
{
    public function testExtendsClassThatDoesNotExist(): void
    {
        $configM4Path = realpath(__DIR__.'/../../../ext/config.m4');
        $configM4Contents = file_get_contents($configM4Path);

        $this->assertTrue(strpos($configM4Contents, 'stub/oo/abstractstatic.zep.c') !== false);
    }
}
