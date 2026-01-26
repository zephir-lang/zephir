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

namespace Zephir\Test\Operators\Bitwise;

use PHPUnit\Framework\TestCase;
use Zephir\Operators\Bitwise\BitwiseOrOperator;

final class BitwiseOrOperatorTest extends TestCase
{
    public function testShouldCreateBitwiseOrOperator(): void
    {
        $operator = new BitwiseOrOperator();

        $this->assertInstanceOf(BitwiseOrOperator::class, $operator);
    }
}
