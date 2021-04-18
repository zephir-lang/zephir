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
use Stub\Properties\StaticProtectedProperties;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1536
 */
final class StaticProtectedPropertiesTest extends TestCase
{
    public function testCompareStaticNullOnBuiltIn(): void
    {
        $class = new StaticProtectedProperties();
        $this->assertTrue($class->compareStaticNull());
    }

    public function testCompareStaticNullOnExtending(): void
    {
        $spp = new \TestStaticProtectedProperties();
        $this->assertTrue($spp->compareStaticNull());
    }
}
