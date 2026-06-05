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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Name;

final class NameTest extends TestCase
{
    public function testCamelize(): void
    {
        $name = 'foo_Bar_Foo_bar';
        $this->assertSame(Name::camelize($name), 'FooBarFooBar');
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
            // \xHH reads at most two hex digits (PHP), but C is greedy; the
            // trailing hex digits must stay literal. See #2030.
            'with hexadecimal followed by hex digits' => ['\x41BC', '\101BC'],
            // PHP Unicode codepoint escapes have no C equivalent and are
            // encoded to UTF-8 bytes emitted as octal escapes. See #2030.
            'with unicode ascii' => ['\u{41}', '\101'],
            'with unicode 2-byte' => ['\u{E9}', '\303\251'],
            'with unicode 3-byte' => ['\u{20AC}', '\342\202\254'],
            'with unicode 4-byte' => ['\u{1F600}', '\360\237\230\200'],
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
     */
    public function testAddSlashes(string $testString, string $expected): void
    {
        $this->assertSame($expected, Name::addSlashes($testString));
    }
}
