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
use Stub\Issue914;

final class Issue914Test extends TestCase
{
    public function testIssue914(): void
    {
        $test = new Issue914();

        $this->assertInstanceOf(\mysqli::class, $test);
    }
}
