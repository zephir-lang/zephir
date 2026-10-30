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
 * The `is_*()` optimizers emit a C comparison, which must stay one operand
 * when another comparison takes it as its right-hand side: a bare
 * `1 != Z_TYPE_P(v) == IS_ARRAY` groups as `(1 != Z_TYPE_P(v)) == IS_ARRAY`.
 *
 * The runtime behaviour is covered by tests/zept/issue2810_is_type_precedence.zept.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2810
 */
final class Issue2810Test extends TestCase
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

    /**
     * @return array<string, array{0: string, 1: string}>
     */
    public static function typeFunctionProvider(): array
    {
        return [
            'is_array'    => ['is_array', 'IS_ARRAY'],
            'is_null'     => ['is_null', 'IS_NULL'],
            'is_object'   => ['is_object', 'IS_OBJECT'],
            'is_string'   => ['is_string', 'IS_STRING'],
            'is_int'      => ['is_int', 'IS_LONG'],
            'is_resource' => ['is_resource', 'IS_RESOURCE'],
        ];
    }

    /**
     * @dataProvider typeFunctionProvider
     */
    public function testConditionIsParenthesized(string $function, string $type): void
    {
        $generated = $this->generateOk(
            'i2810' . str_replace('_', '', $function),
            "    public static function f(var v) -> bool\n    {\n        return true !== " . $function . "(v);\n    }"
        );

        $this->assertStringContainsString('(Z_TYPE_P(v) == ' . $type . ')', $generated);
        $this->assertDoesNotMatchRegularExpression('/[!=]= Z_TYPE_P\(v\) == /', $generated);
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
