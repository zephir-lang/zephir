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
use Stub\Issue1875;

/**
 * Covers issue #1875 "About variables initialized".
 *
 * The cphalcon trigger was that a `var` declared inside the method body, only
 * conditionally assigned, and then read, surfaced as undef at runtime and
 * tripped the kernel memory observer in debug builds. The behavior expected
 * from PHP semantics: an unassigned local reads as null.
 */
final class Issue1875Test extends TestCase
{
    public function testConditionalThenReadWithKeyPresent(): void
    {
        $obj = new Issue1875();
        $this->assertSame('yes', $obj->conditionalThenRead(['included' => 'yes']));
    }

    public function testConditionalThenReadWithFallbackKey(): void
    {
        $obj = new Issue1875();
        $this->assertSame('fallback', $obj->conditionalThenRead(['includedMinimum' => 'fallback']));
    }

    public function testConditionalThenReadWithNoKey(): void
    {
        $obj = new Issue1875();
        $this->assertNull($obj->conditionalThenRead([]));
    }

    public function testReassignInBranchesWithArray(): void
    {
        $obj = new Issue1875();
        $this->assertTrue($obj->reassignInBranches(['seed' => ['x' => 'truthy']]));
    }

    public function testReassignInBranchesWithScalar(): void
    {
        $obj = new Issue1875();
        $this->assertTrue($obj->reassignInBranches(['seed' => 'anything']));
        $this->assertFalse($obj->reassignInBranches(['seed' => '']));
    }

    public function testNeverAssignedReturnsNull(): void
    {
        $obj = new Issue1875();
        $this->assertNull($obj->neverAssigned());
    }

    public function testForwardedToNewInstanceWithoutKey(): void
    {
        $obj = new Issue1875();
        $out = $obj->forwardedToNewInstance(['min' => 1]);

        $this->assertIsArray($out);
        $this->assertSame(1, $out['min']);
        $this->assertArrayHasKey('included', $out);
        $this->assertNull($out['included']);
    }
}
