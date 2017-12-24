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

use Error;
use Test\Instance;

require DATA_PATH . '/TestAbstractClass.php';

class InstanceTest extends \PHPUnit_Framework_TestCase
{
    public function testIssue1339()
    {
        $t = Instance::testIssue1339();
        $this->assertInstanceOf("Test\\Instance", $t);
    }

    public function testInstanceAbstract()
    {
        if (PHP_VERSION_ID < 70000) {
            $this->markTestSkipped(
                "We can't catch fatal errors on php 5.x"
            );
        }

        try {
            $t = Instance::testInstanceCreate(\TestAbstractClass::class);
        } catch (Error $e) {
            $this->assertEquals("Cannot instantiate abstract class TestAbstractClass", $e->getMessage());
        }
    }
}
