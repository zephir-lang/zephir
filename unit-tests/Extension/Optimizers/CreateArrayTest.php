<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Test\Optimizers\CreateArray;

class CreateArrayTest extends TestCase
{
    public function testCreateArrayNoSize()
    {
        $t = new CreateArray();
        $this->assertSame(\count($t->createNoSize()), 0);
    }

    public function testCreateArraySize()
    {
        $t = new CreateArray();
        $this->assertSame(\count($t->createSize(3)), 3);
        $this->assertSame(\count($t->createSize(10)), 10);
    }
}
