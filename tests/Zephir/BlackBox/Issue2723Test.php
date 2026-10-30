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
 * Every shape of a static call whose method name, or class, is only known at
 * run time. The method name of `{className}::{name}()` was emitted as the bare
 * C name and read by value, so a parameter (a `zval *`, or a `zend_string *`
 * for a native string) did not compile; a dynamic name on a known class, and
 * every `{"name"}` form, compiled to nothing at all.
 *
 * Generate-only on purpose: one project per shape pins the emitted call, so
 * each case fails for its own reason. That the calls then behave as PHP's do is
 * pinned by Extension\Issue2723Test and the issue2723 zept.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2723
 */
final class Issue2723Test extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    /**
     * @return iterable<string, array{0: string, 1: string}>
     */
    public static function callProvider(): iterable
    {
        yield 'dynamic class, var parameter name' => [
            'public function m(var className, var name) { return {className}::{name}(); }',
            'ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, name, NULL, 0);',
        ];
        yield 'dynamic class, native string parameter name' => [
            'public function m(var className, string name) { return {className}::{name}(); }',
            'ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, &name_zv, NULL, 0);',
        ];
        yield 'dynamic class, mixed parameter name' => [
            'public function m(var className, mixed name) { return {className}::{name}(); }',
            'ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, name, NULL, 0);',
        ];
        yield 'dynamic class, local name' => [
            'public function m(var className) { var name; let name = "target"; return {className}::{name}(); }',
            'ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, &name, NULL, 0);',
        ];
        yield 'dynamic class, parameter name, arguments' => [
            'public function m(var className, var name, var a) { return {className}::{name}(a); }',
            'ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, name, NULL, 0, a);',
        ];
        yield 'dynamic class, parameter name, assigned' => [
            'public function m(var className, var name) { var r; let r = {className}::{name}(); return r; }',
            'ZEPHIR_CALL_CE_STATIC_ZVAL(&r, _0, name, NULL, 0);',
        ];
        yield 'dynamic class, parameter name, statement' => [
            'public function m(var className, var name) { {className}::{name}(); }',
            'ZEPHIR_CALL_CE_STATIC_ZVAL(NULL, _0, name, NULL, 0);',
        ];
        yield 'dynamic class, literal name, statement' => [
            'public function m(var className) { {className}::target(); }',
            'ZEPHIR_CALL_CE_STATIC(NULL, _0, "target", NULL, 0);',
        ];
        yield 'mixed class' => [
            'public function m(mixed className) { return {className}::target(); }',
            '_0 = zephir_fetch_class(className);',
        ];
        yield 'self, dynamic name' => [
            'public function m(var name) { return self::{name}(); }',
            'ZEPHIR_RETURN_CALL_SELF_ZVAL(stub_probe_ce, name, NULL, 0);',
        ];
        yield 'self, dynamic name, assigned' => [
            'public function m(var name) { var r; let r = self::{name}(); return r; }',
            'ZEPHIR_CALL_SELF_ZVAL(&r, stub_probe_ce, name, NULL, 0);',
        ];
        yield 'self, dynamic name, statement' => [
            'public function m(var name) { self::{name}(); }',
            'ZEPHIR_CALL_SELF_ZVAL(NULL, stub_probe_ce, name, NULL, 0);',
        ];
        yield 'parent, dynamic name' => [
            'public function m(var name) { return parent::{name}(); }',
            'ZEPHIR_RETURN_CALL_PARENT_ZVAL(stub_probe_ce, getThis(), name, NULL, 0);',
        ];
        yield 'parent, dynamic name, arguments' => [
            'public function m(var name, var a) { var r; let r = parent::{name}(a); return r; }',
            'ZEPHIR_CALL_PARENT_ZVAL(&r, stub_probe_ce, getThis(), name, NULL, 0, a);',
        ];
        yield 'named class, dynamic name' => [
            'public function m(var name) { return ProbeBase::{name}(); }',
            'ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(stub_probebase_ce, name, NULL, 0);',
        ];
        yield 'named class, dynamic name, statement' => [
            'public function m(var name) { ProbeBase::{name}(); }',
            'ZEPHIR_CALL_CE_STATIC_ZVAL(NULL, stub_probebase_ce, name, NULL, 0);',
        ];
        yield 'internal class, dynamic name' => [
            'public function m(var name) { return \ArrayObject::{name}(); }',
            'ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, name, NULL, 0);',
        ];
        yield 'self, string name' => [
            'public function m() { return self::{"target"}(); }',
            'ZEPHIR_RETURN_CALL_SELF("target", NULL, 0);',
        ];
        yield 'static, string name' => [
            'public function m() { return static::{"target"}(); }',
            'ZEPHIR_RETURN_CALL_STATIC("target", NULL, 0);',
        ];
        yield 'named class, string name' => [
            'public function m() { return ProbeBase::{"baseTarget"}(); }',
            'ZEPHIR_RETURN_CALL_CE_STATIC(stub_probebase_ce, "baseTarget", NULL, 0);',
        ];
        yield 'constant class, string name' => [
            'public function m() { return PROBEBASE::{"baseTarget"}(); }',
            'ZEPHIR_RETURN_CALL_CE_STATIC(stub_probebase_ce, "baseTarget", NULL, 0);',
        ];
        yield 'dynamic class, string name' => [
            'public function m(var className) { return {className}::{"target"}(); }',
            'ZEPHIR_RETURN_CALL_CE_STATIC(_0, "target", NULL, 0);',
        ];
        yield 'parent result into a parameter' => [
            'public function m(var p) { let p = parent::baseTarget(); return p; }',
            'ZEPHIR_CALL_PARENT(p, stub_probe_ce, getThis(), "baseTarget", NULL, 0);',
        ];
        yield 'named class, dynamic name, in a closure' => [
            'public function m() { return function (name) { return ProbeBase::{name}(); }; }',
            'ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(stub_probebase_ce, name, NULL, 0);',
        ];
    }

    /**
     * @dataProvider callProvider
     */
    public function testEmitsTheCall(string $method, string $expectedCall): void
    {
        $this->assertStringContainsString($expectedCall, $this->generate($method));
    }

    /**
     * PHP fetches the class before it evaluates a single argument, and stops at
     * a class it cannot find.
     */
    public function testFetchesTheClassBeforeTheArgumentsAndStopsIfItThrew(): void
    {
        $generated = $this->generate(
            'public function m(var className) { return {className}::target(this->sideEffect()); }'
            . ' public function sideEffect() { return 1; }'
        );

        $this->assertMatchesRegularExpression(
            '/_\d+ = zephir_fetch_class\(className\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{/',
            $generated
        );
        $this->assertLessThan(
            strpos($generated, '"sideEffect"'),
            strpos($generated, 'zephir_fetch_class(className)'),
            'The class is fetched before the argument is evaluated'
        );
    }

    /**
     * Inside a closure `self` and `parent` name the closure class, which the
     * literal forms already refuse to compile against.
     *
     * @return iterable<string, array{0: string, 1: string}>
     */
    public static function closureScopeProvider(): iterable
    {
        yield 'self'   => ['self::{name}()', 'Cannot call self::{name}() inside a closure'];
        yield 'parent' => ['parent::{name}()', 'class stub\\0__closure does not extend any class'];
    }

    /**
     * @dataProvider closureScopeProvider
     */
    public function testRejectsScopeKeywordsInAClosure(string $call, string $expectedError): void
    {
        $result = $this->runGenerate('public function m() { return function (name) { return ' . $call . '; }; }');

        $this->assertNotSame(0, $result['exitCode']);
        $this->assertStringContainsString($expectedError, $result['stdout'] . $result['stderr']);
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue2723-' . bin2hex(random_bytes(6));
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
    public static function baseTarget() -> string
    {
        return "base";
    }
}
ZEP);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    /**
     * @return array{exitCode: int, stdout: string, stderr: string}
     */
    private function runGenerate(string $methods): array
    {
        file_put_contents($this->projectDir . '/stub/probe.zep', <<<ZEP
namespace Stub;

use Stub\ProbeBase as PROBEBASE;

class Probe extends ProbeBase
{
    public static function target() -> string
    {
        return "probe";
    }

    {$methods}
}
ZEP);

        return $this->runZephir('generate --no-ansi', $this->projectDir);
    }

    private function generate(string $methods): string
    {
        $result = $this->runGenerate($methods);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        $generated = (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c');
        foreach (glob($this->projectDir . '/ext/stub/*__closure.zep.c') ?: [] as $closure) {
            $generated .= file_get_contents($closure);
        }

        return $generated;
    }
}
