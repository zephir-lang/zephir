<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Issue2542;

/**
 * A class constant may be initialized from a predefined PHP constant
 * (e.g. `const BAR = PHP_VERSION_ID;`). The constant is folded to its
 * literal value at compile time.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2542
 */
final class Issue2542Test extends TestCase
{
    public function testIntegerEnvConstantIsFolded(): void
    {
        $this->assertSame(PHP_VERSION_ID, Issue2542::VERSION_ID);
        $this->assertSame(PHP_INT_SIZE, Issue2542::INT_SIZE);
    }

    public function testStringEnvConstantIsFolded(): void
    {
        $this->assertSame(PHP_OS, Issue2542::OS);
    }
}
