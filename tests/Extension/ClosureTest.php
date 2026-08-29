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

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Closures;

final class ClosureTest extends TestCase
{
    public function testUseCommand(): void
    {
        $test = new Closures();

        $this->assertSame(2, $test->testUseCommand()());
        $this->assertInstanceOf(\stdClass::class, $test->issue642());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1036
     */
    public function testIssue1036(): void
    {
        $test = new Closures();

        $test->issue1036SetArgument(true);
        $test->issue1036SetFunction(fn ($argument) => $argument);

        $this->assertTrue($test->issue1036Call());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2497
     */
    public function testIssue2497ClosureThis(): void
    {
        $test = new Closures();

        $closure = $test->issue2497ClosureThis();
        $this->assertInstanceOf(\Closure::class, $closure);
        $this->assertSame('hello', $closure());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2497
     */
    public function testIssue2497ClosureThisWithUse(): void
    {
        $test = new Closures();

        $closure = $test->issue2497ClosureThisWithUse('world');
        $this->assertInstanceOf(\Closure::class, $closure);
        $this->assertSame('hello:world', $closure());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2497
     */
    public function testIssue2497PropertyAccess(): void
    {
        $test = new Closures();

        $closure = $test->issue2497PropertyAccess();
        $this->assertSame('default', $closure());

        $test->issue2497SetName('custom');
        $closure2 = $test->issue2497PropertyAccess();
        $this->assertSame('custom', $closure2());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1873
     *
     * Original reporter's minimal repro from 2019: a closure returned from
     * a method that reads `this->property` should bind to the enclosing
     * instance and return the property's value when invoked.
     *
     * The exact scenario was resolved by the closure `this`/`use` support
     * added for #2497; this test pins the #1873 fixture to prevent regression.
     */
    public function testIssue1873ClosureUsesProperty(): void
    {
        $test = new Closures();

        $closure = $test->issue1873();
        $this->assertInstanceOf(\Closure::class, $closure);
        $this->assertSame('call from closure', $closure());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1873
     *
     * Variant: array-typed property accessed inside closure.
     */
    public function testIssue1873ClosureReadsArrayProperty(): void
    {
        $test = new Closures();
        $test->issue1036SetArgument(['a', 'b', 'c']);

        $closure = $test->issue1873ArrayProperty();
        $this->assertSame(['a', 'b', 'c'], $closure());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1873
     *
     * Variant: closure reads two properties and concatenates them.
     */
    public function testIssue1873ClosureReadsMultipleProperties(): void
    {
        $test = new Closures();
        $test->issue2497SetName('Hello');

        $closure = $test->issue1873MultipleProperties();
        $this->assertSame('Hello:call from closure', $closure());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1873
     *
     * Variant: closure WRITES to an enclosing property. The binding must
     * keep the same underlying instance so the mutation is visible after
     * the closure returns.
     */
    public function testIssue1873ClosureWritesProperty(): void
    {
        $test = new Closures();
        $test->issue2497SetName('initial');

        $writer = $test->issue1873PropertyWriter();
        $writer('mutated');

        // The reader from #2497 should now see the mutated value.
        $reader = $test->issue2497PropertyAccess();
        $this->assertSame('mutated', $reader());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1873
     *
     * Variant: combines #1873 (property access) with #2497 `use()` capture.
     * Both features must compose.
     */
    public function testIssue1873PropertyAndUseCompose(): void
    {
        $test = new Closures();

        $closure = $test->issue1873PropertyAndUse('prefix');
        $this->assertSame('prefix:call from closure', $closure());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2562
     *
     * Capturing a scalar `string`-typed variable in a closure via `use()`.
     */
    public function testIssue2562StringUse(): void
    {
        $test = new Closures();

        $closure = $test->issue2562StringUse('world');
        $this->assertInstanceOf(\Closure::class, $closure);
        $this->assertSame('hello:world', $closure());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2321
     *
     * `preg_replace_callback` with a private callback. The Zephir fixture
     * uses a closure-with-`this` wrapper around the private method (the
     * working idiom) rather than `[this, 'method']` which PHP's internal
     * callable-visibility walk-back rejects for Zephir frames.
     */
    public function testIssue2321PrivateCallback(): void
    {
        $test = new Closures();

        // No special chars — callback returns the (unmodified) match.
        $this->assertSame('hello', $test->issue2321CallPrivateCallback('hello'));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2321
     *
     * Variant: callback IS actually invoked (matches contain special chars),
     * proving private-method dispatch works through the closure wrapper.
     */
    public function testIssue2321PrivateCallbackInvoked(): void
    {
        $test = new Closures();

        $this->assertSame('hello%20world', $test->issue2321CallPrivateCallback('hello world'));
        $this->assertSame('%C3%A5lc%C3%B3', $test->issue2321CallPrivateCallback('ålcó'));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2321
     *
     * Variant: protected callback. Same idiom should work.
     */
    public function testIssue2321ProtectedCallback(): void
    {
        $test = new Closures();

        $this->assertSame('HELLO123', $test->issue2321ProtectedCallback('hello123'));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2321
     *
     * Variant: `array_map` with a private callback through a closure.
     */
    public function testIssue2321ArrayMapPrivateCallback(): void
    {
        $test = new Closures();

        $this->assertSame([2, 4, 6, 8], $test->issue2321ArrayMapPrivate([1, 2, 3, 4]));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2638
     *
     * Capturing a `string` local. Held as a zval, so it used to be boxed a
     * second time with ZVAL_STRING() and the extension failed to compile.
     */
    public function testIssue2638StringLocalUse(): void
    {
        $test = new Closures();

        $closure = $test->issue2638StringLocalUse();
        $this->assertInstanceOf(\Closure::class, $closure);
        $this->assertSame('abc', $closure());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2638
     *
     * Variant: a `string` parameter reassigned in the body is disqualified
     * from the native `zend_string *` strategy, so it reaches the capture as
     * a zval just like a local.
     */
    public function testIssue2638StringParamMutatedUse(): void
    {
        $test = new Closures();

        $closure = $test->issue2638StringParamMutatedUse('abc');
        $this->assertInstanceOf(\Closure::class, $closure);
        $this->assertSame('abc!', $closure());
    }

    /**
     * Two closures created from one source line must not share their capture.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652ScalarCaptureIsPerClosure(): void
    {
        $test = new Closures();

        $first  = $test->issue2652Scalar(1);
        $second = $test->issue2652Scalar(2);

        $this->assertSame(1, $first());
        $this->assertSame(2, $second());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652StringCaptureIsPerClosure(): void
    {
        $test = new Closures();

        $first  = $test->issue2652Str('one');
        $second = $test->issue2652Str('two');

        $this->assertSame('one', $first());
        $this->assertSame('two', $second());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652ArrayCaptureIsPerClosure(): void
    {
        $test = new Closures();

        $first  = $test->issue2652Arr(['a']);
        $second = $test->issue2652Arr(['b', 'c']);

        $this->assertSame(['a'], $first());
        $this->assertSame(['b', 'c'], $second());
    }

    /**
     * An object capture is by handle, so a later mutation is visible inside
     * the closure, but the two closures still hold different objects.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652ObjectCaptureIsByHandle(): void
    {
        $test = new Closures();

        $one        = new \stdClass();
        $one->value = 1;
        $two        = new \stdClass();
        $two->value = 2;

        $first  = $test->issue2652Obj($one);
        $second = $test->issue2652Obj($two);

        $one->value = 11;

        $this->assertSame(11, $first());
        $this->assertSame(2, $second());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652CaptureWithThisIsPerClosure(): void
    {
        $test = new Closures();

        $first  = $test->issue2652WithThis('one');
        $second = $test->issue2652WithThis('two');

        $this->assertSame('hello:one', $first());
        $this->assertSame('hello:two', $second());
    }

    /**
     * `return this->prop` inside a capturing closure must read the enclosing
     * object, not the capture carrier.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652ReturnPropertyReadsEnclosingObject(): void
    {
        $test = new Closures();

        $bare     = $test->issue2652ReturnProperty('');
        $suffixed = $test->issue2652ReturnProperty('!');

        $this->assertSame('default', $bare());
        $this->assertSame('default!', $suffixed());
    }

    /**
     * `return this` inside a capturing closure must return the enclosing
     * object, not the capture carrier.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652ReturnThisReturnsEnclosingObject(): void
    {
        $test = new Closures();

        $self = $test->issue2652ReturnThis('self');
        $tag  = $test->issue2652ReturnThis('plain');

        $this->assertSame($test, $self());
        $this->assertSame('plain', $tag());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652LoopCapturesPerIteration(): void
    {
        $test = new Closures();

        $closures = $test->issue2652Loop();

        $this->assertCount(3, $closures);
        $this->assertSame([0, 1, 2], array_map(static fn ($closure) => $closure(), $closures));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652CaptureIsASnapshot(): void
    {
        $test = new Closures();

        $this->assertSame(5, $test->issue2652Snapshot()());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652BodyMutationDoesNotPersist(): void
    {
        $test = new Closures();

        $closure = $test->issue2652BodyMutation(0);

        $this->assertSame(1, $closure());
        $this->assertSame(1, $closure());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652ByRefCaptureIsShared(): void
    {
        $test = new Closures();

        [$bump, $read] = $test->issue2652ByRefShared();

        $this->assertSame(1, $bump());
        $this->assertSame(2, $bump());
        $this->assertSame(2, $read());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652ByRefWriteIsVisibleOutside(): void
    {
        $test = new Closures();

        $this->assertSame(20, $test->issue2652ByRefWritesOut());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function testIssue2652ByRefSeesLaterOuterWrite(): void
    {
        $test = new Closures();

        $this->assertSame(42, $test->issue2652ByRefReadsLateWrite()());
    }
}
