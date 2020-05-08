<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Oo;

use PHPUnit\Framework\TestCase;

class OoParamsTest extends TestCase
{
    /** @var \Stub\Oo\OoParams */
    private $test;

    public function setUp()
    {
        $this->test = new \Stub\Oo\OoParams();
    }

    public function testOoParams()
    {
        $this->assertInstanceOf('Stub\Oo\OoParams', $this->test->createThisClassWithoutWriteCurrentNamespace());
        $this->assertinstanceOf('Stub\Oo\OoDynamicA', $this->test->createOtherClassWithoutWriteCurrentNamespace());

        $this->assertSame($this->test->setAge(17), 17);
        $this->assertSame($this->test->setAge('17'), 17);
        $this->assertSame($this->test->setAge(17.0), 17);

        $this->assertSame($this->test->setAverage(17.1), 17.1);
        $this->assertSame($this->test->setAverage(17), 17.0);
        $this->assertSame($this->test->setAverage('17.5'), 17.5);

        $this->assertSame($this->test->setName('peter'), 'peter');
        $this->assertSame($this->test->setName(12.5), '12.5');
        $this->assertSame($this->test->setName(17), '17');

        $this->assertSame($this->test->setEnabled(true), true);
        $this->assertSame($this->test->setEnabled(false), false);
        $this->assertSame($this->test->setEnabled(1), true);
        $this->assertSame($this->test->setEnabled(0), false);
        $this->assertSame($this->test->setEnabled('1'), true);
        $this->assertSame($this->test->setEnabled('0'), false);

        $this->assertSame($this->test->setList([1, 2, 3, 4, 5]), [1, 2, 3, 4, 5]);
        $this->assertSame($this->test->setList([]), []);
    }

    public function setObjectClassCast()
    {
        $result = $this->test->setStrictName(new \Stub\Oo\Param());

        $this->assertInstanceOf('Stub\Oo\Param', $result);
    }
}
