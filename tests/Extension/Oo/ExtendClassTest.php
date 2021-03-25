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

namespace Extension\Oo;

use PDO;
use PHPUnit\Framework\TestCase;
use Stub\Integration\Psr\Http\Message\MessageInterfaceEx;
use Stub\Oo\ConcreteStatic;
use Stub\Oo\ExtendPdoClass;

final class ExtendClassTest extends TestCase
{
    public function testPDOExtending(): void
    {
        if (!\extension_loaded('pdo')) {
            $this->markTestSkipped('The PDO extension is not loaded');
        }

        $this->assertSame(PDO::getAvailableDrivers(), ExtendPdoClass::getAvailableDrivers());
        $this->assertSame(PDO::PARAM_STR, ExtendPdoClass::PARAM_STR);
    }

    public function testPDOStatementExtending(): void
    {
        $pdo = new ExtendPdoClass('sqlite::memory:', '', '', [PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION]);
        $stmt = $pdo->prepare('SELECT CURRENT_TIME');

        $this->assertInstanceof('Stub\\PdoStatement', $stmt);
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1686
     */
    public function testShouldExtendMiddlewareInterface(): void
    {
        if (!\extension_loaded('psr')) {
            $this->markTestSkipped('The psr extension is not loaded');
        }

        $this->assertTrue(
            is_subclass_of(MessageInterfaceEx::class, 'Psr\Http\Message\MessageInterface')
        );
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1392
     */
    public function testShouldCorrectWorkWithLateStaticBinding(): void
    {
        $this->assertSame(ConcreteStatic::class, ConcreteStatic::getCalledClass());
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1372
     */
    public function testShouldCallParentMethodFromStaticByUsingSelf(): void
    {
        $this->assertSame('ConcreteStatic:parentFunction', ConcreteStatic::parentFunction());
        $this->assertSame('ConcreteStatic:parentFunction', ConcreteStatic::childFunction());
    }

    public function testShouldCallParentMethodFromStaticByUsingParent(): void
    {
        $this->assertSame(ConcreteStatic::class, ConcreteStatic::callParentFunction());
    }

    public function testShouldCallStaticMethodWithSimpleValue(): void
    {
        $this->assertSame(1337, ConcreteStatic::simpleStaticIntReturn());
    }
}
