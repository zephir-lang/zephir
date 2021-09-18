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

namespace Extension\Types;

use PHPUnit\Framework\TestCase;
use Stub\Types\MixedType;

final class MixedTest extends TestCase
{
    public function testReturnsOfMixedType(): void
    {
        $returns = new MixedType();

        $this->assertEquals((new \stdClass()), $returns->returnMixedObject());
        $this->assertSame([], $returns->returnMixedArray());
        $this->assertSame('mixed string', $returns->returnMixedString());
        $this->assertSame(1, $returns->returnMixedInt());
        $this->assertSame(3.14, $returns->returnMixedFloat());
        $this->assertSame(true, $returns->returnMixedBool());
        $this->assertSame(null, $returns->returnMixedNull());

        $this->assertNotNull($returns->returnMixedObject());
        $this->assertNotNull($returns->returnMixedArray());
        $this->assertNotNull($returns->returnMixedString());
        $this->assertNotNull($returns->returnMixedInt());
        $this->assertNotNull($returns->returnMixedFloat());
        $this->assertNotNull($returns->returnMixedBool());
        $this->assertNull($returns->returnMixedNull());

        $this->assertEquals((new \stdClass()), $returns->returnMixed74());
        $this->assertSame('string', $returns->returnMixed74(true));
        $this->assertEquals((new \stdClass()), $returns->returnMixed74(false));
    }
}
