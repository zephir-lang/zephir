<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Globals;

use PHPUnit\Framework\TestCase;
use Stub\Globals\Session\Child;

class SessionTest extends TestCase
{
    /**
     * This method is called before a test is executed.
     */
    protected function setUp(): void
    {
        parent::setUp();

        if (!isset($_SESSION)) {
            $_SESSION = [];
        }
    }

    protected function tearDown(): void
    {
        unset($_SESSION);
    }

    public function testDestroy()
    {
        $this->assertSame([], $_SESSION);

        $tester = new Child();
        $tester->test = 'Some session data here';

        $this->assertArrayHasKey('test', $_SESSION);
        $this->assertStringContainsString('Some session data here', $_SESSION['test']);

        $result = $tester->destroy();

        $this->assertSame([], $result);
        $this->assertSame([], $_SESSION);
        $this->assertSame($_SESSION, $result);
    }

    public function testDirectUnset()
    {
        $this->assertSame([], $_SESSION);
        $_SESSION['bar'] = 'foo';

        $tester = new Child();
        $tester->__unset('bar');

        $this->assertSame([], $_SESSION);
    }

    public function testRemove()
    {
        $this->assertSame([], $_SESSION);
        $_SESSION['foo'] = 'bar';

        $tester = new Child();
        $tester->remove('foo');

        $this->assertSame([], $_SESSION);
    }

    public function testRemoveFromUnset()
    {
        $this->assertSame([], $_SESSION);
        $_SESSION['bar'] = 'foo';

        $tester = new Child();
        unset($tester->bar);

        $this->assertSame([], $_SESSION);
    }
}
