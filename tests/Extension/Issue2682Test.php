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

use ArrayAccess;
use Issue2682DefaultsHolder;
use Issue2682FreshArrayContainer;
use Issue2682FreshValueContainer;
use Issue2682ProbeContainer;
use PHPUnit\Framework\TestCase;
use Stub\Issue2682;

/**
 * A read-only subscript read of an ArrayAccess object used to apply
 * Z_TRY_DELREF_P() to the value offsetGet() owns.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2682
 */
final class Issue2682Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2682 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2682();
    }

    /**
     * The `fetch` family, kernel/array.c zephir_array_isset_string_fetch().
     *
     * Released early, the copy taken inside the method holds the only
     * reference and dropping it destroys the value while the target still
     * points at it. The method reports what the destructor saw rather than
     * reading the freed slot, which is undefined behaviour and does not
     * reliably abort.
     */
    public function testFetchDoesNotReleaseTheValueOffsetGetOwns(): void
    {
        $container = new Issue2682ProbeContainer();

        $this->assertFalse(
            $this->subject->fetchReleasedEarly($container),
            'The fetched value was destroyed before the method that fetched it returned.'
        );
    }

    /**
     * The other half of the same contract: released once, not leaked. The
     * memory frame owns the value, so the destructor runs when the method
     * returns and not before.
     */
    public function testFetchReleasesTheValueWhenTheMethodReturns(): void
    {
        $container = new Issue2682ProbeContainer();

        $this->subject->fetchReleasedEarly($container);

        $this->assertTrue($container->destroyed, 'The fetched value was never released.');
    }

    /**
     * The array-access family, kernel/array.c zephir_array_fetch_string(),
     * reached by `let value = container["namespace"]`.
     */
    public function testReadDoesNotReleaseTheValueOffsetGetOwns(): void
    {
        $container = new Issue2682ProbeContainer();

        $this->assertFalse(
            $this->subject->readReleasedEarly($container),
            'The value read from the container was destroyed before the method returned.'
        );
    }

    public function testReadReleasesTheValueWhenTheMethodReturns(): void
    {
        $container = new Issue2682ProbeContainer();

        $this->subject->readReleasedEarly($container);

        $this->assertTrue($container->destroyed, 'The value read from the container was never released.');
    }

    /**
     * PHP owns what offsetGet() returns and releases it once. Zephir has to
     * agree, so the same shape written in PHP is the reference.
     */
    public function testMatchesPlainPhpSemantics(): void
    {
        $viaPhp = new Issue2682ProbeContainer();
        $value  = $viaPhp['namespace'];
        $copy   = $value;
        $copy   = null;
        $early  = $viaPhp->destroyed;
        $value  = null;

        $viaZephir = new Issue2682ProbeContainer();

        $this->assertSame($early, $this->subject->readReleasedEarly($viaZephir));
        $this->assertSame($viaPhp->destroyed, $viaZephir->destroyed);
    }

    /**
     * The positive control: without it a flat probe reading proves nothing,
     * because it would also be flat if the probe could not measure at all.
     */
    public function testLeakProbeCanMeasureGrowth(): void
    {
        $this->assertGreaterThan(
            0,
            $this->subject->leakProbeControl(2000),
            'The leak probe cannot detect retention, so its flat readings are meaningless.'
        );
    }

    /**
     * With nothing copied out of the read, the delref left the allocation with
     * no owner at all. A leak scales with the iteration count, so compare two
     * runs rather than pinning an absolute figure.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testAReadOnlySubscriptReadDoesNotLeak(): void
    {
        $container = new Issue2682FreshValueContainer();

        // Settle interned strings and call caches.
        $this->subject->leakProbe($container, 100);

        $small = $this->subject->leakProbe($container, 1000);
        $large = $this->subject->leakProbe($container, 50000);

        $this->assertSame(
            $small,
            $large,
            'Retained memory grows with the iteration count, so a read-only subscript read leaks.'
        );
    }

    /**
     * A native array container still owns its values, so the read-only
     * shortcut has to survive there. This asserts the behaviour; the generated
     * C is pinned by Zephir\Test\CodeGen\ReadOnlyArrayAccessTest.
     */
    public function testANativeArrayContainerStillReadsCorrectly(): void
    {
        $this->assertSame('kept', $this->subject->nativeArrayStaysReadOnly(0));
    }

    /**
     * The other half of the contract, the write context: a subscript handed to
     * a by-reference parameter.
     *
     * PHP separates the element inside the container and turns it into a real
     * reference, so the push lands in the container and the alias taken first
     * keeps what it was given. Borrowing the value instead only wrote through
     * while nothing else held it, so this used to come back with an empty
     * shelf.
     */
    public function testAByReferenceSubscriptWriteReachesASharedArrayContainer(): void
    {
        $this->assertMatchesPhp(
            fn () => $this->subject->pushShared('k', 'v'),
            static function () {
                $shelf      = [];
                $shelf['k'] = [];
                $alias      = $shelf['k'];

                array_push($shelf['k'], 'v');

                return ['shelf' => $shelf['k'], 'alias' => $alias];
            }
        );
    }

    /**
     * The container is a persistent array default, one immutable table every
     * instance shares until something writes to it. Repeated pushes have to
     * accumulate on the instance, and a fresh instance has to start from the
     * default again.
     *
     * Not asserted, because Zephir cannot match it: an array returned out of
     * one of these calls and held on to still aliases the property, so a later
     * push shows through it. PHP separates the container when anything else can
     * see it, and the kernel is handed a borrowed copy of the property rather
     * than the property slot, so it has no container to separate.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2691
     */
    public function testAByReferenceSubscriptWritesThroughToAPersistentArrayDefault(): void
    {
        $this->assertMatchesPhp(
            static function (): array {
                $first = new Issue2682();
                $first->pushIntoDefault('v');

                return [
                    $first->pushIntoDefault('w'),
                    (new Issue2682())->pushIntoDefault('z'),
                ];
            },
            static function (): array {
                $first = new Issue2682DefaultsHolder();
                $first->push('v');

                return [
                    $first->push('w'),
                    (new Issue2682DefaultsHolder())->push('z'),
                ];
            }
        );
    }

    /**
     * The same construct against an ArrayAccess container. PHP cannot carry the
     * write home, says so with a notice, and leaves the container alone. The
     * transcript covers the diagnostic as well as the value, so this pins the
     * notice without hard-coding its text.
     */
    public function testAByReferenceSubscriptOnAnArrayAccessContainerMatchesPhp(): void
    {
        $viaZephir = new Issue2682FreshArrayContainer();
        $viaPhp    = new Issue2682FreshArrayContainer();

        $this->assertMatchesPhp(
            fn () => $this->subject->pushIntoContainer($viaZephir, 'v'),
            static function () use ($viaPhp) {
                array_push($viaPhp['bucket'], 'v');

                return true;
            }
        );
    }

    /**
     * And it must not leak while doing it. The borrow left the offsetGet()
     * result with no owner at all, 344 bytes a call.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testAByReferenceSubscriptDoesNotLeakForAnArrayAccessContainer(): void
    {
        $container = new Issue2682FreshArrayContainer();

        /**
         * Every iteration raises the notice asserted above, and PHPUnit turns
         * notices into exceptions.
         */
        set_error_handler(static fn (): bool => true);

        try {
            // Settle interned strings and call caches.
            $this->subject->writeLeakProbe($container, 200);

            $small = $this->subject->writeLeakProbe($container, 1000);
            $large = $this->subject->writeLeakProbe($container, 50000);
        } finally {
            restore_error_handler();
        }

        $this->assertSame(
            $small,
            $large,
            'Retained memory grows with the iteration count, so the write-context fetch leaks.'
        );
    }

    /**
     * The container mocks have to behave like the PHP they stand in for, or
     * the tests above measure the mock instead of the kernel.
     */
    public function testTheProbeContainerReportsDestructionThroughItself(): void
    {
        $container = new Issue2682ProbeContainer();

        $this->assertInstanceOf(ArrayAccess::class, $container);
        $this->assertFalse($container->destroyed);

        $value = $container['namespace'];
        $this->assertFalse($container->destroyed, 'Holding the value must keep it alive.');

        $value = null;
        $this->assertTrue($container->destroyed, 'Dropping the last reference must destroy it.');
    }
}
