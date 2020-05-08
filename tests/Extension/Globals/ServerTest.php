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
use Stub\Globals\Server;

class ServerTest extends TestCase
{
    private $server;

    /**
     * This method is called before each test.
     */
    protected function setUp()
    {
        $this->server = $_SERVER;
    }

    /**
     * This method is called after each test.
     */
    protected function tearDown()
    {
        $_SERVER = $this->server;
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/1961
     */
    public function shouldPrintPhpSelf()
    {
        $test = new Server();

        ob_start();
        $test->f1();
        $actual = ob_get_contents();
        ob_end_clean();

        $this->assertSame(str_repeat($_SERVER['PHP_SELF'], 3), $actual);
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/1961
     */
    public function shouldPrintUserAgent()
    {
        $test = new Server();
        $_SERVER['HTTP_USER_AGENT'] = 'Test';

        $this->assertSame('Test', $test->check());
    }
}
