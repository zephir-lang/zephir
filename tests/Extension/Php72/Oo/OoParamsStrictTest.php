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

namespace Extension\Php72\Oo;

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

    public function testShouldThrowTypeErrorForInt1()
    {
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->expectException(\Error::class);
        } else {
            $this->expectException(\TypeError::class);
        }

        $this->expectExceptionMessageRegExp(
            // PHP 7.3 : int
            // PHP 7.2 : integer
            '/Argument 1 passed to Stub\\\Oo\\\OoParams::setStrictAge\(\) '.
            'must be of the type int(eger)?, float given/'
        );

        $this->test->setStrictAge(17.0);
    }

    public function testShouldThrowInvalidArgumentExceptionForInt2()
    {
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->expectException(\Error::class);
        } else {
            $this->expectException(\TypeError::class);
        }

        $this->expectExceptionMessageRegExp(
            // PHP 7.3 : int
            // PHP 7.2 : integer
            '/Argument 1 passed to Stub\\\Oo\\\OoParams::setStrictAge\(\) '.
            'must be of the type int(eger)?, string given/'
        );

        $this->test->setStrictAge('17');
    }

    public function testShouldThrowInvalidArgumentExceptionForDoublePhp7()
    {
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->markTestIncomplete('Test requires PHP 7.x');
        }

        $this->expectException(\TypeError::class);
        $this->expectExceptionMessage(
            'Argument 1 passed to Stub\Oo\OoParams::setStrictAverage() must be of the type float, string given'
        );

        $this->test->setStrictAverage('17');
    }

    public function testShouldThrowInvalidArgumentExceptionForDoublePhp8()
    {
        if (version_compare(PHP_VERSION, '8.0.0', '<')) {
            $this->markTestIncomplete('Test requires PHP 8.x');
        }

        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage(
            'Parameter \'average\' must be of the type double'
        );

        $this->test->setStrictAverage('17');
    }

    public function testShouldThrowInvalidArgumentExceptionForString()
    {
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->expectException(\Error::class);
        } else {
            $this->expectException(\TypeError::class);
        }

        $this->expectExceptionMessageRegExp(
            // PHP 7.3 : int
            // PHP 7.2 : integer
            '/Argument 1 passed to Stub\\\Oo\\\OoParams::setStrictName\(\) '.
            'must be of the type string, int(eger)? given/'
        );

        $this->test->setStrictName(1234);
    }

    public function testShouldThrowInvalidArgumentExceptionForCallFromZephirLand()
    {
        $this->expectException(\InvalidArgumentException::class);
        $this->expectExceptionMessage("Parameter 'name' must be of the type string");

        $this->test->setStrictNameFromZephirLand();
    }
}
