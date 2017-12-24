<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

class StringTest extends \PHPUnit_Framework_TestCase
{
    /**
     * @dataProvider providerHashEquals
     * @param string $knownString
     * @param string $userString
     * @param string $expected
     */
    public function testHashEquals($knownString, $userString, $expected)
    {
        $t = new \Test\Strings();

        $salt = '$2a$07$usesomesillystringforsalt$';
        $knownString = crypt($knownString, $salt);
        $userString = crypt($userString, $salt);

        $this->assertSame($expected, $t->testHashEquals($knownString, $userString));
    }

    /**
     * @dataProvider providerHashEqualsNonString
     * @param string $knownString
     * @param string $userString
     */
    public function testHashEqualsNonString($knownString, $userString)
    {
        $t = new \Test\Strings();

        $this->assertFalse($t->testHashEquals($knownString, $userString));
    }

    /**
     * @dataProvider providerCamelize
     */
    public function testCamelize($actual, $expected, $delimiter)
    {
        $t = new \Test\Strings();

        $this->assertSame($expected, $t->camelize($actual, $delimiter));
    }

    /**
     * @dataProvider providerCamelizeWrongSecondParam
     * @expectedException \PHPUnit_Framework_Error_Warning
     * @expectedExceptionMessage The second argument passed to the camelize() must be a string containing at least one character
     */
    public function testCamelizeWrongSecondParam($delimiter)
    {
        $t = new \Test\Strings();

        $t->camelize('CameLiZe', $delimiter);
    }

    /**
     * @dataProvider providerUnCamelize
     */
    public function testUnCamelize($actual, $expected, $delimiter)
    {
        $t = new \Test\Strings();

        $this->assertSame($expected, $t->uncamelize($actual, $delimiter));
    }

    /**
     * @dataProvider providerCamelizeWrongSecondParam
     * @expectedException \PHPUnit_Framework_Error_Warning
     * @expectedExceptionMessage Second argument passed to the uncamelize() must be a string of one character
     */
    public function testUnCamelizeWrongSecondParam($delimiter)
    {
        $t = new \Test\Strings();

        $t->uncamelize('CameLiZe', $delimiter);
    }

    public function testTrim()
    {
        $t = new \Test\Strings();

        $this->assertSame($t->testTrim(' hello '), 'hello');
        $this->assertSame($t->testTrim('hello '), 'hello');
        $this->assertSame($t->testTrim(' hello'), 'hello');

        $this->assertSame($t->testTrim2Params('Hello World', 'Hdle'), 'o Wor');
    }

    public function testLtrim()
    {
        $t = new \Test\Strings();

        $this->assertSame($t->testLtrim(' hello '), 'hello ');
        $this->assertSame($t->testLtrim('hello '), 'hello ');
        $this->assertSame($t->testLtrim(' hello'), 'hello');

        $this->assertSame($t->testLtrim2Params('Hello World', 'Hdle'), 'o World');
    }

    public function testRtrim()
    {
        $t = new \Test\Strings();

        $this->assertSame($t->testRtrim(' hello '), ' hello');
        $this->assertSame($t->testRtrim('hello '), 'hello');
        $this->assertSame($t->testRtrim(' hello'), ' hello');

        $this->assertSame($t->testRtrim2Params('Hello World', 'Hdle'), 'Hello Wor');
    }

    public function testStrpos()
    {
        $t = new \Test\Strings();

        $this->assertSame($t->testStrpos('abcdef abcdef', 'a'), 0);
        $this->assertSame($t->testStrposOffset('abcdef abcdef', 'a', 1), 7);
    }

    public function testImplode()
    {
        $t = new \Test\Strings();

        $pieces = array('a', 'b', 'c');
        $this->assertSame($t->testImplode(',', $pieces), 'a,b,c');
    }

    public function testExplode()
    {
        $t = new \Test\Strings();

        $pizza  = "piece1,piece2,piece3,piece4,piece5,piece6";
        $ar1 = $t->testExplode(',', $pizza);
        $ar2 = $t->testExplodeStr($pizza);

        $this->assertSame($ar1[0], 'piece1');
        $this->assertSame($ar1[2], 'piece3');

        $this->assertSame($ar2[0], 'piece1');
        $this->assertSame($ar2[2], 'piece3');

        $ar3 = $t->testExplodeLimit($pizza, 3);
        $this->assertCount(3, $ar3);
        $this->assertSame($ar3[2], 'piece3,piece4,piece5,piece6');
    }

