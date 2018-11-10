<?php declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension\Oo;

use Zephir\Support\TestCase;

class OoParamsStrictTest extends TestCase
{
    public function testSetStrictAgeSuccess()
    {
        $t = new \Test\Oo\OoParams();

        $this->assertSame($t->setStrictAge(17), 17);
    }

    public function testSetStrictAgeException1()
    {
        $t = new \Test\Oo\OoParams();

        if (version_compare(PHP_VERSION, '7.0.0', '<')) {
            $except = '\InvalidArgumentException';
        } elseif (version_compare(PHP_VERSION, '7.2.0', '>=')) {
            $except = '\TypeError';
        }

        if (isset($except)) {
            if (!method_exists('PHPUnit_Runner_Version', 'id') ||
                version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
                $this->setExpectedException($except);
            } else {
                $this->expectException($except);
            }
        }

        $t->setStrictAge(17.0);
    }

    public function testSetStrictAgeException2()
    {
        $t = new \Test\Oo\OoParams();

        if (version_compare(PHP_VERSION, '7.0.0', '<')) {
            $except = '\InvalidArgumentException';
        } elseif (version_compare(PHP_VERSION, '7.2.0', '>=')) {
            $except = '\TypeError';
        }

        if (isset($except)) {
            if (!method_exists('PHPUnit_Runner_Version', 'id') ||
                version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
                $this->setExpectedException($except);
            } else {
                $this->expectException($except);
            }
        }

        $t->setStrictAge('17');
    }

    public function testSetStrictAverageSuccess()
    {
        $t = new \Test\Oo\OoParams();

        $this->assertSame($t->setStrictAverage(17.1), 17.1);
    }

    public function testSetStrictAverageException1()
    {
        $t = new \Test\Oo\OoParams();

        // PHP 7 strictness coerces INT types to DOUBLE without complaint
        // This is the only exception to PHP's strictness rules
        if (version_compare(PHP_VERSION, '7.0.0', '<')) {
            if (!method_exists('PHPUnit_Runner_Version', 'id') ||
                version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
                $this->setExpectedException('\InvalidArgumentException');
            } else {
                $this->expectException('\InvalidArgumentException');
            }
        }

        $t->setStrictAverage(17);
    }

    public function testSetStrictAverageException2()
    {
        $t = new \Test\Oo\OoParams();

        if (version_compare(PHP_VERSION, '7.0.0', '<')) {
            $except = '\InvalidArgumentException';
        } elseif (version_compare(PHP_VERSION, '7.2.0', '>=')) {
            $except = '\TypeError';
        }

        if (isset($except)) {
            if (!method_exists('PHPUnit_Runner_Version', 'id') ||
                version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
                $this->setExpectedException($except);
            } else {
                $this->expectException($except);
            }
        }

        $t->setStrictAverage('17');
    }

    public function testSetStrictNameSuccess()
    {
        $t = new \Test\Oo\OoParams();
        $this->assertSame($t->setStrictName('peter'), 'peter');
    }

    public function testSetStrictNameException()
    {
        $t = new \Test\Oo\OoParams();

        if (version_compare(PHP_VERSION, '7.0.0', '<')) {
            $except = '\InvalidArgumentException';
        } elseif (version_compare(PHP_VERSION, '7.2.0', '>=')) {
            $except = '\TypeError';
        }

        if (isset($except)) {
            if (!method_exists('PHPUnit_Runner_Version', 'id') ||
                version_compare(\PHPUnit_Runner_Version::id(), '5.2.0', '<')) {
                $this->setExpectedException($except);
            } else {
                $this->expectException($except);
            }
        }

        $t->setStrictName(1234);
    }
}
