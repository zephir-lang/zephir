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
 * A do-while condition is evaluated by code printed at the end of the body,
 * before `} while (cond);`. A C `continue` jumps straight to the test and
 * skips that code, so `continue` jumps to a label placed in front of it.
 *
 * The runtime behaviour is covered by tests/zept/do_while_continue_condition.zept.
 */
final class DoWhileContinueTest extends TestCase
{
    use RunsZephirCommands;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testContinueJumpsInFrontOfTheCondition(): void
    {
        $generated = $this->generateOk(
            'dwcont',
            "    public function run(a)\n    {\n        long i = 0;\n        do {\n            let i++;\n"
            . "            if i == 2 {\n                continue;\n            }\n        } while this->check(a, i);\n    }\n\n"
            . "    public function check(a, long i) -> bool\n    {\n        return i < 3;\n    }"
        );

        $this->assertStringContainsString('goto do_cond_1;', $generated);
        $this->assertMatchesRegularExpression('/do_cond_1: ;[^}]*ZEPHIR_CALL_METHOD[^}]*\} while \(/', $generated);
        $this->assertStringNotContainsString('continue;', $generated);
    }

    public function testNoContinueNoLabel(): void
    {
        $generated = $this->generateOk(
            'dwnone',
            "    public function run()\n    {\n        long i = 0;\n        do {\n            let i++;\n        } while i < 3;\n    }"
        );

        $this->assertStringNotContainsString('do_cond_', $generated);
    }

    public function testContinueOfAnInnerLoopStaysACContinue(): void
    {
        $generated = $this->generateOk(
            'dwinner',
            "    public function run()\n    {\n        long i = 0, j = 0;\n        do {\n            let i++;\n"
            . "            while j < 2 {\n                let j++;\n                continue;\n            }\n        } while i < 3;\n    }"
        );

        $this->assertStringContainsString('continue;', $generated);
        $this->assertStringNotContainsString('do_cond_', $generated);
    }

    private function generateOk(string $name, string $body): string
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        $source = 'namespace ' . ucfirst($name) . ";\n\nclass Sample\n{\n" . $body . "\n}\n";
        file_put_contents($projectDir . '/' . $name . '/sample.zep', $source);

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        $generatedFile = sprintf('%s/ext/%s/sample.zep.c', $projectDir, $name);
        $this->assertFileExists($generatedFile);

        return (string) file_get_contents($generatedFile);
    }
}
