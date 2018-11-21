<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use Test\Globals;
use Zephir\Support\TestCase;

/**
 * Extension\GlobalsTest
 *
 */
class GlobalsTest extends TestCase
{
    /** @test */
    public function shouldGetGlobalExtensionSettings()
    {
        $t = new Globals();

        $this->assertTrue($t->getDefaultGlobals1());
        $this->assertTrue($t->getDefaultGlobals4());

        $this->assertSame(100, $t->getDefaultGlobals2());
        $this->assertSame(7.5, $t->getDefaultGlobals3());
        $this->assertSame(10, $t->getDefaultGlobals5());
        $this->assertSame(15.2, $t->getDefaultGlobals6());
        $this->assertSame(65, $t->getDefaultGlobals7());
    }

    /** @test */
    public function shouldSetGlobalExtensionSetting()
    {
        $t = new Globals();

        $t->setBoolValueUsingDotNotation(false);
        $this->assertFalse($t->getDefaultGlobals1());
    }

    /** @test */
    public function setIntValueUsingDotNotation()
    {
        $t = new Globals();

        $t->setIntValueUsingDotNotation(900);
        $this->assertSame(900, $t->getDefaultGlobals2());
    }

    /** @test */
    public function setCharValueUsingString()
    {
        $t = new Globals();

        $t->setCharValue('B');
        $this->assertSame(66, $t->getDefaultGlobals7());
    }

    /** @test */
    public function setCharValueUsingInt()
    {
        $t = new Globals();

        $t->setCharValue(90);
        $this->assertSame(90, $t->getDefaultGlobals7());
    }

    /** @test */
    public function setBoolValueUsingInt()
    {
        $t = new Globals();

        $t->setBoolValue(0);
        $this->assertFalse($t->getDefaultGlobals4());
    }

    /** @test */
    public function setBoolValueUsingBool()
    {
        $t = new Globals();

        $t->setBoolValue(true);
        $this->assertTrue($t->getDefaultGlobals4());

        $t->setBoolValue(false);
        $this->assertFalse($t->getDefaultGlobals4());
    }
}
