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

namespace Zephir\Cache;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Passes\CallGathererPass;

/**
 * SlotsCache
 *
 * In order to reduce memory allocation when calling functions and method_exists
 * Zephir provides a global cache that store pointers to resolved functions
 * that aren't dynamical reducing the time to lookup functions and methods
 */
class SlotsCache
{
    private static $slot = 1;

    private static $cacheSlots = array();

    public static function getMethodSlot($method)
    {
        $className = $method->getClassDefinition()->getCompleteName();
        $methodName = $method->getName();

        if (isset(self::$cacheSlots[$className][$methodName])) {
            return self::$cacheSlots[$className][$methodName];
        }

        $slot = self::$slot++;
        self::$cacheSlots[$className][$methodName] = $slot;        
        return $slot;
    }
}
