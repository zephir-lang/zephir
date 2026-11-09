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

use function bin2hex;
use function dirname;
use function file_get_contents;
use function file_put_contents;
use function is_dir;
use function json_encode;
use function mkdir;
use function random_bytes;
use function sys_get_temp_dir;

/**
 * A throwaway `stub` extension project in the temp directory: a test writes
 * `.zep` sources into it and reads the C that `zephir generate` makes.
 */
trait GeneratesStubProject
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUpStubProject(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-stub-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'])
        );
    }

    /**
     * Writes each source under `stub/` and runs `zephir generate`.
     *
     * @param array<string, string> $files source by path relative to `stub/`
     *
     * @return array{exitCode: int, stdout: string, stderr: string}
     */
    private function generate(array $files): array
    {
        foreach ($files as $name => $source) {
            $path = $this->projectDir . '/stub/' . $name;
            if (!is_dir(dirname($path))) {
                mkdir(dirname($path), 0777, true);
            }

            file_put_contents($path, $source . "\n");
        }

        return $this->runZephir('generate --no-ansi', $this->projectDir);
    }

    /**
     * Generates one source, which has to succeed, and returns its C file.
     */
    private function generateOk(string $fileName, string $source): string
    {
        $result = $this->generate([$fileName => $source]);
        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        return (string) file_get_contents($this->projectDir . '/ext/stub/' . $fileName . '.c');
    }
}
