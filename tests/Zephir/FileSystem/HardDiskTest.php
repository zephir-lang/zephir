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

namespace Zephir\Test\FileSystem;

use PHPUnit\Framework\TestCase;
use Zephir\Exception\InvalidArgumentException;
use Zephir\FileSystem\HardDisk;

final class HardDiskTest extends TestCase
{
    private string $tempDir;

    protected function setUp(): void
    {
        $this->tempDir = sys_get_temp_dir() . '/zephir_test_' . uniqid();
        mkdir($this->tempDir, 0755, true);
    }

    protected function tearDown(): void
    {
        if (is_dir($this->tempDir)) {
            $this->removeDirectory($this->tempDir);
        }
    }

    private function removeDirectory(string $dir): void
    {
        if (!is_dir($dir)) {
            return;
        }

        $files = array_diff(scandir($dir), ['.', '..']);
        foreach ($files as $file) {
            $path = $dir . '/' . $file;
            is_dir($path) ? $this->removeDirectory($path) : unlink($path);
        }
        rmdir($dir);
    }

    public function testShouldCreateHardDiskInstance(): void
    {
        $hardDisk = new HardDisk($this->tempDir, 'test');

        $this->assertInstanceOf(HardDisk::class, $hardDisk);
    }

    public function testShouldReturnAbsolutePathOfAnEntry(): void
    {
        $hardDisk = new HardDisk($this->tempDir, 'test');

        $this->assertSame(
            $this->tempDir . DIRECTORY_SEPARATOR . 'test' . DIRECTORY_SEPARATOR . 'zephir_pch.h',
            $hardDisk->path('zephir_pch.h')
        );
    }

    public function testShouldReturnContainerPathForAnEmptyEntry(): void
    {
        $hardDisk = new HardDisk($this->tempDir, 'test');

        $this->assertSame($this->tempDir . DIRECTORY_SEPARATOR . 'test', $hardDisk->path());
    }

    public function testShouldReturnUnversionedPathBesideTheContainer(): void
    {
        $hardDisk = new HardDisk($this->tempDir, 'test');

        $this->assertSame(
            $this->tempDir . DIRECTORY_SEPARATOR . 'pch/zephir_pch.h',
            $hardDisk->path('pch/zephir_pch.h', false)
        );
    }

    /**
     * The pre-compiled header lives beside the per-version containers and is
     * hundreds of megabytes, so a clean has to reclaim it too.
     */
    public function testShouldCleanUnversionedEntriesAsWell(): void
    {
        $hardDisk = new HardDisk($this->tempDir, 'test');
        $hardDisk->makeDirectory('.');
        mkdir($this->tempDir . '/pch', 0755, true);
        file_put_contents($this->tempDir . '/pch/zephir_pch.h.gch', 'object');

        $hardDisk->clean();

        $this->assertDirectoryDoesNotExist($this->tempDir . '/pch');
        $this->assertDirectoryDoesNotExist($this->tempDir . '/test');
    }

    public function testShouldThrowExceptionForEmptyLocalPath(): void
    {
        $this->expectException(InvalidArgumentException::class);
        $this->expectExceptionMessage('The temporary container can not be empty.');

        new HardDisk($this->tempDir, '');
    }

    public function testShouldCheckIfFileExists(): void
    {
        $testFile = $this->tempDir . '/test.txt';
        file_put_contents($testFile, 'test content');

        $this->assertTrue(file_exists($testFile));
    }

    public function testExistsReturnsTrueWhenVersionContainerDirectoryExists(): void
    {
        $disk = new HardDisk($this->tempDir, 'v1');
        mkdir($this->tempDir . '/v1', 0755, true);

        // The version container is a directory; exists('.') must detect it.
        $this->assertTrue($disk->exists('.'));
    }

    public function testExistsReturnsFalseWhenVersionContainerIsMissing(): void
    {
        $disk = new HardDisk($this->tempDir, 'v1');

        $this->assertFalse($disk->exists('.'));
    }

    public function testExistsStillDetectsFilesInsideContainer(): void
    {
        $disk = new HardDisk($this->tempDir, 'v1');
        mkdir($this->tempDir . '/v1', 0755, true);
        file_put_contents($this->tempDir . '/v1/cache.txt', 'x');

        $this->assertTrue($disk->exists('cache.txt'));
        $this->assertFalse($disk->exists('missing.txt'));
    }

    public function testGetVersionReturnsLocalPath(): void
    {
        $disk = new HardDisk($this->tempDir, 'v2');

        $this->assertSame('v2', $disk->getVersion());
    }

    public function testGetPreviousVersionReturnsStaleDirectory(): void
    {
        mkdir($this->tempDir . '/v1', 0755, true);
        mkdir($this->tempDir . '/v2', 0755, true);
        $disk = new HardDisk($this->tempDir, 'v2');

        $this->assertSame('v1', $disk->getPreviousVersion());
    }

    public function testGetPreviousVersionIsNullWhenOnlyCurrentExists(): void
    {
        mkdir($this->tempDir . '/v2', 0755, true);
        $disk = new HardDisk($this->tempDir, 'v2');

        $this->assertNull($disk->getPreviousVersion());
    }

    public function testGetPreviousVersionIsNullWhenBaseIsEmpty(): void
    {
        $disk = new HardDisk($this->tempDir, 'v2');

        $this->assertNull($disk->getPreviousVersion());
    }
}
