<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Issue2654;

/**
 * A declared variable that is never assigned used to keep the ZVAL_UNDEF it
 * received at declaration, and that zval reached userland unchanged, so
 * var_dump() printed UNKNOWN:0. PHP evaluates an unset variable as null.
 *
 * Only the value is compared against PHP, not the diagnostics: PHP raises
 * "Undefined variable" at runtime, whereas a Zephir declaration is mandatory
 * and the equivalent diagnostic is the compile-time `unassigned-variable`
 * warning.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2654
 */
final class Issue2654Test extends TestCase
{
    private Issue2654 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2654();
    }

    public function testUnassignedVarReadsAsNull(): void
    {
        $this->assertNull($this->subject->readVar());
    }

    public function testUnassignedStringReadsAsNull(): void
    {
        $this->assertNull($this->subject->readString());
    }

    public function testUnassignedArrayReadsAsNull(): void
    {
        $this->assertNull($this->subject->readArray());
    }

    /**
     * Native locals cannot hold null, so they keep their type-zero default.
     * This is a deliberate divergence from PHP, pinned here so it stays one.
     */
    public function testUnassignedIntKeepsTypeZeroDefault(): void
    {
        $this->assertSame(0, $this->subject->readInt());
    }

    public function testUnassignedLocalPassedAsArgumentArrivesAsNull(): void
    {
        $this->assertSame('NULL', $this->subject->passAsArgument());
    }

    public function testUnassignedLocalCapturedByClosureIsNull(): void
    {
        $this->assertNull($this->subject->captureInClosure());
    }

    public function testEmptyOnUnassignedLocalIsTrue(): void
    {
        $this->assertTrue($this->subject->emptyOnUnassigned());
    }

    /**
     * `-null` is int 0 in PHP. With an IS_UNDEF zval this fell through to
     * convert_scalar_to_number() inside zephir_negate().
     */
    public function testNegatingUnassignedLocalYieldsZero(): void
    {
        $this->assertSame(0, $this->subject->negateUnassigned());
    }

    public function testConcatenatingUnassignedLocalDropsIt(): void
    {
        $this->assertSame('tail', $this->subject->concatUnassigned());
    }

    public function testGeneratorYieldsUnassignedLocalAsNull(): void
    {
        $this->assertSame([null], iterator_to_array($this->subject->yieldUnassigned()));
    }

    public function testAssignedLocalIsUnaffected(): void
    {
        $this->assertSame('assigned', $this->subject->writeVar());
    }

    public function testOmittedOptionalParameterIsUnaffected(): void
    {
        $this->assertSame('prefix', $this->subject->optionalParam());
        $this->assertSame('prefixtail', $this->subject->optionalParam('tail'));
    }

    /**
     * The positive control: without it a flat leakProbe() reading proves
     * nothing, because it would also be flat if the probe could not measure.
     */
    public function testLeakProbeCanMeasureGrowth(): void
    {
        $this->assertGreaterThan(
            0,
            $this->subject->leakProbeControl(2000),
            'The leak probe cannot detect retention, so its flat reading is meaningless.'
        );
    }

    /**
     * A leak scales with the iteration count, so compare two runs a hundred
     * times apart rather than pinning an absolute figure: the first call to
     * memory_get_usage() itself costs a call-cache slot, and that one-off is
     * not what this is looking for.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testReadingUnassignedLocalsDoesNotLeak(): void
    {
        // Settle interned strings, call caches and the closure class.
        $this->subject->leakProbe(100);

        $small = $this->subject->leakProbe(1000);
        $large = $this->subject->leakProbe(100000);

        $this->assertSame(
            $small,
            $large,
            'Retained memory grows with the iteration count, so reading a '
            . 'never-assigned local leaks.'
        );
    }
}
