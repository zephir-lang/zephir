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

namespace Zephir\Test\Compiler;

use PHPUnit\Framework\TestCase;
use Psr\Log\Test\TestLogger;
use Zephir\Backend\Backend;
use Zephir\Compiler;
use Zephir\Compiler\CompilerFileFactory;
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;
use Zephir\Zephir;

/**
 * Covers external-dependency class resolution on case-sensitive filesystems.
 *
 * @see https://github.com/zephir-lang/zephir/pull/2499
 */
final class LoadExternalClassTest extends TestCase
{
    private string $outputBase;

    protected function setUp(): void
    {
        $this->outputBase = sys_get_temp_dir() . '/zephir-ext-' . uniqid('', true);
        // HardDisk writes the parser cache under <base>/<Zephir::VERSION>/.
        mkdir($this->outputBase . DIRECTORY_SEPARATOR . Zephir::VERSION, 0o777, true);
    }

    protected function tearDown(): void
    {
        $this->removeRecursively($this->outputBase);
    }

    public function testResolvesClassUsingNamespaceCasingAsWritten(): void
    {
        // Namespace written PSR-4 style; file lives at Acme/Support/ExternalThing.zep.
        // The historic code lower-cased the whole path (acme/support/externalthing.zep),
        // which does not exist on a case-sensitive filesystem.
        $compiler = $this->createCompiler();
        $compiler->addExternalDependency('Acme', $this->externalLocation());

        $this->assertTrue($compiler->isClass('Acme\Support\ExternalThing'));
        $this->assertTrue(
            $compiler->loadExternalClass('Acme\Support\ExternalThing', $this->externalLocation())
        );
    }

    public function testResolvesLowercaseLayoutForBackwardCompatibility(): void
    {
        // File stored at an all-lowercase path is still found via the fallback.
        $compiler = $this->createCompiler();
        $compiler->addExternalDependency('Legacy', $this->externalLocation());

        $this->assertTrue($compiler->isClass('Legacy\Widget'));
    }

    public function testReturnsFalseWhenNoFileMatches(): void
    {
        // Must fail cleanly and must not recurse forever when already lowercase.
        $compiler = $this->createCompiler();
        $location = $this->externalLocation();

        $this->assertFalse($compiler->loadExternalClass('Acme\Support\DoesNotExist', $location));
        $this->assertFalse($compiler->loadExternalClass('acme\support\doesnotexist', $location));
    }

    public function testAlreadyLoadedClassIsReturnedFromRegistry(): void
    {
        $compiler = $this->createCompiler();
        $location = $this->externalLocation();

        $this->assertTrue($compiler->loadExternalClass('Acme\Support\ExternalThing', $location));
        // Reloading short-circuits via the registry key (no second parse).
        $this->assertTrue($compiler->loadExternalClass('Acme\Support\ExternalThing', $location));
        // Once loaded, the class resolves case-insensitively everywhere else.
        $this->assertTrue($compiler->isClass('acme\support\externalthing'));
    }

    private function createCompiler(): Compiler
    {
        $logger          = new TestLogger();
        $config          = new Config();
        $backend         = new Backend($config, 'kernel', 'templates');
        $disk            = new HardDisk($this->outputBase);
        $compilerFactory = new CompilerFileFactory($config, $disk, $logger);

        return new Compiler($config, $backend, new Manager(new Parser()), $disk, $compilerFactory);
    }

    private function externalLocation(): string
    {
        return realpath(__DIR__ . '/../../fixtures/external');
    }

    private function removeRecursively(string $path): void
    {
        if (!is_dir($path)) {
            return;
        }

        foreach (scandir($path) as $entry) {
            if ('.' === $entry || '..' === $entry) {
                continue;
            }
            $full = $path . DIRECTORY_SEPARATOR . $entry;
            is_dir($full) ? $this->removeRecursively($full) : @unlink($full);
        }

        @rmdir($path);
    }
}
