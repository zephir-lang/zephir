<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Extension\BuiltIn;

use Test\BuiltIn\StringMethods;

class StringMethodTest extends \PHPUnit_Framework_TestCase
{
    /**
     * @dataProvider providerCamelize
     */
    public function testCamelize($actual, $expected, $delimiter)
    {
        $t = new StringMethods();

        $this->assertEquals($expected, $t->camelize($actual, $delimiter));
    }

    /**
     * @dataProvider providerCamelizeWrongSecondParam
     * @expectedException \PHPUnit_Framework_Error_Warning
     * @expectedExceptionMessage The second argument passed to the camelize() must be a string containing at least one character
     */
    public function testCamelizeWrongSecondParam($delimiter)
    {
        $t = new StringMethods();

        $t->camelize('CameLiZe', $delimiter);
    }

    /**
     * @dataProvider providerUnCamelize
     */
    public function testUnCamelize($actual, $expected, $delimiter)
    {
        $t = new StringMethods();

        $this->assertEquals($expected, $t->uncamelize($actual, $delimiter));
    }

    /**
     * @dataProvider providerCamelizeWrongSecondParam
     * @expectedException \PHPUnit_Framework_Error_Warning
     * @expectedExceptionMessage Second argument passed to the uncamelize() must be a string of one character
     */
    public function testUnCamelizeWrongSecondParam($delimiter)
    {
        $t = new StringMethods();

        $t->uncamelize('CameLiZe', $delimiter);
    }

    public function testLength()
    {
        $t = new StringMethods();

        $this->assertEquals(5, $t->getLength1());
        $this->assertEquals(5, $t->getLength2());
        $this->assertEquals(10, $t->getLength3());
        $this->assertEquals(3, $t->getLength4('foo'));
        $this->assertEquals(8, $t->getLength5('bar'));
    }

    public function testIndex()
    {
        $t = new StringMethods();

        $this->assertEquals(0, $t->getIndex('test', 't'));
        $this->assertEquals(3, $t->getIndexWithPosition('test', 't', 1));
    }

    public function testTrim()
    {
        $t = new StringMethods();

        $this->assertEquals('hello', $t->getTrimmed());
        $this->assertEquals('foo bar', $t->getTrimmed1(' foo bar  '));
        $this->assertEquals('foo bar  ', $t->getLeftTrimmed(' foo bar  '));
        $this->assertEquals(' foo bar', $t->getRightTrimmed(' foo bar  '));
    }

    public function testChangeCase()
    {
        $t = new StringMethods();

        $this->assertEquals('FOO', $t->getUpper('foo'));
        $this->assertEquals('foo', $t->getLower('FOO'));

        $this->assertEquals('Foo', $t->getUpperFirst('foo'));
        $this->assertEquals('fOO', $t->getLowerFirst('FOO'));
    }

    public function testFormat()
    {
        $t = new StringMethods();

        $this->assertEquals('hello zephir!', $t->getFormatted('zephir'));
    }

    public function testHashes()
    {
        $t = new StringMethods();
        $str = 'hello world!';

        $this->assertEquals(md5($str), $t->getMd5($str));
        $this->assertEquals(sha1($str), $t->getSha1($str));
    }

    public function testModifications()
    {
        $t = new StringMethods();
        $this->assertEquals("<br />\nfoo<br />\n", $t->getNl2br("\nfoo\n"));
        $this->assertEquals('foobarfoobar', $t->getRepeatted('foobar', 2));

        $shuffled = $t->getShuffled('hello world');
        $this->assertNotEquals('hello world', $shuffled);
        $this->assertEquals(strlen('hello world'), strlen($shuffled));

        $this->assertEquals('olleh', $t->getReversed('hello'));
    }

    public function testParsers()
    {
        $t = new StringMethods();
        // $this->assertEquals(['foo' => 'bar'], $t->getParsedJson('{ "foo" : "bar" }', true));
        $this->assertEquals(array('foo', 'bar', 'baz'), $t->getParsedCsv('foo,bar,"baz"'));
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
