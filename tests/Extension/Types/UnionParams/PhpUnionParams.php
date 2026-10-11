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

namespace Extension\Types\UnionParams;

use ArrayObject;
use Stub\Types\MayBe;

/**
 * The PHP oracle: `Stub\Types\UnionParams` written by hand.
 */
final class PhpUnionParams
{
    public function number(int|float $value)
    {
        return $value;
    }

    public function scalar(string|bool $value)
    {
        return $value;
    }

    public function floatOrString(float|string $value)
    {
        return $value;
    }

    public function intOrFalse(int|false $value)
    {
        return $value;
    }

    public function objectOrArray(MayBe|array $value)
    {
        return $value;
    }

    public function classes(MayBe|ArrayObject $value)
    {
        return $value;
    }

    public function second(string $label, int|float $value)
    {
        return $value;
    }

    public function optional(int|string $value = 5)
    {
        return $value;
    }

    public function nullDefault(int|string|null $value = null)
    {
        return $value;
    }

    public static function staticNumber(int|float $value)
    {
        return $value;
    }
}
