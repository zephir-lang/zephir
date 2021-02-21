<?php

declare(strict_types=1);

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
use Stub\Oo\OoParams;

class OoParamsStrictTest extends TestCase
{
    /** @var OoParams */
    private $test;

    protected function setUp(): void
    {
        $this->test = new OoParams();
    }

    public function testSetStrictAgeSuccess()
    {
        $this->assertSame($this->test->setStrictAge(17), 17);
    }

    public function testSetStrictAverageSuccess()
    {
        $this->assertSame($this->test->setStrictAverage(17.1), 17.1);
    }

    public function testSetStrictNameSuccess()
    {
        $this->assertSame($this->test->setStrictName('peter'), 'peter');
    }

    public function testShouldThrowTypeErrorForInt1()
    {
        $this->expectException(\Error::class);
        $this->expectExceptionMessageRegExp(
            '/Argument 1 passed to Stub\\\Oo\\\OoParams::setStrictAge\(\) '.
            'must be of the type int, float given/'
        );

        $this->test->setStrictAge(17.0);
    }

    public function testShouldThrowInvalidArgumentExceptionForInt2()
    {
        $this->expectException(\Error::class);
        $this->expectExceptionMessageRegExp(
            '/Argument 1 passed to Stub\\\Oo\\\OoParams::setStrictAge\(\) '.
            'must be of the type int, string given/'
        );

        $this->test->setStrictAge('17');
    }

    public function testShouldThrowInvalidArgumentExceptionForDouble()
    {
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->expectException(\InvalidArgumentException::class);
            $message = "Parameter 'average' must be of the type double";
        } else {
            $this->expectException(\Error::class);
            $message = 'Argument 1 passed to Stub\Oo\OoParams::setStrictAverage() must be of the type float, string given';
        }

        $this->expectExceptionMessage($message);

        $this->test->setStrictAverage('a');
    }

    public function testShouldThrowInvalidArgumentExceptionForString()
    {
        $this->expectException(\Error::class);
        $this->expectExceptionMessageRegExp(
            '/Argument 1 passed to Stub\\\Oo\\\OoParams::setStrictName\(\) '.
            'must be of the type string, int given/'
        );

        $this->test->setStrictName(1234);
    }

    public function testShouldThrowInvalidArgumentExceptionForCallFromZephirLand()
    {
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->markTestSkipped('Move types check into fcall, instead of PHP_METHOD');
        }

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'name' must be of the type string");

        $this->test->setStrictNameFromZephirLand();
    }
}
