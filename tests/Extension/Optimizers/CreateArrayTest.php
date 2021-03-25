<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Stub\Optimizers\CreateArray;

final class CreateArrayTest extends TestCase
{
    public function testCreateArrayNoSize(): void
    {
        $test = new CreateArray();
        $this->assertSame(\count($test->createNoSize()), 0);
    }

    public function testCreateArraySize(): void
    {
        $test = new CreateArray();
        $this->assertSame(\count($test->createSize(3)), 3);
        $this->assertSame(\count($test->createSize(10)), 10);
    }
}
