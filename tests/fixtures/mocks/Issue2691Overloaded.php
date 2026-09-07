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

/**
 * A property that only exists through __get(), which is where PHP's write
 * context runs out of slot: `zend_std_get_property_ptr_ptr()` returns NULL,
 * the engine reads an owned copy instead and says
 * `Indirect modification of overloaded property ... has no effect`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2691
 */
final class Issue2691Overloaded
{
    public function __get(string $name): mixed
    {
        return ['seed'];
    }
}
