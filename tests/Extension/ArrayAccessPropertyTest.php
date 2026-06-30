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
use Stub\ArrayAccessProperty;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2465
 */
final class ArrayAccessPropertyTest extends TestCase
{
    public function testOffsetSetIsTriggeredOnObjectProperty(): void
    {
        $collection = new ArrayAccessObj();
        $holder     = new ArrayAccessProperty($collection);

        $holder->set('k', 'v');

        // offsetSet() must have been called on the ArrayAccess object.
        $this->assertTrue($collection->offsetExists('k'));
        $this->assertSame('v', $collection->offsetGet('k'));
    }

    public function testObjectPropertyIsNotConvertedToArray(): void
    {
        $collection = new ArrayAccessObj();
        $holder     = new ArrayAccessProperty($collection);

        $holder->set('k', 'v');

        // The property must remain the same ArrayAccess object, not a plain array.
        $this->assertInstanceOf(ArrayAccessObj::class, $holder->data);
        $this->assertSame($collection, $holder->data);
    }

    /**
     * A chained write (this->data[a][b] = v) through an ArrayAccess property
     * must behave exactly like native PHP: it raises an "Indirect modification
     * of overloaded element" notice, the write has no effect, and the property
     * stays the same object instead of being converted into an array.
     */
    public function testNestedWriteMatchesNativePhpBehaviour(): void
    {
        $collection = new ArrayAccessObj();
        $holder     = new ArrayAccessProperty($collection);

        $message = null;
        set_error_handler(static function (int $code, string $msg) use (&$message): bool {
            $message = $msg;

            return true;
        });

        try {
            $holder->setNested('a', 'b', 'v');
        } finally {
            restore_error_handler();
        }

        $this->assertNotNull($message, 'Expected an indirect-modification notice');
        $this->assertStringContainsString('Indirect modification of overloaded element', $message);
        $this->assertStringContainsString(ArrayAccessObj::class, $message);

        // Object preserved, not converted to a plain array.
        $this->assertInstanceOf(ArrayAccessObj::class, $holder->data);
        $this->assertSame($collection, $holder->data);

        // The write had no effect.
        $this->assertFalse($collection->offsetExists('a'));
    }
}
