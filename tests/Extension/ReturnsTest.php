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
}
