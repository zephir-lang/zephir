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

class ScallTest extends \PHPUnit_Framework_TestCase
{
    public function testScall()
    {
        $t = new \Test\Scall();

        $this->assertSame($t->testMethod1(), 'hello public');
        $this->assertSame($t::testMethod1(), 'hello public');
        $this->assertSame(\Test\Scall::testMethod1(), 'hello public');

        $this->assertSame($t->testCall1(), 'hello public');
        $this->assertSame($t->testCall2(), 'hello protected');
        $this->assertSame($t->testCall3(), 'hello private');

        $this->assertSame($t->testCall4(2, 3), 5);
        $this->assertSame($t->testCall5(6, 7), 13);
        $this->assertSame($t->testCall6(4, 5), 9);

        $this->assertSame($t->testCall7(), 'hello public');
        $this->assertSame($t->testCall8(), 'hello protected');
        $this->assertSame($t->testCall9(), 'hello private');

        $this->assertSame($t->testCall10(2, 3), 5);
        $this->assertSame($t->testCall11(6, 7), 13);
        $this->assertSame($t->testCall12(4, 5), 9);

        $this->assertSame($t->testCall13(), 'hello parent public');
        $this->assertSame($t->testCall14(), 'hello parent protected');

        $this->assertSame('hello ScallParent', \Test\Scallparent::testCallStatic());
        $this->assertSame('hello Scall', $t::testCallStatic());
    }
}
