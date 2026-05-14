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
use Zephir\Operators\Arithmetical\SubOperator;

final class SubOperatorTest extends TestCase
{
    public function testShouldCreateSubOperator(): void
    {
        $operator = new SubOperator();

        $this->assertInstanceOf(SubOperator::class, $operator);
    }
}
