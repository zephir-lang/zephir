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
use Test\Unsettest as UnsetTestExt;

class UnsetTest extends TestCase
{
    public function testUnsetFromArray()
    {
        $t = new UnsetTestExt();

        $testVar = [1, 2, 3, 4, 5, 6];
        $this->assertSame([1 => 2, 3, 4, 5, 6], $t->testUnsetFromArray($testVar));
        $this->assertSame([1 => 2, 3, 4, 5, 6], $t->testUnsetFromArrayByIndexVar($testVar, 0));

        $this->assertSame(['foo' => 'bar'], $t->testUnsetTypedArray(['foo' => 'bar', 'bar' => 'foo'], 'bar'));
    }

    public function xtestUnsetValueFromProperty()
    {
        //@todo Zephir is not generating the correct code to make this work
        $t = new UnsetTestExt();

        $testVar = [1, 2, 3, 4, 5, 6];
        $this->assertFalse($t->has('testVar'));
        $t->addValueToProperty('testVar', $testVar);
        $this->assertTrue($t->has('testVar'));
        $t->testUnsetValueFromProperty('testVar');
        $this->assertFalse($t->has('testVar'));

        $t->testUnsetProperty();
        $this->assertNull($t->getProperty());
    }

    public function xtestStdClassUnset()
    {
        //@todo Zephir is not generating the correct code to make this work
        $t = new UnsetTestExt();

        $equalsStdClass = new \StdClass();
        $equalsStdClass->property1 = 12345;
        $equalsStdClass->property3 = 12345;

        $this->assertSame($equalsStdClass, $t->testStdClassUnset());
    }
}
