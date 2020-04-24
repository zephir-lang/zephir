<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class CharsTest extends TestCase
{
    /** @var \Test\Chars */
    private $test;

    public function setUp()
    {
        $this->test = new \Test\Chars();
    }

    public function testSumChars()
    {
        $this->assertSame(\chr($this->test->sumChars1()), 'a');
        $this->assertSame(\chr($this->test->sumChars2('A')), 'a');
    }

    public function testDiffChars()
    {
        $this->assertSame(\chr($this->test->diffChars1()), 'A');
        $this->assertSame(\chr($this->test->diffChars2('a')), 'A');
    }
}
