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
use Stub\Globals\Env;

class EnvTest extends TestCase
{
    protected function setUp(): void
    {
        parent::setUp();

        if (false === strpos(ini_get('variables_order'), 'E')) {
            $this->markTestSkipped(
                "variables_order ini directive does not contain 'E'. ".
                "Make sure you have set variables_order to 'EGPCS' in php.ini."
            );
        }

        if (!isset($_ENV)) {
            $_ENV = [];
        }
    }

    public function testEmpty()
    {
        $tester = new Env();
        $var = 'non-existing';

        $this->assertSame(false, $tester->read($var));
        $this->assertSame(false, getenv($var));
    }

    public function testReadStandard()
    {
        $tester = new Env();

        if ('WIN' === strtoupper(substr(PHP_OS, 0, 3))) {
            $var = 'USERNAME';
        } else {
            $var = 'USER';
        }

        $this->assertSame($_ENV[$var], $tester->read($var));
        $this->assertSame($_ENV[$var], getenv($var));
    }

    public function testReadNew()
    {
        $_ENV['NEW_VARIABLEFROM'] = __FUNCTION__;
        $tester = new Env();

        $this->assertSame($_ENV['NEW_VARIABLEFROM'], $tester->read('NEW_VARIABLEFROM'));
        $this->assertSame(__FUNCTION__, $tester->read('NEW_VARIABLEFROM'));
    }
}
