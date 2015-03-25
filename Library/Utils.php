<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

/**
 * Utils
 *
 * Utility functions
 */
class Utils
{
    /**
     * Prepares a class name to be used as a C-string
     *
     * @param string $className
     * @return string
     */
    public static function escapeClassName($className)
    {
        return str_replace('\\', '\\\\', $className);
    }

    /**
     * Prepares a string to be used as a C-string
     *
     * @param string $str
     * @param bool $escapeSlash
     * @return string
     */
    public static function addSlashes($str, $escapeSlash = false)
    {
        $newstr = "";
        $after = null;
        $before = null;
        $length = strlen($str);
        for ($i = 0; $i < $length; $i++) {
            $ch = substr($str, $i, 1);
            if ($i != ($length -1)) {
                $after = substr($str, $i + 1, 1);
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
                            $before = null;
                            continue;
                        default:
                            $newstr .= "\\\\";
                            break;
                    }
                    break;
                default:
                    $newstr .= $ch;
            }
            $before = $ch;
        }
        return $newstr;
    }

    /**
     * Camelize a string
     *
     * @param       string $str
     * @return      string
     */
    public static function camelize($str)
    {
        return str_replace(' ', '', ucwords(str_replace('_', ' ', $str)));
    }

    /**
     * Checks if the content of the file on the disk is the same as the content.
     *
     * @param $content
     * @param $path
     * @return bool
     */
    public static function checkAndWriteIfNeeded($content, $path)
    {
        if (file_exists($path)) {
            $contentMd5 = md5($content);
            $existingMd5 = md5_file($path);

            if ($contentMd5 != $existingMd5) {
                file_put_contents($path, $content);
                return true;
            }
        } else {
            file_put_contents($path, $content);
            return true;
        }

        return false;
    }

    /**
     * Transform class/interface name to FQN format
     *
     * @param string $className
     * @param string $currentNamespace
     * @param AliasManager $aliasManager
     * @return string
     */
    public static function getFullName($className, $currentNamespace, AliasManager $aliasManager = null)
    {
        if (!is_string($className)) {
            throw new \InvalidArgumentException('Class name must be a string ' . print_r($className, true));
        }

        if ($className[0] !== '\\') {
            // If class/interface name not begin with \ maybe a alias or a sub-namespace
            $firstSepPos = strpos($className, '\\');
            if (false !== $firstSepPos) {
                $baseName = substr($className, 0, $firstSepPos);
                if ($aliasManager->isAlias($baseName)) {
                    return $aliasManager->getAlias($baseName) . '\\' . substr($className, $firstSepPos + 1);
                }
            } else {
                if ($aliasManager->isAlias($className)) {
                    return $aliasManager->getAlias($className);
                }
            }

            // Relative class/interface name
            if ($currentNamespace) {
                return $currentNamespace . '\\' . $className;
            } else {
                return $className;
            }
        }

        // Absolute class/interface name
        return substr($className, 1);
    }

    /**
     * Check if the host OS is windows
     *
     * @return boolean
     */
    public static function isWindows()
    {
        return strncasecmp(PHP_OS, 'WIN', 3) == 0;
    }
}
