<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

/**
 * The parser accepts the three `yield` forms (bare, expression, key+value),
 * but the Zephir compiler does not yet generate generator bodies. The
 * statement-level handler must surface a precise, located diagnostic
 * naming the unimplemented feature and pointing at the issue. The
 * previous behavior was a cryptic `Unsupported statement: yield` plus
 * side-pass debug echoes that landed before the real error.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1849
 */
final class YieldStatementTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-yield-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        $this->setUpZephirProject();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testYieldExpressionReportsUnimplementedDiagnostic(): void
    {
        $this->writeZep('counter.zep', <<<'ZEP'
namespace Stub;

class Counter
{
    public function range(int n)
    {
        int i;
        for i in range(1, n) {
            yield i;
        }
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertNotSame(0, $result['exitCode']);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertStringContainsString(
            "'yield' is parsed but code generation for generator methods is not yet implemented",
            $output
        );
        $this->assertStringContainsString(
            'https://github.com/zephir-lang/zephir/issues/1849',
            $output
        );
        $this->assertStringContainsString('counter.zep', $output);

        $this->assertStringNotContainsString('Statement=yield', $output, 'side-pass debug echo must not surface');
        $this->assertStringNotContainsString('SSTI=yield', $output, 'side-pass debug echo must not surface');
        $this->assertStringNotContainsString('SCGP=yield', $output, 'side-pass debug echo must not surface');
        $this->assertStringNotContainsString('Unsupported statement: yield', $output, 'must hit YieldStatement, not factory default');
    }

    public function testBareYieldReportsUnimplementedDiagnostic(): void
    {
        $this->writeZep('bare.zep', <<<'ZEP'
namespace Stub;

class Bare
{
    public function ping()
    {
        yield;
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertNotSame(0, $result['exitCode']);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertStringContainsString(
            "'yield' is parsed but code generation for generator methods is not yet implemented",
            $output
        );
    }

    public function testKeyValueYieldReportsUnimplementedDiagnostic(): void
    {
        $this->writeZep('kv.zep', <<<'ZEP'
namespace Stub;

class Kv
{
    public function pairs()
    {
        yield "a", 1;
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertNotSame(0, $result['exitCode']);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertStringContainsString(
            "'yield' is parsed but code generation for generator methods is not yet implemented",
            $output
        );
    }

    public function testSiblingMethodWithoutYieldCompilesCleanly(): void
    {
        $this->writeZep('plain.zep', <<<'ZEP'
namespace Stub;

class Plain
{
    public function twice(int n) -> int
    {
        return n * 2;
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
    }

    private function setUpZephirProject(): void
    {
        mkdir($this->projectDir . '/stub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'])
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
