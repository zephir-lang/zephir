<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

class VarsTest extends \PHPUnit_Framework_TestCase
{
    public function testVarDump()
    {
        ob_start();

        $t = new \Test\Vars();
        $t->testVarDump();
        $t->testVarDump2param(3.1, true);
        $t->testVarDump3param(3.1, true, array(1, 2, 3));
        $t->testVarDump2param(3.1, true);

        ob_clean();
    }

    public function testVarExport()
    {
        ob_start();

        $t = new \Test\Vars();
        $this->assertEquals($t->testVarExport(), "'hello'");

        ob_clean();
    }

    public function test88Issue()
    {
        ob_start();

        $t = new \Test\Vars();
        $t->test88Issue('foo', 'bar');
        $t->test88IssueParam2InitString('foo', 'bar');

        ob_clean();
    }

    public function testVarDumpAndCountOptimizer()
    {
        ob_start();

        $t = new \Test\Vars();
        $t->testCountOptimizerVarDumpAndExport(array(1, 2, 3, 4, 5));

        ob_clean();
    }

    public function testArrayTypeVarDumpAndExport()
    {
        ob_start();

        $t = new \Test\Vars();
        $t->testArrayTypeVarDumpAndExport(array(1, 2, 3));

        ob_clean();
    }

    public function testIntVarDump()
    {
        ob_start();

        $t = new \Test\Vars();
        $t->testIntVarDump();

        ob_clean();
    }

    public function testDoubleVarDump()
    {
        ob_start();

        $t = new \Test\Vars();
        $t->testDoubleVarDump();

        ob_clean();
    }

    public function testBoolVarDump()
    {
        ob_start();

        $t = new \Test\Vars();
        $t->testBoolVarDump();

        ob_clean();
    }
}
