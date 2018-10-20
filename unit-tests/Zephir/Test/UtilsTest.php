<?php

/**
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use Zephir\Support\TestCase;
use Zephir\Utils;

class UtilsTest extends TestCase
{
    /** @test */
    public function shouldEscapeClassName()
    {
        $classname = '\Bar\Foo';
        $this->assertSame(
            Utils::escapeClassName($classname),
            '\\\\Bar\\\\Foo'
        );
    }

    /** @test */
    public function shouldCamelizeClassName()
    {
        $name = 'foo_Bar_Foo_bar';
        $this->assertSame(
            Utils::camelize($name),
            'FooBarFooBar'
        );
    }
}
