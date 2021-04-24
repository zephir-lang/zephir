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

namespace Extension\BuiltIn;

use PHPUnit\Framework\TestCase;
use Stub\BuiltIn\ArrayMethods;

final class ArrayMethodsTest extends TestCase
{
    private ?ArrayMethods $arrayMethods;

    public function setUp(): void
    {
        $this->arrayMethods = new ArrayMethods();
    }

    public function testGetJoin1(): void
    {
        $this->assertSame('1-2-3', $this->arrayMethods->getJoin1());
    }

    public function testGetReversed1(): void
    {
        $this->assertSame([3, 2, 1], $this->arrayMethods->getReversed1());
    }

    public function testGetMap1(): void
    {
        $this->assertSame([100, 200, 300], $this->arrayMethods->getMap1());
    }

    public function testIssue733BuiltInJoinSpecialChars(): void
    {
        $this->assertSame('có', $this->arrayMethods->issue733BuiltInJoinSpecialChars('ålcó', ['å', 'l']));
    }
}
