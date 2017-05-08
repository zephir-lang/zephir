<?php
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension\Globals;

use Test\Globals\Session\Child;

class SessionTest extends \PHPUnit_Framework_TestCase
{
    public function setUp()
    {
        parent::setUp();

        if (!isset($_SESSION)) {
            $_SESSION = [];
        }
    }

    /** @test */
    public function destroy()
    {
        $this->assertSame([], $_SESSION);

        $tester = new Child();
        $tester->test = "Some session data here";

        $this->assertArrayHasKey('test', $_SESSION);
        $this->assertContains("Some session data here", $_SESSION['test']);

        $result = $tester->destroy();

        $this->assertSame([], $result);
        $this->assertSame([], $_SESSION);
        $this->assertSame($_SESSION, $result);
    }
}
