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
 * A closure body is compiled as a separate anonymous class, so it has to
 * inherit the name-resolution scope of the file it is written in: the
 * `use ... as` aliases and the namespace. Arrow closures dropped the aliases,
 * and closures in a sub-namespace resolved unqualified names against the
 * extension root namespace.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2686
 */
final class Issue2686Test extends TestCase
{
    use RunsZephirCommands;

    private const HELPER = <<<'ZEP'
        class Helper
        {
            const VALUE = 7;

            public static function make(var n)
            {
                return n;
            }
        }
        ZEP;

    private string $projectDir;

    public static function aliasInArrowProvider(): array
    {
        return [
            'class constant'       => ['return n => Aliased::VALUE + n;'],
            'static method call'   => ['return n => Aliased::make(n);'],
            'new'                  => ['return n => new Aliased();'],
            'instanceof'           => ['return n => n instanceof Aliased;'],
            'class name'           => ['return n => Aliased::class;'],
            'arrow inside closure' => ['return function () { return n => Aliased::VALUE + n; };'],
            'closure inside arrow' => ['return n => function () { return Aliased::VALUE; };'],
        ];
    }

    public static function subNamespaceProvider(): array
    {
        return [
            'closure'         => ['return function () { return Helper::VALUE; };'],
            'arrow'           => ['return n => Helper::VALUE + n;'],
            'nested closure'  => ['return function () { return function () { return Helper::VALUE; }; };'],
            'new in closure'  => ['return function () { return new Helper(); };'],
        ];
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue2686-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/probe/sub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'probe', 'name' => 'probe'])
        );
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    /**
     * @dataProvider aliasInArrowProvider
     */
    public function testArrowClosureResolvesAlias(string $body): void
    {
        file_put_contents($this->projectDir . '/probe/helper.zep', "namespace Probe;\n\n" . self::HELPER . "\n");
        $this->writeProbe('Probe', "use Probe\\Helper as Aliased;\n", $body);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertStringNotContainsString('nonexistent-class', $result['stdout'] . $result['stderr']);
        $this->assertStringNotContainsString('Aliased', $this->closureSources());
    }

    public function testUnknownAliasInArrowIsStillACompileError(): void
    {
        file_put_contents($this->projectDir . '/probe/helper.zep', "namespace Probe;\n\n" . self::HELPER . "\n");
        $this->writeProbe('Probe', '', 'return n => Aliased::VALUE + n;');

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertStringContainsString("Cannot locate class 'Probe\\Aliased'", $result['stdout'] . $result['stderr']);
        $this->assertNotSame(0, $result['exitCode']);
    }

    /**
     * @dataProvider subNamespaceProvider
     */
    public function testClosureInSubNamespaceResolvesUnqualifiedName(string $body): void
    {
        file_put_contents($this->projectDir . '/probe/sub/helper.zep', "namespace Probe\\Sub;\n\n" . self::HELPER . "\n");
        $this->writeProbe('Probe\\Sub', '', $body, 'sub/');

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertStringNotContainsString('nonexistent-class', $result['stdout'] . $result['stderr']);
        $this->assertStringNotContainsString('probe_helper_ce', $this->closureSources());
    }

    /**
     * A sub-namespace class with the same short name must not shadow the root one.
     */
    public function testClosureInSubNamespaceDoesNotBindRootClass(): void
    {
        file_put_contents($this->projectDir . '/probe/helper.zep', "namespace Probe;\n\n" . self::HELPER . "\n");
        file_put_contents($this->projectDir . '/probe/sub/helper.zep', "namespace Probe\\Sub;\n\n" . self::HELPER . "\n");
        $this->writeProbe('Probe\\Sub', '', 'return function () { return new Helper(); };', 'sub/');

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertStringContainsString('probe_sub_helper_ce', $this->closureSources());
        $this->assertStringNotContainsString('probe_helper_ce', $this->closureSources());
    }

    private function closureSources(): string
    {
        $sources = '';
        foreach (glob($this->projectDir . '/ext/probe/*__closure.zep.c') as $file) {
            $sources .= file_get_contents($file);
        }

        return $sources;
    }

    private function writeProbe(string $namespace, string $uses, string $body, string $dir = ''): void
    {
        file_put_contents(
            $this->projectDir . '/probe/' . $dir . 'probe.zep',
            "namespace {$namespace};\n\n{$uses}\nclass Probe\n{\n    public function run()\n    {\n        {$body}\n    }\n}\n"
        );
    }
}
