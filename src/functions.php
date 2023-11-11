<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Zephir\Exception\InvalidArgumentException;

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
 *  \nnn            | any             | The byte whose numerical value is given by nnn interpreted as an octal number
 *  \xhh…           | any             | The byte whose numerical value is given by hh… interpreted as a hexadecimal number
 *
 * @param string $string
 *
 * @return string
 */
function add_slashes(string $string): string
{
    $escape = '\\';
    $controlChar = [
        'a', 'b', 'e', 'f', 'n', 'r', 't', 'v', '\\', '\'', '"', '?', 'x',
    ];

    $newstr = '';
    $after = null;
    $last = \strlen($string) - 1;

    for ($i = 0, $next = 1; $i <= $last; ++$i, ++$next) {
        $ch = $string[$i];

        if ($i !== $last) {
            $after = $string[$next];
        } else {
            $after = null;
        }

        if ($ch === $escape) {
            if (\in_array($after, $controlChar, true) || is_numeric($after)) {
                // should not escape native C control chars
                $newstr .= $ch.$after;
                ++$i;
                ++$next;
                continue;
            }
            $newstr .= $escape.$ch;
            continue;
        }

        if ('"' === $ch) {
            $newstr .= $escape.$ch;
            continue;
        }

        $newstr .= $ch;
    }

    return $newstr;
}

/**
 * Transform class/interface name to FQN format.
 *
 * @param string       $className
 * @param string       $currentNamespace
 * @param AliasManager $aliasManager
 *
 * @return string
 */
function fqcn($className, $currentNamespace, AliasManager $aliasManager = null)
{
    if (!\is_string($className)) {
        throw new InvalidArgumentException('Class name must be a string, got '.\gettype($className));
    }

    // Absolute class/interface name
    if ('\\' === $className[0]) {
        return substr($className, 1);
    }

    // If class/interface name not begin with \ maybe a alias or a sub-namespace
    $firstSepPos = strpos($className, '\\');
    if (false !== $firstSepPos) {
        $baseName = substr($className, 0, $firstSepPos);
        if ($aliasManager && $aliasManager->isAlias($baseName)) {
            return $aliasManager->getAlias($baseName).'\\'.substr($className, $firstSepPos + 1);
        }
    } elseif ($aliasManager && $aliasManager->isAlias($className)) {
        return $aliasManager->getAlias($className);
    }

    // Relative class/interface name
    if ($currentNamespace) {
        return $currentNamespace.'\\'.$className;
    }

    return $className;
}
