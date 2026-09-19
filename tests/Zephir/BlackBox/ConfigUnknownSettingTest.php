<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

/**
 * A config.json key Zephir does not read has no effect, and used to produce no
 * output of any kind. Someone reading the documentation added an `ini` section
 * that never existed, rebuilt, and had nothing at all to go on.
 *
 * The build still succeeds: projects are allowed to carry keys of their own.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2449
 */
final class ConfigUnknownSettingTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-config-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);

        $this->writeZep('settings.zep', <<<'ZEP'
namespace Stub;

class Settings
{
    public function run() -> int
    {
        return 1;
    }
}
ZEP);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testShouldReportAnUnknownSetting(): void
    {
        $this->writeConfig([
            'namespace' => 'stub',
            'name'      => 'stub',
            'ini'       => ['stub.some_setting' => ['type' => 'int', 'default' => 10]],
        ]);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringContainsString(
            'config.json setting "ini" is not recognized and will be ignored',
            $output
        );
    }

    public function testShouldSuggestTheSettingAMisspellingIsClosestTo(): void
    {
        $this->writeConfig([
            'namespace' => 'stub',
            'name'      => 'stub',
            'gobals'    => ['flag' => ['type' => 'bool', 'default' => true]],
        ]);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringContainsString('Did you mean "globals"?', $output);
    }

    public function testShouldSayNothingAboutAConfigItUnderstands(): void
    {
        $this->writeConfig([
            'namespace' => 'stub',
            'name'      => 'stub',
            'globals'   => ['flag' => ['type' => 'bool', 'default' => true]],
        ]);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertSame(0, $result['exitCode'], $output);
        $this->assertStringNotContainsString('is not recognized', $output);
    }

    private function writeConfig(array $config): void
    {
        file_put_contents($this->projectDir . '/config.json', json_encode($config));
    }

    private function writeZep(string $relativePath, string $content): void
    {
        file_put_contents($this->projectDir . '/stub/' . $relativePath, $content);
    }
}
