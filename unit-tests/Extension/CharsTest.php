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
    public function testSumChars()
    {
        $t = new \Test\Chars();

        $this->assertSame(\chr($t->sumChars1()), 'a');
        $this->assertSame(\chr($t->sumChars2('A')), 'a');
    }

    public function testDiffChars()
    {
        $t = new \Test\Chars();

        $this->assertSame(\chr($t->diffChars1()), 'A');
        $this->assertSame(\chr($t->diffChars2('a')), 'A');
    }
}
