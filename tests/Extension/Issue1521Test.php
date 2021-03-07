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
use Stub\Issue1521;

final class Issue1521Test extends TestCase
{
    public function testIssue1521(): void
    {
        $test = new Issue1521();
        $actual = $test->test();

        $this->assertTrue('0' === $actual['zero']);
        $this->assertTrue('0818217122' === $actual['merchantNumber']);
    }
}
