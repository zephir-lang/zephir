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
use Stub\Returns;

final class ReturnsTest extends TestCase
{
    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1534
     */
    public function testShouldReturnWithParameter(): void
    {
        $tester = new Returns();
        $this->assertSame('Return back', $tester->returnWithParameter('Hello World!'));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1534
     */
    public function testShouldReturnWithParameterStaticVersion(): void
    {
        $this->assertSame('Return back', Returns::returnWithParameter('Hello World!'));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1534
     */
    public function testShouldReturnWithoutParameter(): void
    {
        $tester = new Returns();
        $this->assertSame('Return back', $tester->returnWithoutParameter());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1534
     */
    public function testShouldReturnWithoutParameterStaticVersion(): void
    {
        $this->assertSame('Return back', Returns::returnWithoutParameter());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     */
    public function testIssue1991ShouldThrow(): void
    {
        $tester = new Returns();

        $this->expectException(\TypeError::class);

        $tester->returnNullOnString();
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     */
    public function testReturnNullOnIntShouldThrowTypeError(): void
    {
        $tester = new Returns();

        $this->expectException(\TypeError::class);

        $tester->returnNullOnInt();
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     */
    public function testReturnNullOnArrayShouldThrowTypeError(): void
    {
        $tester = new Returns();

        $this->expectException(\TypeError::class);

        $tester->returnNullOnArray();
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     */
    public function testReturnNullOnDoubleShouldThrowTypeError(): void
    {
        $tester = new Returns();

        $this->expectException(\TypeError::class);

        $tester->returnNullOnDouble();
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     * Sanity check: returning a value that matches the declared type must not throw.
     */
    public function testReturnValidStringDoesNotThrow(): void
    {
        $tester = new Returns();
        $this->assertSame('hello', $tester->returnValidString());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     * Nullable returns must keep working — `-> string | null` accepts null.
     */
    public function testNullableReturnTypeAcceptsNull(): void
    {
        $tester = new Returns();
        $this->assertNull($tester->returnNullOnNullableString());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1991
     * Error message must match PHP's TypeError format so existing error
     * handlers / log parsers don't have to learn a new shape.
     */
    public function testTypeErrorMessageMatchesPhpFormat(): void
    {
        $tester = new Returns();
        try {
            $tester->returnNullOnString();
            $this->fail('Expected TypeError was not thrown');
        } catch (\TypeError $e) {
            $this->assertSame(
                'Stub\Returns::returnNullOnString(): Return value must be of type string, null returned',
                $e->getMessage()
            );
        }
    }
}
