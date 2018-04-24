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

namespace Extension\Oo\Scope;

use PHPUnit\Framework\TestCase;
use Test\Oo\Scopes\PrivateScopeTester;

class PrivateScopeTest extends TestCase
{
    /** @test */
    public function shouldCallPrivateMethod()
    {
        $this->assertSame('isPrivate', (new PrivateScopeTester())->run());
    }
}
