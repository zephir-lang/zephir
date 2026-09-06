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
 * The plain PHP counterpart of Stub\Issue2691, method for method.
 *
 * PHP is the oracle for the write context, so every divergence is stated as
 * "run this here and there, compare the whole transcript" rather than as an
 * expected literal, which would pin the assertion to one PHP version.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2691
 */
final class Issue2691Holder
{
    protected $prefixes = [];

    protected $rows = [];

    protected $value = null;

    public function pushAndReturn(string $key, string $value): array
    {
        if (!isset($this->prefixes[$key])) {
            $this->prefixes[$key] = [];
        }

        array_push($this->prefixes[$key], $value);

        return $this->prefixes;
    }

    public function pushIntoDynamic(string $name, string $key, string $value): array
    {
        if (!isset($this->prefixes[$key])) {
            $this->prefixes[$key] = [];
        }

        array_push($this->{$name}[$key], $value);

        return $this->prefixes;
    }

    public function shiftDynamic(string $name): mixed
    {
        return array_shift($this->{$name});
    }

    public function pushIntoBracedLiteral(string $key, string $value): array
    {
        if (!isset($this->prefixes[$key])) {
            $this->prefixes[$key] = [];
        }

        array_push($this->{'prefixes'}[$key], $value);

        return $this->prefixes;
    }

    public function pushThenCopy(string $key, string $value): array
    {
        if (!isset($this->prefixes[$key])) {
            $this->prefixes[$key] = [];
        }

        array_push($this->prefixes[$key], $value);

        $copy   = $this->prefixes[$key];
        $copy[] = 'copy only';

        $fetched   = $this->prefixes[$key] ?? null;
        $fetched[] = 'fetched only';

        return ['property' => $this->prefixes[$key], 'copy' => $copy, 'fetched' => $fetched];
    }

    protected static $staticPrefixes = [];

    public static function pushStatic(string $key, string $value): array
    {
        if (!isset(self::$staticPrefixes[$key])) {
            self::$staticPrefixes[$key] = [];
        }

        array_push(self::$staticPrefixes[$key], $value);

        return self::$staticPrefixes;
    }

    public static function resetStatic(): void
    {
        self::$staticPrefixes = [];
    }

    public function pushIntoLocalShared(string $key, string $value): array
    {
        $container       = [];
        $container[$key] = [];
        $alias           = $container;

        array_push($container[$key], $value);

        return ['container' => $container, 'alias' => $alias];
    }

    public function shiftRows(): mixed
    {
        return array_shift($this->rows);
    }

    public function retypeValue(string $type): bool
    {
        return settype($this->value, $type);
    }

    public function pushMissingKey(string $key, string $value): array
    {
        array_push($this->prefixes[$key], $value);

        return $this->prefixes;
    }

    public function pushIntoOverloaded(object $holder, string $key, string $value): bool
    {
        array_push($holder->missing[$key], $value);

        return true;
    }

    public function setRows(mixed $rows): void
    {
        $this->rows = $rows;
    }

    public function getRows(): mixed
    {
        return $this->rows;
    }

    public function setValue(mixed $value): void
    {
        $this->value = $value;
    }

    public function getValue(): mixed
    {
        return $this->value;
    }

    public function getPrefixes(): array
    {
        return $this->prefixes;
    }
}
