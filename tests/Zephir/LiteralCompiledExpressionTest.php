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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\LiteralCompiledExpression;

final class LiteralCompiledExpressionTest extends TestCase
{
    public function testShouldExtendCompiledExpression(): void
    {
        $expr = new LiteralCompiledExpression('int', '42');

        $this->assertSame('int', $expr->getType());
        $this->assertSame('42', $expr->getCode());
    }

    public function testShouldWorkWithStringLiterals(): void
    {
        $expr = new LiteralCompiledExpression('string', '"hello"');

        $this->assertSame('string', $expr->getType());
        $this->assertSame('"hello"', $expr->getCode());
    }

    public function testShouldWorkWithBooleanLiterals(): void
    {
        $expr = new LiteralCompiledExpression('bool', 'true');

        $this->assertSame('bool', $expr->getType());
        $this->assertSame('1', $expr->getBooleanCode());
    }
}
