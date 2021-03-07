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

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Stub\Properties\PrivateProperties;

final class PrivatePropertiesTest extends TestCase
{
    public function testAssertations(): void
    {
        $test = new PrivateProperties();

        $this->assertNull($test->getSomeNull());
        $this->assertNull($test->getSomeNullInitial());
        $this->assertFalse($test->getSomeFalse());
        $this->assertTrue($test->getSomeTrue());
        $this->assertSame($test->getSomeInteger(), 10);
        $this->assertSame($test->getSomeDouble(), 10.25);
        $this->assertSame($test->getSomeString(), 'test');
    }
}
