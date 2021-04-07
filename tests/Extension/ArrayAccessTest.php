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

namespace Extension;

use PHPUnit\Framework\TestCase;

final class ArrayAccessTest extends TestCase
{
    public function testTest(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertTrue($class->exits('one'));
        $this->assertSame(2, $class->get());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/645
     */
    public function testIssue645(): void
    {
        $class = new \Stub\ArrayAccessTest();

        $this->assertSame([], $class->issue645());
    }
}
