<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Test\Optimizers\Substr;

class SubstrTest extends TestCase
{
    public function testTwoArguments1()
    {
        $t = new Substr();

        $strings_array = [null, '', 12345, 'abcdef', '123abc', '_123abc'];
        $results1 = [false, false, '2345', 'bcdef', '23abc', '123abc'];
        $results2 = [false, false, '12345', 'abcdef', '123abc', '_123abc'];
        $results3 = [false, false, '45', 'ef', 'bc', 'bc'];

        $c = 0;
        foreach ($strings_array as $str) {
            $this->assertSame($t->testTwoArguments($str, 1), $results1[$c]);
            $this->assertSame($t->testTwoArguments($str, 0), $results2[$c]);
            $this->assertSame($t->testTwoArguments($str, -2), $results3[$c]);
            ++$c;
        }
    }

    public function testThreeArguments1()
    {
        $t = new Substr();
        $this->assertSame($t->testThreeArguments('abcdef', 0, -3), 'abc');
        $this->assertSame($t->testThreeArguments('123abc', 0, 3), '123');
        $this->assertSame($t->testThreeArguments('abcdef', 1, -3), 'bc');
        $this->assertSame($t->testThreeArguments('abcdef', -2, 0), '');
        $this->assertSame($t->testThreeArguments(12345, 1, -1), '234');
    }
}
