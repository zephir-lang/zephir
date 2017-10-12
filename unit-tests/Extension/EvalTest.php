<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

class EvalTest extends \PHPUnit_Framework_TestCase
{
    public function testEval()
    {
        $t = new \Test\EvalTest();

        $this->assertSame(2, $t->evalCode('return 1 + 1;'));
        $this->assertSame(8, $t->evalCode('$g = pow(2, 2); return $g * 2;'));
    }
}
