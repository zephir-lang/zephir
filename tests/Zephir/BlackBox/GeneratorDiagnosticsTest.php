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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

/**
 * Every construct the v1 generator implementation does not support must fail
 * at compile time with a located, actionable diagnostic (issue #1849).
 */
final class GeneratorDiagnosticsTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-gendiag-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);

        mkdir($this->projectDir . '/stub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'])
        );
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    /**
     * @dataProvider unsupportedConstructsProvider
     */
    public function testUnsupportedConstructReportsDiagnostic(string $zep, string $expectedMessage): void
    {
        file_put_contents($this->projectDir . '/stub/kase.zep', $zep);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertNotSame(0, $result['exitCode'], "expected failure, got:\n" . $output);
        $this->assertStringContainsString($expectedMessage, $output);
        $this->assertStringContainsString('kase.zep', $output, 'diagnostic must locate the source file');
    }

    public static function unsupportedConstructsProvider(): array
    {
        return [
            'yield inside try/catch' => [
                <<<'ZEP'
namespace Stub;

class Kase
{
    public function gen()
    {
        try {
            yield 1;
        } catch \Exception {
        }
    }
}
ZEP,
                "'yield' inside a try/catch block is not supported",
            ],
            'yield inside for-in-iterator()' => [
                <<<'ZEP'
namespace Stub;

class Kase
{
    public function gen(<\ArrayIterator> obj)
    {
        var v;
        for v in iterator(obj) {
            yield v;
        }
    }
}
ZEP,
                "'yield' inside `for ... in iterator(...)` is not supported",
            ],
            'yield inside closure' => [
                <<<'ZEP'
namespace Stub;

class Kase
{
    public function maker()
    {
        return function () {
            yield 1;
        };
    }
}
ZEP,
                "'yield' is only supported inside class or trait methods",
            ],
            'yield in constructor' => [
                <<<'ZEP'
namespace Stub;

class Kase
{
    public function __construct()
    {
        yield 1;
    }
}
ZEP,
                'method cannot be a generator',
            ],
            'yield in internal method' => [
                <<<'ZEP'
namespace Stub;

class Kase
{
    internal function gen()
    {
        yield 1;
    }

    public function pub()
    {
        return 1;
    }
}
ZEP,
                "'yield' cannot be used in an 'internal' method",
            ],
            'scalar return type on generator' => [
                <<<'ZEP'
namespace Stub;

class Kase
{
    public function gen() -> int
    {
        yield 1;
    }
}
ZEP,
                'Generator methods return a compiler-provided object implementing \Iterator',
            ],
            'explicit \\Generator return type' => [
                <<<'ZEP'
namespace Stub;

class Kase
{
    public function gen() -> <\Generator>
    {
        yield 1;
    }
}
ZEP,
                "PHP's \\Generator is final and cannot be produced by extensions",
            ],
            'yield in global function' => [
                <<<'ZEP'
namespace Stub;

class Kase
{
    public function noop()
    {
        return 1;
    }
}

function stubGenFunc()
{
    yield 1;
}
ZEP,
                "'yield' is only supported inside class or trait methods",
            ],
        ];
    }

    public function testUserClassNamedGeneratorCollides(): void
    {
        file_put_contents($this->projectDir . '/stub/generator.zep', <<<'ZEP'
namespace Stub;

class Generator
{
    public function noop()
    {
        return 1;
    }
}
ZEP);
        file_put_contents($this->projectDir . '/stub/gen.zep', <<<'ZEP'
namespace Stub;

class Gen
{
    public function counter()
    {
        yield 1;
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertNotSame(0, $result['exitCode'], $output);
        $this->assertStringContainsString('collides with the compiler-provided generator', $output);
    }
}
