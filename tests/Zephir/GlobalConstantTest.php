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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\GlobalConstant;

final class GlobalConstantTest extends TestCase
{
    public function testShouldCreateGlobalConstant(): void
    {
        $constant = new GlobalConstant('MY_CONSTANT');

        $this->assertSame('MY_CONSTANT', $constant->getName());
    }

    public function testShouldNotBeTemporal(): void
    {
        $constant = new GlobalConstant('MY_CONSTANT');

        $this->assertFalse($constant->isTemporal());
    }

    public function testShouldHandleDifferentConstantNames(): void
    {
        $constant1 = new GlobalConstant('CONST_ONE');
        $constant2 = new GlobalConstant('CONST_TWO');

        $this->assertSame('CONST_ONE', $constant1->getName());
        $this->assertSame('CONST_TWO', $constant2->getName());
    }
}
