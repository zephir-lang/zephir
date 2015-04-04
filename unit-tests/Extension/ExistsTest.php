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

class ExistsTEst extends \PHPUnit_Framework_TestCase
{
    public function testAssertations()
    {
        $t = new \Test\Exists();

        $this->assertTrue($t->testClassExists("TestExClass") == true);
        $this->assertTrue($t->testClassExists("TestExClassx") == false);
        $this->assertTrue($t->testClassExists("TestExInterface") == false);
        // with namespace
        $this->assertTrue($t->testClassExists("\\Test\\Exists") == true);


        $this->assertTrue($t->testInterfaceExists("TestExClass") == false); // class not interface
        $this->assertTrue($t->testInterfaceExists("TestExInterface") == true);
        $this->assertTrue($t->testInterfaceExists("TestExInterfacex") == false);

        $this->assertTrue($t->testMethodExists($t, "testMethodExists") == true);

        $this->assertTrue($t->testFileExists(__DIR__ . "/php/exists.php") == true);
        $this->assertTrue($t->testFileExists(__DIR__ . "/php/existsxxxx.php") == false);
    }
}