    public function testSubstr()
    {
        $t = new \Test\Strings();

        $this->assertSame($t->testSubstr('abcdef', 1, 3), 'bcd');
        $this->assertSame($t->testSubstr('abcdef', 0, 4), 'abcd');
        $this->assertSame($t->testSubstr('abcdef', 0, 8), 'abcdef');
        $this->assertSame($t->testSubstr('abcdef', -1, 1), 'f');
        $this->assertSame($t->testSubstr('abcdef', -3, -1), 'de');
        $this->assertSame($t->testSubstr('abcdef', 2, -1), 'cde');

        $this->assertSame($t->testSubstr2('abcdef', -1), 'f');
        $this->assertSame($t->testSubstr2('abcdef', -2), 'ef');
        $this->assertSame($t->testSubstr2('abcdef', 2), 'cdef');

        $this->assertSame($t->testSubstr3('abcdef'), 'f');
        $this->assertSame($t->testSubstr4('abcdef'), 'abcde');
    }

    public function testAddslashes()
    {
        $t = new \Test\Strings();

        $this->assertSame($t->testAddslashes("How's everybody"), addslashes("How's everybody"));
        $this->assertSame($t->testAddslashes('Are you "JOHN"?'), addslashes('Are you "JOHN"?'));
        $this->assertSame($t->testAddslashes("hello\0world"), addslashes("hello\0world"));
    }

    public function testStripslashes()
    {
        $t = new \Test\Strings();

        $this->assertSame($t->testStripslashes(addslashes("How's everybody")), "How's everybody");
        $this->assertSame($t->testStripslashes(addslashes('Are you "JOHN"?')), 'Are you "JOHN"?');
        $this->assertSame($t->testStripslashes(addslashes("hello\0world")), "hello\0world");
    }

    public function testStripcslashes()
    {
        $t = new \Test\Strings();

        parent::assertSame(
            stripcslashes('\abcd\e\f\g\h\i\j\k\l\m\n\o\pqrstuvwxy\z'),
            $t->testStripcslashes('\abcd\e\f\g\h\i\j\k\l\m\n\o\pqrstuvwxy\z')
        );
        parent::assertSame(stripcslashes('\065\x64'), $t->testStripcslashes('\065\x64'));
        parent::assertSame(stripcslashes(''), $t->testStripcslashes(''));
    }

    public function testMultilineStrings()
    {
        $hardcodedString = '
            Hello world
        ';

        $t = new \Test\Strings();

        $this->assertSame($hardcodedString, $t->testHardcodedMultilineString());
        ob_start();
        $t->testEchoMultilineString();
        $this->assertSame($hardcodedString, ob_get_clean());
        $this->assertSame(trim($hardcodedString), $t->testTrimMultilineString());

        $escapedString = '\"\}\$hello\$\"\\\'';
        $this->assertSame($escapedString, $t->testWellEscapedMultilineString());
    }

    public function testStrToHex()
    {
        $t = new \Test\Strings();
        $this->assertSame('746573742073656e74656e73652e2e2e', $t->strToHex("test sentense..."));
    }

    public function providerHashEquals()
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

    public function providerHashEqualsNonString()
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

    public function providerCamelize()
    {
        return [
            ["=_camelize",      '=Camelize', "_" ],
            ["camelize",        'Camelize',  "_" ],
            ["came_li_ze",      'CameLiZe',  "_" ],
            ["came_li_ze",      'CameLiZe',  null],
            ["came#li#ze",      'CameLiZe',  "#" ],
            ["came li ze",      'CameLiZe',  " " ],
            ["came.li^ze",      'CameLiZe',  ".^"],
            ["c_a-m_e-l_i-z_e", 'CAMELIZE',  "-_"],
            ["c_a-m_e-l_i-z_e", 'CAMELIZE',  null],
            ["came.li.ze",      'CameLiZe',  "." ],
            ["came-li-ze",      'CameLiZe',  "-" ],
            ["c+a+m+e+l+i+z+e", 'CAMELIZE',  "+" ],
        ];
    }

    public function providerUnCamelize()
    {
        return [
            ["=Camelize", '=_camelize',      "_" ],
            ["Camelize",  'camelize',        "_" ],
            ["Camelize",  'camelize',        null],
            ["CameLiZe",  'came_li_ze',      "_" ],
            ["CameLiZe",  'came#li#ze',      "#" ],
            ["CameLiZe",  'came li ze',      " " ],
            ["CameLiZe",  'came.li.ze',      "." ],
            ["CameLiZe",  'came-li-ze',      "-" ],
            ["CAMELIZE",  'c/a/m/e/l/i/z/e', "/" ],
        ];
    }

    public function providerCamelizeWrongSecondParam()
    {
        return [
            [""                         ],
            [true                       ],
            [false                      ],
            [1                          ],
            [0                          ],
            [[]                         ],
            [
                function () {
                    return "-";
                }
            ],
            [new \stdClass              ],
        ];
    }
}
