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
use Stub\Globals\ServerRequestFactory;

class ServerRequestFactoryTest extends TestCase
{
    private $server;
    private $get;
    private $post;

    /**
     * This method is called before each test.
     */
    protected function setUp()
    {
        $this->server = $_SERVER;
        $this->get = $_GET;
        $this->post = $_POST;
    }

    /**
     * This method is called after each test.
     */
    protected function tearDown()
    {
        $_SERVER = $this->server;
        $_GET = $this->get;
        $_POST = $this->post;
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1934
     */
    public function shouldNotTriggerAnyErrorIfPostIsUndefined()
    {
        $tester = new ServerRequestFactory();

        $_SERVER = null;
        $_GET = null;
        $_POST = null;

        $tester->load($_SERVER, $_GET, $_POST);
        $tester->load($_SERVER, $_GET, $_POST);
        $tester->load($_SERVER, $_GET, $_POST);

        $this->assertEquals(
            [
                'server' => [],
                'files' => [],
                'cookies' => [],
                'get' => [],
                'post' => [],
            ],
            $tester->load()
        );
    }
}
