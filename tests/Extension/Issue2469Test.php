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
use Stub\Issue2469;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2469
 */
final class Issue2469Test extends TestCase
{
    private Issue2469 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2469();
    }

    public function testPeekAcceptsCharLiteral(): void
    {
        $this->assertSame('a', \chr($this->test->callWithLiteral()));
    }

    public function testPeekAcceptsCharVariable(): void
    {
        $this->assertSame('>', \chr($this->test->callWithVariable()));
    }

    public function testPeekAcceptsAdditionalCharLiteral(): void
    {
        $this->assertSame('Z', \chr($this->test->callWithLiteralUchar()));
    }
}
