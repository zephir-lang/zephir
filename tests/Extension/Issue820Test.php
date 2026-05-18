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

use ArrayIterator;
use IteratorAggregate;
use PHPUnit\Framework\TestCase;
use Stub\Issue820;
use TypeError;

/**
 * Regression coverage for `for k, v in iterator(...)` when given a
 * non-Iterator object (e.g. `stdClass`). It used to segfault — the kernel
 * helper returned NULL, codegen dereferenced it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/820
 */
final class Issue820Test extends TestCase
{
    public function testStdClassThrowsTypeError(): void
    {
        $obj = (object)['prop0' => 0, 'prop1' => 1, 'prop2' => 2];

        $this->expectException(TypeError::class);

        Issue820::testIterator($obj);
    }

    public function testArrayThrowsTypeError(): void
    {
        $this->expectException(TypeError::class);

        Issue820::testIterator(['a' => 1, 'b' => 2]);
    }

    public function testScalarThrowsTypeError(): void
    {
        $this->expectException(TypeError::class);

        Issue820::testIterator(42);
    }

    public function testIteratorImplementorWorks(): void
    {
        $iter = new ArrayIterator(['a' => 1, 'b' => 2, 'c' => 3]);

        $this->assertSame(
            ['a' => 1, 'b' => 2, 'c' => 3],
            Issue820::testIterator($iter)
        );
    }

    public function testIteratorAggregateImplementorWorks(): void
    {
        $agg = new class implements IteratorAggregate {
            public function getIterator(): ArrayIterator
            {
                return new ArrayIterator(['x' => 10, 'y' => 20]);
            }
        };

        $this->assertSame(
            ['x' => 10, 'y' => 20],
            Issue820::testIterator($agg)
        );
    }
}
