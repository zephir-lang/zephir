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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Config;
use Zephir\Os;

final class ConfigTest extends TestCase
{
    /**
     * Common directory.
     *
     * @var string
     */
    private string $pwd;

    private Config $config;

    protected function setUp(): void
    {
        /* Store the current directory before to be change */
        $this->pwd = getcwd();
        $this->config = new Config();
    }

    protected function tearDown(): void
    {
        if (getcwd() != $this->pwd) {
            chdir($this->pwd);
        }

        $this->cleanTmpConfigFile();
    }

    /**
     * Clean config.json file into tmp dir.
     */
    private function cleanTmpConfigFile(): void
    {
        /* clean config.json into tmp dir */
        $tmpConfigFile = sys_get_temp_dir().\DIRECTORY_SEPARATOR.'config.json';

        if (file_exists($tmpConfigFile)) {
            unlink($tmpConfigFile);
        }
    }

    /**
     * Returns Stubs banner for test suite.
     *
     * @return string
     */
    private function stubsBanner(): string
    {
        return <<<DOC
            /**
             * This file is part of the Zephir.
             *
             * (c) Phalcon Team <team@zephir-lang.com>
             *
             * For the full copyright and license information, please view
             * the LICENSE file that was distributed with this source code.
             */
            DOC;
    }

    /**
     * Test when we have a bad config.json file.
     */
    public function testConstructWithBadConfigFile(): void
    {
        $this->expectException(\Zephir\Exception::class);
        $this->expectExceptionMessage('The config.json file is invalid: Syntax error, malformed JSON');

        chdir(\constant('ZEPHIRPATH').'/tests/fixtures/badconfig');
        new Config();
    }

    /**
     * Test data provider.
     *
     * [
     *     'test name' => [
     *          [$key, $value, $namespace], $expected,
     *      ],
     *      ...
     * ]
     *
     * @return array
     */
    public function setConfigProvider(): array
    {
        return [
            'set with namespace' => [
                ['unused-variable', false, 'warnings'], false,
            ],
            'set without namespace' => [
                ['config', true, null], true,
            ],
            'get with namespace' => [
                ['unused-variable', true, 'warnings'], true,
            ],
            'get without namespace' => [
                ['verbose', false, null], false,
            ],
            'directive don`t have duplicates with namespace' => [
                ['ext.some_key', 'some_value', 'test'], 'some_value',
            ],
            'directive don`t have duplicates without namespace' => [
                ['test.my_setting_1', 'test', null], 'test',
            ],
        ];
    }

    /**
     * @dataProvider setConfigProvider
     *
     * @param array $test
     * @param mixed $expected
     */
    public function testShouldSetConfigParams(array $test, $expected): void
    {
        [$key, $value, $namespace] = $test;
        $this->config->set($key, $value, $namespace);

        $actual = $this->config->get($key, $namespace);

        $this->assertSame($expected, $actual);
    }

    public function defaultConfigProvider(): array
    {
        return [
            // 'test suite name' => [$namespace, $key, $expected,]
            'not existing param' => ['test.my_setting_1', 'test', null],
            'stubs path' => ['stubs', 'path', 'ide/%version%/%namespace%/'],
            'stubs run' => ['stubs', 'stubs-run-after-generate', false],
            'stubs banner' => ['stubs', 'banner', $this->stubsBanner()],
            'api path' => ['api', 'path', 'doc/%version%'],
            'warnings unused-variable' => ['warnings', 'unused-variable', true],
            'optimizations static-type-inference' => ['optimizations', 'static-type-inference', true],
            'extra indent' => ['extra', 'indent', 'spaces'],
            'namespace' => [null, 'namespace', 'stub'],
            'name' => [null, 'name', 'Test Extension'],
            'author' => [null, 'author', 'Phalcon Team and contributors'],
            'globals test_setting_1' => ['globals', 'test_setting_1', ['type' => 'bool', 'default' => true]],
            'globals db.my_setting_1' => ['globals', 'db.my_setting_1', ['type' => 'bool', 'default' => false]],
        ];
    }

    /**
     * @dataProvider defaultConfigProvider
     *
     * @param mixed  $namespace
     * @param string $key
     * @param mixed  $expected
     */
    public function testShouldGetDefaultConfigParams($namespace, string $key, $expected): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $actual = $this->config->get($key, $namespace);

        $this->assertSame($expected, $actual);
    }

    public function offsetConfigProvider(): array
    {
        return [
            // 'test suite name' => [$key, $expected,]
            'globals test_setting_1 with namespace' => [
                ['globals' => 'test_setting_1'], ['type' => 'bool', 'default' => true],
            ],
            'globals test_setting_1 without namespace' => [
                ['globals' => 'test_setting_1'], ['type' => 'bool', 'default' => true],
            ],
            'info header without namespace' => [
                ['requires' => 'extensions'], ['PDO', 'SPL', 'standard', 'hash', 'json'],
            ],
        ];
    }

    /**
     * @dataProvider offsetConfigProvider
     *
     * @param array $key
     * @param array $expected
     */
    public function testShouldGetWithOffsetConfigParams(array $key, array $expected): void
    {
        $actual = $this->config->offsetGet($key);

        $this->assertSame($expected, $actual);
    }

    public function testShouldUnsetConfigParams(): void
    {
        $initials = $this->config->get('test_setting_1', 'globals');

        $this->assertTrue($this->config->offsetExists('globals'));
        $this->assertSame(
            [
                'type' => 'bool',
                'default' => true,
            ],
            $initials
        );

        $this->config->offsetUnset('globals');
        $actual = $this->config->get('test_setting_1', 'globals');

        $this->assertFalse($this->config->offsetExists('globals'));
        $this->assertNull($actual);
    }

    public function testShouldGetBannerFromConfig(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $this->assertSame($this->stubsBanner(), $this->config->get('banner', 'stubs'));

        unset($this->config['stubs']);
        $this->assertNull($this->config->get('banner', 'stubs'));
    }

    public function testShouldSaveConfig(): void
    {
        chdir(sys_get_temp_dir());

        $this->config->set('name', 'foo');
        $this->config->dumpToFile();

        $configJson = json_decode(file_get_contents('config.json'), true);

        $this->assertIsArray($configJson);
        $this->assertSame($configJson['name'], 'foo');

        $this->cleanTmpConfigFile();
    }
}
