<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Php70\Oo;

use PHPUnit\Framework\TestCase;
use Test\Oo\OoParams;

class OoParamsStrictTest extends TestCase
{
    /** @var OoParams */
    private $test;

    public function setUp()
    {
        $this->test = new OoParams();
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForInt1()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'age' must be of the type int");

        $this->test->setStrictAge(17.0);
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForInt2()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'age' must be of the type int");

        $this->test->setStrictAge('17');
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForDouble()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'average' must be of the type double");

        $this->test->setStrictAverage('17');
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForInt()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'average' must be of the type double");

        $this->test->setStrictAverage(17);
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForString()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'name' must be of the type string");

        $this->test->setStrictName(1234);
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForCallFromZephirLand()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'name' must be of the type string");

        $this->test->setStrictNameFromZephirLand();
    }
}
