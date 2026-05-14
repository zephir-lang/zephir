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
}
