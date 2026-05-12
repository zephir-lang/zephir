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
use Zephir\Operators\Unary\NotOperator;

final class NotOperatorTest extends TestCase
{
    public function testShouldCreateNotOperator(): void
    {
        $operator = new NotOperator();

        $this->assertInstanceOf(NotOperator::class, $operator);
    }
}
