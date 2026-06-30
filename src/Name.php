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

namespace Zephir;

use function ctype_xdigit;
use function hexdec;
use function in_array;
use function is_numeric;
use function sprintf;
use function str_replace;
use function strlen;
use function strpos;
use function substr;
use function ucwords;

final class Name
{
    /**
     * Prepares a string to be used as a C-string.
     *
     * Should NOT escape next `escape sequences`:
     *  Escape sequence | ASCII hex value | Char represented
     *  --------------- | --------------- | ----------------
     *  \a              | 07              | Audible bell (Alert, Beep)
     *  \b              | 08              | Backspace
     *  \e              | 1B              | Escape character
     *  \f              | 0C              | Formed page brake
     *  \n              | 0A              | Newline (Line Feed)
     *  \r              | 0D              | Carriage Return
     *  \t              | 09              | Horizontal Tab
     *  \v              | 0B              | Vertical Tab
     *  \\              | 5C              | Backslash
     *  \'              | 27              | Apostrophe or single quotation mark
     *  \"              | 22              | Double quotation mark
     *  \?              | 3F              | Question mark (used to avoid trigraphs)
     *  \nnn            | any             | The byte whose numerical value is given by nnn interpreted as an octal
     *  number
     *  \xhh…           | any             | The byte whose numerical value is given by hh… interpreted as a hexadecimal
     *  number
     */
    public static function addSlashes(string $string): string
    {
        $escape      = '\\';
        $controlChar = [
            'a',
            'b',
            'f',
            'n',
            'r',
            't',
            'v',
            '\\',
            '\'',
            '"',
            '?',
            'x',
        ];

        $new  = '';
        $last = strlen($string) - 1;

        for ($i = 0, $next = 1; $i <= $last; ++$i, ++$next) {
            $ch    = $string[$i];
            $after = $i !== $last ? $string[$next] : null;

            if ($ch === $escape) {
                /**
                 * PHP Unicode codepoint escape "\u{XXXX}" has no C equivalent.
                 * Encode the codepoint to UTF-8 and emit the bytes as octal
                 * escapes so the produced C string matches PHP. See #2030.
                 */
                if ('u' === $after && isset($string[$i + 2]) && '{' === $string[$i + 2]) {
                    $close = strpos($string, '}', $i + 3);
                    if (false !== $close && $close > $i + 3) {
                        $hex = substr($string, $i + 3, $close - $i - 3);
                        // Reject anything that is not a valid Unicode codepoint
                        // (PHP rejects > U+10FFFF). Out-of-range/invalid values
                        // fall through and are emitted literally, never as
                        // malformed octal that could leak bytes.
                        if (ctype_xdigit($hex) && hexdec($hex) <= 0x10FFFF) {
                            $new .= self::encodeCodepointToOctal((int) hexdec($hex));
                            $i    = $close;
                            $next = $close + 1;
                            continue;
                        }
                    }
                }

                /**
                 * PHP's "\xHH" reads at most two hex digits, but C is greedy and
                 * would absorb any following hex digits (e.g. "\x41BC"). When a
                 * further hex digit follows, re-emit the byte as a fixed-width
                 * octal escape so the trailing characters stay literal. See #2030.
                 */
                if ('x' === $after) {
                    $hex = '';
                    $j   = $i + 2;
                    while (strlen($hex) < 2 && isset($string[$j]) && ctype_xdigit($string[$j])) {
                        $hex .= $string[$j];
                        ++$j;
                    }

                    if ('' === $hex) {
                        // PHP keeps a literal "\x" when no hex digit follows;
                        // emit a literal backslash so the C compiler does not
                        // choke on a "\x" with no hex digits. The 'x' is emitted
                        // on the next iteration.
                        $new .= $escape . $escape;
                        continue;
                    }

                    if (isset($string[$j]) && ctype_xdigit($string[$j])) {
                        $new .= $escape . sprintf('%03o', hexdec($hex));
                        $i    = $j - 1;
                        $next = $j;
                        continue;
                    }
                }

                /**
                 * PHP's "\e" is the ESC byte (0x1B). C only recognises "\e" as
                 * a GCC/Clang extension; MSVC rejects it and degrades to a
                 * literal "e". Emit the portable octal escape "\033" so the
                 * produced C string matches PHP on every compiler. See #2030.
                 */
                if ('e' === $after) {
                    $new .= $escape . '033';
                    ++$i;
                    ++$next;
                    continue;
                }

                if (in_array($after, $controlChar, true) || is_numeric($after)) {
                    // should not escape native C control chars
                    $new .= $ch . $after;
                    ++$i;
                    ++$next;
                    continue;
                }

                $new .= $escape . $ch;
                continue;
            }

            if ('"' === $ch) {
                $new .= $escape . $ch;
                continue;
            }

            $new .= $ch;
        }

        return $new;
    }

    /**
     * Encodes a Unicode codepoint as UTF-8 and returns the bytes as a sequence
     * of fixed-width octal C escapes (e.g. U+1F600 => "\360\237\230\200").
     */
    private static function encodeCodepointToOctal(int $codepoint): string
    {
        if ($codepoint < 0x80) {
            $bytes = [$codepoint];
        } elseif ($codepoint < 0x800) {
            $bytes = [
                0xC0 | ($codepoint >> 6),
                0x80 | ($codepoint & 0x3F),
            ];
        } elseif ($codepoint < 0x10000) {
            $bytes = [
                0xE0 | ($codepoint >> 12),
                0x80 | (($codepoint >> 6) & 0x3F),
                0x80 | ($codepoint & 0x3F),
            ];
        } else {
            $bytes = [
                0xF0 | ($codepoint >> 18),
                0x80 | (($codepoint >> 12) & 0x3F),
                0x80 | (($codepoint >> 6) & 0x3F),
                0x80 | ($codepoint & 0x3F),
            ];
        }

        $escaped = '';
        foreach ($bytes as $byte) {
            // Mask to a single byte so the result is always exactly three octal
            // digits; this prevents any chance of leaking extra digits into the
            // surrounding C string literal.
            $escaped .= '\\' . sprintf('%03o', $byte & 0xFF);
        }

        return $escaped;
    }

    public static function camelize(string $string): string
    {
        return str_replace(' ', '', ucwords(str_replace('_', ' ', $string)));
    }

    /**
     * Transform class/interface name to
     * Fully qualified path name (FQN) format.
     */
    public static function fetchFQN(
        string $class,
        ?string $namespace = null,
        ?AliasManager $aliasManager = null
    ): string {
        /**
         * Absolute class/interface name
         */
        if ('\\' === $class[0]) {
            return substr($class, 1);
        }

        /**
         * If class/interface name not begin with \
         * maybe an alias or a sub-namespace
         */
        $firstSepPos = strpos($class, '\\');
        if (false !== $firstSepPos) {
            $baseName = substr($class, 0, $firstSepPos);
            if ($aliasManager && $aliasManager->isAlias($baseName)) {
                return $aliasManager->getAlias($baseName) . '\\' . substr($class, $firstSepPos + 1);
            }
        } elseif ($aliasManager && $aliasManager->isAlias($class)) {
            return $aliasManager->getAlias($class);
        }

        return $namespace ? $namespace . '\\' . $class : $class;
    }
}
