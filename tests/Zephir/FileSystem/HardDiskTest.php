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
}
