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
 * A closure capture is a local of the closure, seeded from the capture
 * carrier when the closure is invoked. It used to be recognised by being both
 * `external` and `localOnly`, but `localOnly` already meant "a local the
 * local-context pass proved is never mutated through", so every statement that
 * mutates a variable in place rejected a capture, by value and by reference:
 *
 *     Cannot mutate variable 'items' because it is local only
 *
 * Borrowing the flag had a second effect: the ternary operator and a runtime
 * constant clear `localOnly` on the variable they write, which stopped the
 * capture from being read off the carrier at all.
 *
 * The runtime half is tests/zept/issue2736_closure_capture_mutation.zept.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2736
 */
final class Issue2736Test extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir = '';

    public static function mutationProvider(): array
    {
        $shapes = [
            'append'          => ['var items = ["k": 0];', 'let items[] = 1;'],
            'index'           => ['var items = ["k": 0];', 'let items["k"] = 1;'],
            'nested append'   => ['var items = ["k": []];', 'let items["k"][] = 1;'],
            'compound index'  => ['var items = ["k": 0];', 'let items["k"] += 1;'],
            'property incr'   => ['var items = new \stdClass();', 'let items->count++;'],
            'property decr'   => ['var items = new \stdClass();', 'let items->count--;'],
        ];

        $cases = [];
        foreach ($shapes as $shape => [$declaration, $statement]) {
            foreach (['by value' => 'items', 'by reference' => '&items'] as $kind => $capture) {
                $cases[$shape . ', ' . $kind] = [$declaration, $capture, $statement];
            }
        }

        return $cases;
    }

    /**
     * @dataProvider mutationProvider
     */
    public function testMutatingACaptureCompiles(string $declaration, string $capture, string $statement): void
    {
        $result = $this->generate(
            'public function run() -> <\Closure> { ' . $declaration
            . ' return function () use (' . $capture . ') { ' . $statement . ' return items; }; }'
        );
        $output = $result['stdout'] . $result['stderr'];

        $this->assertStringNotContainsString('local only', $output);
        $this->assertSame(0, $result['exitCode'], $output);
    }

    public function testAppendToAByValueCaptureWritesTheClosureLocal(): void
    {
        $this->generate(
            'public function run() -> <\Closure> { var items = [];'
            . ' return function () use (items) { let items[] = 1; return items; }; }'
        );

        $this->assertStringContainsString('zephir_array_append(&items, ', $this->closureCode());
    }

    public function testAppendToAByReferenceCaptureWritesTheSharedSlot(): void
    {
        $this->generate(
            'public function run() -> <\Closure> { var items = [];'
            . ' return function () use (&items) { let items[] = 1; return items; }; }'
        );

        $this->assertStringContainsString('zephir_array_append(Z_REFVAL_P(&items), ', $this->closureCode());
    }

    /**
     * The receiver used to be printed by its bare name, which is a `zval`, so
     * the C build failed for anything but `this` and a parameter.
     */
    public function testPropertyIncrementOnALocalPassesAPointer(): void
    {
        $this->generate(
            'public function run() { var box; let box = new \stdClass(); let box->count = 0;'
            . ' let box->count++; let box->count--; return box; }'
        );

        $code = $this->classCode();
        $this->assertStringContainsString('zephir_property_incr(&box, SL("count"))', $code);
        $this->assertStringContainsString('zephir_property_decr(&box, SL("count"))', $code);
    }

    public function testPropertyIncrementOnACapturePassesAPointer(): void
    {
        $this->generate(
            'public function run() -> <\Closure> { var box, shared; let box = new \stdClass(); let shared = box;'
            . ' return function () use (box, &shared) { let box->count++; let shared->count--; return box; }; }'
        );

        $code = $this->closureCode();
        $this->assertStringContainsString('zephir_property_incr(&box, SL("count"))', $code);
        $this->assertStringContainsString('zephir_property_decr(Z_REFVAL_P(&shared), SL("count"))', $code);
    }

    /**
     * `.=` used to name the reference itself instead of the value it shares,
     * in the enclosing method and inside the closure alike.
     */
    public function testConcatAssignOnAReferenceAppendsToTheSharedValue(): void
    {
        $this->generate(
            'public function run() -> <\Closure> { var s; let s = "a";'
            . ' let s .= "!"; let s .= 5; let s .= true;'
            . ' return function () use (&s) { let s .= "?"; return s; }; }'
        );

        $method = $this->classCode();
        $this->assertStringContainsString('zephir_concat_self_str(Z_REFVAL_P(&s), SL("!"))', $method);
        $this->assertStringContainsString('zephir_concat_self_long(Z_REFVAL_P(&s), 5)', $method);
        $this->assertStringContainsString('zephir_concat_self_bool(Z_REFVAL_P(&s), 1)', $method);
        $this->assertStringContainsString('zephir_concat_self_str(Z_REFVAL_P(&s), SL("?"))', $this->closureCode());
    }

    public static function writeThatUsedToDropTheCaptureProvider(): array
    {
        return [
            'ternary'          => ['let x = flip ? "a" : "b";'],
            'runtime constant' => ['let x = ISSUE_2736_RUNTIME_CONSTANT;'],
        ];
    }

    /**
     * @dataProvider writeThatUsedToDropTheCaptureProvider
     */
    public function testCaptureIsStillReadWhenTheBodyAssignsIt(string $write): void
    {
        $result = $this->generate(
            'public function run(var x) -> <\Closure> {'
            . ' return function (bool flip) use (x) { var before; let before = x; ' . $write
            . ' return [before, x]; }; }'
        );

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertMatchesRegularExpression(
            '/zephir_read_property\(&x, this_ptr, SL\("x"\)/',
            $this->closureCode()
        );
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue2736-' . bin2hex(random_bytes(6));
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

    private function classCode(): string
    {
        return (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c');
    }

    private function closureCode(): string
    {
        $files = glob($this->projectDir . '/ext/stub/*__closure.zep.c') ?: [];
        $this->assertNotEmpty($files, 'no closure class was generated');

        return implode("\n", array_map('file_get_contents', $files));
    }

    /**
     * @return array{exitCode: int, stdout: string, stderr: string}
     */
    private function generate(string $members): array
    {
        file_put_contents(
            $this->projectDir . '/stub/probe.zep',
            "namespace Stub;\n\nclass Probe\n{\n    " . $members . "\n}\n"
        );

        return $this->runZephir('generate --no-ansi', $this->projectDir);
    }
}
