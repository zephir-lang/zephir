<?php declare(strict_types=1);

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Php72\Oo;

use PHPUnit\Framework\TestCase;
use Test\Oo\OoParams;

class OoParamsStrictTest extends TestCase
{
    public function testSetStrictAgeException1()
    {
        $t = new OoParams();

        $this->expectException(\TypeError::class);

        $t->setStrictAge(17.0);
    }

    public function testSetStrictAgeException2()
    {
        $t = new OoParams();

        $this->expectException(\TypeError::class);

        $t->setStrictAge('17');
    }

    public function testSetStrictAverageException2()
    {
        $t = new OoParams();

        $this->expectException(\TypeError::class);

        $t->setStrictAverage('17');
    }

    public function testSetStrictNameException()
    {
        $t = new OoParams();

        $this->expectException(\TypeError::class);

        $t->setStrictName(1234);
    }
}
