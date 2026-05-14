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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Os;

final class OsTest extends TestCase
{
    public function testShouldDetectWindows(): void
    {
        $isWindows = Os::isWindows();

        $this->assertIsBool($isWindows);

        // The actual result depends on the OS running the test
        // We just verify it returns a boolean
        if (stripos(PHP_OS, 'WIN') === 0) {
            $this->assertTrue($isWindows);
        } else {
            $this->assertFalse($isWindows);
        }
    }
}
