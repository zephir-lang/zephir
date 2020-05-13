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
use Stub\Oo\OoParams;

class OoParamsStrictTest extends TestCase
{
    /** @var OoParams */
    private $test;

    public function setUp()
    {
        $this->test = new OoParams();
    }

    public function testSetStrictAgeSuccess()
    {
        $this->assertSame($this->test->setStrictAge(17), 17);
    }

    public function testSetStrictAverageSuccess()
    {
        $this->assertSame($this->test->setStrictAverage(17.1), 17.1);
    }

    public function testSetStrictNameSuccess()
    {
        $this->assertSame($this->test->setStrictName('peter'), 'peter');
    }
}
