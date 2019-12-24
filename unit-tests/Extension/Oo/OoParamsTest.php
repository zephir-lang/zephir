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
    public function testOoParams()
    {
        $t = new \Test\Oo\OoParams();

        $this->assertInstanceOf('Test\Oo\OoParams', $t->createThisClassWithoutWriteCurrentNamespace());
        $this->assertinstanceOf('Test\Oo\OoDynamicA', $t->createOtherClassWithoutWriteCurrentNamespace());

        $this->assertSame($t->setAge(17), 17);
        $this->assertSame($t->setAge('17'), 17);
        $this->assertSame($t->setAge(17.0), 17);

        $this->assertSame($t->setAverage(17.1), 17.1);
        $this->assertSame($t->setAverage(17), 17.0);
        $this->assertSame($t->setAverage('17.5'), 17.5);

        $this->assertSame($t->setName('peter'), 'peter');
        $this->assertSame($t->setName(12.5), '12.5');
        $this->assertSame($t->setName(17), '17');

        $this->assertSame($t->setEnabled(true), true);
        $this->assertSame($t->setEnabled(false), false);
        $this->assertSame($t->setEnabled(1), true);
        $this->assertSame($t->setEnabled(0), false);
        $this->assertSame($t->setEnabled('1'), true);
        $this->assertSame($t->setEnabled('0'), false);

        $this->assertSame($t->setList([1, 2, 3, 4, 5]), [1, 2, 3, 4, 5]);
        $this->assertSame($t->setList([]), []);
    }

    public function setObjectClassCast()
    {
        $t = new \Test\Oo\OoParams();
        $result = $t->setStrictName(new \Test\Oo\Param());

        $this->assertInstanceOf('Test\Oo\Param', $result);
    }
}
