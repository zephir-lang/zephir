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
use Stub\Strings;
use Zephir\Os;

final class StringTest extends TestCase
{
    private ?Strings $test = null;

    protected function setUp(): void
    {
        $this->test = new Strings();
    }

    /**
     * @dataProvider providerHashEquals
     *
     * @param mixed $knownString
     * @param mixed $userString
     * @param mixed $expected
     */
    public function testHashEquals($knownString, $userString, $expected): void
    {
        $salt = '$2a$07$usesomesillystringforsalt$';
        $knownString = crypt($knownString, $salt);
        $userString = crypt($userString, $salt);

        $this->assertSame($expected, $this->test->testHashEquals($knownString, $userString));
    }

    /**
     * @dataProvider providerHashEqualsNonString
     *
     * @param mixed $knownString
     * @param mixed $userString
     */
    public function testHashEqualsNonString($knownString, $userString): void
    {
        $this->assertFalse($this->test->testHashEquals($knownString, $userString));
    }

    /**
     * @dataProvider providerCamelize
     *
     * @param mixed $actual
     * @param mixed $expected
     * @param mixed $delimiter
     */
    public function testCamelize($actual, $expected, $delimiter): void
    {
        $this->assertSame($expected, $this->test->camelize($actual, $delimiter));
    }

    /**
     * @dataProvider providerCamelizeWrongSecondParam
     *
     * @param mixed $delimiter
     */
    public function testCamelizeWrongSecondParam($delimiter): void
    {
        $this->expectWarning();
        $this->expectExceptionMessage(
            'The second argument passed to the camelize() must be a string containing at least one character'
        );

        $this->test->camelize('CameLiZe', $delimiter);
    }

    /**
     * @dataProvider providerUnCamelize
     *
     * @param mixed $actual
     * @param mixed $expected
     * @param mixed $delimiter
     */
    public function testUnCamelize($actual, $expected, $delimiter): void
    {
        $this->assertSame($expected, $this->test->uncamelize($actual, $delimiter));
    }

    /**
     * @dataProvider providerCamelizeWrongSecondParam
     *
     * @param mixed $delimiter
     */
    public function testUnCamelizeWrongSecondParam($delimiter): void
    {
        $this->expectWarning();
        $this->expectExceptionMessage(
            'Second argument passed to the uncamelize() must be a string of one character'
        );

        $this->test->uncamelize('CameLiZe', $delimiter);
    }

    public function testTrim(): void
    {
        $testSuite = [
            [' hello ', 'hello'],
            ['hello ', 'hello'],
            [' hello', 'hello'],
        ];

        foreach ($testSuite as $itm) {
            $this->assertSame($this->test->testTrim($itm[0]), $itm[1]);
        }

        $this->assertSame($this->test->testTrim2Params('Hello World', 'Hdle'), 'o Wor');
    }

    public function testLtrim(): void
    {
        $testSuite = [
            [' hello ', 'hello '],
            ['hello ', 'hello '],
            [' hello', 'hello'],
        ];

        foreach ($testSuite as $itm) {
            $this->assertSame($this->test->testLtrim($itm[0]), $itm[1]);
        }

        $this->assertSame($this->test->testLtrim2Params('Hello World', 'Hdle'), 'o World');
    }

    public function testRtrim(): void
    {
        $testSuite = [
            [' hello ', ' hello'],
            ['hello ', 'hello'],
            [' hello', ' hello'],
        ];

        foreach ($testSuite as $itm) {
            $this->assertSame($this->test->testRtrim($itm[0]), $itm[1]);
        }

        $this->assertSame($this->test->testRtrim2Params('Hello World', 'Hdle'), 'Hello Wor');
    }

    public function testStrpos(): void
    {
        $this->assertSame($this->test->testStrpos('abcdef abcdef', 'a'), 0);
        $this->assertSame($this->test->testStrposOffset('abcdef abcdef', 'a', 1), 7);
    }

    public function testImplode(): void
    {
        $pieces = ['a', 'b', 'c'];
        $this->assertSame($this->test->testImplode(',', $pieces), 'a,b,c');
    }

