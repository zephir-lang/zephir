<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Issue2533;

/**
 * Array class constants must be declared on the class entry (readable from
 * PHP) and resolvable from Zephir code via `self::CONST`.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2533
 */
final class Issue2533Test extends TestCase
{
    public function testArrayConstantReadableFromPhp(): void
    {
        $this->assertSame([1, 2, 3], Issue2533::BAR);
        $this->assertSame(['a' => 1, 'b' => 2], Issue2533::MAP);
        $this->assertSame([[1, 2], ['k' => 'v']], Issue2533::NESTED);
        $this->assertSame([], Issue2533::EMPTY_ARR);
    }

    public function testArrayConstantReturnedFromMethod(): void
    {
        $object = new Issue2533();

        $this->assertSame([1, 2, 3], $object->bar());
        $this->assertSame(['a' => 1, 'b' => 2], $object->map());
        $this->assertSame([[1, 2], ['k' => 'v']], $object->nested());
    }
}
