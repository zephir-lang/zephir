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

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\McallChained;

final class McallChainedTest extends TestCase
{
    private McallChained $test;

    protected function setUp(): void
    {
        $this->test = new McallChained();
    }

    public function testChained1(): void
    {
        $this->assertSame($this->test->testChained1(), $this->test);
    }

    public function testChained2(): void
    {
        $this->assertSame($this->test->testChained2(), $this->test);
    }

    public function testChained3(): void
    {
        $this->assertSame($this->test->testChained3(), $this->test);
    }
}
