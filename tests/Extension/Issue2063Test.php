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
use Stub\Issue2063;

final class Issue2063Test extends TestCase
{
    public function testConcatAssignProperty(): void
    {
        $test = new Issue2063();
        $this->assertSame('', $test->get());

        $test->add('phalcon');
        $test->add(' framework');
        $this->assertSame('phalcon framework', $test->get());
    }

    public function testExplicitConcatProperty(): void
    {
        $test = new Issue2063();

        $test->add2('phalcon');
        $test->add2(' framework');
        $this->assertSame('phalcon framework', $test->get());
    }

    public function testConcatAssignLiteral(): void
    {
        $test = new Issue2063();

        $test->addLiteral();
        $test->addLiteral();
        $this->assertSame('abcdabcd', $test->get());
    }
}
