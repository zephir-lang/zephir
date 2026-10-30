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
 * A class extending one from a loaded extension calls the inherited methods
 * through `this->`, and the compiler checks the argument count against the
 * reflected signature. Optional parameters with a `null` default were counted
 * as required, and variadic ones were not recognised, so calls PHP accepts
 * failed to compile.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2763
 */
final class Issue2763Test extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    public static function acceptedCallProvider(): array
    {
        return [
            'required argument, null default omitted' => [
                '\SplObjectStorage',
                'var item; let item = new \stdClass(); this->attach(item);',
            ],
            'only null default omitted' => [
                '\SplFileInfo',
                'return this->getFileInfo();',
            ],
            'nullable int default omitted' => [
                '\ReflectionClass',
                'return this->getConstants();',
            ],
            'several arguments to a variadic' => [
                '\ReflectionFunction',
                'return this->invoke(1, 2, 3);',
            ],
        ];
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue2763-' . bin2hex(random_bytes(6));
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
     * @dataProvider acceptedCallProvider
     */
    public function testCallAcceptedByPhpCompiles(string $parent, string $body): void
    {
        $result = $this->generate($parent, $body);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
    }

    public function testTooManyArgumentsIsStillACompileError(): void
    {
        $result = $this->generate('\SplFileInfo', 'return this->getFileInfo("A", "B");');
        $output = $result['stdout'] . $result['stderr'];

        $this->assertStringContainsString(
            "Method 'Stub\\Probe::getFileInfo' called with a wrong number of parameters, the method has: 0, passed: 2",
            $output
        );
        $this->assertNotSame(0, $result['exitCode']);
    }

    /**
     * @return array{exitCode: int, stdout: string, stderr: string}
     */
    private function generate(string $parent, string $body): array
    {
        file_put_contents(
            $this->projectDir . '/stub/probe.zep',
            "namespace Stub;\n\nclass Probe extends {$parent}\n{\n    public function run()\n    {\n        {$body}\n    }\n}\n"
        );

        return $this->runZephir('generate --no-ansi', $this->projectDir);
    }
}
