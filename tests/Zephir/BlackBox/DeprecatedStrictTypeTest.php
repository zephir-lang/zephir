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
 * The `!` (strict type) modifier on an argument type is redundant now that PHP
 * enforces scalar argument types itself. `zephir generate` must emit a
 * deprecation notice for every `!`-typed parameter, regardless of the data type,
 * telling the author to remove it before a future parser stops recognizing it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2274
 */
final class DeprecatedStrictTypeTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-strict-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'])
        );
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testStrictIntParameterEmitsDeprecation(): void
    {
        $this->writeZep('strictint.zep', <<<'ZEP'
namespace Stub;

class StrictInt
{
    public function run(int! value)
    {
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $output = $result['stdout'] . $result['stderr'];
        $this->assertStringContainsString('deprecated-strict-type', $output);
        $this->assertStringContainsString("'value'", $output);
    }

    public function testStrictStringParameterEmitsDeprecation(): void
    {
        $this->writeZep('strictstr.zep', <<<'ZEP'
namespace Stub;

class StrictStr
{
    public function run(string! value)
    {
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $this->assertStringContainsString(
            'deprecated-strict-type',
            $result['stdout'] . $result['stderr']
        );
    }

    public function testNonStrictParameterEmitsNoDeprecation(): void
    {
        $this->writeZep('plain.zep', <<<'ZEP'
namespace Stub;

class Plain
{
    public function run(int value)
    {
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $this->assertStringNotContainsString(
            'deprecated-strict-type',
            $result['stdout'] . $result['stderr']
        );
    }

    public function testDeprecationCanBeSuppressed(): void
    {
        $this->writeZep('suppress.zep', <<<'ZEP'
namespace Stub;

class Suppress
{
    public function run(int! value)
    {
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi -Wdeprecated-strict-type', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $this->assertStringNotContainsString(
            'deprecated-strict-type',
            $result['stdout'] . $result['stderr']
        );
    }

    private function writeZep(string $relativePath, string $content): void
    {
        $full = $this->projectDir . '/stub/' . $relativePath;
        if (!is_dir(dirname($full))) {
            mkdir(dirname($full), 0777, true);
        }
        file_put_contents($full, $content);
    }
}
