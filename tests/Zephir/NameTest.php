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
            // PHP's \e is ESC (0x1B); C's \e is a GCC/Clang-only extension that
            // MSVC rejects, so emit the portable octal escape \033. See #2030.
            'with escape char' => ['\e', '\033'],
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

            /*
             * Exotic / security-sensitive inputs. addSlashes output is embedded
             * verbatim into generated C string literals, so it must never let a
             * crafted value break out of the literal or emit malformed escapes.
             * See #2030.
             */
            // A bare double quote must always be escaped.
            'with bare double quote' => ['"', '\"'],
            // Attempt to break out and inject C: quotes stay escaped.
            'with C breakout attempt' => ['"); zend_eval_string("x"); ("', '\"); zend_eval_string(\"x\"); (\"'],
            // A single trailing backslash must double so it can't escape the
            // closing quote of the C literal.
            'with trailing backslash' => ['foo\\', 'foo\\\\'],
            'with lone backslash' => ['\\', '\\\\'],
            'with three backslashes' => ['\\\\\\', '\\\\\\\\'],
            // Backslash immediately followed by a quote.
            'with backslash then quote' => ['\\"', '\\"'],
            // Out-of-range Unicode codepoints (PHP rejects > U+10FFFF): must be
            // emitted literally, never as malformed octal that leaks bytes.
            'with unicode max valid' => ['\u{10FFFF}', '\364\217\277\277'],
            'with unicode just over max' => ['\u{110000}', '\\\\u{110000}'],
            'with unicode 8 hex digits' => ['\u{FFFFFFFF}', '\\\\u{FFFFFFFF}'],
            'with unicode 16 hex digits' => ['\u{FFFFFFFFFFFFFFFF}', '\\\\u{FFFFFFFFFFFFFFFF}'],
            'with unicode NUL' => ['\u{0}', '\000'],
            'with unicode empty braces' => ['\u{}', '\\\\u{}'],
            'with unicode unclosed' => ['\u{41', '\\\\u{41'],
            'with unicode non-hex' => ['\u{ZZ}', '\\\\u{ZZ}'],
            // "\x" with no following hex digit would make the C compiler choke;
            // it must become a literal backslash-x like PHP keeps it.
            'with x and no hex' => ['\x', '\\\\x'],
            'with x and non-hex' => ['\xZ', '\\\\xZ'],
            // Greedy "\x": only two hex digits are consumed, rest stays literal.
            'with x and three hex' => ['\xAAA', '\252A'],
            'with x lower then hex' => ['\xabc', '\253c'],
        ];
    }

    /**
     * @dataProvider cStringProvider
     */
    public function testAddSlashes(string $testString, string $expected): void
    {
        $this->assertSame($expected, Name::addSlashes($testString));
    }

    public function dangerousStringProvider(): array
    {
        return [
            'bare quote' => ['"'],
            'quote with C payload' => ['"); system("id"); ("'],
            'trailing backslash' => ['end\\'],
            'lone backslash' => ['\\'],
            'odd backslashes' => ['\\\\\\'],
            'even backslashes' => ['\\\\\\\\'],
            'quote then backslash' => ['"\\'],
            'backslash then quote' => ['\\"'],
            'mixed quotes' => ['a"b"c"'],
            'unicode overflow' => ['\u{FFFFFFFFFFFFFFFF}'],
            'unicode over max' => ['\u{110000}'],
            'x no hex' => ['\x'],
            'x bad hex' => ['\xZZ'],
            'null byte' => ["\0"],
            'esc and quote' => ['\e"'],
        ];
    }

    /**
     * Whatever the input, the escaped result must be a safe C string literal:
     * every double quote is escaped and the value cannot terminate the literal
     * early via a dangling backslash.
     *
     * @dataProvider dangerousStringProvider
     */
    public function testAddSlashesCannotBreakOutOfCLiteral(string $input): void
    {
        $output       = Name::addSlashes($input);
        $backslashRun = 0;

        for ($i = 0, $len = strlen($output); $i < $len; ++$i) {
            $char = $output[$i];

            if ('\\' === $char) {
                ++$backslashRun;
                continue;
            }

            if ('"' === $char) {
                $this->assertSame(
                    1,
                    $backslashRun % 2,
                    sprintf('Unescaped double quote at offset %d in: %s', $i, $output)
                );
            }

            $backslashRun = 0;
        }

        $this->assertSame(
            0,
            $backslashRun % 2,
            sprintf('Dangling backslash would escape the closing quote in: %s', $output)
        );
    }
}
