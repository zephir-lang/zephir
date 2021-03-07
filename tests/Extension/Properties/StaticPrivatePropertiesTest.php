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
use Stub\Properties\App;
use Stub\Properties\StaticPrivateProperties;

final class StaticPrivatePropertiesTest extends TestCase
{
    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1530
     */
    public function testShouldGetInstanceByUsingSingleton(): void
    {
        $test = new App();

        $this->assertInstanceOf(StaticPrivateProperties::class, $test->getInstance());
        $this->assertInstanceOf(StaticPrivateProperties::class, StaticPrivateProperties::getInstance());
        $this->assertSame($test->getInstance(), StaticPrivateProperties::getInstance());
    }
}
