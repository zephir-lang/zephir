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
use Stub\Issue2679;

/**
 * A local whose only assignment sits inside a conditional used to keep the
 * ZVAL_UNDEF it received at declaration whenever that branch did not run, and
 * that zval reached userland, so var_dump() printed UNKNOWN:0. PHP evaluates a
 * read of an unset variable as null (EG(uninitialized_zval), see
 * Zend/zend_execute.c zval_undefined_cv()), identically on 8.0 through 8.5.
 *
 * Only the value is compared against PHP, not the diagnostics: PHP raises
 * "Undefined variable" at runtime, whereas Zephir reports the same condition at
 * compile time.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2679
 */
final class Issue2679Test extends TestCase
{
    private Issue2679 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2679();
    }

    public function testUntakenBranchLeavesTheLocalNull(): void
    {
        $this->assertNull($this->subject->conditional(false));
    }

    public function testTakenBranchIsUnaffected(): void
    {
        $this->assertSame('set', $this->subject->conditional(true));
    }

    public function testTwoConditionalWritesLeaveTheLocalNull(): void
    {
        $this->assertNull($this->subject->multiBranch(false, false));
        $this->assertSame('a', $this->subject->multiBranch(true, false));
        $this->assertSame('b', $this->subject->multiBranch(false, true));
        $this->assertSame('b', $this->subject->multiBranch(true, true));
    }

    public function testLoopThatNeverRunsLeavesTheLocalNull(): void
    {
        $this->assertNull($this->subject->loopOnly([]));
        $this->assertSame(2, $this->subject->loopOnly([1, 2]));
    }

    public function testWhileThatNeverRunsLeavesTheLocalNull(): void
    {
        $this->assertNull($this->subject->whileOnly(0));
        $this->assertSame('iter1', $this->subject->whileOnly(2));
    }

    public function testSwitchWithoutDefaultLeavesTheLocalNull(): void
    {
        $this->assertNull($this->subject->switchWithoutDefault(9));
        $this->assertSame('one', $this->subject->switchWithoutDefault(1));
    }

    public function testAssignmentSkippedByAThrowLeavesTheLocalNull(): void
    {
        $this->assertNull($this->subject->tryOnly(true));
        $this->assertSame('ok', $this->subject->tryOnly(false));
    }

    public function testReadBeforeTheOnlyWriteSeesNull(): void
    {
        $this->assertSame('NULL/string', $this->subject->readBeforeWrite());
    }

    public function testConditionallyAssignedStringLocal(): void
    {
        $this->assertNull($this->subject->stringLocal(false));
        $this->assertSame('set', $this->subject->stringLocal(true));
    }

    public function testConditionallyAssignedArrayLocal(): void
    {
        $this->assertNull($this->subject->arrayLocal(false));
        $this->assertSame([1, 2, 3], $this->subject->arrayLocal(true));
    }

    /**
     * The null has to be a real value, not something only var_dump() smooths
     * over, so hand it to a function instead of returning it.
     */
    public function testUnassignedLocalArrivesAtAFunctionAsNull(): void
    {
        $this->assertSame('NULL', $this->subject->passAsArgument(false));
        $this->assertSame('string', $this->subject->passAsArgument(true));
    }

    /**
     * `fetch` writes its target on both paths, the value on a hit and
     * ZVAL_NULL on a miss, so these already behaved and must keep behaving
     * without the compiler touching the slot.
     */
    public function testFetchTargetIsAlreadyNullOnAMiss(): void
    {
        $this->assertSame([false, null], $this->subject->fetchCase([]));
        $this->assertSame([true, 'v'], $this->subject->fetchCase(['k' => 'v']));
        $this->assertNull($this->subject->fetchStatement([]));
        $this->assertSame('v', $this->subject->fetchStatement(['k' => 'v']));
        $this->assertSame('missing', $this->subject->fetchGuarded([]));
        $this->assertSame('v', $this->subject->fetchGuarded(['k' => 'v']));
    }

    public function testDefinitelyAssignedLocalsAreUnaffected(): void
    {
        $this->assertSame('no', $this->subject->exhaustiveIfElse(false));
        $this->assertSame('yes', $this->subject->exhaustiveIfElse(true));
        $this->assertSame('second', $this->subject->conditionalThenReassign(false));
        $this->assertSame('other', $this->subject->switchWithDefault(9));
        $this->assertSame('one', $this->subject->switchWithDefault(1));
    }

    /**
     * A generator step is compiled as a resumable function whose first-init
     * macros are rewritten to their resume-safe N forms.
     */
    public function testGeneratorStepIsUnaffected(): void
    {
        $this->assertSame([null, 'after'], iterator_to_array($this->subject->conditionalInGenerator(false)));
        $this->assertSame(['set', 'after'], iterator_to_array($this->subject->conditionalInGenerator(true)));
    }

    public function testUnassignedLocalIsCapturedAsNull(): void
    {
        $this->assertSame('NULL', $this->subject->conditionalCapturedByClosure(false));
        $this->assertSame('string', $this->subject->conditionalCapturedByClosure(true));
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
     * ZVAL_UNDEF at declaration is what makes the first write register the
     * variable with the memory frame, so anything that starts a conditionally
     * assigned local elsewhere risks losing that registration. A leak scales
     * with the iteration count, so compare two runs a hundred times apart
     * rather than pinning an absolute figure.
     *
     * @depends testLeakProbeCanMeasureGrowth
     */
    public function testConditionallyAssignedLocalsDoNotLeak(): void
    {
        // Settle interned strings and call caches.
        $this->subject->leakProbe(100);

        $small = $this->subject->leakProbe(1000);
        $large = $this->subject->leakProbe(100000);

        $this->assertSame(
            $small,
            $large,
            'Retained memory grows with the iteration count, so a '
            . 'conditionally assigned local leaks.'
        );
    }
}
