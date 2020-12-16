<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Stub\Issue1404;

/**
 * Extension\Optimizers\IsPhpVersionTest.
 *
 * @issue https://github.com/phalcon/zephir/issues/1404
 */
class IsPhpVersionTest extends TestCase
{
    const PHP_RELEASES_LIMIT = 17;
    const PHP_MINOR_LIMIT = 3;

    protected $test;

    protected function setUp(): void
    {
        $this->test = new Issue1404();
    }

    protected function tearDown(): void
    {
        $this->test = null;
    }

    public function phpVersionProvider()
    {
        return [
            [PHP_VERSION, true],
            [PHP_MAJOR_VERSION,   true],
            [PHP_MAJOR_VERSION.'.'.PHP_MINOR_VERSION, true],
            [PHP_MAJOR_VERSION.'.'.PHP_MINOR_VERSION.'.'.PHP_RELEASE_VERSION, true],
            [PHP_MAJOR_VERSION.'.'.PHP_MINOR_VERSION.'.'.(PHP_RELEASE_VERSION + 1), false],
            [PHP_MAJOR_VERSION.'.'.(PHP_MINOR_VERSION + 1), false],
            [(PHP_MAJOR_VERSION + 1).'.'.PHP_MINOR_VERSION, false],
            [(PHP_MAJOR_VERSION + 1).'.'.(PHP_MINOR_VERSION + 1), false],
            [PHP_MAJOR_VERSION.'.1.90009', false],
            [0.1, false],
            [0, false],
        ];
    }

    /**
     * @dataProvider phpVersionProvider
     *
     * @param mixed $version
     * @param mixed $expected
     */
    public function testOptimizer($version, $expected)
    {
        $isPhpVersion = (bool) $this->isPhpVersion($version);

        $this->assertEquals($expected, $isPhpVersion);
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage Could not parse PHP version
     */
    public function testOptimizerExceptionLLU()
    {
        $this->isPhpVersion(92233720368547758079);
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage Could not parse PHP version
     */
    public function testOptimizerExceptionNegativeNumber()
    {
        $this->isPhpVersion(-7);
    }

    /**
     * @expectedException \Exception
     * @expectedExceptionMessage Could not parse PHP version
     */
    public function testOptimizerExceptionNull()
    {
        $this->isPhpVersion(null);
    }

    /* -------------- Zephir Tests -------------- */
    public function testZephirUsingInteger70000()
    {
        $actual = $this->test->testIsPhpVersionUsingInteger70000();
        $expected = $this->isPhpVersion(7);

        $this->assertEquals($expected, $actual);
    }

    public function testZephirUsingDouble70100()
    {
        $actual = $this->test->testIsPhpVersionUsingDouble70100();
        $expected = $this->isPhpVersion(7.1);

        $this->assertEquals($expected, $actual);
    }

    public function testZephirUsingLong70000()
    {
        $actual = $this->test->testIsPhpVersionUsingLong70000();
        $expected = $this->isPhpVersion(2147483647);

        $this->assertEquals($expected, $actual);
    }

    public function testZephirUsingZeroFalse()
    {
        $this->assertFalse($this->test->testIsPhpVersionUsingZero());
    }

    public function testZephirUsingString70000()
    {
        $actual = $this->test->testIsPhpVersionUsingString70000();
        $expected = $this->isPhpVersion('7');

        $this->assertEquals($expected, $actual);
    }

    public function releaseVersionProvider()
    {
        $versionProvider = [];

        for ($i = 1, $id = 70101; $i <= self::PHP_RELEASES_LIMIT; $i++, $id++) {
            $versionProvider[] = ['testIsPhpVersionUsing'.$id, "7.1.$i"];
        }

        return $versionProvider;
    }

    /**
     * @dataProvider releaseVersionProvider
     *
     * @param mixed $testName
     * @param mixed $version
     */
    public function testIsPhpVersionVersionUsing701XX($testName, $version)
    {
        $actual = $this->test->$testName();
        $expected = $this->isPhpVersion($version);

        $this->assertEquals($expected, $actual);
    }

    public function minorVersionProvider()
    {
        $versionProvider = [];

        for ($i = 1, $id = 70100; $i <= self::PHP_MINOR_LIMIT; $i++, $id += 100) {
            $versionProvider[] = ['testIsPhpVersionUsing'.$id, "7.$i"];
        }

        return $versionProvider;
    }

    /**
     * @dataProvider minorVersionProvider
     *
     * @param mixed $testName
     * @param mixed $version
     */
    public function testIsPhpVersionVersionUsing70X00($testName, $version)
    {
        $actual = $this->test->$testName();
        $expected = $this->isPhpVersion($version);

        $this->assertEquals($expected, $actual);
    }

    public function testZephirUsingString50000()
    {
        $actual = $this->test->testIsPhpVersionUsing50000();
        $expected = $this->isPhpVersion('5');

        $this->assertEquals($expected, $actual);
    }

    public function testZephirUsingString50500()
    {
        $actual = $this->test->testIsPhpVersionUsing50500();
        $expected = $this->isPhpVersion('5.5');

        $this->assertEquals($expected, $actual);
    }

    public function testZephirUsingString50600()
    {
        $actual = $this->test->testIsPhpVersionUsing50600();
        $expected = $this->isPhpVersion('5.6');

        $this->assertEquals($expected, $actual);
    }

    /**
     * {@inheritdoc}
     *
     * @param \Throwable $error
     *
     * @throws \Throwable
     */
    protected function onNotSuccessfulTest(\Throwable $error): void
    {
        $phpVer = sprintf(
            'PHP_VERSION_ID:%d(%d.%d.%d)',
            \PHP_VERSION_ID,
            PHP_MAJOR_VERSION,
            PHP_MINOR_VERSION,
            PHP_RELEASE_VERSION
        );

        fwrite(STDOUT, "{$phpVer}\nError: {$error}");

        throw $error;
    }

    /**
     * Optimizer: isPhpVersion
     * Compare user entered PHP version with Environment and return Boolean
     * Check only MAJOR or MAJOR + MINOR or MAJOR + MINOR + RELEASE.
     *
     * @param float|int|string $version - PHP version in any format: 7, 7.1, "7.1.1"
     *
     * @return bool
     */
    private function isPhpVersion($version)
    {
        preg_match('/^(?<major>\d+)(?:\.(?<minor>!?\d+))?(?:\.(?<patch>!?\d+))?(?:[^Ee0-9.]+.*)?$/', $version, $matches);
        if (!\count($matches)) {
            throw new \Exception('Could not parse PHP version');
        }

        $majorVersion = 0;
        $minorVersion = 0;
        $releaseVersion = 0;

        $phpMinorVersion = 0;
        $phpReleaseVersion = 0;

        $majorVersion = $matches['major'] * 10000;
        $phpMajorVersion = PHP_MAJOR_VERSION * 10000;

        if (isset($matches['minor'])) {
            $minorVersion = $matches['minor'] * 100;
            $phpMinorVersion = PHP_MINOR_VERSION * 100;
        }

        if (isset($matches['patch'])) {
            $releaseVersion = $matches['patch'];
            $phpReleaseVersion = PHP_RELEASE_VERSION;
        }

        $versionId = (int) ($majorVersion + $minorVersion + $releaseVersion);
        $phpVersionId = $phpMajorVersion + $phpMinorVersion + $phpReleaseVersion;

        return (bool) ($phpVersionId == $versionId ? 1 : 0);
    }
}
