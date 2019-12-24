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
use Test\Oo\OoParams;

class OoParamsStrictTest extends TestCase
{
    public function testSetStrictAgeSuccess()
    {
        $t = new OoParams();

        $this->assertSame($t->setStrictAge(17), 17);
    }

    public function testSetStrictAverageSuccess()
    {
        $t = new OoParams();

        $this->assertSame($t->setStrictAverage(17.1), 17.1);
    }

    public function testSetStrictNameSuccess()
    {
        $t = new OoParams();
        $this->assertSame($t->setStrictName('peter'), 'peter');
    }
}
