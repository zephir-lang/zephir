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

class TryTest extends \PHPUnit_Framework_TestCase
{
    public function testThrow1()
    {
        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\Exception', 'error');
        } else {
            $this->expectException('\Exception', 'error');
        }

        $t = new \Test\TryTest();
        $t->testThrow1();
    }

    public function testThrow2()
    {
        if (!method_exists('PHPUnit_Runner_Version', 'id') ||
            version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
            $this->setExpectedException('\Exception', 'error');
        } else {
            $this->expectException('\Exception', 'error');
        }

        $t = new \Test\TryTest();
        $t->testThrow2();
    }
}
