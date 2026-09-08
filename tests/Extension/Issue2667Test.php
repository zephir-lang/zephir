<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Extension;

use Closure;
use PHPUnit\Framework\TestCase;
use stdClass;
use Stub\Closures;

/**
 * Target of a rebind. Userland on purpose: every Zephir class is a
 * ZEND_INTERNAL_CLASS, and PHP refuses to rebind a closure's scope to an
 * internal class, which is what Closure::call() always does.
 */
final class Issue2667Target
{
    public $_name = 'target';
    public $tally = null;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2667
 */
final class Issue2667Test extends TestCase
{
    public function testBindToKeepsAScalarCapture(): void
    {
        $closure = (new Closures())->issue2667Scalar(7);

        $this->assertSame(7, $closure());
        $this->assertSame(7, $closure->bindTo(new stdClass())());
    }

    public function testBindToKeepsAStringCapture(): void
    {
        $closure = (new Closures())->issue2667Str('abc');

        $this->assertSame('abc', $closure->bindTo(new stdClass())());
    }

    public function testBindToKeepsAnArrayCapture(): void
    {
        $closure = (new Closures())->issue2667Arr([1, 2, 3]);

        $this->assertSame([1, 2, 3], $closure->bindTo(new stdClass())());
    }

    public function testBindToKeepsAnObjectCaptureByHandle(): void
    {
        $box     = new stdClass();
        $closure = (new Closures())->issue2667Obj($box);

        $this->assertSame($box, $closure->bindTo(new stdClass())());
    }

    public function testBindToReplacesTheEnclosingThis(): void
    {
        $closure = (new Closures())->issue2667WithThis('tag');

        $this->assertSame('tag:default', $closure());
        $this->assertSame('tag:target', $closure->bindTo(new Issue2667Target())());
    }

    public function testBindToLeavesTheOriginalClosureUntouched(): void
    {
        $closure = (new Closures())->issue2667WithThis('tag');
        $rebound = $closure->bindTo(new Issue2667Target());

        $this->assertNotSame($closure, $rebound);
        $this->assertSame('tag:default', $closure());
    }

    public function testStaticBindKeepsCaptures(): void
    {
        $closure = (new Closures())->issue2667Scalar(7);

        $this->assertSame(7, Closure::bind($closure, new stdClass())());
    }

    public function testCallBindsAndInvokesWithCaptures(): void
    {
        $test = new Closures();

        $this->assertSame(7, $test->issue2667Scalar(7)->call(new Issue2667Target()));
        $this->assertSame('tag:target', $test->issue2667WithThis('tag')->call(new Issue2667Target()));
    }

    public function testByRefCaptureHeldByOneClosureIsCopiedOnRebind(): void
    {
        [$bump] = (new Closures())->issue2667ByRef();

        $rebound = $bump->bindTo(new stdClass());
        $rebound();
        $rebound();

        // The rebound closure advanced its own copy, not the original's slot.
        $this->assertSame(1, $bump());
    }

    public function testByRefCaptureSharedByTwoClosuresStaysSharedOnRebind(): void
    {
        [$bump, $read] = (new Closures())->issue2667ByRefShared();

        $rebound = $bump->bindTo(new stdClass());
        $rebound();
        $rebound();

        $this->assertSame(2, $read());
    }

    public function testCloneThenBindToKeepsCaptures(): void
    {
        $closure = clone (new Closures())->issue2667Scalar(7);

        $this->assertSame(7, $closure());
        $this->assertSame(7, $closure->bindTo(new stdClass())());
    }

    public function testCaptureLessClosureStillRebinds(): void
    {
        $closure = (new Closures())->issue2667Plain();

        $this->assertSame('plain', $closure->bindTo(new stdClass())());
    }

    public function testRebindingTwiceKeepsCaptures(): void
    {
        $closure = (new Closures())->issue2667Scalar(7);

        $this->assertSame(7, $closure->bindTo(new stdClass())->bindTo(new stdClass())());
    }

    public function testUnbindIsRefusedWhenTheBodyReadsThis(): void
    {
        $closure = (new Closures())->issue2667WithThis('tag');

        $this->assertNull(@Closure::bind($closure, null, null));
    }

    public function testUnbindIsAllowedWhenTheBodyIgnoresThis(): void
    {
        $closure = (new Closures())->issue2667Scalar(7);
        $unbound = Closure::bind($closure, null, null);

        $this->assertInstanceOf(Closure::class, $unbound);
        $this->assertSame(7, $unbound());
    }

    /**
     * zend_closure_get_debug_info() reads a field that only exists on a user
     * function, which used to segfault for every closure Zephir compiles.
     */
    public function testDebugInfoReportsTheClosureInsteadOfCrashing(): void
    {
        $dump = print_r((new Closures())->issue2667Scalar(7), true);

        $this->assertStringContainsString('__invoke', $dump);
        $this->assertStringContainsString('[n] => 7', $dump);
    }

    public function testDebugInfoOfACaptureLessClosureDoesNotCrash(): void
    {
        $this->assertStringContainsString(
            '__invoke',
            print_r((new Closures())->issue2667Plain(), true),
        );
    }

    public function testDebugInfoOfAReboundClosureDoesNotCrash(): void
    {
        $test = new Closures();

        $this->assertStringContainsString(
            '[n] => 7',
            print_r($test->issue2667Scalar(7)->bindTo(new stdClass()), true),
        );
        $this->assertStringContainsString(
            '__invoke',
            print_r($test->issue2667Plain()->bindTo(new stdClass()), true),
        );
    }

    public function testDebugInfoOfAClonedClosureDoesNotCrash(): void
    {
        $this->assertStringContainsString(
            '[n] => 7',
            print_r(clone (new Closures())->issue2667Scalar(7), true),
        );
    }

    /**
     * Known limitation, and PHP's own rule rather than Zephir's: a closure
     * cannot be rebound to the scope of an internal class, and every Zephir
     * class is one. The default-scope form, which is what the issue reports,
     * works.
     */
    public function testRebindingScopeToAZephirClassIsRefused(): void
    {
        $test    = new Closures();
        $closure = $test->issue2667Scalar(7);

        $this->assertNull(@$closure->bindTo($test, Closures::class));
    }

    /**
     * A closure Zephir did not create must behave exactly as it does without
     * the extension loaded.
     */
    public function testPlainPhpClosuresAreUnaffected(): void
    {
        $n       = 7;
        $closure = function () {
            return [$this->_name, $this->tally ?? null];
        };

        $target        = new Issue2667Target();
        $target->tally = $n;

        $this->assertSame(['target', 7], $closure->bindTo($target, Issue2667Target::class)());
        $this->assertSame(['target', 7], Closure::bind($closure, $target, Issue2667Target::class)());
        $this->assertSame(['target', 7], $closure->call($target));
    }
}
