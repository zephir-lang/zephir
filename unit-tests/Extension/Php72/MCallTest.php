<?php

declare(strict_types=1);

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Php72;

use PHPUnit\Framework\TestCase;
use Test\Mcall;

class MCallTest extends TestCase
{
    /** @test */
    public function shouldThrowInvalidArgumentExceptionForOptionalBoolean()
    {
        $t = new Mcall();

        $this->expectException(\TypeError::class);
        $this->expectExceptionMessage(
            'Argument 1 passed to Test\Mcall::optionalParameterBoolean() must be of the type bool, string given'
        );

        $t->optionalParameterBoolean('test');
    }

    /** @test */
    public function shouldThrowTypeErrorForOptionalBoolean()
    {
        $t = new Mcall();

        $this->expectException(\TypeError::class);
        $this->expectExceptionMessage(
            'Argument 1 passed to Test\Mcall::optionalParameterBoolean() must be of the type bool, array given'
        );

        $t->optionalParameterBoolean([]);
    }
}
