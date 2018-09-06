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

use PHPUnit\Framework\TestCase;
use Test\Globals\Env;
use Zephir\Utils;

class EnvTest extends TestCase
{
    /**
     * This method is called before a test is executed.
     */
    public function setUp()
    {
        parent::setUp();

        if (strpos(ini_get('variables_order'), 'E') === false) {
            $this->markTestSkipped(
                "variables_order ini directive does not contain 'E'." .
                "Make sure you have set variables_order to 'EGPCS' in php.ini."
            );
        }

        if (!isset($_ENV)) {
            $_ENV = [];
        }
    }

    /** @test */
    public function readStandard()
    {
        $tester = new Env();

        if (Utils::isWindows()) {
            $var = 'USERNAME';
        } else {
            $var = 'USER';
        }

        $this->assertSame($_ENV[$var], $tester->read($var));
        $this->assertSame($_ENV[$var], getenv($var));
    }

    /** @test */
    public function readNew()
    {
        $_ENV['NEW_VARIABLEFROM'] = __FUNCTION__;
        $tester = new Env();

        $this->assertSame($_ENV['NEW_VARIABLEFROM'], $tester->read('NEW_VARIABLEFROM'));
        $this->assertSame(__FUNCTION__, $tester->read('NEW_VARIABLEFROM'));
    }
}
