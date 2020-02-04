<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Oo;

use PHPUnit\Framework\TestCase;
use Test\Oo\Scopes\PrivateScopeTester;
use Test\Oo\Scopes\PropertyTester;
use TestScopeExtending;
use TestScopeExtendingMagic;
use TestScopePhp;
use TestScopePhpExtending;
use TestScopePhpMagic;
use TestScopePhpMagicExtending;

class PrivateScopeTest extends TestCase
{
    /** @test */
    public function testShouldCallPrivateMethod()
    {
        $this->assertSame('isPrivate', (new PrivateScopeTester())->run());
    }

    /**
     * @expectedException \Error
     */
    public function testShouldNotSetPrivatePropertyObjPhp()
    {
        $object = new TestScopePhp();
        $tester = new PropertyTester();
        $tester->setPropertyObj($object, 'privateProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    /**
     * @expectedException \Error
     */
    public function testShouldNotSetPrivatePropertyNewPhp()
    {
        $tester = new PropertyTester();
        $tester->setPropertyNew(TestScopePhp::class, 'privateProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    /**
     * @expectedException \Error
     */
    public function testShouldNotSetPrivatePropertyObjInternal()
    {
        $object = new TestScopeExtending();
        $tester = new PropertyTester();
        $tester->setPropertyObj($object, 'privateProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    /**
     * @expectedException \Error
     */
    public function testShouldNotSetPrivatePropertyNewInternal()
    {
        $tester = new PropertyTester();
        $tester->setPropertyNew(TestScopeExtending::class, 'privateProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function testShouldSetPrivatePropertyObjPhp()
    {
        $object = new TestScopePhpMagic();
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyObj($object, 'privateProperty', 'test'));
        $this->assertEquals(1, $object->setCount);
    }

    public function testShouldSetPrivatePropertyNewPhp()
    {
        $tester = new PropertyTester();
        $obj = $tester->setPropertyNew(TestScopePhpMagic::class, 'privateProperty', 'test');
        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    public function testShouldSetPrivatePropertyObjInternal()
    {
        $object = new TestScopeExtendingMagic();
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyObj($object, 'privateProperty', 'test'));
        $this->assertEquals(1, $object->setCount);
    }

    public function testShouldSetPrivatePropertyNewInternal()
    {
        $tester = new PropertyTester();
        $obj = $tester->setPropertyNew(TestScopeExtendingMagic::class, 'privateProperty', 'test');
        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    /**
     * @expectedException \Error
     */
    public function testShouldNotSetPrivatePropertyViaThis()
    {
        $obj = new TestScopeExtending();
        $obj->setProperty('privateProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    /**
     * @expectedException \Error
     */
    public function testShouldSetPrivatePropertyViaThis()
    {
        $obj = new TestScopeExtending();
        $obj->setProperty('privateProperty2', 'test');
        $this->assertEquals('test', $obj->getPrivateProperty2());
    }

    /**
     * @expectedException \Error
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicObjInternal()
    {
        $obj = new TestScopeExtendingMagic();
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyObj($obj, 'privateProperty2', 'test'));
        $this->assertFalse(true, 'This should not be called');
    }

    /**
     * @expectedException \Error
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicNewInternal()
    {
        $tester = new PropertyTester();
        $tester->setPropertyNew(TestScopeExtendingMagic::class, 'privateProperty2', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    /**
     * @expectedException \Error
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicObjPhp()
    {
        $obj = new TestScopePhpMagicExtending();
        $tester = new PropertyTester();
        $tester->setPropertyObj($obj, 'privateProperty2', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    /**
     * @expectedException \Error
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicNewPhp()
    {
        $tester = new PropertyTester();
        $tester->setPropertyNew(TestScopePhpMagicExtending::class, 'privateProperty2', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function testShouldNotGetObjectVarsPrivatePropertyObjPhp()
    {
        $tester = new PropertyTester();
        $object = new TestScopePhp();
        $objectVars = $tester->getObjVars($object);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    public function testShouldNotGetObjectVarsPrivatePropertyNewPhp()
    {
        $tester = new PropertyTester();
        $objectVars = $tester->getNewVars(TestScopePhp::class);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    public function testShouldNotGetObjectVarsPrivatePropertyObjInternal()
    {
        $tester = new PropertyTester();
        $object = new TestScopeExtending();
        $objectVars = $tester->getObjVars($object);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    public function testShouldNotGetObjectVarsPrivatePropertyNewInternal()
    {
        $tester = new PropertyTester();
        $objectVars = $tester->getNewVars(TestScopeExtending::class);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }
}
