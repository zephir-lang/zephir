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

namespace Extension\Globals;

use Test\Globals\Session\Child;

class SessionTest extends \PHPUnit_Framework_TestCase
{
    /**
     * This method is called before a test is executed.
     */
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

    /** @test */
    public function directUnset()
    {
        $this->assertSame([], $_SESSION);
        $_SESSION['bar'] = 'foo';

        $tester = new Child();
        $tester->__unset('bar');

        $this->assertSame([], $_SESSION);
    }

    /** @test */
    public function remove()
    {
        $this->assertSame([], $_SESSION);
        $_SESSION['foo'] = 'bar';

        $tester = new Child();
        $tester->remove('foo');

        $this->assertSame([], $_SESSION);
    }

    /** @test */
    public function removeFromUnset()
    {
        $this->assertSame([], $_SESSION);
        $_SESSION['bar'] = 'foo';

        $tester = new Child();
        unset($tester->bar);

        $this->assertSame([], $_SESSION);
    }
}
