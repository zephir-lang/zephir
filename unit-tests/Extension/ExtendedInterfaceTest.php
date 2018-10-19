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

use Test\ExtendedInterface;
use PHPUnit\Framework\TestCase;

class ExtendedInterfaceTest extends TestCase
{
    /** @test */
    public function shouldExtendsInterfaces()
    {
        try {
            $reflection = new \ReflectionClass(ExtendedInterface::class);
        } catch (\ReflectionException $e) {
            $this->fail($e->getMessage());
            return;
        }

        $this->assertTrue($reflection->isInterface());
        $this->assertContains('IteratorAggregate', $reflection->getInterfaceNames());
        $this->assertContains('Countable', $reflection->getInterfaceNames());
    }
}
