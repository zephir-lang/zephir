<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Php56;

use PHPUnit\Framework\TestCase;
use Test\Mcall;

class MCallTest extends TestCase
{
    public function testOptionalParameterBooleanException()
    {
        $t = new Mcall();

        $this->expectException(\InvalidArgumentException::class);

        $t->optionalParameterBoolean('test');
    }
}
