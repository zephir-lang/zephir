<?php

namespace Extension\Oo\Scopes;

use TestScopeExtending;
use TestScopePhp;
use Test\Oo\Scopes\ProtectedScopeTester;
use Test\Oo\Scopes\PropertyTester;
use PHPUnit\Framework\TestCase;
use TestScopePhpMagic;

class ProtectedScopeTest extends TestCase
{
    public function testShouldNotSetProtectedPropertyObjPhp()
    {
        $object = new TestScopePhp();
        $tester = new PropertyTester();
        $tester->setPropertyObj($object, 'protectedProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function testShouldNotSetProtectedPropertyNewPhp()
    {
        $tester = new PropertyTester();
        $tester->setPropertyNew(TestScopePhp::class, 'protectedProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function testShouldNotSetProtectedPropertyObjInternal()
    {
        $object = new TestScopeExtending();
        $tester = new PropertyTester();
        $tester->setPropertyObj($object, 'protectedProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function testShouldNotSetProtectedPropertyNewInternal()
    {
        $tester = new PropertyTester();
        $tester->setPropertyNew(TestScopeExtending::class, 'protectedProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function testShouldSetProtectedPropertyObjPhp()
    {
        $object = new TestScopePhpMagic();
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyObj($object, 'protectedProperty', 'test'));
        $this->assertEquals(1, $object->setCount);
    }

    public function testShouldSetProtectedPropertyNewPhp()
    {
        $tester = new PropertyTester();
        $obj = $tester->setPropertyNew(TestScopePhpMagic::class, 'protectedProperty', 'test');
        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    public function testShouldSetProtectedPropertyObjInternal()
    {
        $object = new TestScopeExtendingMagic();
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyObj($object, 'protectedProperty', 'test'));
        $this->assertEquals(1, $object->setCount);
    }

    public function testShouldSetProtectedPropertyNewInternal()
    {
        $tester = new PropertyTester();
        $obj = $tester->setPropertyNew(TestScopeExtendingMagic::class, 'protectedProperty', 'test');
        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    public function testShouldSetProtectedPropertyViaThis()
    {
        $obj = new TestScopeExtending();
        $obj->setProperty('protectedProperty', 'test');
        $this->assertEquals('test', $obj->getProtectedProperty());
    }

    public function testShouldNotGetObjectVarsProtectedPropertyObjPhp()
    {
        $tester = new PropertyTester();
        $object = new TestScopePhp();
        $objectVars = $tester->getObjVars($object);
        $this->assertArrayNotHasKey('protectedProperty', $objectVars);
    }

    public function testShouldNotGetObjectVarsProtectedPropertyNewPhp()
    {
        $tester = new PropertyTester();
        $objectVars = $tester->getNewVars(TestScopePhp::class);
        $this->assertArrayNotHasKey('protectedProperty', $objectVars);
    }

    public function testShouldNotGetObjectVarsProtectedPropertyObjInternal()
    {
        $tester = new PropertyTester();
        $object = new TestScopeExtending();
        $objectVars = $tester->getObjVars($object);
        $this->assertArrayNotHasKey('protectedProperty', $objectVars);
    }

    public function testShouldNotGetObjectVarsProtectedPropertyNewInternal()
    {
        $tester = new PropertyTester();
        $objectVars = $tester->getNewVars(TestScopeExtending::class);
        $this->assertArrayNotHasKey('protectedProperty', $objectVars);
    }
}
