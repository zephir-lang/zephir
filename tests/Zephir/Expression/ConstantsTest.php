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

namespace Zephir\Test\Expression;

use PHPUnit\Framework\TestCase;
use Zephir\CompilationContext;
use Zephir\Expression\Constants;

final class ConstantsTest extends TestCase
{
    public function testShouldCompileTrueConstant(): void
    {
        $context = $this->createMock(CompilationContext::class);
        $constants = new Constants();

        $expression = ['value' => 'true'];
        $result = $constants->compile($expression, $context);

        $this->assertSame('boolean', $result->getType());
        $this->assertTrue($result->getBooleanCode() === '1');
    }

    public function testShouldCompileFalseConstant(): void
    {
        $context = $this->createMock(CompilationContext::class);
        $constants = new Constants();

        $expression = ['value' => 'false'];
        $result = $constants->compile($expression, $context);

        $this->assertSame('boolean', $result->getType());
        $this->assertTrue($result->getBooleanCode() === '0');
    }

    public function testShouldCompileNullConstant(): void
    {
        $context = $this->createMock(CompilationContext::class);
        $constants = new Constants();

        $expression = ['value' => 'null'];
        $result = $constants->compile($expression, $context);

        $this->assertSame('null', $result->getType());
    }
}
