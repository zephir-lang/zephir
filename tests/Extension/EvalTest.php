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

final class EvalTest extends TestCase
{
    public function testEval(): void
    {
        $test = new \Stub\EvalTest();

        $this->assertSame(2, $test->evalCode('return 1 + 1;'));
        $this->assertSame(8, $test->evalCode('$g = pow(2, 2); return $g * 2;'));
    }
}
