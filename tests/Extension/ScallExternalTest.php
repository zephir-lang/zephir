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
use Stub\ScallExternal;

final class ScallExternalTest extends TestCase
{
    public function testCall1(): void
    {
        $test = new ScallExternal();
        $this->assertSame($test->testCall1(), 'hello public');
    }

    public function testCall2(): void
    {
        $test = new ScallExternal();
        $this->assertSame($test->testCall2(2, 3), 5);
    }
}
