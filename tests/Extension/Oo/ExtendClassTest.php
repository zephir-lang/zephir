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

use PDO;
use PHPUnit\Framework\TestCase;
use Stub\Integration\Psr\Http\Message\MessageInterfaceEx;
use Stub\Oo\ConcreteStatic;
use Stub\Oo\ExtendPdoClass;

class ExtendClassTest extends TestCase
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
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->markTestSkipped('TypeError : PHPUnit\Framework\TestCase::__construct(): Argument #2 ($data) must be of type array, string given');
        }

        $pdo = new ExtendPdoClass('sqlite::memory:', '', '', [PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION]);
        $stmt = $pdo->prepare('SELECT CURRENT_TIME');

        $this->assertInstanceof('Stub\\PdoStatement', $stmt);
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1686
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
     * @issue https://github.com/phalcon/zephir/issues/1392
     */
    public function testShouldCorrectWorkWithLateStaticBinding(): void
    {
        $this->assertSame('Stub\Oo\ConcreteStatic', ConcreteStatic::getCalledClass());
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1372
     */
    public function testShouldCallParentMethodFromStaticByUsingSelf(): void
    {
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->markTestSkipped('Should be fixed static call before run this test (Invalid callback , no array or string given)');
        }

        $this->assertSame('ConcreteStatic:parentFunction', ConcreteStatic::parentFunction());
        $this->assertSame('ConcreteStatic:parentFunction', ConcreteStatic::childFunction());
    }
}
