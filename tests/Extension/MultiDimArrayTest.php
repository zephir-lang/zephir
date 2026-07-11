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
use Stub\MultiDimArray;

/**
 * Multi-dimensional array assignment (`let a[i][j] = v`). Locks the runtime
 * behaviour across every offset shape so the kernel final-offset fast-path
 * from issue #1884 provably preserves semantics.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884
 */
final class MultiDimArrayTest extends TestCase
{
    private MultiDimArray $array;

    protected function setUp(): void
    {
        $this->array = new MultiDimArray();
    }

    public function testGenerateBuildsInclusiveMatrix(): void
    {
        // range(0, 2) is inclusive -> a 3x3 matrix of 1s.
        $row      = [1, 1, 1];
        $expected = [$row, $row, $row];

        $this->assertSame($expected, $this->array->generate(2));
    }

    public function testGenerateTypedMatchesDynamicKeys(): void
    {
        $this->assertSame(
            $this->array->generate(3),
            $this->array->generateTyped(3),
            'int-typed loop vars ("ll" offsets) must produce the same matrix as dynamic vars ("zz").'
        );
    }

    public function testGenerateZeroStillBuildsSingleCell(): void
    {
        $this->assertSame([[1]], $this->array->generate(0));
    }

    public function testIntKeys(): void
    {
        $this->assertSame([0 => [1 => 10]], $this->array->intKeys());
    }

    public function testStringKeys(): void
    {
        $this->assertSame(['x' => ['y' => 10]], $this->array->stringKeys());
    }

    public function testThreeLevel(): void
    {
        $this->assertSame(['x' => ['y' => ['z' => 10]]], $this->array->threeLevel());
    }

    public function testMixedKeys(): void
    {
        $this->assertSame(['k' => [0 => 1], 1 => ['k' => 2]], $this->array->mixedKeys());
    }

    public function testAppendFinal(): void
    {
        $this->assertSame(
            [0 => ['first', 'second'], 'bag' => [1]],
            $this->array->appendFinal()
        );
    }

    public function testOverwriteFinalKeepsLastValue(): void
    {
        $this->assertSame([0 => [0 => 2]], $this->array->overwriteFinal());
    }

    public function testOverwriteArrayWithScalar(): void
    {
        $this->assertSame(['p' => ['q' => 99]], $this->array->overwriteArrayWithScalar());
    }
}
