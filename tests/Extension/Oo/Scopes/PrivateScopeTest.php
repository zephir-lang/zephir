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
use UserExample;

final class PrivateScopeTest extends TestCase
{
    public function testShouldCallPrivateMethod(): void
    {
        $this->assertSame('isPrivate', (new PrivateScopeTester())->run());
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyObjPhp(): void
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopePhp::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyObj(new TestScopePhp(), 'privateProperty', 'test');
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyNewPhp(): void
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopePhp::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyNew(TestScopePhp::class, 'privateProperty', 'test');
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyObjInternal(): void
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtending::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyObj(new TestScopeExtending(), 'privateProperty', 'test');
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyNewInternal(): void
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtending::$privateProperty'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyNew(TestScopeExtending::class, 'privateProperty', 'test');
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldSetPrivatePropertyObjPhp(): void
    {
        $object = new TestScopePhpMagic();
        $tester = new PrivateScopeTester();

        $actual = $tester->setPropertyObj($object, 'privateProperty', 'test');
        $this->assertEquals('test', $actual);
        $this->assertEquals(1, $object->setCount);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldSetPrivatePropertyNewPhp(): void
    {
        $tester = new PrivateScopeTester();
        $obj = $tester->setPropertyNew(TestScopePhpMagic::class, 'privateProperty', 'test');

        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldSetPrivatePropertyObjInternal(): void
    {
        $tester = new PrivateScopeTester();
        $object = new TestScopeExtendingMagic();

        $actual = $tester->setPropertyObj($object, 'privateProperty', 'test');
        $this->assertEquals('test', $actual);
        $this->assertEquals(1, $object->setCount);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldSetPrivatePropertyNewInternal(): void
    {
        $tester = new PrivateScopeTester();
        $obj = $tester->setPropertyNew(TestScopeExtendingMagic::class, 'privateProperty', 'test');

        $this->assertEquals('test', $obj->privateProperty);
        $this->assertEquals(1, $obj->setCount);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyViaThis(): void
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtending::$privateProperty'
        );

        $obj = new TestScopeExtending();
        $obj->setProperty('privateProperty', 'test');
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldSetPrivatePropertyViaThis(): void
    {
        $obj = new TestScopeExtending();
        $obj->setProperty('privateProperty2', 'test');

        $this->assertEquals('test', $obj->getPrivateProperty2());
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicObjInternal(): void
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtendingMagic::$privateProperty2'
        );

        $object = new TestScopeExtendingMagic();
        $tester = new PrivateScopeTester();

        $this->assertEquals('private', $object->getPrivateProperty2());
        $tester->setPropertyObj($object, 'privateProperty2', 'CHANGED');
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicNewInternal(): void
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopeExtendingMagic::$privateProperty2'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyNew(
            TestScopeExtendingMagic::class,
            'privateProperty2',
            'CHANGED'
        );
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicObjPhp(): void
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
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotSetPrivatePropertyExtendedMagicNewPhp(): void
    {
        $this->expectException(Error::class);
        $this->expectExceptionMessage(
            'Cannot access private property TestScopePhpMagicExtending::$privateProperty2'
        );

        $tester = new PrivateScopeTester();
        $tester->setPropertyNew(
            TestScopePhpMagicExtending::class,
            'privateProperty2',
            'CHANGED'
        );
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotGetObjectVarsPrivatePropertyObjPhp(): void
    {
        $tester = new PrivateScopeTester();
        $object = new TestScopePhp();

        $objectVars = $tester->getObjVars($object);
        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotGetObjectVarsPrivatePropertyNewPhp(): void
    {
        $tester = new PrivateScopeTester();
        $objectVars = $tester->getNewVars(TestScopePhp::class);

        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotGetObjectVarsPrivatePropertyObjInternal(): void
    {
        $tester = new PrivateScopeTester();
        $object = new TestScopeExtending();
        $objectVars = $tester->getObjVars($object);

        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/2057
     */
    public function testShouldNotGetObjectVarsPrivatePropertyNewInternal(): void
    {
        $tester = new PrivateScopeTester();
        $objectVars = $tester->getNewVars(TestScopeExtending::class);

        $this->assertArrayNotHasKey('privateProperty', $objectVars);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/1851
     */
    public function testShouldGetAndSetPrivatePropertyUsingParentGetterAndSetter(): void
    {
        $tester = new UserExample();
        $tester->setPrivateVariable('test');

        $this->assertEquals('test', $tester->getPrivateVariable());
    }
}
