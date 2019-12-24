<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use function Zephir\camelize;
use function Zephir\escape_class;

class HelpersTest extends TestCase
{
    /** @test */
    public function shouldEscapeClassName()
    {
        $classname = '\Bar\Foo';
        $this->assertSame(
            escape_class($classname),
            '\\\\Bar\\\\Foo'
        );
    }

    /** @test */
    public function shouldCamelizeClassName()
    {
        $name = 'foo_Bar_Foo_bar';
        $this->assertSame(
            camelize($name),
            'FooBarFooBar'
        );
    }
}
