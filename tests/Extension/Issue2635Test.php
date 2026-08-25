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

use PHPUnit\Framework\TestCase;
use ReflectionClass;
use Stub\Issue2635\Caller;
use Stub\Issue2635\Impl;

/**
 * A method reached through a parent interface has to resolve at compile time
 * and dispatch correctly at run time.
 *
 * The fixtures cannot even be generated unless the compiler walks the
 * interface chain, so reaching this test at all is part of the assertion.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2635
 */
final class Issue2635Test extends TestCase
{
    public function testCallsInheritedAndOwnInterfaceMethodsOnThis(): void
    {
        $this->assertSame('inner/outer', (new Impl())->run());
    }

    public function testCallsInheritedInterfaceMethodOnTypedVariable(): void
    {
        $this->assertSame('inner', (new Caller())->call(new Impl()));
    }

    public function testImplementsWholeInterfaceChain(): void
    {
        $interfaces = (new ReflectionClass(Impl::class))->getInterfaceNames();

        $this->assertContains('Stub\Issue2635\Outer', $interfaces);
        $this->assertContains('Stub\Issue2635\Inner', $interfaces);
    }
}
