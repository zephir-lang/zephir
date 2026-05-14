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

namespace Zephir\Test\Operators\Comparison;

use PHPUnit\Framework\TestCase;
use Zephir\Operators\Comparison\EqualsOperator;

final class EqualsOperatorTest extends TestCase
{
    public function testShouldCreateEqualsOperator(): void
    {
        $operator = new EqualsOperator();

        $this->assertInstanceOf(EqualsOperator::class, $operator);
    }
}
