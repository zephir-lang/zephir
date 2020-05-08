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

use Phalcon\Di;
use Phalcon\Mvc\Model\Query;
use PHPUnit\Framework\TestCase;
use Test\InternalClasses;

class InternalClassesTest extends TestCase
{
    public function testStaticMethodCall()
    {
        if (false === class_exists(Di::class)) {
            $this->markTestSkipped('Class Phalcon\Di not found');
        }

        $class = new InternalClasses();
        $this->assertSame(Di::getDefault(), $class->testStaticCall());
    }

    public function testStaticPropertyFetch()
    {
        if (false === class_exists(Query::class)) {
            $this->markTestSkipped('Class Phalcon\Mvc\Model\Query not found');
        }

        $class = new InternalClasses();
        $this->assertSame(Query::TYPE_DELETE, $class->testStaticPropertyFetch());
    }
}
