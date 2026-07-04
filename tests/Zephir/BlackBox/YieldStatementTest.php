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
 * Generator methods (`yield`) compile since issue #1849 landed: the parser
 * accepts the three forms (bare, expression, key+value) and the compiler
 * splits each generator method into a visible creator plus a hidden internal
 * step method resumed by the kernel <Ns>\Generator runtime. `zephir generate`
 * must succeed and emit the split, with no side-pass debug echoes.
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

    public function testYieldExpressionInRangeLoopGenerates(): void
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

        $output = $result['stdout'] . $result['stderr'];
        $this->assertSame(0, $result['exitCode'], $output);

        $generated = $this->projectDir . '/ext/stub/counter.zep.c';
        $this->assertFileExists($generated);
        $code = (string)file_get_contents($generated);

        $this->assertStringContainsString('zephir_generator_create(return_value', $code);
        $this->assertStringContainsString('void zep_Stub_Counter_zephir_gen_step_range(', $code);
        $this->assertStringContainsString('zephir_yield_resume_1:;', $code);

        $this->assertStringNotContainsString('Statement=yield', $output, 'side-pass debug echo must not surface');
        $this->assertStringNotContainsString('SSTI=yield', $output, 'side-pass debug echo must not surface');
        $this->assertStringNotContainsString('SCGP=yield', $output, 'side-pass debug echo must not surface');
        $this->assertStringNotContainsString('Unsupported statement: yield', $output);
    }

    public function testBareYieldGenerates(): void
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

        $output = $result['stdout'] . $result['stderr'];
        $this->assertSame(0, $result['exitCode'], $output);

        $code = (string)file_get_contents($this->projectDir . '/ext/stub/bare.zep.c');
        $this->assertStringContainsString('zephir_generator_yield(zephir_gen, NULL, NULL, 1);', $code);
    }

    public function testKeyValueYieldGenerates(): void
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

        $output = $result['stdout'] . $result['stderr'];
        $this->assertSame(0, $result['exitCode'], $output);

        $code = (string)file_get_contents($this->projectDir . '/ext/stub/kv.zep.c');
        $this->assertStringContainsString('zephir_generator_yield(zephir_gen, ', $code);
        $this->assertStringNotContainsString('zephir_generator_yield(zephir_gen, NULL', $code, 'key must be passed');
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

        $code = (string)file_get_contents($this->projectDir . '/ext/stub/plain.zep.c');
        $this->assertStringNotContainsString(
            'zephir_generator_create',
            $code,
            'non-generator methods must not be split'
        );
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
