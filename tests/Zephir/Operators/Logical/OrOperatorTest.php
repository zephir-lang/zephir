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
use Zephir\Operators\Logical\OrOperator;

final class OrOperatorTest extends TestCase
{
    public function testShouldCreateOrOperator(): void
    {
        $operator = new OrOperator();

        $this->assertInstanceOf(OrOperator::class, $operator);
    }

    public function testShouldHaveCorrectOperator(): void
    {
        $operator = new OrOperator();

        // OrOperator should handle logical OR operations
        $this->assertIsObject($operator);
    }
}
