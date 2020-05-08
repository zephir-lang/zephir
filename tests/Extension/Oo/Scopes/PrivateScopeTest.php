<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Oo;

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
    /** @test */
    public function shouldCallPrivateMethod()
    {
        $this->assertSame('isPrivate', (new PrivateScopeTester())->run());
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotSetPrivatePropertyObjPhp()
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopePhp::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyObj(new TestScopePhp(), 'privateProperty', 'test');
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotSetPrivatePropertyNewPhp()
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopePhp::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyNew(TestScopePhp::class, 'privateProperty', 'test');
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotSetPrivatePropertyObjInternal()
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtending::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyObj(new TestScopeExtending(), 'privateProperty', 'test');
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotSetPrivatePropertyNewInternal()
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtending::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyNew(TestScopeExtending::class, 'privateProperty', 'test');
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldSetPrivatePropertyObjPhp()
    {
        $object = new TestScopePhpMagic();
        $tester = new PrivateScopeTester();

        $actual = $tester->setPropertyObj($object, 'privateProperty', 'test');
        $this->assertEquals('test', $actual);
        $this->assertEquals(1, $object->setCount);
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldSetPrivatePropertyNewPhp()
    {
        $tester = new PrivateScopeTester();
        $obj = $tester->setPropertyNew(TestScopePhpMagic::class, 'privateProperty', 'test');

        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldSetPrivatePropertyObjInternal()
    {
        $tester = new PrivateScopeTester();
        $object = new TestScopeExtendingMagic();

        $actual = $tester->setPropertyObj($object, 'privateProperty', 'test');
        $this->assertEquals('test', $actual);
        $this->assertEquals(1, $object->setCount);
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldSetPrivatePropertyNewInternal()
    {
        $tester = new PrivateScopeTester();
        $obj = $tester->setPropertyNew(TestScopeExtendingMagic::class, 'privateProperty', 'test');

        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotSetPrivatePropertyViaThis()
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtending::$privateProperty'
        );

        $obj = new TestScopeExtending();
        $obj->setProperty('privateProperty', 'test');
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldSetPrivatePropertyViaThis()
    {
        $obj = new TestScopeExtending();
        $obj->setProperty('privateProperty2', 'test');

        $this->assertEquals('test', $obj->getPrivateProperty2());
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotSetPrivatePropertyExtendedMagicObjInternal()
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
        // there  is a magic __set method present.
        //
        // Actually we DO NOT change property here (fixed).  Only PHP 7.4
        // throws a Fatal Error.  All previous versions just out a Notice and
        // continue execution.
        if (\PHP_VERSION_ID < 70400) {
            $this->assertEquals('private', $object->getPrivateProperty2());
        }
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotSetPrivatePropertyExtendedMagicNewInternal()
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
        // there  is a magic __set method present.
        //
        // Actually we DO NOT change property here (fixed).  Only PHP 7.4
        // throws a Fatal Error.  All previous versions just out a Notice and
        // continue execution.
        if (\PHP_VERSION_ID < 70400) {
            $this->assertEquals('private', $object->getPrivateProperty2());
        }
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotSetPrivatePropertyExtendedMagicObjPhp()
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
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotSetPrivatePropertyExtendedMagicNewPhp()
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
        // there  is a magic __set method present.
        //
        // Actually we DO NOT change property here (fixed).  Only PHP 7.4
        // throws a Fatal Error.  All previous versions just out a Notice and
        // continue execution.
        if (\PHP_VERSION_ID < 70400) {
            $this->assertEquals('private2', $object->getPrivateProperty2());
        }
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotGetObjectVarsPrivatePropertyObjPhp()
    {
        $tester = new PrivateScopeTester();
        $object = new TestScopePhp();

        $objectVars = $tester->getObjVars($object);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotGetObjectVarsPrivatePropertyNewPhp()
    {
        $tester = new PrivateScopeTester();
        $objectVars = $tester->getNewVars(TestScopePhp::class);

        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotGetObjectVarsPrivatePropertyObjInternal()
    {
        $tester = new PrivateScopeTester();
        $object = new TestScopeExtending();
        $objectVars = $tester->getObjVars($object);

        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @test
     *
     * @see https://github.com/phalcon/zephir/issues/2057
     */
    public function shouldNotGetObjectVarsPrivatePropertyNewInternal()
    {
        $tester = new PrivateScopeTester();
        $objectVars = $tester->getNewVars(TestScopeExtending::class);

        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }
}
