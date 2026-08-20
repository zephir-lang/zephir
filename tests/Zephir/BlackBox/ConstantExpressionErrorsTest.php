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
 * Both parsers accept a full expression in declaration position, so anything
 * that is not resolvable at compile time has to be rejected by the compiler —
 * with a message naming the problem, not the bare "Syntax error" this syntax
 * used to produce.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2061
 */
final class ConstantExpressionErrorsTest extends TestCase
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

    public static function rejectedInitializerProvider(): iterable
    {
        yield 'function call' => [
            'constexprcall',
            'const A = strlen("abc");',
            'not a compile-time constant expression',
        ];

        yield 'object instantiation' => [
            'constexprnew',
            'const A = new stdClass();',
            'not a compile-time constant expression',
        ];

        yield 'division by zero' => [
            'constexprdivzero',
            'const A = 1 / 0;',
            'Division by zero',
        ];

        yield 'property default call' => [
            'constexprpropcall',
            'public size = strlen("abc");',
            'not a compile-time constant expression',
        ];
    }

    /**
     * @dataProvider rejectedInitializerProvider
     */
    public function testRejectsAnInitializerThatIsNotConstant(
        string $project,
        string $member,
        string $expectedMessage
    ): void {
        [$exitCode, $stderr] = $this->generateProject($project, [
            'limits.zep' => sprintf(
                "namespace %s;\n\nclass Limits\n{\n    %s\n}\n",
                ucfirst($project),
                $member
            ),
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString($expectedMessage, $stderr);
    }

    public function testRejectsAConstantWhoseInitializerReadsItself(): void
    {
        [$exitCode, $stderr] = $this->generateProject('constexprcycle', [
            'limits.zep' => "namespace Constexprcycle;\n\nclass Limits\n{\n"
                . "    const A = self::B + 1;\n"
                . "    const B = self::A + 1;\n"
                . "}\n",
        ]);

        $this->assertSame(1, $exitCode);
        $this->assertStringContainsString('Cyclic reference', $stderr);
    }

    /**
     * @return array{0: int, 1: string} exit code and stderr
     */
    private function generateProject(string $name, array $files): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        foreach ($files as $fileName => $source) {
            file_put_contents($projectDir . '/' . $name . '/' . $fileName, $source);
        }

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        return [$result['exitCode'], $result['stderr']];
    }
}
