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

class BranchPredictionTest extends \PHPUnit_Framework_TestCase
{
    public function testVariants()
    {
        $b = new \Test\BranchPrediction();

        $this->assertTrue($b->testLikely1());
        $this->assertFalse($b->testLikely2(false));
        $this->assertTrue($b->testUnlikely1());
        $this->assertFalse($b->testUnlikely2(false));
    }
}
