<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;

class StaticPropertiyReferenceTest extends TestCase
{
    public function testAssertations()
    {
        $value = &\Test\Properties\StaticPropertyReference::$reference;

        $value = 'test1';
        $this->assertSame(\Test\Properties\StaticPropertyReference::$reference, $value);

        \Test\Properties\StaticPropertyReference::$reference = 'test2';
        $this->assertSame(\Test\Properties\StaticPropertyReference::$reference, $value);

        \Test\Properties\StaticPropertyReference::setValue('test3');
        $this->assertSame(\Test\Properties\StaticPropertyReference::$reference, $value);
    }
}
