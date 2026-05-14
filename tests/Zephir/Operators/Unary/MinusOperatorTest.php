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

namespace Zephir\Test\Operators\Unary;

use PHPUnit\Framework\TestCase;
use Zephir\Operators\Unary\MinusOperator;

final class MinusOperatorTest extends TestCase
{
    public function testShouldCreateMinusOperator(): void
    {
        $operator = new MinusOperator();

        $this->assertInstanceOf(MinusOperator::class, $operator);
    }
}
