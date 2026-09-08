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
 * A local that can be read on a path where nothing has assigned it evaluates to
 * null, the same value PHP gives an unset variable. PHP reports it at runtime;
 * Zephir knows it at compile time and reports it there.
 *
 * The diagnostic is driven by the same decision that adds the initialization,
 * so the two cannot disagree about which locals are affected.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2679
 */
final class ConditionalInitializationWarningTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    /**
     * @return array<string, array{0: string}>
     */
    public static function reportedProvider(): array
    {
        return [
            'if without else' => ['
        var x;

        if a {
            let x = 1;
        }

        return x;
'],
            'two separate ifs' => ['
        var x;

        if a {
            let x = 1;
        }

        if a {
            let x = 2;
        }

        return x;
'],
            'loop that may not run' => ['
        var x, item;

        for item in [1, 2] {
            let x = item;
        }

        return x;
'],
            'switch without a default' => ['
        var x;

        switch a {
            case true:
                let x = 1;
                break;
        }

        return x;
'],
            'read before the only write' => ['
        var x, t;

        let t = typeof x;
        let x = 1;

        return t;
'],
            'assignment inside try' => ['
        var x, e;

        try {
            let x = 1;
        } catch \Exception, e {
            let a = false;
        }

        return x;
'],
            'native local' => ['
        int x;

        if a {
            let x = 1;
        }

        return x;
'],
        ];
    }

    /**
     * @return array<string, array{0: string}>
     */
    public static function silentProvider(): array
    {
        return [
            'both arms assign' => ['
        var x;

        if a {
            let x = 1;
        } else {
            let x = 2;
        }

        return x;
'],
            'switch with a default' => ['
        var x;

        switch a {
            case true:
                let x = 1;
                break;
            default:
                let x = 2;
                break;
        }

        return x;
'],
            'unconditional write after a conditional one' => ['
        var x;

        if a {
            let x = 1;
        }

        let x = 2;

        return x;
'],
            'fetch target' => ['
        var v;

        if fetch v, data["k"] {
            let a = false;
        }

        return v;
'],
            'declaration with a value' => ['
        var x = 1;

        if a {
            let x = 2;
        }

        return x;
'],
        ];
    }

    /**
     * @dataProvider reportedProvider
     */
    public function testLocalReadBeforeItIsAssignedIsReported(string $body): void
    {
        $output = $this->compileBody($body);

        $this->assertStringContainsString('conditional-initialization', $output);
        $this->assertStringContainsString('may be read before it is assigned', $output);
        $this->assertStringContainsString('Stub\Probe::run', $output);
    }

    /**
     * @dataProvider silentProvider
     */
    public function testDefinitelyAssignedLocalIsNotReported(string $body): void
    {
        $this->assertStringNotContainsString(
            'conditional-initialization',
            $this->compileBody($body)
        );
    }

    public function testWarningCanBeDisabled(): void
    {
        $body = '
        var x;

        if a {
            let x = 1;
        }

        return x;
';

        $this->assertStringNotContainsString(
            'conditional-initialization',
            $this->compileBody($body, '-Wconditional-initialization')
        );
    }

    /**
     * A local nothing writes to at all is #2654's case. The compiler starts it
     * at IS_NULL in the declaration instead of registering it, so only the one
     * warning applies.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2654
     */
    public function testNeverAssignedLocalIsOnlyReportedAsUnassigned(): void
    {
        $output = $this->compileBody('
        var x;

        return x;
');

        $this->assertStringContainsString('unassigned-variable', $output);
        $this->assertStringNotContainsString('conditional-initialization', $output);
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-conditional-' . bin2hex(random_bytes(6));
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

    private function compileBody(string $body, string $extraArgs = ''): string
    {
        file_put_contents(
            $this->projectDir . '/stub/probe.zep',
            "namespace Stub;\n\nclass Probe\n{\n    public function run(bool a, array data)\n    {"
            . $body
            . "    }\n}\n"
        );

        $result = $this->runZephir(trim('generate --no-ansi ' . $extraArgs), $this->projectDir);

        $this->assertSame(0, $result['exitCode'], 'A warning must not fail the build');

        return $result['stdout'] . $result['stderr'];
    }
}
