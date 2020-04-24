<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Test\Exists;

class ExistsTest extends TestCase
{
    public function testAssertations()
    {
        $test = new Exists();

        $this->assertTrue($test->testClassExists(\TestExClass::class));
        $this->assertFalse($test->testClassExists('TestExClassx'));
        $this->assertFalse($test->testClassExists(\TestExInterface::class));
        // with namespace
        $this->assertTrue($test->testClassExists(Exists::class));

        // class not interface
        $this->assertFalse($test->testInterfaceExists(\TestExClass::class));
        $this->assertTrue($test->testInterfaceExists(\TestExInterface::class));
        $this->assertFalse($test->testInterfaceExists('TestExInterfacex'));

        $this->assertTrue($test->testMethodExists($test, 'testMethodExists'));

        $this->assertTrue($test->testFileExists(__DIR__.'/../fixtures/exists.php'));
        $this->assertFalse($test->testFileExists(__DIR__.'/php/existsxxxx.php'));
    }
}
