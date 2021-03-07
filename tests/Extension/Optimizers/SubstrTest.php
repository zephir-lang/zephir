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

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Stub\Optimizers\Substr;

final class SubstrTest extends TestCase
{
    public function testTwoArguments1(): void
    {
        $test = new Substr();

        $strings_array = [null, '', 12345, 'abcdef', '123abc', '_123abc'];
        $results1 = [false, false, '2345', 'bcdef', '23abc', '123abc'];
        $results2 = [false, false, '12345', 'abcdef', '123abc', '_123abc'];
        $results3 = [false, false, '45', 'ef', 'bc', 'bc'];

        $c = 0;
        foreach ($strings_array as $str) {
            $this->assertSame($test->testTwoArguments($str, 1), $results1[$c]);
            $this->assertSame($test->testTwoArguments($str, 0), $results2[$c]);
            $this->assertSame($test->testTwoArguments($str, -2), $results3[$c]);
            ++$c;
        }
    }

    public function testThreeArguments1(): void
    {
        $test = new Substr();
        $this->assertSame($test->testThreeArguments('abcdef', 0, -3), 'abc');
        $this->assertSame($test->testThreeArguments('123abc', 0, 3), '123');
        $this->assertSame($test->testThreeArguments('abcdef', 1, -3), 'bc');
        $this->assertSame($test->testThreeArguments('abcdef', -2, 0), '');
        $this->assertSame($test->testThreeArguments(12345, 1, -1), '234');
    }
}
