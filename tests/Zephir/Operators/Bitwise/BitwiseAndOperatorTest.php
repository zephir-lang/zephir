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
use Zephir\Operators\Bitwise\BitwiseAndOperator;

final class BitwiseAndOperatorTest extends TestCase
{
    public function testShouldCreateBitwiseAndOperator(): void
    {
        $operator = new BitwiseAndOperator();

        $this->assertInstanceOf(BitwiseAndOperator::class, $operator);
    }
}
