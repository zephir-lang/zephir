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
use Stub\Mcall;

class MCallTest extends TestCase
{
    public function testShouldThrowInvalidArgumentExceptionForOptionalBoolean()
    {
        $test = new Mcall();

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'start' must be of the type bool");

        $test->optionalParameterBoolean('test');
    }

    public function testShouldThrowTypeErrorForOptionalBoolean()
    {
        $test = new Mcall();

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'start' must be of the type bool");

        $test->optionalParameterBoolean([]);
    }
}
