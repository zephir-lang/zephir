<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Unsettest as UnsetTestExt;

class UnsetTest extends TestCase
{
    /** @var UnsetTestExt */
    private $test;

    protected function setUp(): void
    {
        $this->test = new UnsetTestExt();
    }

    public function testUnsetFromArray()
    {
        $testVar = [1, 2, 3, 4, 5, 6];
        $this->assertSame([1 => 2, 3, 4, 5, 6], $this->test->testUnsetFromArray($testVar));
        $this->assertSame([1 => 2, 3, 4, 5, 6], $this->test->testUnsetFromArrayByIndexVar($testVar, 0));

        $this->assertSame(['foo' => 'bar'], $this->test->testUnsetTypedArray(['foo' => 'bar', 'bar' => 'foo'], 'bar'));
    }

    public function xtestUnsetValueFromProperty()
    {
        //TODO: Zephir is not generating the correct code to make this work
        $testVar = [1, 2, 3, 4, 5, 6];
        $this->assertFalse($this->test->has('testVar'));
        $this->test->addValueToProperty('testVar', $testVar);
        $this->assertTrue($this->test->has('testVar'));
        $this->test->testUnsetValueFromProperty('testVar');
        $this->assertFalse($this->test->has('testVar'));

        $this->test->testUnsetProperty();
        $this->assertNull($this->test->getProperty());
    }

    public function xtestStdClassUnset()
    {
        //TODO: Zephir is not generating the correct code to make this work
        $equalsStdClass = new \StdClass();
        $equalsStdClass->property1 = 12345;
        $equalsStdClass->property3 = 12345;

        $this->assertSame($equalsStdClass, $this->test->testStdClassUnset());
    }
}
