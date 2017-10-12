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

use Test\Instance;

class InstanceTest extends \PHPUnit_Framework_TestCase
{
    public function testIssue1339()
    {
        $t = Instance::testIssue1339();
        $this->assertInstanceOf("Test\\Instance", $t);
    }
}
