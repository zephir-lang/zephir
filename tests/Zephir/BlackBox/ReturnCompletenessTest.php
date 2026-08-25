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
 * One predicate decides whether control can reach the end of a method body. It
 * gates the return-type error, `ZEPHIR_MM_RESTORE()` and the generator finish
 * all at once, so it has to be right in both directions: a false negative is a
 * bogus compile error, a false positive a leaked memory frame.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1704
 */
final class ReturnCompletenessTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-completeness-' . bin2hex(random_bytes(6));
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

    /**
     * An infinite loop with no `break` cannot be left except by returning.
     */
    public function testInfiniteLoopWithoutBreakIsComplete(): void
    {
        $this->assertAccepted(<<<'ZEP'
namespace Stub;

class Forever
{
    public function pick() -> int
    {
        loop {
            return 1;
        }
    }
}
ZEP);
    }

    /**
     * A `break` leaves the loop, and then the end of the method is reachable.
     */
    public function testInfiniteLoopWithBreakIsNotComplete(): void
    {
        $this->assertRejected(<<<'ZEP'
namespace Stub;

class Escapes
{
    public function pick(bool flag) -> int
    {
        loop {
            if flag {
                break;
            }

            return 1;
        }
    }
}
ZEP);
    }

    /**
     * A `for` over an empty iterand never enters its body, so the end of the
     * method is reachable. PHP returns null there and fails the return type.
     */
    public function testForLoopIsNotComplete(): void
    {
        $this->assertRejected(<<<'ZEP'
namespace Stub;

class Iterating
{
    public function pick(array items) -> int
    {
        var item;

        for item in items {
            return 1;
        }
    }
}
ZEP);
    }

    /**
     * Every body of a `try`/`catch` returns, so the statement does.
     */
    public function testTryCatchWhereEveryBodyReturnsIsComplete(): void
    {
        $this->assertAccepted(<<<'ZEP'
namespace Stub;

class Guarded
{
    public function pick(bool flag) -> int
    {
        var e;

        try {
            if flag {
                throw new \Exception("boom");
            }

            return 1;
        } catch \Exception, e {
            return 2;
        }
    }
}
ZEP);
    }

    /**
     * An empty `catch` swallows the exception and falls through to the end.
     */
    public function testTryCatchWithAnEmptyCatchIsNotComplete(): void
    {
        $this->assertRejected(<<<'ZEP'
namespace Stub;

class Swallowed
{
    public function pick() -> int
    {
        var e;

        try {
            return 1;
        } catch \Exception, e {
        }
    }
}
ZEP);
    }

    /**
     * An `if`/`elseif`/`else` chain whose every arm returns is complete - the
     * `elseif` arms used to be skipped entirely.
     */
    public function testExhaustiveElseIfChainIsComplete(): void
    {
        $this->assertAccepted(<<<'ZEP'
namespace Stub;

class Arms
{
    public function pick(int a) -> int
    {
        if a > 5 {
            return 10;
        } elseif a > 2 {
            return 20;
        } else {
            return 30;
        }
    }
}
ZEP);
    }

    /**
     * A non-returning `elseif` arm leaves the end reachable.
     */
    public function testElseIfChainWithANonReturningArmIsNotComplete(): void
    {
        $this->assertRejected(<<<'ZEP'
namespace Stub;

class Gappy
{
    public function pick(int a) -> int
    {
        var unused;

        if a > 5 {
            return 10;
        } elseif a > 2 {
            let unused = 1;
        } else {
            return 30;
        }
    }
}
ZEP);
    }

    /**
     * Dead code written after a `return` does not make the end reachable.
     */
    public function testStatementsAfterAReturnAreNotReachable(): void
    {
        $this->assertAccepted(<<<'ZEP'
namespace Stub;

class Dead
{
    public function pick() -> int
    {
        var unused;

        return 1;
        let unused = 2;
    }
}
ZEP);
    }

    private function assertAccepted(string $zep): void
    {
        $output = $this->generate($zep);

        $this->assertStringNotContainsString('Reached end of the method', $output);
    }

    private function assertRejected(string $zep): void
    {
        $output = $this->generate($zep);

        $this->assertStringContainsString('Reached end of the method', $output);
    }

    private function generate(string $zep): string
    {
        // Zephir requires the file name to match the class it declares.
        preg_match('/^class (\w+)/m', $zep, $matches);
        file_put_contents($this->projectDir . '/stub/' . strtolower($matches[1]) . '.zep', $zep);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        return $result['stdout'] . $result['stderr'];
    }
}
