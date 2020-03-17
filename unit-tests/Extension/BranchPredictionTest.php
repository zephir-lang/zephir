<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class BranchPredictionTest extends TestCase
{
    public function testVariants()
    {
        $br = new \Test\BranchPrediction();

        $this->assertTrue($br->testLikely1());
        $this->assertFalse($br->testLikely2(false));
        $this->assertTrue($br->testUnlikely1());
        $this->assertFalse($br->testUnlikely2(false));
    }
}
