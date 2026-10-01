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
 * A project command run outside a Zephir project, typically from the
 * directory `zephir init` was run in, used to fail with the bare
 * "Extension namespace cannot be loaded", and `compile` / `stubs` with an
 * uncaught PHP fatal error. It has to say which directory lacks config.json
 * and point at the project below it.
 *
 * Also pins `zephir api`, which did nothing at all since the CLI rewrite.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2431
 */
final class Issue2431Test extends TestCase
{
    use RunsZephirCommands;

    private string $cwd;

    /**
     * @return iterable<string, array{0: string}>
     */
    public static function projectCommandProvider(): iterable
    {
        yield 'build'    => ['build'];
        yield 'generate' => ['generate'];
        yield 'compile'  => ['compile'];
        yield 'install'  => ['install'];
        yield 'stubs'    => ['stubs'];
        yield 'api'      => ['api'];
    }

    /**
     * @dataProvider projectCommandProvider
     */
    public function testProjectCommandOutsideProjectNamesMissingConfig(string $command): void
    {
        $result = $this->runZephir($command . ' --no-ansi', $this->cwd);

        $this->assertSame(1, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertStringContainsString(
            sprintf('No config.json found in "%s"', $this->cwd),
            $result['stderr']
        );
        $this->assertStringNotContainsString('Fatal error', $result['stderr']);
        $this->assertStringNotContainsString('Extension namespace cannot be loaded', $result['stderr']);
    }

    /**
     * @dataProvider projectCommandProvider
     */
    public function testProjectCommandOutsideProjectCreatesNothing(string $command): void
    {
        $this->runZephir($command . ' --no-ansi', $this->cwd);

        $this->assertSame([], array_values(array_diff(scandir($this->cwd), ['.', '..'])));
    }

    public function testBuildFromInitDirectorySuggestsTheProject(): void
    {
        $this->writeConfig('utils', ['namespace' => 'utils']);

        $result = $this->runZephir('build --no-ansi', $this->cwd);

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringContainsString('Found a project in "utils": run "cd utils" first.', $result['stderr']);
    }

    public function testSubdirectoryWithoutNamespaceIsNotSuggested(): void
    {
        $this->writeConfig('other', ['name' => 'other']);

        $result = $this->runZephir('build --no-ansi', $this->cwd);

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringNotContainsString('other', $result['stderr']);
    }

    public function testConfigWithoutNamespaceSaysSo(): void
    {
        $this->writeConfig('.', ['name' => 'utils']);

        $result = $this->runZephir('generate --no-ansi', $this->cwd);

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringContainsString('config.json has no "namespace" setting', $result['stderr']);
    }

    public function testCleanOutsideProjectIsSilent(): void
    {
        $result = $this->runZephir('clean --no-ansi', $this->cwd);

        $this->assertSame(0, $result['exitCode']);
        $this->assertSame('', $result['stderr']);
    }

    public function testApiGeneratesTheDocumentation(): void
    {
        $this->writeProject();

        $result = $this->runZephir(
            'api --no-ansi --url=https://example.com/api -o ' . escapeshellarg($this->cwd . '/out'),
            $this->cwd
        );

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertFileExists($this->cwd . '/out/index.html');
        $this->assertStringContainsString(
            '<loc>https://example.com/api/index.html</loc>',
            (string) file_get_contents($this->cwd . '/out/sitemap.xml')
        );
        $this->assertStringContainsString('say', (string) file_get_contents($this->cwd . '/out/class/Stub/Greeting.html'));
        $this->assertStringContainsString('hello', (string) file_get_contents($this->cwd . '/out/source/Stub/Greeting.html'));
        $this->assertFileExists($this->cwd . '/out/namespace/Stub/Sub.html');
    }

    public function testApiRejectsAnOptionWithoutValue(): void
    {
        $this->writeProject();

        $result = $this->runZephir('api --no-ansi --path', $this->cwd);

        $this->assertSame(1, $result['exitCode']);
        $this->assertStringContainsString('The "--path" option requires a value.', $result['stderr']);
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->cwd = sys_get_temp_dir() . '/zephir-issue2431-' . bin2hex(random_bytes(6));
        mkdir($this->cwd, 0777, true);
        $this->cleanupPath($this->cwd);
        // The command reports getcwd(), which is canonical: macOS resolves the
        // /var -> /private/var symlink and Windows uses backslashes.
        $this->cwd = realpath($this->cwd);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    private function writeConfig(string $dir, array $config): void
    {
        if (!is_dir($this->cwd . '/' . $dir)) {
            mkdir($this->cwd . '/' . $dir, 0777, true);
        }

        file_put_contents($this->cwd . '/' . $dir . '/config.json', json_encode($config));
    }

    private function writeProject(): void
    {
        $this->writeConfig('.', ['namespace' => 'stub', 'name' => 'stub']);
        mkdir($this->cwd . '/stub');
        file_put_contents($this->cwd . '/stub/greeting.zep', <<<'ZEP'
namespace Stub;

class Greeting
{
    public static function say()
    {
        echo "hello world!";
    }
}
ZEP);
        // A nested namespace, so the namespace tree is walked below the root.
        mkdir($this->cwd . '/stub/sub');
        file_put_contents($this->cwd . '/stub/sub/nested.zep', <<<'ZEP'
namespace Stub\Sub;

class Nested
{
}
ZEP);
    }
}
