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

namespace Extension;

use Attribute;
use PHPUnit\Framework\TestCase;
use ReflectionClass;
use ReflectionMethod;
use Stub\Issue2738;

/**
 * Every value below must match the PHP running the test, which is the PHP
 * that compiled the extension. PHP 8.5 renumbered `TARGET_ALL` and
 * `IS_REPEATABLE`, so a C file generated on another version used to carry
 * the wrong numbers.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2738
 */
final class Issue2738Test extends TestCase
{
    public function testClassConstantsFollowTheRunningPhp(): void
    {
        $this->assertSame(Attribute::TARGET_METHOD | Attribute::IS_REPEATABLE, Issue2738::TARGETS);
        $this->assertSame([Attribute::TARGET_CLASS, 'all' => Attribute::TARGET_ALL], Issue2738::LIST);
    }

    public function testParameterDefaultFollowsTheRunningPhp(): void
    {
        $parameter = (new ReflectionMethod(Issue2738::class, 'withDefault'))->getParameters()[0];
        $userland  = (new ReflectionMethod(new class () {
            public function withDefault(int $flags = \Attribute::IS_REPEATABLE): int
            {
                return $flags;
            }
        }, 'withDefault'))->getParameters()[0];

        $this->assertSame(Attribute::IS_REPEATABLE, $parameter->getDefaultValue());
        $this->assertTrue($parameter->isDefaultValueConstant());
        $this->assertSame($userland->getDefaultValueConstantName(), $parameter->getDefaultValueConstantName());
        $this->assertSame(Attribute::IS_REPEATABLE, (new Issue2738())->withDefault());
    }

    public function testExpressionsInAMethodBodyFollowTheRunningPhp(): void
    {
        $test = new Issue2738();

        $this->assertSame(Attribute::TARGET_ALL, $test->returned());
        $this->assertSame(Attribute::TARGET_ALL | Attribute::IS_REPEATABLE, $test->combined());
        $this->assertSame('flags=' . Attribute::TARGET_ALL, $test->described());
    }

    public function testAttributeFlagsFollowTheRunningPhp(): void
    {
        $attribute = (new ReflectionClass(Issue2738::class))->getAttributes()[0];

        $this->assertSame([Attribute::TARGET_METHOD | Attribute::IS_REPEATABLE], $attribute->getArguments());
    }
}
