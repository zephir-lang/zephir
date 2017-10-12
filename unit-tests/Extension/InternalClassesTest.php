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

use Phalcon\Di;
use Phalcon\Mvc\Model\Query;
use Test\InternalClasses;

/**
 * Class InternalClassesTest
 * @package Extension
 */
class InternalClassesTest extends \PHPUnit_Framework_TestCase
{
    public function testStaticMethodCall()
    {
        $class = new InternalClasses();
        $this->assertSame(Di::getDefault(), $class->testStaticCall());
    }

    public function testStaticPropertyFetch()
    {
        $class = new InternalClasses();
        $this->assertSame(Query::TYPE_DELETE, $class->testStaticPropertyFetch());
    }
}
