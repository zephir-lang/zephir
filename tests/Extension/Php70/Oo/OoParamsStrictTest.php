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
use Stub\Oo\OoParams;

class OoParamsStrictTest extends TestCase
{
    /** @var OoParams */
    private $test;

    protected function setUp(): void
    {
        $this->test = new OoParams();
    }

    public function testShouldThrowInvalidArgumentExceptionForInt1()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'age' must be of the type int");

        $this->test->setStrictAge(17.0);
    }

    public function testShouldThrowInvalidArgumentExceptionForInt2()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'age' must be of the type int");

        $this->test->setStrictAge('17');
    }

    public function testShouldThrowInvalidArgumentExceptionForDouble()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'average' must be of the type double");

        $this->test->setStrictAverage('17');
    }

    public function testShouldThrowInvalidArgumentExceptionForInt()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'average' must be of the type double");

        $this->test->setStrictAverage(17);
    }

    public function testShouldThrowInvalidArgumentExceptionForString()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'name' must be of the type string");

        $this->test->setStrictName(1234);
    }

    public function testShouldThrowInvalidArgumentExceptionForCallFromZephirLand()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'name' must be of the type string");

        $this->test->setStrictNameFromZephirLand();
    }
}
