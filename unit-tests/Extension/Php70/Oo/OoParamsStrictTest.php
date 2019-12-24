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
    /** @test */
    public function shouldThrowInvalidArgumentExceptionForInt1()
    {
        $t = new OoParams();

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'age' must be of the type int");

        $t->setStrictAge(17.0);
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForInt2()
    {
        $t = new OoParams();

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'age' must be of the type int");

        $t->setStrictAge('17');
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForDouble()
    {
        $t = new OoParams();

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'average' must be of the type double");

        $t->setStrictAverage('17');
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForInt()
    {
        $t = new OoParams();

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'average' must be of the type double");

        $t->setStrictAverage(17);
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForString()
    {
        $t = new OoParams();

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'name' must be of the type string");

        $t->setStrictName(1234);
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForCallFromZephirLand()
    {
        $t = new OoParams();

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'name' must be of the type string");

        $t->setStrictNameFromZephirLand();
    }
}
