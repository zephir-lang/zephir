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
 * Assigns a stable, project-global inline-cache slot index to each distinct
 * (class, property-name) pair for cached object-property access. The index
 * points at a per-request-zeroed 3-pointer region in the generated
 * extension's `pcache` module-global array (see kernel/globals.h). Keying by
 * (class, property) — rather than per call site — bounds the slot count to
 * the number of distinct properties and keeps `this->prop` sites, which are
 * near-monomorphic, sharing one warm slot.
 *
 * Slot 0 is the "uncached" sentinel (the kernel then passes a NULL cache_slot
 * to the engine, i.e. today's behavior). When the cap is exhausted, further
 * pairs get slot 0 — correct, just unoptimized.
 *
 * Must stay in sync with ZEPHIR_MAX_PROPERTY_CACHE_SLOTS in kernel/globals.h.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1902
 */
class PropertyCacheSlots
{
    public const MAX_SLOTS_NUMBER = 2048;

    /** @var array<string, array<string, int>> */
    private static array $slots = [];

    private static int $slot = 1;

    /**
     * Resets the counter for a fresh compilation run. Called from the
     * Compiler constructor so slot indices are stable per build (and the
     * codegen golden tests are deterministic).
     */
    public static function reset(): void
    {
        self::$slots = [];
        self::$slot  = 1;
    }

    /**
     * Returns (or assigns) the inline-cache slot for a (class, property) pair.
     * Returns 0 when the cap is exhausted (uncached fallback).
     */
    public static function getSlot(string $className, string $property): int
    {
        if (isset(self::$slots[$className][$property])) {
            return self::$slots[$className][$property];
        }

        $slot = self::$slot++;
        if ($slot >= self::MAX_SLOTS_NUMBER) {
            return 0;
        }

        self::$slots[$className][$property] = $slot;

        return $slot;
    }
}
