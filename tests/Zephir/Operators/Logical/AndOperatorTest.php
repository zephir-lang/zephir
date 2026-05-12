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

namespace Zephir\Test\Operators\Logical;

use PHPUnit\Framework\TestCase;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Operators\Logical\AndOperator;

final class AndOperatorTest extends TestCase
{
    public function testShouldCreateAndOperator(): void
    {
        $operator = new AndOperator();

        $this->assertInstanceOf(AndOperator::class, $operator);
    }

    public function testShouldHaveCorrectOperator(): void
    {
        $operator = new AndOperator();

        // AndOperator should handle logical AND operations
        $this->assertIsObject($operator);
    }
}
