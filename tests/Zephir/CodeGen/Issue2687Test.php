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

namespace Zephir\Test\CodeGen;

use PHPUnit\Framework\TestCase;

/**
 * Every Compiler starts its own numbering: closure class names and call cache
 * slots must depend on the source, not on how many compilations the process
 * ran before.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2687
 */
final class Issue2687Test extends TestCase
{
    use CompilesZephirSource;

    private const SOURCE_DIRS = ['stub/issue2687'];

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_issue2687_test_', self::SOURCE_DIRS);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    public function testClosureNumberingRestartsPerCompiler(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2687;

class One
{
    public function cb() -> <\Closure>
    {
        return function () {
            return 1;
        };
    }
}
ZEP;

        $first = $this->compileOnce('Stub\\Issue2687\\One', 'stub/issue2687/one.zep', $zep);
        $this->assertStringContainsString('stub_0__closure_ce', $first);

        $this->restartCompiler();

        $second = $this->compileOnce('Stub\\Issue2687\\One', 'stub/issue2687/one.zep', $zep);
        $this->assertSame($first, $second);
    }

    public function testArrowClosureNumberingRestartsPerCompiler(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2687;

class Arrow
{
    public function cb() -> <\Closure>
    {
        return x => x + 1;
    }
}
ZEP;

        $first = $this->compileOnce('Stub\\Issue2687\\Arrow', 'stub/issue2687/arrow.zep', $zep);
        $this->assertStringContainsString('stub_0__closure_ce', $first);

        $this->restartCompiler();

        $second = $this->compileOnce('Stub\\Issue2687\\Arrow', 'stub/issue2687/arrow.zep', $zep);
        $this->assertSame($first, $second);
    }

    public function testCallCacheSlotsRestartPerCompiler(): void
    {
        $first = $this->compileOnce('Stub\\Issue2687\\Calls', 'stub/issue2687/calls.zep', $this->privateCall('a'));
        $this->assertStringContainsString('ZEPHIR_CALL_METHOD(NULL, this_ptr, "a", NULL, 1);', $first);

        $this->restartCompiler();

        $second = $this->compileOnce('Stub\\Issue2687\\Calls', 'stub/issue2687/calls.zep', $this->privateCall('b'));
        $this->assertStringContainsString('ZEPHIR_CALL_METHOD(NULL, this_ptr, "b", NULL, 1);', $second);
    }

    private function compileOnce(string $fqcn, string $relPath, string $zep): string
    {
        $this->compileSource($fqcn, $relPath, $zep);

        return $this->generatedC($relPath);
    }

    private function privateCall(string $method): string
    {
        return <<<ZEP
namespace Stub\Issue2687;

class Calls
{
    public function run() -> void
    {
        this->{$method}();
    }

    private function {$method}() -> void
    {
    }
}
ZEP;
    }

    /**
     * A fresh Compiler in a fresh project, inside the same process: what an
     * embedder calling generate() twice does.
     */
    private function restartCompiler(): void
    {
        $this->tearDownCodeGen();
        $this->setUpCodeGen('zephir_issue2687_test_', self::SOURCE_DIRS);
    }
}
