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
 * The plain PHP twin of the evaluation-order methods of Stub\Issue2747: each
 * method is the same statement, so PHP itself says what the order must be.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2747
 */
final class Issue2747Order
{
    public static $sp = [];

    public $p = [];

    public $trace = [];

    public $counter = 0;

    public function key($n)
    {
        $this->trace[] = 'k' . $n;

        return $n;
    }

    public function value($n)
    {
        $this->trace[] = 'v' . $n;

        return $n;
    }

    public function bump()
    {
        $this->counter++;

        return $this->counter;
    }

    public function orderLocal(): array
    {
        $a = [];
        $a[$this->key(1)] = $this->value(2);

        return [$this->trace, $a];
    }

    public function orderLocalNested(): array
    {
        $a = [];
        $a[$this->key(1)][$this->key(2)] = $this->value(3);

        return [$this->trace, $a];
    }

    public function orderLocalAppend(): array
    {
        $a = [];
        $a[$this->key(1)][] = $this->value(2);

        return [$this->trace, $a];
    }

    public function orderCompound(): array
    {
        $a = [1 => 'a'];
        $a[$this->key(1)] .= $this->value(2);

        return [$this->trace, $a];
    }

    public function orderThis(): array
    {
        $this->p[$this->key(1)] = $this->value(2);

        return [$this->trace, $this->p];
    }

    public function orderThisNested(): array
    {
        $this->p[$this->key(1)][$this->key(2)][] = $this->value(3);

        return [$this->trace, $this->p];
    }

    public function orderStatic(): array
    {
        self::$sp = [];
        self::$sp[$this->key(1)][$this->key(2)] = $this->value(3);

        return [$this->trace, self::$sp];
    }

    public function orderStaticAppend(): array
    {
        self::$sp = [];
        self::$sp[$this->key(1)][] = $this->value(2);

        return [$this->trace, self::$sp];
    }

    public function orderPropertyIndex(): array
    {
        $a = [];
        $a[$this->counter] = $this->bump();

        return $a;
    }

    public function orderPropertyValue(): array
    {
        $a = [];
        $a[$this->bump()] = $this->counter;

        return $a;
    }

    public function orderComputedIndex(): array
    {
        $a = [];
        $a[$this->counter + 10] = $this->bump();

        return $a;
    }

    public function orderStringOffset(): string
    {
        $s = 'abc';
        $s[$this->counter] = (string) $this->bump();

        return $s;
    }

    public function orderVariableIndex(): array
    {
        $i = 0;
        $f = function () use (&$i) {
            $i = 5;

            return 'v';
        };
        $b = [];
        $b[$i] = $f();

        return $b;
    }

    public function orderVariableExpressionIndex(): array
    {
        $i = 0;
        $f = function () use (&$i) {
            $i = 5;

            return 'v';
        };
        $b = [];
        $b[$i + 1] = $f();

        return $b;
    }

    public function orderLiteralIndex(): array
    {
        $a = [];
        $a[0] = $this->value(1);
        $a['k'] = $this->value(2);

        return [$this->trace, $a];
    }
}
