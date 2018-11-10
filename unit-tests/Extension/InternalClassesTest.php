<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use Phalcon\Di;
use Phalcon\Mvc\Model\Query;
use Test\InternalClasses;
use Zephir\Support\TestCase;

class InternalClassesTest extends TestCase
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
