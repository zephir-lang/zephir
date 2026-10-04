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
 * A local declared `array` must hold an array, as a non-nullable PHP `array`
 * typed property does: PHP never coerces anything into `array`, null included.
 *
 * A dynamic source is checked at runtime and throws a TypeError before the
 * write. A source that can never be an array is rejected at build time, as a
 * literal already is.
 *
 * The runtime behaviour is covered by tests/zept/issue2689_array_local_type.zept.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2689
 */
final class Issue2689Test extends TestCase
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

    public function testDynamicSourceIsCheckedBeforeTheWrite(): void
    {
        $generated = $this->generateOk(
            'ald',
            "    public function read(container)\n    {\n        array table;\n"
            . "        let table = container;\n        return table;\n    }"
        );

        $this->assertMatchesRegularExpression(
            '/if \(UNEXPECTED\(Z_TYPE_P\(container\) != IS_ARRAY\)\) \{\s*'
            . 'zephir_throw_variable_type_error\(container, "table", "array"\);\s*'
            . 'ZEPHIR_MM_RESTORE\(\);\s*return;\s*\}\s*'
            . 'ZEPHIR_CPY_WRT\(&table, container\);/',
            $generated
        );
    }

    public function testCheckInsideTryJumpsToTheTryEnd(): void
    {
        $generated = $this->generateOk(
            'alt',
            "    public function read(container)\n    {\n        array table = [];\n"
            . "        try {\n            let table = container;\n        } catch \\TypeError {\n"
            . "        }\n        return table;\n    }"
        );

        $this->assertMatchesRegularExpression(
            '/zephir_throw_variable_type_error\(container, "table", "array"\);\s*goto try_end_1;/',
            $generated
        );
    }

    /**
     * An `array` local can still be null: an omitted `array p = null`
     * parameter is, and PHP's non-nullable `array` rejects null.
     */
    public function testArraySourceIsCheckedToo(): void
    {
        $generated = $this->generateOk(
            'ala',
            "    public function read(array p = null)\n    {\n        array table;\n"
            . "        let table = p;\n        return table;\n    }"
        );

        $this->assertStringContainsString('zephir_throw_variable_type_error(&p, "table", "array");', $generated);
    }

    public function testArrayLiteralIsNotChecked(): void
    {
        $generated = $this->generateOk(
            'all',
            "    public function read()\n    {\n        array table;\n"
            . "        let table = [1, 2];\n        return table;\n    }"
        );

        $this->assertStringNotContainsString('zephir_throw_variable_type_error', $generated);
    }

    public function testVarTargetIsNotChecked(): void
    {
        $generated = $this->generateOk(
            'alv',
            "    public function read(array p)\n    {\n        var table;\n"
            . "        let table = p;\n        return table;\n    }"
        );

        $this->assertStringNotContainsString('zephir_throw_variable_type_error', $generated);
    }

    public function testIntSourceIsRejected(): void
    {
        $this->assertRejected(
            'alri',
            "    public function read(int i)\n    {\n        array table;\n"
            . "        let table = i;\n        return table;\n    }",
            "Cannot 'assign' int for array type"
        );
    }

    public function testStringSourceIsRejected(): void
    {
        $this->assertRejected(
            'alrs',
            "    public function read(string s)\n    {\n        array table;\n"
            . "        let table = s;\n        return table;\n    }",
            "Cannot 'assign' string for array type"
        );
    }

    public function testClosureIsRejected(): void
    {
        $this->assertRejected(
            'alrc',
            "    public function read()\n    {\n        array table;\n"
            . "        let table = function () { return 1; };\n        return table;\n    }",
            "Cannot 'assign' closure for array type"
        );
    }

    public function testRangeLoopIntoArrayIsRejected(): void
    {
        $this->assertRejected(
            'alrr',
            "    public function read()\n    {\n        array v;\n"
            . "        for v in range(1, 3) {\n        }\n    }",
            'for array type'
        );
    }

    private function assertRejected(string $name, string $body, string $message): void
    {
        $result = $this->generateProject($name, $body);

        $this->assertSame(1, $result['exitCode'], $result['stderr']);
        $this->assertStringContainsString($message, $result['stderr']);
    }

    private function generateOk(string $name, string $body): string
    {
        $result = $this->generateProject($name, $body);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        $generatedFile = sprintf('%s/%s/ext/%s/sample.zep.c', $this->outputDir(), $name, $name);
        $this->assertFileExists($generatedFile);

        return (string) file_get_contents($generatedFile);
    }

    /**
     * @return array{exitCode: int, stderr: string}
     */
    private function generateProject(string $name, string $body): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        $source = 'namespace ' . ucfirst($name) . ";\n\nclass Sample\n{\n" . $body . "\n}\n";
        file_put_contents($projectDir . '/' . $name . '/sample.zep', $source);

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        return ['exitCode' => $result['exitCode'], 'stderr' => $result['stderr']];
    }
}
