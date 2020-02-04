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
    public function shouldCallPrivateMethod()
    {
        $this->assertSame('isPrivate', (new PrivateScopeTester())->run());
    }

    public function shouldNotSetPrivatePropertyObjPhp()
    {
        $object = new TestScopePhp();
        $tester = new PropertyTester();
        $tester->setPropertyObj($object, 'privateProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function shouldNotSetPrivatePropertyNewPhp()
    {
        $tester = new PropertyTester();
        $tester->setPropertyNew(TestScopePhp::class, 'privateProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function shouldNotSetPrivatePropertyObjInternal()
    {
        $object = new TestScopeExtending();
        $tester = new PropertyTester();
        $tester->setPropertyObj($object, 'privateProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function shouldNotSetPrivatePropertyNewInternal()
    {
        $tester = new PropertyTester();
        $tester->setPropertyNew(TestScopeExtending::class, 'privateProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function shouldSetPrivatePropertyObjPhp()
    {
        $object = new TestScopePhpMagic();
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyObj($object, 'privateProperty', 'test'));
        $this->assertEquals(1, $object->setCount);
    }

    public function shouldSetPrivatePropertyNewPhp()
    {
        $tester = new PropertyTester();
        $obj = $tester->setPropertyNew(TestScopePhpMagic::class, 'privateProperty', 'test');
        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    public function shouldSetPrivatePropertyObjInternal()
    {
        $object = new TestScopeExtendingMagic();
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyObj($object, 'privateProperty', 'test'));
        $this->assertEquals(1, $object->setCount);
    }

    public function shouldSetPrivatePropertyNewInternal()
    {
        $tester = new PropertyTester();
        $obj = $tester->setPropertyNew(TestScopeExtendingMagic::class, 'privateProperty', 'test');
        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    public function shouldNotSetPrivatePropertyViaThis()
    {
        $obj = new TestScopeExtending();
        $obj->setProperty('privateProperty', 'test');
        $this->assertFalse(true, 'This should not be called');
    }

    public function shouldNotSetPrivatePropertyExtendedMagicObjInternal()
    {
        $obj = new TestScopeExtendingMagic();
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyObj($obj, 'privateProperty2', 'test'));
        $this->assertFalse(true, 'This should not be called');
    }

    public function shouldNotSetPrivatePropertyExtendedMagicNewInternal()
    {
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyNew(TestScopeExtendingMagic::class, 'privateProperty2', 'test'));
        $this->assertFalse(true, 'This should not be called');
    }

    public function shouldNotSetPrivatePropertyExtendedMagicObjPhp()
    {
        $obj = new TestScopePhpMagicExtending();
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyObj($obj, 'privateProperty2', 'test'));
        $this->assertFalse(true, 'This should not be called');
    }

    public function shouldNotSetPrivatePropertyExtendedMagicNewPhp()
    {
        $tester = new PropertyTester();
        $this->assertEquals('test', $tester->setPropertyNew(TestScopePhpMagicExtending::class, 'privateProperty2', 'test'));
        $this->assertFalse(true, 'This should not be called');
    }

    public function shouldNotGetObjectVarsPrivatePropertyObjPhp()
    {
        $tester = new PropertyTester();
        $object = new TestScopePhp();
        $objectVars = $tester->getObjVars($object);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    public function shouldNotGetObjectVarsPrivatePropertyNewPhp()
    {
        $tester = new PropertyTester();
        $objectVars = $tester->getNewVars(TestScopePhp::class);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    public function shouldNotGetObjectVarsPrivatePropertyObjInternal()
    {
        $tester = new PropertyTester();
        $object = new TestScopeExtending();
        $objectVars = $tester->getObjVars($object);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    public function shouldNotGetObjectVarsPrivatePropertyNewInternal()
    {
        $tester = new PropertyTester();
        $objectVars = $tester->getNewVars(TestScopeExtending::class);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }
}
