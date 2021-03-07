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

final class ResourceTest extends TestCase
{
    private \Stub\ResourceTest $test;

    protected function setUp(): void
    {
        $this->test = new \Stub\ResourceTest();
    }

    public function testLetStatementAssign(): void
    {
        $this->assertIsResource($this->test->testLetStatementSTDIN());
        $this->assertIsResource($this->test->testLetStatementSTDOUT());
        $this->assertIsResource($this->test->testLetStatementSTDERR());
    }

    public function testTypeOffResource(): void
    {
        $this->assertSame('resource', $this->test->testTypeOffResource());
    }

    public function testIsResource(): void
    {
        $this->assertTrue($this->test->testIsResource());
    }
}
