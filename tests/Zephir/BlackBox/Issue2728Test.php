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

use function bin2hex;
use function file_put_contents;
use function json_encode;
use function mkdir;
use function random_bytes;
use function sys_get_temp_dir;

/**
 * `--quiet` and `--verbose` never reached the switch that handles them, so
 * only `-q` and `-v` worked. Any of these flags placed before the command
 * also made the CLI parser read the command as the flag's value, so nothing
 * ran and the help screen was printed instead.
 *
 * `verbose` itself was read by nothing. It now shows the same detail as
 * `--trace` for a failed `generate`, and `-V` turns it back off.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2728
 */
final class Issue2728Test extends TestCase
{
    use RunsZephirCommands;

    /** A `!` typed parameter emits the `deprecated-strict-type` warning. */
    private const WARNS = <<<'ZEP'
        namespace Stub;

        class Strict
        {
            public function run(int! value)
            {
            }
        }
        ZEP;

    /** Reads an undeclared variable, so `generate` fails. */
    private const FAILS = <<<'ZEP'
        namespace Stub;

        class Broken
        {
            public function run()
            {
                var a;

                let a = b;

                return a;
            }
        }
        ZEP;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-2728-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testShouldPrintTheWarningWithoutQuiet(): void
    {
        $this->writeProject('strict.zep', self::WARNS);

        $result = $this->runZephir('generate', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stderr']);
        $this->assertStringContainsString('deprecated-strict-type', $result['stdout'] . $result['stderr']);
    }

    /**
     * @dataProvider quietProvider
     */
    public function testShouldSuppressTheWarning(string $args): void
    {
        $this->writeProject('strict.zep', self::WARNS);

        $result = $this->runZephir($args, $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stderr']);
        $this->assertFileExists($this->projectDir . '/ext/stub/strict.zep.c');
        $this->assertStringNotContainsString('deprecated-strict-type', $result['stdout'] . $result['stderr']);
    }

    public function quietProvider(): array
    {
        return [
            'short after the command'  => ['generate -q'],
            'long after the command'   => ['generate --quiet'],
            'short before the command' => ['-q generate'],
            'long before the command'  => ['--quiet generate'],
        ];
    }

    /**
     * @dataProvider verbosityBeforeCommandProvider
     */
    public function testShouldRunTheCommandAfterAVerbosityFlag(string $args): void
    {
        $this->writeProject('strict.zep', self::WARNS);

        $result = $this->runZephir($args, $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stderr']);
        $this->assertStringNotContainsString('Usage:', $result['stdout']);
        $this->assertFileExists($this->projectDir . '/ext/stub/strict.zep.c');
    }

    public function verbosityBeforeCommandProvider(): array
    {
        return [
            '-v'        => ['-v generate'],
            '--verbose' => ['--verbose generate'],
            '-V'        => ['-V generate'],
        ];
    }

    public function testShouldNotPrintATraceByDefault(): void
    {
        $this->writeProject('broken.zep', self::FAILS);

        $result = $this->runZephir('generate', $this->projectDir);

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringContainsString("'b'", $result['stderr']);
        $this->assertDoesNotMatchRegularExpression('/^#0 /m', $result['stderr']);
    }

    /**
     * @dataProvider verboseProvider
     */
    public function testShouldPrintATraceWhenVerbose(string $args): void
    {
        $this->writeProject('broken.zep', self::FAILS);

        $result = $this->runZephir($args, $this->projectDir);

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringContainsString("'b'", $result['stderr']);
        $this->assertMatchesRegularExpression('/^#0 /m', $result['stderr']);
    }

    public function verboseProvider(): array
    {
        return [
            'short after the command'  => ['generate -v'],
            'long after the command'   => ['generate --verbose'],
            'short before the command' => ['-v generate'],
        ];
    }

    public function testShouldPrintATraceWhenConfigIsVerbose(): void
    {
        $this->writeProject('broken.zep', self::FAILS, ['verbose' => true]);

        $result = $this->runZephir('generate', $this->projectDir);

        $this->assertSame(1, $result['exitCode']);
        $this->assertMatchesRegularExpression('/^#0 /m', $result['stderr']);
    }

    public function testShouldLetTheShortFlagDisableAVerboseConfig(): void
    {
        $this->writeProject('broken.zep', self::FAILS, ['verbose' => true]);

        $result = $this->runZephir('generate -V', $this->projectDir);

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringContainsString("'b'", $result['stderr']);
        $this->assertDoesNotMatchRegularExpression('/^#0 /m', $result['stderr']);
    }

    private function writeProject(string $file, string $source, array $settings = []): void
    {
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'] + $settings)
        );
        file_put_contents($this->projectDir . '/stub/' . $file, $source);
    }
}
