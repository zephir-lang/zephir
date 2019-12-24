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

namespace Extension\Php72;

use PHPUnit\Framework\TestCase;
use Test\Mcall;

class MCallTest extends TestCase
{
    /** @test */
    public function shouldThrowTypeErrorForOptionalBoolean1()
    {
        $t = new Mcall();

        $this->expectException(\TypeError::class);
        $this->expectExceptionMessageRegExp(
            // PHP 7.3 : bool
            // PHP 7.2 : boolean
            '/Argument 1 passed to Test\\\Mcall::optionalParameterBoolean\(\) '.
            'must be of the type bool(ean)?, string given/'
        );

        $t->optionalParameterBoolean('test');
    }

    /** @test */
    public function shouldThrowTypeErrorForOptionalBoolean2()
    {
        $t = new Mcall();

        $this->expectException(\TypeError::class);
        $this->expectExceptionMessageRegExp(
            // PHP 7.3 : bool
            // PHP 7.2 : boolean
            '/Argument 1 passed to Test\\\Mcall::optionalParameterBoolean\(\) '.
            'must be of the type bool(ean)?, array given/'
        );

        $t->optionalParameterBoolean([]);
    }
}
