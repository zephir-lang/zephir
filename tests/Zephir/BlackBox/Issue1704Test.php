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
 * A `break` written after a `return` inside a `case` is dead code that PHP
 * accepts without any diagnostic. Zephir must neither warn about it nor let it
 * stop the `switch` from satisfying the method's return-type hint.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1704
 */
final class Issue1704Test extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue1704-' . bin2hex(random_bytes(6));
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

    public function testDeadBreakAfterReturnDoesNotWarnAndSatisfiesReturnType(): void
    {
        $this->writeZep('runmode.zep', <<<'ZEP'
namespace Stub;

class Runmode
{
    public function pick(int value) -> string
    {
        switch value {
            case 123:
                return "abc";
                break;
            default:
                throw new \Exception("Invalid application runmode.");
        }
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringNotContainsString('Unreachable code', $output);
        $this->assertStringNotContainsString('Reached end of the method', $output);
    }

    public function testGenuinelyUnreachableStatementStillWarns(): void
    {
        // A `let` after the `return` is real dead code, not padding: keep warning.
        $this->writeZep('dead.zep', <<<'ZEP'
namespace Stub;

class Dead
{
    public function pick(int value) -> string
    {
        var unused;

        switch value {
            case 123:
                return "abc";
                let unused = 1;
            default:
                throw new \Exception("bad");
        }
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertStringContainsString('Unreachable code', $output);
    }

    public function testBreakEscapingTheSwitchDefeatsExhaustiveness(): void
    {
        // The `break` path leaves the switch and falls off the end of the
        // method, so the return-type hint is not satisfied. PHP fails the same
        // program at runtime with a TypeError.
        $this->writeZep('escaping.zep', <<<'ZEP'
namespace Stub;

class Escaping
{
    public function pick(int a, bool flag) -> int
    {
        switch a {
            case 1:
                if flag {
                    break;
                }

                return 1;
            default:
                throw new \Exception("bad");
        }
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertNotSame(0, $result['exitCode'], $output);
        $this->assertStringContainsString('Reached end of the method', $output);
    }

    public function testFallThroughIntoAReturningClauseIsExhaustive(): void
    {
        // `case 1` neither returns nor breaks, so it falls into `case 2`,
        // which returns. Every path therefore returns.
        $this->writeZep('through.zep', <<<'ZEP'
namespace Stub;

class Through
{
    public function pick(int a) -> int
    {
        var x;

        switch a {
            case 1:
                let x = 1;
            case 2:
                return 1;
            default:
                throw new \Exception("bad");
        }
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringNotContainsString('Reached end of the method', $output);
    }

    public function testExhaustiveIfElseifElseInsideAClauseIsExhaustive(): void
    {
        $this->writeZep('arms.zep', <<<'ZEP'
namespace Stub;

class Arms
{
    public function pick(int a) -> int
    {
        switch a {
            case 1:
                if a > 5 {
                    return 10;
                } elseif a > 2 {
                    return 20;
                } else {
                    return 30;
                }
            default:
                throw new \Exception("bad");
        }
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringNotContainsString('Reached end of the method', $output);
    }

    public function testSwitchWithoutDefaultIsNotExhaustive(): void
    {
        // Without a `default` a non-matching value skips the whole `switch`.
        // PHP fails the same program at runtime with a TypeError.
        $this->writeZep('nodefault.zep', <<<'ZEP'
namespace Stub;

class NoDefault
{
    public function pick(int a) -> int
    {
        switch a {
            case 1:
                return 1;
                break;
        }
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertNotSame(0, $result['exitCode'], $output);
        $this->assertStringContainsString('Reached end of the method', $output);
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
