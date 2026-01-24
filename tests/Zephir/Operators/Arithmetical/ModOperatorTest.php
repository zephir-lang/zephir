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

namespace Zephir\Test\Operators\Arithmetical;

use PHPUnit\Framework\TestCase;
use Zephir\Operators\Arithmetical\ModOperator;

final class ModOperatorTest extends TestCase
{
    public function testShouldCreateModOperator(): void
    {
        $operator = new ModOperator();

        $this->assertInstanceOf(ModOperator::class, $operator);
    }
}
