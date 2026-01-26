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
use Zephir\Operators\Arithmetical\AddOperator;

final class AddOperatorTest extends TestCase
{
    public function testShouldCreateAddOperator(): void
    {
        $operator = new AddOperator();

        $this->assertInstanceOf(AddOperator::class, $operator);
    }
}
