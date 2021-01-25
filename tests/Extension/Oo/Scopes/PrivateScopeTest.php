<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Oo\Scopes;

use Error;
use PHPUnit\Framework\Error\Notice;
use PHPUnit\Framework\TestCase;
use Stub\Oo\Scopes\PrivateScopeTester;
use TestScopeExtending;
use TestScopeExtendingMagic;
use TestScopePhp;
use TestScopePhpMagic;
use TestScopePhpMagicExtending;

class PrivateScopeTest extends TestCase
{
    public function testShouldCallPrivateMethod()
    {
        $this->assertSame('isPrivate', (new PrivateScopeTester())->run());
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyObjPhp()
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopePhp::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyObj(new TestScopePhp(), 'privateProperty', 'test');
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyNewPhp()
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopePhp::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyNew(TestScopePhp::class, 'privateProperty', 'test');
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyObjInternal()
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtending::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyObj(new TestScopeExtending(), 'privateProperty', 'test');
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyNewInternal()
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtending::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyNew(TestScopeExtending::class, 'privateProperty', 'test');
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldSetPrivatePropertyObjPhp()
    {
        $object = new TestScopePhpMagic();
        $tester = new PrivateScopeTester();

        $actual = $tester->setPropertyObj($object, 'privateProperty', 'test');
        $this->assertEquals('test', $actual);
        $this->assertEquals(1, $object->setCount);
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldSetPrivatePropertyNewPhp()
    {
        $tester = new PrivateScopeTester();
        $obj = $tester->setPropertyNew(TestScopePhpMagic::class, 'privateProperty', 'test');

        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldSetPrivatePropertyObjInternal()
    {
        $tester = new PrivateScopeTester();
        $object = new TestScopeExtendingMagic();

        $actual = $tester->setPropertyObj($object, 'privateProperty', 'test');
        $this->assertEquals('test', $actual);
        $this->assertEquals(1, $object->setCount);
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldSetPrivatePropertyNewInternal()
    {
        $tester = new PrivateScopeTester();
        $obj = $tester->setPropertyNew(TestScopeExtendingMagic::class, 'privateProperty', 'test');

        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyViaThis()
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtending::$privateProperty'
        );

        $obj = new TestScopeExtending();
        $obj->setProperty('privateProperty', 'test');
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldSetPrivatePropertyViaThis()
    {
        $obj = new TestScopeExtending();
        $obj->setProperty('privateProperty2', 'test');

        $this->assertEquals('test', $obj->getPrivateProperty2());
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicObjInternal()
    {
        if (\PHP_VERSION_ID >= 70400) {
            $this->expectException(Error::class);
            $this->expectExceptionMessage(
                'Cannot access private property TestScopeExtendingMagic::$privateProperty2'
            );
        }

        $object = new TestScopeExtendingMagic();
        $tester = new PrivateScopeTester();

        $this->assertEquals('private', $object->getPrivateProperty2());
        $tester->setPropertyObj($object, 'privateProperty2', 'CHANGED');

        // This related the way PHP < 7.4 handles object's properties when
        // there is a magic __set method present.
        //
        // Actually we DO NOT change property here (fixed).  However, only
        // PHP 7.4 throws a fatal error.  All previous versions just out a
        // notice and continue to execution.
        if (\PHP_VERSION_ID < 70400) {
            $this->assertEquals('private', $object->getPrivateProperty2());
        }
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicNewInternal()
    {
        if (\PHP_VERSION_ID >= 70400) {
            $this->expectException(Error::class);
            $this->expectExceptionMessage(
                'Cannot access private property TestScopeExtendingMagic::$privateProperty2'
            );
        }

        $tester = new PrivateScopeTester();
        $object = $tester->setPropertyNew(
            TestScopeExtendingMagic::class,
            'privateProperty2',
            'CHANGED'
        );

        // This related the way PHP < 7.4 handles object's properties when
        // there is a magic __set method present.
        //
        // Actually we DO NOT change property here (fixed).  However, only
        // PHP 7.4 throws a fatal error.  All previous versions just out a
        // notice and continue to execution.
        if (\PHP_VERSION_ID < 70400) {
            $this->assertEquals('private', $object->getPrivateProperty2());
        }
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicObjPhp()
    {
        if (\PHP_VERSION_ID < 70400) {
            $this->expectException(Notice::class);
            $this->expectExceptionMessage(
                'Undefined property: TestScopePhpMagicExtending::$privateProperty2'
            );
        } else {
            $this->expectException(Error::class);
            $this->expectExceptionMessage(
                'Cannot access private property TestScopePhpMagicExtending::$privateProperty2'
            );
        }

        $obj = new TestScopePhpMagicExtending();

        $tester = new PrivateScopeTester();
        $tester->setPropertyObj($obj, 'privateProperty2', 'test');
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicNewPhp()
    {
        if (\PHP_VERSION_ID >= 70400) {
            $this->expectException(Error::class);
            $this->expectExceptionMessage(
                'Cannot access private property TestScopePhpMagicExtending::$privateProperty2'
            );
        }

        $tester = new PrivateScopeTester();
        $object = $tester->setPropertyNew(
            TestScopePhpMagicExtending::class,
            'privateProperty2',
            'CHANGED'
        );

        // This related the way PHP < 7.4 handles object's properties when
        // there is a magic __set method present.
        //
        // Actually we DO NOT change property here (fixed).  However, only
        // PHP 7.4 throws a fatal error.  All previous versions just out a
        // notice and continue to execution.
        if (\PHP_VERSION_ID < 70400) {
            $this->assertEquals('private2', $object->getPrivateProperty2());
        }
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotGetObjectVarsPrivatePropertyObjPhp()
    {
        $tester = new PrivateScopeTester();
        $object = new TestScopePhp();

        $objectVars = $tester->getObjVars($object);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotGetObjectVarsPrivatePropertyNewPhp()
    {
        $tester = new PrivateScopeTester();
        $objectVars = $tester->getNewVars(TestScopePhp::class);

        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotGetObjectVarsPrivatePropertyObjInternal()
    {
        $tester = new PrivateScopeTester();
        $object = new TestScopeExtending();
        $objectVars = $tester->getObjVars($object);

        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function testShouldNotGetObjectVarsPrivatePropertyNewInternal()
    {
        $tester = new PrivateScopeTester();
        $objectVars = $tester->getNewVars(TestScopeExtending::class);

        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @see https://github.com/phalcon/zephir/issues/1851
     */
    public function testShouldGetAndSetPrivatePropertyUsingParentGetterAndSetter()
    {
        $tester = new \UserExample();
        $tester->setPrivateVariable('test');

        $this->assertEquals('test', $tester->getPrivateVariable());
    }
}
