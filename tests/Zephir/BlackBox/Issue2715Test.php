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
 * The name of a called method is lower-cased to look the method up in the class
 * definition, and the same lowered string used to be emitted as the C literal.
 * PHP defines the first argument of `__call()` as the name as written at the
 * call site, so the spelling has to survive compilation.
 *
 * Generate-only on purpose: this pins the emitted literal. That the spelling
 * then reaches `__call()` and the undefined-call errors is pinned by
 * tests/zept/issue2715_call_name_spelling.zept and Extension\Issue2715Test.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2715
 */
final class Issue2715Test extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    /**
     * @return iterable<string, array{0: string}>
     */
    public static function callProvider(): iterable
    {
        yield 'method'           => ['ZEPHIR_RETURN_CALL_METHOD(receiver, "NoPeCaSe"'];
        yield 'method in braces' => ['ZEPHIR_RETURN_CALL_METHOD(receiver, "AlSoMiXeD"'];
        yield 'class static'     => ['ZEPHIR_RETURN_CALL_CE_STATIC(stub_probebase_ce, "AnotherMiXeD"'];
        yield 'self'             => ['ZEPHIR_RETURN_CALL_SELF("SelfMiXeD"'];
        yield 'parent'           => ['ZEPHIR_RETURN_CALL_PARENT(stub_probe_ce, getThis(), "BaseMiXeD"'];
        yield 'function'         => ['ZEPHIR_RETURN_CALL_FUNCTION("no_SuCh_FuNcTiOn"'];
    }

    /**
     * @dataProvider callProvider
     */
    public function testCallKeepsTheNameAsWritten(string $expectedCall): void
    {
        $this->assertStringContainsString($expectedCall, $this->generateProbe());
    }

    public function testNoCallIsEmittedWithTheLoweredName(): void
    {
        $generated = $this->generateProbe();

        foreach (['nopecase', 'alsomixed', 'anothermixed', 'selfmixed', 'basemixed'] as $lowered) {
            $this->assertStringNotContainsString(
                '"' . $lowered . '"',
                $generated,
                'The lowered name is what __call() must never be handed.'
            );
        }
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue2715-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'])
        );
        file_put_contents($this->projectDir . '/stub/probebase.zep', <<<'ZEP'
namespace Stub;

class ProbeBase
{
    public function baseMiXeD() -> string
    {
        return "base";
    }

    public static function anotherMiXeD() -> string
    {
        return "another";
    }
}
ZEP);
        file_put_contents($this->projectDir . '/stub/probe.zep', <<<'ZEP'
namespace Stub;

class Probe extends ProbeBase
{
    public function callMethod(var receiver)
    {
        return receiver->NoPeCaSe();
    }

    public function callMethodInBraces(var receiver)
    {
        return receiver->{"AlSoMiXeD"}();
    }

    public function callClassStatic() -> string
    {
        return ProbeBase::AnotherMiXeD();
    }

    public static function selfMiXeD() -> string
    {
        return "self";
    }

    public function callSelf() -> string
    {
        return self::SelfMiXeD();
    }

    public function callParent() -> string
    {
        return parent::BaseMiXeD();
    }

    public function callFunction(var value)
    {
        return no_SuCh_FuNcTiOn(value);
    }
}
ZEP);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    private function generateProbe(): string
    {
        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        return (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c');
    }
}
