<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Globals;

use PHPUnit\Framework\TestCase;
use Stub\Globals\ServerRequestFactory;

final class ServerRequestFactoryTest extends TestCase
{
    private array $server;
    private array $get;
    private array $post;

    protected function setUp(): void
    {
        $this->server = $_SERVER;
        $this->get = $_GET;
        $this->post = $_POST;
    }

    protected function tearDown(): void
    {
        $_SERVER = $this->server;
        $_GET = $this->get;
        $_POST = $this->post;
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1934
     */
    public function testShouldNotTriggerAnyErrorIfPostIsUndefined(): void
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
