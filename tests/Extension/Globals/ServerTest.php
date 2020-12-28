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

    protected function setUp(): void
    {
        $this->server = $_SERVER;
    }

    protected function tearDown(): void
    {
        $_SERVER = $this->server;
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1961
     */
    public function testShouldPrintPhpSelf()
    {
        $test = new Server();

        ob_start();
        $test->f1();
        $actual = ob_get_contents();
        ob_end_clean();

        $this->assertSame(str_repeat($_SERVER['PHP_SELF'], 3), $actual);
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1961
     */
    public function testShouldPrintUserAgent()
    {
        $test = new Server();
        $_SERVER['HTTP_USER_AGENT'] = 'Test';

        $this->assertSame('Test', $test->check());
    }
}
