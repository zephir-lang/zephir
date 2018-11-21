<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir;

use Zephir\Exception\InvalidArgumentException;

/**
 * Attempts to remove recursively the directory with all subdirectories and files.
 *
 * A E_WARNING level error will be generated on failure.
 *
 * @param  string $path
 * @return void
 */
function unlink_recursive($path)
{
    if (\is_dir($path)) {
        $objects = \array_diff(\scandir($path), ['.', '..']);

        foreach ($objects as $object) {
            if (\is_dir("{$path}/{$object}")) {
                unlink_recursive("{$path}/{$object}");
            } else {
                \unlink("{$path}/{$object}");
            }
        }

        \rmdir($path);
    }
}

/**
 * Camelize a string.
 *
 * @param  string $string
 * @return string
 */
function camelize($string)
{
    return \str_replace(' ', '', \ucwords(\str_replace('_', ' ', $string)));
}

/**
 * Prepares a class name to be used as a C-string.
 *
 * @param  string $className
 * @return string
 */
function escape_class($className)
{
    return \str_replace('\\', '\\\\', $className);
}

/**
 * Prepares a string to be used as a C-string.
 *
 * @param  string $string
 * @return string
 */
function add_slashes($string)
{
    $newstr = "";
    $after = null;
    $length = \strlen($string);

    for ($i = 0; $i < $length; $i++) {
        $ch = \substr($string, $i, 1);
        if ($i != ($length -1)) {
            $after = \substr($string, $i + 1, 1);
        } else {
            $after = null;
        }

        switch ($ch) {
            case '"':
                $newstr .= "\\" . '"';
                break;
            case "\n":
                $newstr .= "\\" . 'n';
                break;
            case "\t":
                $newstr .= "\\" . 't';
                break;
            case "\r":
                $newstr .= "\\" . 'r';
                break;
            case "\v":
                $newstr .= "\\" . 'v';
                break;
            case '\\':
                switch ($after) {
                    case "n":
                    case "v":
                    case "t":
                    case "r":
                    case '"':
                    case "\\":
                        $newstr .= $ch . $after;
                        $i++;
                        break;
                    default:
                        $newstr .= "\\\\";
                        break;
                }
                break;
            default:
                $newstr .= $ch;
        }
    }
    return $newstr;
}

/**
 * Transform class/interface name to FQN format
 *
 * @param  string       $className
 * @param  string       $currentNamespace
 * @param  AliasManager $aliasManager
 * @return string
 */
function fqcn($className, $currentNamespace, AliasManager $aliasManager = null)
{
    if (\is_string($className) == false) {
        throw new InvalidArgumentException('Class name must be a string, got ' . \gettype($className));
    }

    // Absolute class/interface name
    if ($className[0] === '\\') {
        return \substr($className, 1);
    }

    // If class/interface name not begin with \ maybe a alias or a sub-namespace
    $firstSepPos = \strpos($className, '\\');
    if (false !== $firstSepPos) {
        $baseName = \substr($className, 0, $firstSepPos);
        if ($aliasManager && $aliasManager->isAlias($baseName)) {
            return $aliasManager->getAlias($baseName) . '\\' . \substr($className, $firstSepPos + 1);
        }
    } elseif ($aliasManager && $aliasManager->isAlias($className)) {
        return $aliasManager->getAlias($className);
    }

    // Relative class/interface name
    if ($currentNamespace) {
        return $currentNamespace . '\\' . $className;
    }

    return $className;
}

/**
 * Checks if the content of the file on the disk is the same as the content.
 *
 * @param  string $content
 * @param  string $path
 * @return bool
 */
function file_put_contents_ex($content, $path)
{
    if (\file_exists($path)) {
        $contentMd5 = \md5($content);
        $existingMd5 = \md5_file($path);

        if ($contentMd5 != $existingMd5) {
            \file_put_contents($path, $content);
            return true;
        }
    } else {
        \file_put_contents($path, $content);
        return true;
    }

    return false;
}
