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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Os;

final class OsTest extends TestCase
{
    public function testShouldDetectWindows(): void
    {
        $isWindows = Os::isWindows();

        $this->assertIsBool($isWindows);

        // The actual result depends on the OS running the test
        // We just verify it returns a boolean
        if (stripos(PHP_OS, 'WIN') === 0) {
            $this->assertTrue($isWindows);
        } else {
            $this->assertFalse($isWindows);
        }
    }

    /**
     * @return iterable<string, array{0: string, 1: bool}>
     */
    public static function pathProvider(): iterable
    {
        yield 'unix absolute'           => ['/tmp/out', true];
        yield 'windows drive backslash' => ['C:\\Users\\RUNNER~1\\out', true];
        yield 'windows drive slash'     => ['D:/a/zephir/templates/Api/sitemap.php', true];
        yield 'windows root-relative'   => ['\\temp\\out', true];
        yield 'stream wrapper'          => ['phar:///zephir.phar/templates/Api/sitemap.php', true];
        yield 'relative'                => ['doc/0.0.1', false];
        yield 'dot relative'            => ['./out', false];
        yield 'drive-relative'          => ['C:out', false];
        yield 'empty'                   => ['', false];
    }

    /**
     * @dataProvider pathProvider
     * @see https://github.com/zephir-lang/zephir/issues/2431
     */
    public function testShouldDetectAbsolutePath(string $path, bool $expected): void
    {
        $this->assertSame($expected, Os::isAbsolutePath($path));
    }
}
