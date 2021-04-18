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

namespace Extension\Constructors;

use PHPUnit\Framework\TestCase;
use Stub\Constructors\Issue1803;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1803
 */
final class Issue1803Test extends TestCase
{
    public function testOldStyleConstructorThatWontBeCalledOnClassInit(): void
    {
        $class = new Issue1803();

        $this->assertInstanceOf(Issue1803::class, $class);
        $this->assertSame('Original value', $class->value);

        $class->Issue1803();

        $this->assertSame("Value that won't be updated on class init", $class->value);
    }
}
