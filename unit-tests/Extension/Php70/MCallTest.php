<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Php70;

use PHPUnit\Framework\TestCase;
use Test\Mcall;

class MCallTest extends TestCase
{
    /** @test */
    public function shouldThrowInvalidArgumentExceptionForOptionalBoolean()
    {
        $test = new Mcall();

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'start' must be of the type bool");

        $test->optionalParameterBoolean('test');
    }

    /** @test */
    public function shouldThrowTypeErrorForOptionalBoolean()
    {
        $test = new Mcall();

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'start' must be of the type bool");

        $test->optionalParameterBoolean([]);
    }
}
