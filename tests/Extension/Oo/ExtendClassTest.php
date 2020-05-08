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
    public function testPDOExtending()
    {
        if (!\extension_loaded('pdo')) {
            $this->markTestSkipped('The PDO extension is not loaded');
        }

        $this->assertSame(PDO::getAvailableDrivers(), ExtendPdoClass::getAvailableDrivers());
        $this->assertSame(PDO::PARAM_STR, ExtendPdoClass::PARAM_STR);
    }

    public function testPDOStatementExtending()
    {
        $pdo = new ExtendPdoClass('sqlite::memory:', '', '', [PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION]);
        $stmt = $pdo->prepare('SELECT CURRENT_TIME');

        $this->assertInstanceof('Test\\PdoStatement', $stmt);
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1686
     */
    public function shouldExtendMiddlewareInterface()
    {
        if (!\extension_loaded('psr')) {
            $this->markTestSkipped('The psr extension is not loaded');
        }

        $this->assertTrue(
            is_subclass_of(MessageInterfaceEx::class, 'Psr\Http\Message\MessageInterface')
        );
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1392
     */
    public function shouldCorrectWorkWithLateStaticBinding()
    {
        $this->assertSame('Test\Oo\ConcreteStatic', ConcreteStatic::getCalledClass());
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1372
     */
    public function shouldCallParentMethodFromStaticByUsingSelf()
    {
        $this->assertSame('ConcreteStatic:parentFunction', ConcreteStatic::parentFunction());
        $this->assertSame('ConcreteStatic:parentFunction', ConcreteStatic::childFunction());
    }
}
