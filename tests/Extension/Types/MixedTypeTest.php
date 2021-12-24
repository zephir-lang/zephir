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

final class MixedTypeTest extends TestCase
{
    public function testReturnsOfMixedType(): void
    {
        $returns = new MixedType();

        $this->assertEquals((new \stdClass()), $returns->returnMixedObject());
        $this->assertSame([], $returns->returnMixedArray());
        $this->assertSame('mixed string', $returns->returnMixedString());
        $this->assertSame(1, $returns->returnMixedInt());
        $this->assertSame(3.14, $returns->returnMixedFloat());
        $this->assertTrue($returns->returnMixedBool());
        $this->assertNull($returns->returnMixedNull());

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

    public function testParamsOfMixedType(): void
    {
        $returns = new MixedType();

        $this->assertEquals((new \stdClass()), $returns->paramMixed((new \stdClass())));
        $this->assertSame([], $returns->paramMixed([]));
        $this->assertSame('mixed string', $returns->paramMixed('mixed string'));
        $this->assertSame(1, $returns->paramMixed(1));
        $this->assertSame(3.14, $returns->paramMixed(3.14));
        $this->assertTrue($returns->paramMixed(true));
        $this->assertNull($returns->paramMixed(null));

        $this->assertEquals([(new \stdClass()), []], $returns->paramMixedTwo((new \stdClass()), []));
        $this->assertSame([[], 'mixed string'], $returns->paramMixedTwo([], 'mixed string'));
        $this->assertSame([1, 3.14], $returns->paramMixedTwo(1, 3.14));
        $this->assertSame([3.14, true], $returns->paramMixedTwo(3.14, true));
        $this->assertSame([true, null], $returns->paramMixedTwo(true, null));
        $this->assertSame([null, null], $returns->paramMixedTwo(null, null));

        $this->assertEquals([1337, 'object', (new \stdClass())], $returns->paramMixedWithMulti(1337, 'object', (new \stdClass())));
        $this->assertSame([1337, 'array', []], $returns->paramMixedWithMulti(1337, 'array', []));
        $this->assertSame([1337, 'string', 'mixed string'], $returns->paramMixedWithMulti(1337, 'string', 'mixed string'));
        $this->assertSame([1337, 'int', 123], $returns->paramMixedWithMulti(1337, 'int', 123));
        $this->assertSame([1337, 'float', 2.44], $returns->paramMixedWithMulti(1337, 'float', 2.44));
        $this->assertSame([1337, 'bool', false], $returns->paramMixedWithMulti(1337, 'bool', false));
        $this->assertSame([1337, 'null', null], $returns->paramMixedWithMulti(1337, 'null', null));
    }

    public function testParamsAndReturnsOfMixedType(): void
    {
        $returns = new MixedType();

        $this->assertEquals((new \stdClass()), $returns->paramAndReturnMixed((new \stdClass())));
        $this->assertSame([], $returns->paramAndReturnMixed([]));
        $this->assertSame('mixed string', $returns->paramAndReturnMixed('mixed string'));
        $this->assertSame(1, $returns->paramAndReturnMixed(1));
        $this->assertSame(3.14, $returns->paramAndReturnMixed(3.14));
        $this->assertTrue($returns->paramAndReturnMixed(true));
        $this->assertNull($returns->paramAndReturnMixed(null));
    }

    public function testCastMixedType(): void
    {
        $returns = new MixedType();

        /**
         * string
         */
        $this->assertSame('123', $returns->castToStringMixedAndReturnMixed(123));
        $this->assertSame('123', $returns->castToStringMixedAndReturnMixed('123'));
        $this->assertSame('string', $returns->castToStringMixedAndReturnMixed('string'));
        $this->assertSame('', $returns->castToStringMixedAndReturnMixed(false));
        $this->assertSame('1', $returns->castToStringMixedAndReturnMixed(true));

        /**
         * int
         */
        $this->assertSame(123, $returns->castToIntMixedAndReturnMixed('123'));
        $this->assertSame(1, $returns->castToIntMixedAndReturnMixed(1.5));

        /**
         * bool
         */
        $this->assertSame(true, $returns->castToBoolMixedAndReturnMixed(1));
        $this->assertSame(false, $returns->castToBoolMixedAndReturnMixed(0));

        /**
         * float
         */
        $this->assertSame(1.0, $returns->castToFloatMixedAndReturnMixed(1));
        $this->assertSame(1.5, $returns->castToFloatMixedAndReturnMixed(1.5));
    }
}
