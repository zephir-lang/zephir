<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Globals;

use PHPUnit\Framework\TestCase;
use Test\Globals\Session\Base;
use Test\Globals\Session\Child;

class SessionTest extends TestCase
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
        $tester->test = 'Some session data here';

        $this->assertArrayHasKey('test', $_SESSION);
        $this->assertContains('Some session data here', $_SESSION['test']);

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

    /**
     * @test
     * @see https://github.com/phalcon/zephir/issues/1775
     */
    public function shouldAccessToTheSession()
    {
        unset($_SESSION);
        $this->assertFalse(isset($_SESSION));

        $t = new Base();
        $t->set('foo', 'bar');

        $this->assertTrue(isset($_SESSION));
        /** @noinspection PhpUndefinedVariableInspection */
        $this->assertSame(['foo' => 'bar'], $_SESSION);
    }
}
