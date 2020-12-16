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
use Stub\BuiltIn\StringMethods;

class StringMethodTest extends TestCase
{
    /** @var StringMethods */
    private $test;

    protected function setUp(): void
    {
        $this->test = new StringMethods();
    }

    /**
     * @dataProvider providerCamelize
     *
     * @param mixed $actual
     * @param mixed $expected
     * @param mixed $delimiter
     */
    public function testCamelize($actual, $expected, $delimiter)
    {
        $this->assertSame($expected, $this->test->camelize($actual, $delimiter));
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

        $this->test->camelize('CameLiZe', $delimiter);
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
        $this->assertSame($expected, $this->test->uncamelize($actual, $delimiter));
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

        $this->test->uncamelize('CameLiZe', $delimiter);
    }

    public function testLength()
    {
        $this->assertSame(5, $this->test->getLength1());
        $this->assertSame(5, $this->test->getLength2());
        $this->assertSame(10, $this->test->getLength3());
        $this->assertSame(3, $this->test->getLength4('foo'));
        $this->assertSame(8, $this->test->getLength5('bar'));
    }

    public function testIndex()
    {
        $this->assertSame(0, $this->test->getIndex('test', 't'));
        $this->assertSame(3, $this->test->getIndexWithPosition('test', 't', 1));
    }

    public function testTrim()
    {
        $this->assertSame('hello', $this->test->getTrimmed());
        $this->assertSame('foo bar', $this->test->getTrimmed1(' foo bar  '));
        $this->assertSame('foo bar  ', $this->test->getLeftTrimmed(' foo bar  '));
        $this->assertSame(' foo bar', $this->test->getRightTrimmed(' foo bar  '));
    }

    public function testChangeCase()
    {
        $this->assertSame('FOO', $this->test->getUpper('foo'));
        $this->assertSame('foo', $this->test->getLower('FOO'));

        $this->assertSame('Foo', $this->test->getUpperFirst('foo'));
        $this->assertSame('fOO', $this->test->getLowerFirst('FOO'));
    }

    public function testFormat()
    {
        $this->assertSame('hello zephir!', $this->test->getFormatted('zephir'));
    }

    public function testHashes()
    {
        $str = 'hello world!';

        $this->assertSame(md5($str), $this->test->getMd5($str));
        $this->assertSame(sha1($str), $this->test->getSha1($str));
    }

    public function testModifications()
    {
        $this->assertSame("<br />\nfoo<br />\n", $this->test->getNl2br("\nfoo\n"));
        $this->assertSame('foobarfoobar', $this->test->getRepeatted('foobar', 2));

        $shuffled = $this->test->getShuffled('hello world');
        $this->assertNotEquals('hello world', $shuffled);
        $this->assertSame(\strlen('hello world'), \strlen($shuffled));

        $this->assertSame('olleh', $this->test->getReversed('hello'));
    }

    public function testParsers()
    {
        // TODO: Do we still need this?
        // $this->assertSame(['foo' => 'bar'], $t->getParsedJson('{ "foo" : "bar" }', true));

        $this->assertSame(['foo', 'bar', 'baz'], $this->test->getParsedCsv('foo,bar,"baz"'));
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
