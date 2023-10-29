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
 * Camelize a string.
 *
 * @param string $string
 *
 * @return string
 */
function camelize($string)
{
    return str_replace(' ', '', ucwords(str_replace('_', ' ', $string)));
}

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

/**
 * Checks if the content of the file on the disk is the same as the content.
 *
 * @param string $content
 * @param string $path
 *
 * @return int|bool
 */
function file_put_contents_ex($content, $path)
{
    if (file_exists($path)) {
        $contentMd5 = md5($content);
        $existingMd5 = md5_file($path);

        if ($contentMd5 !== $existingMd5) {
            return file_put_contents($path, $content);
        }
    } else {
        return file_put_contents($path, $content);
    }

    return false;
}

/**
 * Checks if currently running under MS Windows.
 *
 * @return bool
 */
function is_windows()
{
    return 0 === stripos(\PHP_OS, 'WIN');
}

/**
 * Checks if current PHP is thread safe.
 *
 * @return bool
 */
function is_zts()
{
    if (\defined('PHP_ZTS') && \PHP_ZTS === 1) {
        return true;
    }

    ob_start();
    phpinfo(\INFO_GENERAL);

    return (bool) preg_match('/Thread\s*Safety\s*enabled/i', strip_tags(ob_get_clean()));
}

/**
 * Resolves Windows release folder.
 *
 * @return string
 */
function windows_release_dir()
{
    if (is_zts()) {
        if (\PHP_INT_SIZE === 4) {
            // 32-bit version of PHP
            return 'ext\\Release_TS';
        }

        if (\PHP_INT_SIZE === 8) {
            // 64-bit version of PHP
            return 'ext\\x64\\Release_TS';
        }

        // fallback
        return 'ext\\Release_TS';
    }

    if (\PHP_INT_SIZE === 4) {
        // 32-bit version of PHP
        return 'ext\\Release';
    }

    if (\PHP_INT_SIZE === 8) {
        // 64-bit version of PHP
        return 'ext\\x64\\Release';
    }

    // fallback
    return 'ext\\Release';
}
