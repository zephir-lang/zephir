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
use Stub\Issue1628;

/**
 * A concrete class that implements every inherited abstract method compiles
 * and is instantiable as usual.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/1628
 */
final class Issue1628Test extends TestCase
{
    public function testConcreteSubclassImplementingAbstractMethodWorks(): void
    {
        $object = new Issue1628();

        $this->assertSame('implemented', $object->mustImplement());
        $this->assertSame('concrete', $object->concrete());
    }
}
