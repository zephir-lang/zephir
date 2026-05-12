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
use Zephir\CompiledExpression;

final class CompiledExpressionTest extends TestCase
{
    public function testShouldCreateCompiledExpression(): void
    {
        $expr = new CompiledExpression('int', '42', ['type' => 'int']);

        $this->assertSame('int', $expr->getType());
        $this->assertSame('42', $expr->getCode());
        $this->assertSame(['type' => 'int'], $expr->getOriginal());
    }

    public function testShouldGetBooleanCodeForTrue(): void
    {
        $expr1 = new CompiledExpression('bool', 'true');
        $this->assertSame('1', $expr1->getBooleanCode());

        $expr2 = new CompiledExpression('bool', true);
        $this->assertSame('1', $expr2->getBooleanCode());
    }

    public function testShouldGetBooleanCodeForFalse(): void
    {
        $expr1 = new CompiledExpression('bool', 'false');
        $this->assertSame('0', $expr1->getBooleanCode());

        $expr2 = new CompiledExpression('bool', false);
        $this->assertSame('0', $expr2->getBooleanCode());
    }

    public function testShouldReturnCodeAsIsForNonBooleanValues(): void
    {
        $expr = new CompiledExpression('int', '42');
        $this->assertSame('42', $expr->getBooleanCode());
    }

    public function testShouldCheckIfIntCompatibleType(): void
    {
        $intExpr = new CompiledExpression('int', '42');
        $this->assertTrue($intExpr->isIntCompatibleType());

        $stringExpr = new CompiledExpression('string', 'hello');
        $this->assertFalse($stringExpr->isIntCompatibleType());

        $charExpr = new CompiledExpression('char', 'a');
        $this->assertTrue($charExpr->isIntCompatibleType());
    }
}
