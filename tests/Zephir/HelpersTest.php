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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;

use function Zephir\add_slashes;
use function Zephir\camelize;
use function Zephir\escape_class;

final class HelpersTest extends TestCase
{
    public function testShouldEscapeClassName(): void
    {
        $classname = '\Bar\Foo';
        $this->assertSame(
            escape_class($classname),
            '\\\\Bar\\\\Foo'
        );
    }

    public function testShouldCamelizeClassName(): void
    {
        $name = 'foo_Bar_Foo_bar';
        $this->assertSame(
            camelize($name),
            'FooBarFooBar'
        );
    }

    public function cStringProvider(): array
    {
        return [
            // 'test name' => [ 'test string', 'expected result']
            'with empty char' => ['', ''],
            'with space char' => [' ', ' '],
            'with double quotation mark' => ['\"', '\"'],
            'with single quotation mark' => ['\'', '\''],
            'with questions mark' => ['\?', '\?'],
            'with alert char' => ['\a', '\a'],
            'with backspace char' => ['\b', '\b'],
            'with escape char' => ['\e', '\e'],
            'with form feed page brake char' => ['\f', '\f'],
            'with line feed char' => ['\n', '\n'],
            'with carriage return char' => ['\r', '\r'],
            'with horizontal tab char' => ['\t', '\t'],
            'with vertical tab char' => ['\v', '\v'],
            'with backslash char' => ['\\', '\\\\'],
            'with hexadecimal numerals' => ['\x12', '\x12'],
            'with octal numerals' => ['\123', '\123'],
            'with escaped parenthesis' => ['\(\)', '\\\\(\\\\)'],
            'with C printf format' => ['\033[10;10h%d\n', '\033[10;10h%d\n'],
            'with C \0' => ['\0', '\0'],
            'with regexp 1' => ['/(\w+)\s*=\s*(\[[^\]]*\]|\"[^\"]*\"|[^,)]*)\s*(?:,|$)/', '/(\\\\w+)\\\\s*=\\\\s*(\\\\[[^\\\\]]*\\\\]|\"[^\"]*\"|[^,)]*)\\\\s*(?:,|$)/'],
            'with regexp 2' => ['/@(\w+)(?:\s*(?:\(\s*)?(.*?)(?:\s*\))?)??\s*(?:\n|\*\/)/', '/@(\\\\w+)(?:\\\\s*(?:\\\(\\\\s*)?(.*?)(?:\\\\s*\\\\))?)??\\\\s*(?:\n|\\\\*\\\\/)/'],
            'with regexp 3' => ['/(\w+)\s*=\s*(\[[^\]]*\]|"[^"]*"|[^,)]*)\s*(?:,|$)/', '/(\\\\w+)\\\\s*=\\\\s*(\\\\[[^\\\\]]*\\\\]|\"[^\"]*\"|[^,)]*)\\\\s*(?:,|$)/'],
        ];
    }

    /**
     * @dataProvider cStringProvider
     *
     * @param string $testString
     * @param string $expected
     */
    public function testShouldPrepareStringAsCString(string $testString, string $expected): void
    {
        $this->assertSame($expected, add_slashes($testString));
    }
}
