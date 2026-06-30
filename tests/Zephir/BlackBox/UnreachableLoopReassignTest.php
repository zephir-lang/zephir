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
 * A variable assigned a constant before a loop and then reassigned inside the
 * loop body must not trigger a false "Unreachable code" warning on a guard that
 * reads it, because subsequent iterations observe the new value.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1170
 */
final class UnreachableLoopReassignTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-unreachable-loop-' . bin2hex(random_bytes(6));
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

    public function testReassignedInsideLoopIsNotUnreachable(): void
    {
        $this->writeZep('looped.zep', <<<'ZEP'
namespace Stub;

class Looped
{
    public static function toPropertyString(array properties) -> string
    {
        var x, ret, k, v;

        let x = false;
        let ret = "{";
        for k, v in properties {
            if x {
                let ret .= ", ";
            }
            let x = true;
            let ret .= k . ": " . v;
        }
        return ret . "}";
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringNotContainsString('Unreachable code', $output);
    }

    public function testGenuineUnreachableStillWarns(): void
    {
        // No loop: x is constant false and never reassigned, so the guarded
        // block is genuinely unreachable and the warning must remain.
        $this->writeZep('plain.zep', <<<'ZEP'
namespace Stub;

class Plain
{
    public static function dead() -> string
    {
        var x, ret;

        let x = false;
        let ret = "a";
        if x {
            let ret = "b";
        }
        return ret;
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertStringContainsString('Unreachable code', $output);
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
