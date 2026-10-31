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

use function bin2hex;
use function file_get_contents;
use function file_put_contents;
use function json_encode;
use function mkdir;
use function random_bytes;
use function substr_count;
use function sys_get_temp_dir;

/**
 * A `config.json` section replaced the built-in defaults instead of
 * overriding them, so every key it left out read back as null. cphalcon
 * lists 5 of the 8 optimizations, which turned `call-gatherer-pass` off,
 * and a partial `stubs` or `extra` block broke `zephir stubs`.
 *
 * The compilation context of a function was only reachable through the call
 * gatherer pass, so turning that pass off, or a function with an empty body,
 * crashed `generate`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2731
 */
final class Issue2731Test extends TestCase
{
    use RunsZephirCommands;

    /** cphalcon's `optimizations` block: `call-gatherer-pass` is not listed. */
    private const PARTIAL_OPTIMIZATIONS = [
        'static-type-inference'             => true,
        'static-type-inference-second-pass' => true,
        'local-context-pass'                => true,
        'constant-folding'                  => true,
        'static-constant-class-folding'     => true,
    ];

    /**
     * Two calls to the same function, outside any loop: only the call gatherer
     * pass gives them a shared function cache. A method call has a cache of its
     * own at every call site whatever the pass says (#2790).
     */
    private const REPEATED_CALL = <<<'ZEP'
        namespace Stub;

        class Greeter
        {
            public function run() -> string
            {
                return soundex("a") . soundex("b");
            }
        }
        ZEP;

    /** A file needs a class named after it next to its functions. */
    private const ATTRIBUTED_FUNCTION = <<<'ZEP'
        namespace Stub;

        #[Marker("on-a-function")]
        function tagged(int a) -> int
        {
            return a;
        }

        class Holder
        {
        }
        ZEP;

    private const EMPTY_FUNCTION = <<<'ZEP'
        namespace Stub;

        function nothing()
        {
        }

        class Holder
        {
        }
        ZEP;

    private const NAMESPACED_CALLS = <<<'ZEP'
        namespace Stub;

        function tagged(int a) -> int
        {
            return a;
        }

        class Holder
        {
            public function run() -> int
            {
                return tagged(1) + \Stub\tagged(2);
            }
        }
        ZEP;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-2731-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testShouldKeepCallGathererPassOnWhenTheSectionOmitsIt(): void
    {
        $this->writeProject('greeter.zep', self::REPEATED_CALL, ['optimizations' => self::PARTIAL_OPTIMIZATIONS]);

        $result = $this->runZephir('generate', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stderr']);
        $this->assertStringContainsString('zephir_fcall_cache_entry', $this->generated('stub/greeter.zep.c'));
    }

    /**
     * The negative control: the cache above comes from the pass, not from
     * anything else in the method.
     */
    public function testShouldNotCacheTheCallsWithCallGathererPassOff(): void
    {
        $this->writeProject('greeter.zep', self::REPEATED_CALL, ['optimizations' => self::PARTIAL_OPTIMIZATIONS]);

        $result = $this->runZephir('generate -fno-call-gatherer-pass', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stderr']);
        $this->assertStringNotContainsString('zephir_fcall_cache_entry', $this->generated('stub/greeter.zep.c'));
    }

    public function testShouldGenerateStubsWhenStubsAndExtraArePartial(): void
    {
        $this->writeProject('greeter.zep', self::REPEATED_CALL, [
            'stubs' => ['banner' => '/* partial */'],
            'extra' => ['export-classes' => false],
        ]);

        $result = $this->runZephir('stubs', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        $stub = $this->projectDir . '/ide/0.0.1/Stub/Greeter.php';
        $this->assertFileExists($stub);

        /* The banner comes from the file, the indentation from the defaults */
        $source = (string) file_get_contents($stub);
        $this->assertStringContainsString('/* partial */', $source);
        $this->assertStringContainsString("\n    public function run(", $source);
    }

    public function testShouldGenerateAnAttributedFunctionWithCallGathererPassOff(): void
    {
        $this->writeProject('holder.zep', self::ATTRIBUTED_FUNCTION, [
            'optimizations' => ['call-gatherer-pass' => false],
        ]);

        $result = $this->runZephir('generate', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        $entry = $this->generated('stub.c');
        $this->assertStringContainsString('PHP_FUNCTION(f_Stub_tagged)', $entry);
        $this->assertStringContainsString('zephir_add_function_attribute(SL("stub\\\\tagged")', $entry);
    }

    public function testShouldGenerateAFunctionWithAnEmptyBody(): void
    {
        $this->writeProject('holder.zep', self::EMPTY_FUNCTION);

        $result = $this->runZephir('generate', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertStringContainsString('PHP_FUNCTION(f_Stub_nothing)', $this->generated('stub.c'));
    }

    /**
     * A namespaced function was called by its bare spelling, so the global
     * function table was searched, and `\t` in a qualified name read as a tab
     * in the C string.
     */
    public function testShouldCallANamespacedFunctionByItsQualifiedName(): void
    {
        $this->writeProject('holder.zep', self::NAMESPACED_CALLS);

        $result = $this->runZephir('generate', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertStringNotContainsString('does not exist at compile time', $result['stdout'] . $result['stderr']);

        $source = $this->generated('stub/holder.zep.c');
        $this->assertSame(2, substr_count($source, '"Stub\\\\tagged"'), $source);
    }

    private function generated(string $path): string
    {
        $file = $this->projectDir . '/ext/' . $path;
        $this->assertFileExists($file);

        return (string) file_get_contents($file);
    }

    private function writeProject(string $file, string $source, array $settings = []): void
    {
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'] + $settings)
        );
        file_put_contents($this->projectDir . '/stub/' . $file, $source);
    }
}
