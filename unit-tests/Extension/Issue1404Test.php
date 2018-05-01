<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Extension;

use \Test\Issue1404;

/**
 * Tests for Zephir function is_php_version(id)
 * 
 * @category BugFix
 * @package  Extension
 * @author   AlexNDRmac <AlexNDR@phalconphp.com>
 * @license  MIT http://zephir-lang.com/license.html
 * @link     https://github.com/phalcon/zephir/issues/1404
 */
class Issue1404Test extends \PHPUnit_Framework_TestCase
{
    const PHP_RELEASES_LIMIT = 17;
    const PHP_MINOR_LIMIT = 3;

    protected $test;

    public function setUp()
    {
        $this->test = new \Test\Issue1404();
    }

    public function tearDown()
    {
        $this->test = null;
    }

    public function phpVersionProvider()
    {
        return [
            [PHP_VERSION, true],
            [PHP_MAJOR_VERSION,   true],
            [PHP_MAJOR_VERSION . '.' . PHP_MINOR_VERSION, true],
            [PHP_MAJOR_VERSION . '.' . PHP_MINOR_VERSION . '.' . PHP_RELEASE_VERSION, true],
            [PHP_MAJOR_VERSION . '.' . PHP_MINOR_VERSION . '.' . (PHP_RELEASE_VERSION + 1), false],
            [PHP_MAJOR_VERSION . '.' . (PHP_MINOR_VERSION + 1), false],
            [(PHP_MAJOR_VERSION + 1) . '.' . PHP_MINOR_VERSION, false],
            [(PHP_MAJOR_VERSION + 1) . '.' . (PHP_MINOR_VERSION + 1), false],
            [PHP_MAJOR_VERSION . '.1.90009', false],
        ];
    }

    /**
     * @dataProvider phpVersionProvider
     */
    public function testOptimizer($version, $expected)
    {
        $is_php_version = (bool)$this->is_php_version($version);

        $this->assertEquals($expected, $is_php_version);
    }

    /**
     * Optimizer: is_php_version
     * Compare user entered PHP version with Environment and return Boolean
     * Check only MAJOR or MAJOR + MINOR or MAJOR + MINOR + RELEASE
     * 
     * @param  int|double|string $version - PHP version in any format: 7, 7.1, "7.1.1"
     * @return boolean
     */
    private function is_php_version($version)
    {
        preg_match('/^([0-9]+)\.?([0-9]+)?\.?([0-9]+)?$/', $version, $matches);

        $major_version   = 0;
        $minor_version   = 0;
        $release_version = 0;
        
        $php_minor_version   = 0;
        $php_release_version = 0;

        $major_version      = $matches[1] * 10000;
        $php_major_version  = PHP_MAJOR_VERSION * 10000;
        
        if (isset($matches[2])) {
            $minor_version       = $matches[2] * 100;
            $php_minor_version   = PHP_MINOR_VERSION * 100;
        }
        
        if (isset($matches[3])) {
            $release_version     = $matches[3];
            $php_release_version = PHP_RELEASE_VERSION;
        }

        $version_id     = $major_version + $minor_version + $release_version;
        $php_version_id = $php_major_version + $php_minor_version + $php_release_version;
        
        return ($php_version_id == $version_id ? 1 : 0);
    }

    /* -------------- Zephir Tests -------------- */
    public function testZephirUsingInteger70000()
    {
        $actual   = $this->test->testIsPhpVersionUsingInteger70000();
        $expected = $this->is_php_version(7);

        $this->assertEquals($expected, $actual);
    }

    public function testZephirUsingDouble70100()
    {
        $actual   = $this->test->testIsPhpVersionUsingDouble70100();
        $expected = $this->is_php_version(7.1);

        $this->assertEquals($expected, $actual);
    }

    public function testZephirUsingLong70000()
    {
        $actual   = $this->test->testIsPhpVersionUsingLong70000();
        $expected = $this->is_php_version(2147483647);

        $this->assertEquals($expected, $actual);
    }

    public function testZephirUsingString70000()
    {
        $actual   = $this->test->testIsPhpVersionUsingString70000();
        $expected = $this->is_php_version("7");

        $this->assertEquals($expected, $actual);
    }

    public function testIsPhpVersionVersionUsing701XX()
    {
        for ($i = 1, $id = 70101; $i < Issue1404Test::PHP_RELEASES_LIMIT; $i++, $id++) {
            $testName = 'testIsPhpVersionUsing'. $id;
            $actual   = $this->test->$testName();
            $expected = $this->is_php_version("7.1.$i");

            $this->assertEquals($expected, $actual);
        }
    }

    public function testIsPhpVersionVersionUsing70X00()
    {
        for ($i = 1, $id = 70100; $i < Issue1404Test::PHP_MINOR_LIMIT; $i++, $id += 100) {
            $testName = 'testIsPhpVersionUsing'. $id;
            $actual   = $this->test->$testName();
            $expected = $this->is_php_version("7.$i");

            $this->assertEquals($expected, $actual);
        }
    }
}
