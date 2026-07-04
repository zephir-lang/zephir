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

namespace Extension\Traits;

use PHPUnit\Framework\TestCase;
use Stub\Traits\CounterUser;

final class TraitsTest extends TestCase
{
    public function testTraitMethodsAreMergedIntoUsingClass(): void
    {
        $counter = new CounterUser();

        $this->assertSame(2, $counter->increment());
        $this->assertSame(4, $counter->increment());
    }

    public function testStaticTraitMethodIsMerged(): void
    {
        $this->assertSame('counter', CounterUser::describe());
    }

    public function testTraitConstantIsMerged(): void
    {
        $this->assertSame(2, CounterUser::STEP);
    }

    public function testAbstractTraitMethodIsSatisfiedByClass(): void
    {
        $counter = new CounterUser();
        $counter->increment();

        $this->assertSame('c:2', $counter->tagged());
    }

    public function testMergeIsInvisibleToReflection(): void
    {
        $this->assertSame([], class_uses(CounterUser::class));
    }
}
