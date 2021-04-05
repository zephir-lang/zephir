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
use Stub\Issue663;

final class Issue663Test extends TestCase
{
    public function testIssue663(): void
    {
        $test = new Issue663();

        $this->assertFalse($test->is_array_assoc(false));
        $this->assertFalse($test->is_array_assoc(1));
        $this->assertFalse($test->is_array_assoc([]));
        $this->assertTrue($test->is_array_assoc(['test' => 'test']));
        $this->assertFalse($test->is_array_assoc(['test']));
        $this->assertFalse($test->is_array_assoc([0 => 'test']));

        $this->assertFalse($test->is_array_indexed(false));
        $this->assertFalse($test->is_array_indexed(1));
        $this->assertFalse($test->is_array_indexed([]));
        $this->assertFalse($test->is_array_indexed(['test' => 'test']));
        $this->assertTrue($test->is_array_indexed(['test']));
        $this->assertTrue($test->is_array_indexed([0 => 'test']));
    }
}
