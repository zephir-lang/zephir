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
use Stub\Issue887;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/887
 */
final class Issue887Test extends TestCase
{
    public function testPreventPropertyTypeChangeFromPreviousMethodCall(): void
    {
        $class = new Issue887();

        $this->assertIsArray($class->data);
        $this->assertIsArray($class->fetch_array());
        $this->assertIsObject($class->fetch_object());
        $this->assertIsArray($class->fetch_array());
        $this->assertIsArray($class->data);
    }
}
