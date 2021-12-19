<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\ArrayAccessObj;

final class ArrayAccessObjTest extends TestCase
{
    public function testOffsetSetAndOffsetExists(): void
    {
        $class = new ArrayAccessObj();

        $class->offsetSet('newKey', 'new value');
        $this->assertTrue($class->offsetExists('newKey'));
    }

    public function testOffsetGet(): void
    {
        $class = new ArrayAccessObj();

        $class->offsetSet('newKey', 'new value');
        $this->assertSame('new value', $class->offsetGet('newKey'));

        $class->offsetSet('newKey', 1);
        $this->assertSame(1, $class->offsetGet('newKey'));

        $class->offsetSet('boolean', true);
        $this->assertSame(true, $class->offsetGet('boolean'));
    }

    public function testOffsetUnset(): void
    {
        $class = new ArrayAccessObj();

        $class->offsetSet('newKey', 'new value');
        $this->assertTrue($class->offsetExists('newKey'));
        $class->offsetUnset('newKey');
        $this->assertFalse($class->offsetExists('newKey'));
    }
}
