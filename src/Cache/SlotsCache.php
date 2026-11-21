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

namespace Zephir\Cache;

/**
 * In order to reduce memory allocation when calling functions Zephir provides
 * a global cache that store pointers to resolved functions that aren't
 * dynamical reducing the time to lookup functions. Method and static calls
 * have a cache per call site instead, see MethodCache.
 */
class SlotsCache
{
    public const MAX_SLOTS_NUMBER = 512;

    private static array $cacheFunctionSlots = [];

    private static int $slot = 1;

    /**
     * Resets the slots for a fresh compilation run. Called from the Compiler
     * constructor so slot indices depend only on the project being compiled.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2687
     */
    public static function reset(): void
    {
        self::$cacheFunctionSlots = [];
        self::$slot               = 1;
    }

    /**
     * Returns or creates a cache slot for a function.
     */
    public static function getExistingFunctionSlot(string $functionName): int
    {
        return self::$cacheFunctionSlots[$functionName] ?? 0;
    }

    /**
     * Returns or creates a cache slot for a function.
     */
    public static function getFunctionSlot(string $functionName): int
    {
        if (isset(self::$cacheFunctionSlots[$functionName])) {
            return self::$cacheFunctionSlots[$functionName];
        }

        $slot = self::$slot++;
        if ($slot >= self::MAX_SLOTS_NUMBER) {
            return 0;
        }

        self::$cacheFunctionSlots[$functionName] = $slot;

        return $slot;
    }
}
