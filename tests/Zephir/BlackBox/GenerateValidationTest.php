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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

/**
 * Equivalent of sharness `t0004-generate-errors.sh` and `t0004-generate-success.sh`.
 */
final class GenerateValidationTest extends TestCase
{
    use RunsZephirCommands;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testFailsWhenNoZephirFilesToCompile(): void
    {
        $cwd = $this->outputDir();
        $projectDir = $cwd . '/genfailure1';
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init genfailure1', $cwd)['exitCode']);

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        $this->assertSame(1, $result['exitCode']);
        $this->assertSame(
            $this->normalize((string) file_get_contents($this->fixturesDir() . '/genfailure1')),
            $this->normalize($result['stderr']),
        );
    }

    public function testFailsWhenRequiredExtensionsAreMissing(): void
    {
        $cwd = $this->fixturesDir() . '/deps';

        $result = $this->runZephir('generate --no-ansi', $cwd);

        $this->assertSame(1, $result['exitCode']);
        $this->assertSame(
            $this->normalize((string) file_get_contents($this->fixturesDir() . '/genfailure2')),
            $this->normalize($result['stderr']),
        );
    }

    private function normalize(string $value): string
    {
        return str_replace("\r\n", "\n", $value);
    }

    public function testDifferentProjectsDoNotShareCache(): void
    {
        $cwd = $this->outputDir();
        $gen1 = $cwd . '/gen1';
        $gen2 = $cwd . '/gen2';
        $this->cleanupPath($gen1);
        $this->cleanupPath($gen2);

        $this->assertSame(0, $this->runZephir('init gen1', $cwd)['exitCode']);
        file_put_contents(
            $gen1 . '/gen1/test.zep',
            "namespace Gen1;\nclass Test { }\n",
        );
        $this->assertSame(0, $this->runZephir('generate', $gen1)['exitCode']);
        $this->assertDirectoryExists($gen1 . '/.zephir');

        $this->removePath($gen1 . '/.zephir');

        $this->assertSame(0, $this->runZephir('init gen2', $cwd)['exitCode']);
        file_put_contents(
            $gen2 . '/gen2/test.zep',
            "namespace Gen2;\nclass Test { }\n",
        );
        $this->assertSame(0, $this->runZephir('generate', $gen2)['exitCode']);
        $this->assertDirectoryExists($gen2 . '/.zephir');
        $this->assertDirectoryDoesNotExist($gen1 . '/.zephir');
    }
}
