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

namespace Zephir\Types;

/**
 * Trait for types that use method maps for built-in method transformations
 */
trait MethodMapTrait
{
    /**
     * Merge additional methods into the method map
     */
    protected function mergeMethodMap(array $additionalMethods): void
    {
        $this->methodMap = array_merge($this->methodMap, $additionalMethods);
    }

    /**
     * Check if a method exists in the method map
     */
    protected function hasMethodMapping(string $methodName): bool
    {
        return isset($this->methodMap[$methodName]);
    }

    /**
     * Get the PHP function name for a Zephir method
     */
    protected function getMethodMapping(string $methodName): ?string
    {
        return $this->methodMap[$methodName] ?? null;
    }
}
