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
use Stub\Json;

final class JsonTest extends TestCase
{
    private Json $test;

    protected function setUp(): void
    {
        $this->test = new Json();
    }

    public function testEncode(): void
    {
        $this->assertSame($this->test->testEncodeObject(), '{"a":"hello","b":"world","c":128}');
        $this->assertSame($this->test->testEncodeArray(), '[1,2,3]');
        $this->assertSame($this->test->testEncodeOptions(), '["\\u003Cfoo\\u003E","\'bar\'","&blong&","\\u00e9"]');
    }

    public function testDecode(): void
    {
        $obj1 = $this->test->testDecodeObject();
        $this->assertSame($obj1->a, 'hello');

        $arr1 = $this->test->testDecodeArray();
        $this->assertSame($arr1[1], 2);

        $obj2 = $this->test->testDecodeObject2();
        $this->assertSame($obj2['a'], 'hello');
    }
}
