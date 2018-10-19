<?php

/*
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use Test\Instance;
use PHPUnit\Framework\TestCase;

class InstanceTest extends TestCase
{
    public function testIssue1339()
    {
        $this->assertInstanceOf(Instance::class, Instance::testIssue1339());
    }

    /**
     * @test
     * @expectedException \Error
     * @expectedExceptionMessage Cannot instantiate abstract class TestAbstractClass
     */
    public function shouldThrowErrorOnInstantiateAbstractClass()
    {
        if (PHP_VERSION_ID < 70000) {
            $this->markTestSkipped("We can't catch fatal errors on php 5.x");
        }

        Instance::testInstanceCreate(\TestAbstractClass::class);
    }
}
