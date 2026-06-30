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

    public function testUsageCommandIsolatesFromSystemIni(): void
    {
        $command = ZeptRunner::isolatedUsageCommand('php', '/ext-dir', [], '/tmp/build/stub.so', '/tmp/usage.php');

        // -n must come first so the system php.ini/conf.d (which may enable a
        // colliding same-named extension) is ignored.
        $this->assertSame(['php', '-n'], \array_slice($command, 0, 2));
        // Startup/error noise must go to stderr, not the asserted stdout.
        $this->assertContains('display_errors=stderr', $command);
        $this->assertContains('extension_dir=/ext-dir', $command);
        $this->assertContains('extension=/tmp/build/stub.so', $command);
        $this->assertSame('/tmp/usage.php', $command[\count($command) - 1]);
    }

    public function testUsageCommandLoadsFreshExtensionAfterCaseIniDirectives(): void
    {
        $command = ZeptRunner::isolatedUsageCommand(
            'php',
            '/ext-dir',
            ['extension=mysqli'],
            '/tmp/build/stub.so',
            '/tmp/usage.php'
        );

        // A case dependency declared via --INI-- (e.g. mysqli) must be loaded
        // before the freshly built extension that links against it.
        $mysqli = \array_search('extension=mysqli', $command, true);
        $fresh  = \array_search('extension=/tmp/build/stub.so', $command, true);

        $this->assertNotFalse($mysqli);
        $this->assertNotFalse($fresh);
        $this->assertLessThan($fresh, $mysqli);
    }

    public function testUsageCommandOmitsExtensionDirWhenUnknown(): void
    {
        $command = ZeptRunner::isolatedUsageCommand('php', '', [], '/tmp/build/stub.so', '/tmp/usage.php');

        foreach ($command as $arg) {
            $this->assertStringStartsNotWith('extension_dir=', $arg);
        }
    }
}
