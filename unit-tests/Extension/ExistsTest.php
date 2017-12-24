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
