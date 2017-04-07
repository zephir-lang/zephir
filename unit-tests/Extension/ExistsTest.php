<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team and contributors                     |
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

use Test\Exists;

require DATA_PATH . '/TestExClass.php';
require DATA_PATH . '/TestExInterface.php';

class ExistsTest extends \PHPUnit_Framework_TestCase
{
    public function testAssertations()
    {
        $t = new Exists();

        $this->assertTrue($t->testClassExists('TestExClass'));
        $this->assertFalse($t->testClassExists('TestExClassx'));
        $this->assertFalse($t->testClassExists('TestExInterface'));
        // with namespace
        $this->assertTrue($t->testClassExists('\\Test\\Exists'));

        // class not interface
        $this->assertFalse($t->testInterfaceExists('TestExClass'));
        $this->assertTrue($t->testInterfaceExists('TestExInterface'));
        $this->assertFalse($t->testInterfaceExists('TestExInterfacex'));

        $this->assertTrue($t->testMethodExists($t, 'testMethodExists'));

        $this->assertTrue($t->testFileExists(__DIR__ . '/php/exists.php'));
        $this->assertFalse($t->testFileExists(__DIR__ . '/php/existsxxxx.php'));
    }
}
