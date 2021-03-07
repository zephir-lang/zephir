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
use Stub\Chars;

final class CharsTest extends TestCase
{
    private Chars $test;

    protected function setUp(): void
    {
        $this->test = new Chars();
    }

    public function testSumChars(): void
    {
        $this->assertSame(\chr($this->test->sumChars1()), 'a');
        $this->assertSame(\chr($this->test->sumChars2('A')), 'a');
    }

    public function testDiffChars(): void
    {
        $this->assertSame(\chr($this->test->diffChars1()), 'A');
        $this->assertSame(\chr($this->test->diffChars2('a')), 'A');
    }
}
