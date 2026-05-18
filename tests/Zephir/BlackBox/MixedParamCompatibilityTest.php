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
 * Passing a `string`/`bool`/`array`/`callable` argument to a `mixed`-typed
 * parameter must never trigger the `possible-wrong-parameter` warning —
 * `mixed` is by definition strictly more permissive than `variable`.
 * The compatibility matrix in `MethodCall` previously listed only
 * `variable` (omitting `mixed`), producing a false positive.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2512
 */
final class MixedParamCompatibilityTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-mixed-' . bin2hex(random_bytes(6));
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

    public function testStringArgumentOnMixedParameterEmitsNoWarning(): void
    {
        $this->writeZep('attrs.zep', <<<'ZEP'
namespace Stub;

class Attrs
{
    public function set(string name, var value)
    {
    }

    public function setMixed(string name, mixed value)
    {
    }

    public function caller()
    {
        string action;
        let action = "post";

        // string -> var: must not warn (control case).
        this->set("action", action);

        // string -> mixed: must also not warn (#2512).
        this->setMixed("action", action);
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $this->assertStringNotContainsString(
            'passing: string, expecting: mixed',
            $result['stdout'] . $result['stderr']
        );
    }

    public function testBoolArgumentOnMixedParameterEmitsNoWarning(): void
    {
        $this->writeZep('boolmix.zep', <<<'ZEP'
namespace Stub;

class BoolMix
{
    public function setMixed(mixed value)
    {
    }

    public function caller()
    {
        bool flag = true;
        this->setMixed(flag);
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $this->assertStringNotContainsString(
            'expecting: mixed',
            $result['stdout'] . $result['stderr']
        );
    }

    public function testArrayArgumentOnMixedParameterEmitsNoWarning(): void
    {
        $this->writeZep('arrmix.zep', <<<'ZEP'
namespace Stub;

class ArrMix
{
    public function setMixed(mixed value)
    {
    }

    public function caller()
    {
        array items = [];
        this->setMixed(items);
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $this->assertStringNotContainsString(
            'expecting: mixed',
            $result['stdout'] . $result['stderr']
        );
    }

    public function testGenuineIncompatibilityStillWarns(): void
    {
        // Sanity: relaxing string->mixed must NOT relax string->bool.
        $this->writeZep('still.zep', <<<'ZEP'
namespace Stub;

class Still
{
    public function takesBool(bool flag)
    {
    }

    public function caller()
    {
        string s;
        let s = "hi";
        this->takesBool(s);
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $this->assertStringContainsString(
            'possible-wrong-parameter',
            $result['stdout'] . $result['stderr']
        );
        $this->assertStringContainsString(
            'passing: string, expecting: bool',
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
