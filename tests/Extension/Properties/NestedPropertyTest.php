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

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Stub\Properties\NestedProperty;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2532
 */
final class NestedPropertyTest extends TestCase
{
    public function testAssignNestedIntProperty(): void
    {
        $test = new NestedProperty();

        $this->assertSame(1, $test->getBar()->data);

        $test->setData(42);

        $this->assertSame(42, $test->getBar()->data);
        $this->assertSame(42, $test->bar->data);
    }

    public function testAssignNestedStringProperty(): void
    {
        $test = new NestedProperty();

        $this->assertSame('initial', $test->getBar()->name);

        $test->setName('zephir');

        $this->assertSame('zephir', $test->getBar()->name);
        $this->assertSame('zephir', $test->bar->name);
    }
}