    public function testExplode(): void
    {
        $pizza = 'piece1,piece2,piece3,piece4,piece5,piece6';
        $ar1 = $this->test->testExplode(',', $pizza);
        $ar2 = $this->test->testExplodeStr($pizza);

        $this->assertSame($ar1[0], 'piece1');
        $this->assertSame($ar1[2], 'piece3');

        $this->assertSame($ar2[0], 'piece1');
        $this->assertSame($ar2[2], 'piece3');

        $ar3 = $this->test->testExplodeLimit($pizza, 3);
        $this->assertCount(3, $ar3);
        $this->assertSame($ar3[2], 'piece3,piece4,piece5,piece6');
    }

    public function providerSubstring(): array
    {
        $testStr = 'abcdef';

        return [
            [$testStr, 1, 3, 'bcd'],
            [$testStr, 0, 4, 'abcd'],
            [$testStr, 0, 8, 'abcdef'],
            [$testStr, -1, 1, 'f'],
            [$testStr, -3, -1, 'de'],
            [$testStr, 2, -1, 'cde'],
        ];
    }

    /**
     * @dataProvider providerSubstring
     *
     * @param string $input
     * @param int    $start
     * @param int    $end
     * @param string $expected
     */
    public function testSubstr(string $input, int $start, int $end, string $expected): void
    {
        $this->assertSame($this->test->testSubstr($input, $start, $end), $expected);

        $this->assertSame($this->test->testSubstr2($input, -1), 'f');
        $this->assertSame($this->test->testSubstr2($input, -2), 'ef');
        $this->assertSame($this->test->testSubstr2($input, 2), 'cdef');

        $this->assertSame($this->test->testSubstr3($input), 'f');
        $this->assertSame($this->test->testSubstr4($input), 'abcde');
    }

    public function providerAddStripSlashes(): array
    {
        return [
            ["How's everybody", "How's everybody"],
            ['Are you "JOHN"?', 'Are you "JOHN"?'],
            ["hello\0world",    "hello\0world"],
        ];
    }

    /**
     * @dataProvider providerAddStripSlashes
     *
     * @param string $sample
     * @param string $expected
     */
    public function testAddslashes(string $sample, string $expected): void
    {
        $this->assertSame($this->test->testAddslashes($sample), addslashes($expected));
    }

    /**
     * @dataProvider providerAddStripSlashes
     *
     * @param string $sample
     * @param string $expected
     */
    public function testStripslashes(string $sample, string $expected): void
    {
        $this->assertSame($this->test->testStripslashes(addslashes($sample)), $expected);
    }

    public function testStripcslashes(): void
    {
        parent::assertSame(
            stripcslashes('\abcd\e\f\g\h\i\j\k\l\m\n\o\pqrstuvwxy\z'),
            $this->test->testStripcslashes('\abcd\e\f\g\h\i\j\k\l\m\n\o\pqrstuvwxy\z')
        );
        parent::assertSame(stripcslashes('\065\x64'), $this->test->testStripcslashes('\065\x64'));
        parent::assertSame(stripcslashes(''), $this->test->testStripcslashes(''));
    }

    public function testMultilineStrings(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('String will contain different line endings.');
        }

        $hardcodedString = '
            Hello world
        ';

        $this->assertSame($hardcodedString, $this->test->testHardcodedMultilineString());
        ob_start();
        $this->test->testEchoMultilineString();
        $this->assertSame($hardcodedString, ob_get_clean());
        $this->assertSame(trim($hardcodedString), $this->test->testTrimMultilineString());

