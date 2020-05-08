<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
use Stub\TypeHinting\TestAbstract;

class TestConcreteClass extends TestAbstract
{
    public function testFunc(array $text = [], string $text2 = '', bool $flag = true, int $optional = null)
    {
        return func_get_args();
    }

    public function returnOneOfScalar()
    {
        return __METHOD__;
    }

    public function returnInt(): int
    {
        return 1;
    }

    public function returnUint(): int
    {
        return 1;
    }

    public function returnLong(): int
    {
        return 1;
    }

    public function returnFloat(): float
    {
        return 1.0;
    }

    public function returnDouble(): float
    {
        return 1.0;
    }

    public function returnString(): string
    {
        return __METHOD__;
    }

    public function returnBoolean(): bool
    {
        return true;
    }

    public function returnChar(): int
    {
        return 1;
    }
}
