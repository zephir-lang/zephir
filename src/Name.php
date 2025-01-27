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

use function in_array;
use function is_numeric;
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
            'e',
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
