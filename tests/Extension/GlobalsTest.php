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
use Test\Globals;

/**
 * Extension\GlobalsTest.
 */
class GlobalsTest extends TestCase
{
    /** @var \Stub\Globals */
    private $test;

    public function setUp()
    {
        $this->test = new Globals();
    }

    /** @test */
    public function shouldGetGlobalExtensionSettings()
    {
        $this->assertFalse($this->test->getDefaultGlobals1());

        $this->assertTrue($this->test->getDefaultGlobals4());

        $this->assertSame(100, $this->test->getDefaultGlobals2());
        $this->assertSame(7.5, $this->test->getDefaultGlobals3());
        $this->assertSame(10, $this->test->getDefaultGlobals5());
        $this->assertSame(15.2, $this->test->getDefaultGlobals6());
        $this->assertSame(65, $this->test->getDefaultGlobals7());
    }

    /** @test */
    public function shouldSetGlobalExtensionSetting()
    {
        $this->test->setBoolValueUsingDotNotation(false);

        $this->assertFalse($this->test->getDefaultGlobals1());
    }

    /** @test */
    public function setIntValueUsingDotNotation()
    {
        $this->test->setIntValueUsingDotNotation(900);

        $this->assertSame(900, $this->test->getDefaultGlobals2());
    }

    /** @test */
    public function setCharValueUsingString()
    {
        $this->test->setCharValue('B');

        $this->assertSame(66, $this->test->getDefaultGlobals7());
    }

    /** @test */
    public function setCharValueUsingInt()
    {
        $this->test->setCharValue(90);

        $this->assertSame(90, $this->test->getDefaultGlobals7());
    }

    /** @test */
    public function setBoolValueUsingInt()
    {
        $this->test->setBoolValue(0);

        $this->assertFalse($this->test->getDefaultGlobals4());
    }

    /** @test */
    public function setBoolValueUsingBool()
    {
        $this->test->setBoolValue(true);
        $this->assertTrue($this->test->getDefaultGlobals4());

        $this->test->setBoolValue(false);
        $this->assertFalse($this->test->getDefaultGlobals4());
    }

    /** @test */
    public function shouldSetGlobalExtensionOrmCacheLevel()
    {
        $this->test->setDefaultGlobalsOrmCacheLevel(3);

        $this->assertSame(3, $this->test->getDefaultGlobalsOrmCacheLevel());
    }
}
