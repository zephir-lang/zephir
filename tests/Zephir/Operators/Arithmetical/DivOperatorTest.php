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
use Zephir\Operators\Arithmetical\DivOperator;

final class DivOperatorTest extends TestCase
{
    public function testShouldCreateDivOperator(): void
    {
        $operator = new DivOperator();

        $this->assertInstanceOf(DivOperator::class, $operator);
    }
}
