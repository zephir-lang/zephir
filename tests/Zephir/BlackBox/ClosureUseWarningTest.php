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
 * A variable whose only consumer is a closure's `use (...)` clause is used, so
 * it must not be reported as an unused variable.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2029
 */
final class ClosureUseWarningTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-closure-use-' . bin2hex(random_bytes(6));
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

    public function testAssignedLocalCapturedByClosure(): void
    {
        $this->writeZep('assigned.zep', <<<'ZEP'
namespace Stub;

class Assigned
{
    protected pdo;

    public function make() -> <\Closure>
    {
        var pdo = this->pdo;

        return function () use (pdo) {
            return pdo;
        };
    }
}
ZEP);

        $this->assertNoUnusedVariableWarning();
    }

    public function testCaptureInsideFunctionCallArgument(): void
    {
        // The shape reported in #2029: the closure is an argument of a call.
        $this->writeZep('mapped.zep', <<<'ZEP'
namespace Stub;

class Mapped
{
    protected pdo;

    public function getWords(array strings) -> array
    {
        var pdo = this->pdo;
        var words = array_map(function (a) use (pdo) {
            return pdo->quote(trim(a));
        }, strings);

        return words;
    }
}
ZEP);

        $this->assertNoUnusedVariableWarning();
    }

    public function testDeclaredOnlyLocalCapturedByClosure(): void
    {
        // Never assigned, so the capture is its only use. The same predicate
        // gates the C declaration, so this shape must count as a use too.
        $this->writeZep('declared.zep', <<<'ZEP'
namespace Stub;

class Declared
{
    public function make() -> <\Closure>
    {
        var x;

        return function () use (x) {
            return x;
        };
    }
}
ZEP);

        $this->assertNoUnusedVariableWarning();
    }

    public function testScalarLocalCapturedByClosure(): void
    {
        // A C scalar is boxed into a temp before it reaches the static property.
        $this->writeZep('scalar.zep', <<<'ZEP'
namespace Stub;

class Scalar
{
    public function make() -> <\Closure>
    {
        int n = 5;

        return function () use (n) {
            return n;
        };
    }
}
ZEP);

        $this->assertNoUnusedVariableWarning();
    }

    public function testReferenceAndConstCapturesAreCounted(): void
    {
        $this->writeZep('flags.zep', <<<'ZEP'
namespace Stub;

class Flags
{
    public function make() -> <\Closure>
    {
        var x;
        var y;

        let x = 1;
        let y = 2;

        return function () use (&x, const y) {
            return x + y;
        };
    }
}
ZEP);

        $this->assertNoUnusedVariableWarning();
    }

    public function testNestedClosureCapture(): void
    {
        // The inner lookup runs against the outer closure's __invoke symbol
        // table, where the capture lives as a local static. Only the diagnostic
        // is asserted here: a nested closure registers its inner class in the
        // module init without ever generating its file, so the shape does not
        // link - unrelated to the capture, and true without a `use` clause too.
        $this->writeZep('nested.zep', <<<'ZEP'
namespace Stub;

class Nested
{
    public function make() -> <\Closure>
    {
        var outer;

        let outer = "o";

        return function () use (outer) {
            return function () use (outer) {
                return outer;
            };
        };
    }
}
ZEP);

        $this->assertNoUnusedVariableWarning();
    }

    public function testCaptureOfVariableAssignedInConditionalBranch(): void
    {
        $this->writeZep('branched.zep', <<<'ZEP'
namespace Stub;

class Branched
{
    public function make(bool flag) -> <\Closure>
    {
        var x;

        if flag {
            let x = "y";
        }

        return function () use (x) {
            return x;
        };
    }
}
ZEP);

        $this->assertNoUnusedVariableWarning();
    }

    public function testGenuinelyUnusedLocalStillWarns(): void
    {
        // Positive control on the very predicate the capture now feeds:
        // without a `use (...)` clause the variable stays unused and must keep
        // warning. Without this assertion a harness that never sees warnings
        // would pass every case above vacuously.
        $this->writeZep('junk.zep', <<<'ZEP'
namespace Stub;

class Junk
{
    public function make() -> string
    {
        var junk;

        return "ok";
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringContainsString('unused-variable', $output);
        $this->assertStringContainsString('declared but not used', $output);
    }

    /**
     * A by-reference capture turns the enclosing local into a PHP reference,
     * which a parameter's C shape cannot become. It has to fail loudly instead
     * of emitting C that does not compile.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testByReferenceCaptureOfAParameterIsReportedAsCompilerError(): void
    {
        $this->writeZep('byrefparam.zep', <<<'ZEP'
namespace Stub;

class ByRefParam
{
    public function make(var seed) -> <\Closure>
    {
        return function () use (&seed) {
            let seed = seed + 1;

            return seed;
        };
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertNotSame(0, $result['exitCode'], $output);
        $this->assertStringContainsString("'seed'", $output);
        $this->assertStringContainsString('by reference', $output);
        $this->assertStringNotContainsString('Call to a member function', $output);
    }

    public function testUndeclaredCaptureIsReportedAsCompilerError(): void
    {
        $this->writeZep('undeclared.zep', <<<'ZEP'
namespace Stub;

class Undeclared
{
    public function make() -> <\Closure>
    {
        return function () use (nope) {
            return nope;
        };
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertNotSame(0, $result['exitCode'], $output);
        $this->assertStringContainsString("'nope'", $output);
        // Not a PHP-level failure inside the compiler.
        $this->assertStringNotContainsString('Call to a member function', $output);
        $this->assertStringNotContainsString('must be of type', $output);
    }

    private function assertNoUnusedVariableWarning(): void
    {
        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringNotContainsString('unused-variable', $output);
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