        $escapedString = '\"\}\$hello\$\"\'';
        $this->assertSame($escapedString, $this->test->testWellEscapedMultilineString());
    }

    public function testStrToHex(): void
    {
        $this->assertSame('746573742073656e74656e73652e2e2e', $this->test->strToHex('test sentense...'));
    }

    public function testIssue2186(): void
    {
        $this->assertTrue($this->test->issue2186());
        $this->assertTrue($this->test->issue2186(null));
        $this->assertFalse($this->test->issue2186(''));
        $this->assertFalse($this->test->issue2186('string'));
        $this->assertFalse($this->test->issue2186('0'));
        $this->assertFalse($this->test->issue2186('1'));

        $this->assertTrue($this->test->issue2234Strict());
        $this->assertTrue($this->test->issue2234Strict(null));
        $this->assertFalse($this->test->issue2234Strict(''));
        $this->assertFalse($this->test->issue2234Strict('string'));
        $this->assertFalse($this->test->issue2234Strict('0'));
        $this->assertFalse($this->test->issue2234Strict('1'));

        $this->assertFalse($this->test->issue2186SegFault());
        $this->assertFalse($this->test->issue2186SegFault(null));
        $this->assertFalse($this->test->issue2186SegFault(''));

        /**
         * Assert PHP and Zephir behavior
         */
        $zeroString = false;
        if ('0') {
            $zeroString = true;
        }

        $this->assertFalse(!empty('0'));
        $this->assertFalse($zeroString);
        $this->assertFalse($this->test->issue2186SegFault('0'));

        $this->assertTrue($this->test->issue2186SegFault('string'));

        $this->assertSame('', $this->test->issue2186SegFaultCall());
        $this->assertSame('ok all ok', $this->test->issue2186SegFaultCall('ok'));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1932
     */
    public function testNullableStringReturnType(): void
    {
        $this->assertNull($this->test->nullableStringReturnType());
        $this->assertNull($this->test->nullableStringReturnType(null));
        $this->assertSame('string', $this->test->nullableStringReturnType('string'));
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2299
     */
    public function testIssue2299NullableStringCondition(): void
    {
        $this->assertSame(null, $this->test->issue2299NullableStringCondition());
        $this->assertSame('not-null', $this->test->issue2299NullableStringCondition('not-null'));
        $this->assertSame('test', $this->test->issue2299NullableStringCondition('test'));
    }

    public function providerHashEquals(): array
    {
        return [
            ['Phalcon',    'Phalcony',    false],
            ['Phalcony',   'Phalcon',     false],
            ['Phalcon',    'Phalcon',     true],
            ['kristoffer', 'ingemansson', false],
            ['kris',       'ingemansson', false],
            ['Phalcon',    'phalcon',     false],
            [' phalcon',   'phalcon',     false],
            ['phalcon',    'phalcon',     true],
            ['1234567890', '1234567890',  true],
            ['',           'phalcon',     false],
            ['phalcon',    '',            false],
        ];
    }

    public function providerHashEqualsNonString(): array
    {
        return [
            [null,       123,        false],
            [123,        null,       false],
            [123,        123,        false],
            [123456,     123,        false],
            [null,       'phalcon',  false],
            ['phalcon',  null,       false],
            [[],         false,      false],
            [true,       [],         false],
        ];
    }

    public function providerCamelize(): array
    {
        return [
            ['=_camelize',      '=Camelize', '_'],
            ['camelize',        'Camelize',  '_'],
            ['came_li_ze',      'CameLiZe',  '_'],
            ['came_li_ze',      'CameLiZe',  null],
            ['came#li#ze',      'CameLiZe',  '#'],
            ['came li ze',      'CameLiZe',  ' '],
            ['came.li^ze',      'CameLiZe',  '.^'],
            ['c_a-m_e-l_i-z_e', 'CAMELIZE',  '-_'],
            ['c_a-m_e-l_i-z_e', 'CAMELIZE',  null],
            ['came.li.ze',      'CameLiZe',  '.'],
            ['came-li-ze',      'CameLiZe',  '-'],
            ['c+a+m+e+l+i+z+e', 'CAMELIZE',  '+'],
        ];
    }

    public function providerUnCamelize(): array
    {
        return [
            ['=Camelize', '=_camelize',      '_'],
            ['Camelize',  'camelize',        '_'],
            ['Camelize',  'camelize',        null],
            ['CameLiZe',  'came_li_ze',      '_'],
            ['CameLiZe',  'came#li#ze',      '#'],
            ['CameLiZe',  'came li ze',      ' '],
            ['CameLiZe',  'came.li.ze',      '.'],
            ['CameLiZe',  'came-li-ze',      '-'],
            ['CAMELIZE',  'c/a/m/e/l/i/z/e', '/'],
        ];
    }

    public function providerCamelizeWrongSecondParam(): array
    {
        return [
            [''],
            [true],
            [false],
            [1],
            [0],
            [[]],
            [
                fn () => '-',
            ],
            [new \stdClass()],
        ];
    }
}
