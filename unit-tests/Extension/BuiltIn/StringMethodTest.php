<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\BuiltIn;

use PHPUnit\Framework\Error\Warning;
use PHPUnit\Framework\TestCase;
use Test\BuiltIn\StringMethods;

class StringMethodTest extends TestCase
{
    /**
     * @dataProvider providerCamelize
     *
     * @param mixed $actual
     * @param mixed $expected
     * @param mixed $delimiter
     */
    public function testCamelize($actual, $expected, $delimiter)
    {
        $t = new StringMethods();

        $this->assertSame($expected, $t->camelize($actual, $delimiter));
    }

    /**
     * @dataProvider providerCamelizeWrongSecondParam
     *
     * @param mixed $delimiter
     */
    public function testCamelizeWrongSecondParam($delimiter)
    {
        $this->expectException(Warning::class);
        $this->expectExceptionMessage(
            'The second argument passed to the camelize() must be a string containing at least one character'
        );

        $t = new StringMethods();
        $t->camelize('CameLiZe', $delimiter);
    }

    /**
     * @dataProvider providerUnCamelize
     *
     * @param mixed $actual
     * @param mixed $expected
     * @param mixed $delimiter
     */
    public function testUnCamelize($actual, $expected, $delimiter)
    {
        $t = new StringMethods();

        $this->assertSame($expected, $t->uncamelize($actual, $delimiter));
    }

    /**
     * @dataProvider providerCamelizeWrongSecondParam
     *
     * @param mixed $delimiter
     */
    public function testUnCamelizeWrongSecondParam($delimiter)
    {
        $this->expectException(Warning::class);
        $this->expectExceptionMessage(
            'Second argument passed to the uncamelize() must be a string of one character'
        );

        $t = new StringMethods();

        $t->uncamelize('CameLiZe', $delimiter);
    }

    public function testLength()
    {
        $t = new StringMethods();

        $this->assertSame(5, $t->getLength1());
        $this->assertSame(5, $t->getLength2());
        $this->assertSame(10, $t->getLength3());
        $this->assertSame(3, $t->getLength4('foo'));
        $this->assertSame(8, $t->getLength5('bar'));
    }

    public function testIndex()
    {
        $t = new StringMethods();

        $this->assertSame(0, $t->getIndex('test', 't'));
        $this->assertSame(3, $t->getIndexWithPosition('test', 't', 1));
    }

    public function testTrim()
    {
        $t = new StringMethods();

        $this->assertSame('hello', $t->getTrimmed());
        $this->assertSame('foo bar', $t->getTrimmed1(' foo bar  '));
        $this->assertSame('foo bar  ', $t->getLeftTrimmed(' foo bar  '));
        $this->assertSame(' foo bar', $t->getRightTrimmed(' foo bar  '));
    }

    public function testChangeCase()
    {
        $t = new StringMethods();

        $this->assertSame('FOO', $t->getUpper('foo'));
        $this->assertSame('foo', $t->getLower('FOO'));

        $this->assertSame('Foo', $t->getUpperFirst('foo'));
        $this->assertSame('fOO', $t->getLowerFirst('FOO'));
    }

    public function testFormat()
    {
        $t = new StringMethods();

        $this->assertSame('hello zephir!', $t->getFormatted('zephir'));
    }

    public function testHashes()
    {
        $t = new StringMethods();
        $str = 'hello world!';

        $this->assertSame(md5($str), $t->getMd5($str));
        $this->assertSame(sha1($str), $t->getSha1($str));
    }

    public function testModifications()
    {
        $t = new StringMethods();
        $this->assertSame("<br />\nfoo<br />\n", $t->getNl2br("\nfoo\n"));
        $this->assertSame('foobarfoobar', $t->getRepeatted('foobar', 2));

        $shuffled = $t->getShuffled('hello world');
        $this->assertNotEquals('hello world', $shuffled);
        $this->assertSame(\strlen('hello world'), \strlen($shuffled));

        $this->assertSame('olleh', $t->getReversed('hello'));
    }

    public function testParsers()
    {
        $t = new StringMethods();

        // TODO: Do we still need this?
        // $this->assertSame(['foo' => 'bar'], $t->getParsedJson('{ "foo" : "bar" }', true));

        $this->assertSame(['foo', 'bar', 'baz'], $t->getParsedCsv('foo,bar,"baz"'));
    }

    public function providerCamelize()
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

    public function providerUnCamelize()
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

    public function providerCamelizeWrongSecondParam()
    {
        return [
            [''],
            [true],
            [false],
            [1],
            [0],
            [[]],
            [
                function () {
                    return '-';
                },
            ],
            [new \stdClass()],
        ];
    }
}
