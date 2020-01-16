<?php

declare(strict_types=1);

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Php72\Oo;

use PHPUnit\Framework\TestCase;
use Test\Oo\OoParams;

class OoParamsStrictTest extends TestCase
{
    /** @test */
    public function shouldThrowTypeErrorForInt1()
    {
        $t = new OoParams();

        $this->expectException(\TypeError::class);
        $this->expectExceptionMessageRegExp(
            // PHP 7.3 : int
            // PHP 7.2 : integer
            '/Argument 1 passed to Test\\\Oo\\\OoParams::setStrictAge\(\) '.
            'must be of the type int(eger)?, float given/'
        );

        $t->setStrictAge(17.0);
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForInt2()
    {
        $t = new OoParams();

        $this->expectException(\TypeError::class);
        $this->expectExceptionMessageRegExp(
            // PHP 7.3 : int
            // PHP 7.2 : integer
            '/Argument 1 passed to Test\\\Oo\\\OoParams::setStrictAge\(\) '.
            'must be of the type int(eger)?, string given/'
        );

        $t->setStrictAge('17');
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForDouble()
    {
        $t = new OoParams();

        $this->expectException(\TypeError::class);
        $this->expectExceptionMessage(
            'Argument 1 passed to Test\Oo\OoParams::setStrictAverage() must be of the type float, string given'
        );

        $t->setStrictAverage('17');
    }

    /** @test */
    public function shouldThrowInvalidArgumentExceptionForString()
    {
        $t = new OoParams();

        $this->expectException(\TypeError::class);
        $this->expectExceptionMessageRegExp(
            // PHP 7.3 : int
            // PHP 7.2 : integer
            '/Argument 1 passed to Test\\\Oo\\\OoParams::setStrictName\(\) '.
            'must be of the type string, int(eger)? given/'
        );

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
