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
        $t = new Exists();

        $this->assertTrue($t->testClassExists(\TestExClass::class));
        $this->assertFalse($t->testClassExists('TestExClassx'));
        $this->assertFalse($t->testClassExists(\TestExInterface::class));
        // with namespace
        $this->assertTrue($t->testClassExists(Exists::class));

        // class not interface
        $this->assertFalse($t->testInterfaceExists(\TestExClass::class));
        $this->assertTrue($t->testInterfaceExists(\TestExInterface::class));
        $this->assertFalse($t->testInterfaceExists('TestExInterfacex'));

        $this->assertTrue($t->testMethodExists($t, 'testMethodExists'));

        $this->assertTrue($t->testFileExists(__DIR__.'/../fixtures/exists.php'));
        $this->assertFalse($t->testFileExists(__DIR__.'/php/existsxxxx.php'));
    }
}
