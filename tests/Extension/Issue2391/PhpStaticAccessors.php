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

namespace Extension\Issue2391;

/**
 * The PHP oracle: what each `Stub\Issue2391\StaticAccessors` shortcut means
 * when written by hand.
 */
final class PhpStaticAccessors
{
    protected static $plain = 5;
    protected static $count = 1;
    protected static $flag;
    protected static $label = 'static';

    public function getPlain()
    {
        return self::$plain;
    }

    public function setPlain($plain)
    {
        self::$plain = $plain;

        return $this;
    }

    public function getCount(): int
    {
        return self::$count;
    }

    public function setCount(int $count)
    {
        self::$count = $count;

        return $this;
    }

    public function getFlag(): bool
    {
        return self::$flag;
    }

    public function __toString(): string
    {
        return self::$label;
    }
}
