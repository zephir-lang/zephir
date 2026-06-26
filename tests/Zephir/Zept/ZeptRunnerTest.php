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

namespace Zephir\Test\Zept;

use PHPUnit\Framework\TestCase;
use Zephir\Zept\ZeptRunner;

/**
 * Unit coverage for the platform-dependent bits of {@see ZeptRunner} that have
 * no build dependency, so they can run in the fast (extension-free) Zephir
 * suite on every OS. The full end-to-end build is exercised by
 * {@see \Zephir\Test\BlackBox\ZeptSuiteTest}.
 */
final class ZeptRunnerTest extends TestCase
{
    public function testResolvesLinuxSharedObjectPath(): void
    {
        $this->assertSame(
            'ext/modules/stub.so',
            ZeptRunner::relativeExtensionPath(false, 8, false, 'stub')
        );
    }

    public function testResolvesWindows64BitNonThreadSafeDll(): void
    {
        $this->assertSame(
            'ext/x64/Release/php_stub.dll',
            ZeptRunner::relativeExtensionPath(true, 8, false, 'stub')
        );
    }

    public function testResolvesWindows64BitThreadSafeDll(): void
    {
        $this->assertSame(
            'ext/x64/Release_TS/php_stub.dll',
            ZeptRunner::relativeExtensionPath(true, 8, true, 'stub')
        );
    }

    public function testResolvesWindows32BitDll(): void
    {
        $this->assertSame(
            'ext/Release/php_stub.dll',
            ZeptRunner::relativeExtensionPath(true, 4, false, 'stub')
        );

        $this->assertSame(
            'ext/Release_TS/php_stub.dll',
            ZeptRunner::relativeExtensionPath(true, 4, true, 'stub')
        );
    }
}
