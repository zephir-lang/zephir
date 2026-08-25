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
use Stub\Issue2061;

/**
 * A class constant initializer and a property default may be a full expression;
 * the compiler folds it to a literal.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2061
 */
final class Issue2061Test extends TestCase
{
    public function testTheDeclarationFromTheIssueReportCompiles(): void
    {
        $this->assertSame(-128, Issue2061::INT8_MIN);
        $this->assertSame(127, Issue2061::INT8_MAX);
        $this->assertSame(-32768, Issue2061::INT16_MIN);
        $this->assertSame(32767, Issue2061::INT16_MAX);
    }

    public function testArithmeticAndBitwiseInitializers(): void
    {
        $this->assertSame(1048576, Issue2061::MB);
        $this->assertSame(65295, Issue2061::MASK);
        $this->assertSame(-1, Issue2061::INVERTED);
        $this->assertSame(2.5, Issue2061::RATIO);
        $this->assertSame(1, Issue2061::REMAINDER);
    }

    public function testConcatenationInitializers(): void
    {
        $this->assertSame('issue2061', Issue2061::NAME);
        $this->assertSame('issue2061/limits', Issue2061::PATH);
    }

    public function testInitializersReadingOtherConstants(): void
    {
        $this->assertSame(2048, Issue2061::DOUBLED);
        $this->assertSame(PHP_INT_SIZE * 8, Issue2061::FROM_ENV);
    }

    public function testComparisonLogicalAndTernaryInitializers(): void
    {
        $this->assertTrue(Issue2061::FLAG);
        $this->assertTrue(Issue2061::NEGATED);
        $this->assertSame(PHP_INT_SIZE === 8 ? 64 : 32, Issue2061::WORD);
        $this->assertSame(42, Issue2061::FALLBACK);
    }

    public function testExpressionsInsideAnArrayConstant(): void
    {
        $this->assertSame([0 => 2, 1 => 4, 'sum' => 7], Issue2061::NUMBERS);
    }

    public function testAnExpressionConstantUsedAsAParameterDefault(): void
    {
        $object = new Issue2061();

        $this->assertSame(2048, $object->withDefault());
        $this->assertSame(7, $object->withDefault(7));
    }

    public function testPropertyDefaults(): void
    {
        $object = new Issue2061();

        $this->assertSame(8192, $object->getSize());
        $this->assertSame(65280, $object->getMask());
        $this->assertSame(5, $object->getTotal());
        $this->assertSame('a/b', $object->getLabel());
    }
}
