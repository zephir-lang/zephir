<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Php70\Globals;

use PHPUnit\Framework\TestCase;
use Stub\Globals\Session\Base;

class SessionTest extends TestCase
{
    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/1775
     */
    public function shouldAccessToTheSession()
    {
        unset($_SESSION);
        $this->assertFalse(isset($_SESSION));

        $test = new Base();
        $test->set('foo', 'bar');

        $this->assertTrue(isset($_SESSION));
        /* @noinspection PhpUndefinedVariableInspection */
        $this->assertSame(['foo' => 'bar'], $_SESSION);
    }
}
