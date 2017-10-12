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

use \Test\Scope;

class ScopeTest extends \PHPUnit_Framework_TestCase
{
    public function testScope1()
    {
        $this->assertSame(Scope::test1(), 'testinternal_string');
        $this->assertSame(Scope::test2(), array(15, '0_66_132_198_'));
        $this->assertSame(Scope::test3(), 'internal_0internal_1internal_2internal_3');
    }
}
