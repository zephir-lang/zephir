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

use Issue2691Holder;
use Issue2691Overloaded;
use Issue2691ReadonlyHolder;
use PHPUnit\Framework\TestCase;
use Stub\Issue2691;
use Throwable;

/**
 * A by-reference call argument is fetched in write context, which in PHP means
 * through the container's storage slot: the container is separated before the
 * element is looked up, a missing element is created rather than reported, and
 * the slot itself becomes the reference the callee writes through.
 *
 * Zephir read a borrowed copy of the property instead. Every test here is the
 * same code run twice, once compiled and once as plain PHP, so nothing pins an
 * expected string to a single PHP version.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2691
 */
final class Issue2691Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2691 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2691();
    }

    /**
     * An array returned by an earlier call and still held.
     *
     * PHP separates the property's array on the second push, so what the first
     * call handed out stops tracking the property. Borrowing left one table
     * with two holders, and the later push showed through the copy.
     */
    public function testAKeptReturnValueStopsTrackingTheProperty(): void
    {
        $this->assertMatchesPhp(
            function (): array {
                $kept = $this->subject->pushAndReturn('p', 'v');
                $this->subject->pushAndReturn('p', 'w');

                return $kept;
            },
            static function (): array {
                $holder = new Issue2691Holder();
                $kept   = $holder->pushAndReturn('p', 'v');
                $holder->pushAndReturn('p', 'w');

                return $kept;
            }
        );
    }

    /**
     * The property named at runtime. PHP's write context does not care whether
     * the name was a literal or a variable, so a kept return value has to stop
     * tracking the property here too.
     */
    public function testAWriteReachesADynamicallyNamedProperty(): void
    {
        $this->assertMatchesPhp(
            function (): array {
                $kept = $this->subject->pushIntoDynamic('prefixes', 'p', 'v');
                $this->subject->pushIntoDynamic('prefixes', 'p', 'w');

                return [$kept, $this->subject->getPrefixes()];
            },
            static function (): array {
                $holder = new Issue2691Holder();
                $kept   = $holder->pushIntoDynamic('prefixes', 'p', 'v');
                $holder->pushIntoDynamic('prefixes', 'p', 'w');

                return [$kept, $holder->getPrefixes()];
            }
        );
    }

    /**
     * The whole dynamically named property as the argument, with its array
     * shared. Reading it borrowed and wrapping the borrow in a reference is the
     * same bad free here as with a literal name.
     */
    public function testABareDynamicallyNamedPropertyIsSeparated(): void
    {
        $this->assertMatchesPhp(
            function (): array {
                $this->subject->setRows(['header: a', 'body']);
                $kept = $this->subject->getRows();

                return [
                    'shifted' => $this->subject->shiftDynamic('rows'),
                    'rows'    => $this->subject->getRows(),
                    'kept'    => $kept,
                ];
            },
            static function (): array {
                $holder = new Issue2691Holder();
                $holder->setRows(['header: a', 'body']);
                $kept = $holder->getRows();

                return [
                    'shifted' => $holder->shiftDynamic('rows'),
                    'rows'    => $holder->getRows(),
                    'kept'    => $kept,
                ];
            }
        );
    }

    /**
     * A name written as a literal in braces resolves to an ordinary property
     * access, and has to behave like one.
     */
    public function testAWriteReachesAPropertyNamedByABracedLiteral(): void
    {
        $this->assertMatchesPhp(
            function (): array {
                $kept = $this->subject->pushIntoBracedLiteral('p', 'v');
                $this->subject->pushIntoBracedLiteral('p', 'w');

                return [$kept, $this->subject->getPrefixes()];
            },
            static function (): array {
                $holder = new Issue2691Holder();
                $kept   = $holder->pushIntoBracedLiteral('p', 'v');
                $holder->pushIntoBracedLiteral('p', 'w');

                return [$kept, $holder->getPrefixes()];
            }
        );
    }

    /**
     * A readonly property has no slot to give either, and the engine refuses
     * the indirect modification itself. Nothing may be written, and the message
     * has to be the engine's own.
     */
    public function testAReadonlyPropertyRefusesTheWriteContext(): void
    {
        if (PHP_VERSION_ID < 80100) {
            $this->markTestSkipped('Readonly properties arrived in PHP 8.1.');
        }

        $this->assertMatchesPhp(
            static function (): array {
                $holder = new Issue2691();

                try {
                    $holder->pushIntoReadonly('a', 'v');
                } catch (Throwable $e) {
                    return [self::withoutHolderName($e), $holder->getReadonlyRows()];
                }

                return ['no error', $holder->getReadonlyRows()];
            },
            static function (): array {
                $holder = new Issue2691ReadonlyHolder();

                try {
                    $holder->pushIntoReadonly('a', 'v');
                } catch (Throwable $e) {
                    return [self::withoutHolderName($e), $holder->getReadonlyRows()];
                }

                return ['no error', $holder->getReadonlyRows()];
            }
        );
    }

    /**
     * The two holders are different classes, and the engine names the one it
     * refused in the message. Everything else about it has to match.
     */
    private static function withoutHolderName(Throwable $e): string
    {
        return get_class($e) . ': ' . preg_replace('/property \S+::/', 'property <holder>::', $e->getMessage());
    }

    /**
     * A write context leaves the element it wrote through as a reference, which
     * is what PHP does too. Every later read of it has to dereference, or the
     * copy it hands out is not a copy and writes to it show through the
     * property.
     */
    public function testAnElementLeftAsAReferenceStillReadsAsItsValue(): void
    {
        $this->assertMatchesPhp(
            fn (): array => $this->subject->pushThenCopy('p', 'v'),
            static fn (): array => (new Issue2691Holder())->pushThenCopy('p', 'v')
        );
    }

    /**
     * The same on a static property, whose slot lives in the class. A returned
     * array held across the next push has to stop tracking it, exactly as an
     * instance property's does.
     */
    public function testAKeptReturnValueStopsTrackingAStaticProperty(): void
    {
        Issue2691::resetStatic();
        Issue2691Holder::resetStatic();

        $this->assertMatchesPhp(
            static function (): array {
                $kept = Issue2691::pushStatic('p', 'v');
                Issue2691::pushStatic('p', 'w');

                return $kept;
            },
            static function (): array {
                $kept = Issue2691Holder::pushStatic('p', 'v');
                Issue2691Holder::pushStatic('p', 'w');

                return $kept;
            }
        );
    }

    /**
     * The container is a local array someone else also holds. PHP separates it
     * and the write lands; the element could only be turned into a reference
     * while the table was unshared, so the write reached a copy nobody had.
     */
    public function testAWriteReachesASharedLocalContainer(): void
    {
        $this->assertMatchesPhp(
            fn (): array => $this->subject->pushIntoLocalShared('k', 'v'),
            static fn (): array => (new Issue2691Holder())->pushIntoLocalShared('k', 'v')
        );
    }

    /**
     * The whole property as the by-reference argument, with the property's
     * array shared. array_shift() separates what it is given, and given a
     * borrow that separation ran GC_TRY_DELREF() on a reference the zval never
     * took, so the shift was applied to a table two holders still shared.
     */
    public function testABareSharedPropertyIsSeparatedBeforeTheCallee(): void
    {
        $this->assertMatchesPhp(
            function (): array {
                $this->subject->setRows(['header: a', 'body']);
                $kept = $this->subject->getRows();

                return [
                    'shifted' => $this->subject->shiftRows(),
                    'rows'    => $this->subject->getRows(),
                    'kept'    => $kept,
                ];
            },
            static function (): array {
                $holder = new Issue2691Holder();
                $holder->setRows(['header: a', 'body']);
                $kept = $holder->getRows();

                return [
                    'shifted' => $holder->shiftRows(),
                    'rows'    => $holder->getRows(),
                    'kept'    => $kept,
                ];
            }
        );
    }

    /**
     * A callee that replaces its argument rather than mutating it in place.
     * settype() destroys the old value first, which on a borrow frees the array
     * the property is still pointing at.
     */
    public function testACalleeMayReplaceTheValueOfAPropertyArgument(): void
    {
        $this->assertMatchesPhp(
            function (): array {
                $this->subject->setValue(['a', 'b']);

                return [
                    'returned' => $this->subject->retypeValue('integer'),
                    'value'    => $this->subject->getValue(),
                ];
            },
            static function (): array {
                $holder = new Issue2691Holder();
                $holder->setValue(['a', 'b']);

                return [
                    'returned' => $holder->retypeValue('integer'),
                    'value'    => $holder->getValue(),
                ];
            }
        );
    }

    /**
     * A write context is a lookup-or-create. PHP raises no diagnostic and
     * leaves the created element behind; the read context raised "Undefined
     * index" and left the container untouched.
     */
    public function testAMissingElementIsCreatedRatherThanReported(): void
    {
        $this->assertMatchesPhp(
            function (): array {
                try {
                    $this->subject->pushMissingKey('p', 'v');
                } catch (Throwable $e) {
                    return ['error' => $e->getMessage(), 'prefixes' => $this->subject->getPrefixes()];
                }

                return ['error' => null, 'prefixes' => $this->subject->getPrefixes()];
            },
            static function (): array {
                $holder = new Issue2691Holder();

                try {
                    $holder->pushMissingKey('p', 'v');
                } catch (Throwable $e) {
                    return ['error' => $e->getMessage(), 'prefixes' => $holder->getPrefixes()];
                }

                return ['error' => null, 'prefixes' => $holder->getPrefixes()];
            }
        );
    }

    /**
     * No slot exists for a magic property, so PHP reads an owned copy, says
     * "Indirect modification of overloaded property ... has no effect", and
     * lets the write go nowhere. The transcript carries the notice.
     */
    public function testAnOverloadedPropertyGetsPhpsNotice(): void
    {
        $this->assertMatchesPhp(
            fn (): bool => $this->subject->pushIntoOverloaded(new Issue2691Overloaded(), 'k', 'v'),
            static fn (): bool => (new Issue2691Holder())
                ->pushIntoOverloaded(new Issue2691Overloaded(), 'k', 'v')
        );
    }

    /**
     * The probe has to be able to see growth at all, or a flat reading from
     * the test below would prove nothing.
     */
    public function testLeakProbeCanMeasureGrowth(): void
    {
        $this->subject->leakProbeControl(200);

        $this->assertGreaterThan(
            $this->subject->leakProbeControl(1000),
            $this->subject->leakProbeControl(50000),
            'The probe cannot measure retained memory, so the leak test below is meaningless.'
        );
    }

    /**
     * The write context must not leak. What it fetches is owned on one path and
     * a live slot on the other, and only one of those may be released.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testTheWriteContextDoesNotLeak(): void
    {
        // Settle interned strings and call caches.
        $this->subject->writeLeakProbe(200);

        $this->assertSame(
            $this->subject->writeLeakProbe(1000),
            $this->subject->writeLeakProbe(50000),
            'Retained memory grows with the iteration count, so the write context leaks.'
        );
    }
}
