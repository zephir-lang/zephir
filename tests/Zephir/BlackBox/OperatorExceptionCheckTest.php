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
 * PHP stops at the first operator that throws. The generated C checks for a
 * pending exception right after each operator kernel call that can throw and
 * leaves the method, or jumps to the enclosing try, as a throw does.
 *
 * The runtime behaviour is covered by tests/zept/operator_exception_stops.zept
 * and tests/zept/try_catch_nested_exit.zept.
 */
final class OperatorExceptionCheckTest extends TestCase
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

    public function testZvalAdditionIsFollowedByACheck(): void
    {
        $generated = $this->generateOk(
            'oeadd',
            "    public function run(a, b)\n    {\n        var r;\n        let r = a + b;\n        return r;\n    }"
        );

        $this->assertMatchesRegularExpression(
            '/zephir_add_function\(&r, a, b\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{\s*ZEPHIR_MM_RESTORE\(\);\s*return;\s*\}/',
            $generated
        );
    }

    public function testCheckInsideTryJumpsToTheTryEnd(): void
    {
        $generated = $this->generateOk(
            'oetry',
            "    public function run(a, b)\n    {\n        var r;\n        try {\n            let r = a + b;\n"
            . "        } catch \\TypeError {\n        }\n        return r;\n    }"
        );

        $this->assertMatchesRegularExpression(
            '/zephir_add_function\(&r, a, b\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{\s*goto try_end_1;\s*\}/',
            $generated
        );
    }

    public function testCodeAfterANestedTryJumpsToTheOuterTryEnd(): void
    {
        $generated = $this->generateOk(
            'oenest',
            "    public function run(a, b)\n    {\n        var r;\n        try {\n"
            . "            try {\n                let r = 1;\n            } catch \\TypeError {\n            }\n"
            . "            let r = a + b;\n        } catch \\TypeError {\n        }\n        return r;\n    }"
        );

        $this->assertMatchesRegularExpression(
            '/zephir_add_function\([^;]*, a, b\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{\s*goto try_end_1;\s*\}/',
            $generated
        );
    }

    public function testUnmatchedCatchLeavesTheMethod(): void
    {
        $generated = $this->generateOk(
            'oenomatch',
            "    public function run(a, b)\n    {\n        var r;\n        try {\n            let r = a + b;\n"
            . "        } catch \\RuntimeException {\n        }\n        return r;\n    }"
        );

        $this->assertMatchesRegularExpression(
            '/zend_clear_exception\(\);[^}]*\}\s*else \{\s*ZEPHIR_MM_RESTORE\(\);\s*return;\s*\}/',
            $generated
        );
    }

    public function testPropertyIsNotWrittenBackAfterAFailedOperator(): void
    {
        $generated = $this->generateOk(
            'oeprop',
            "    protected p = [];\n\n    public function run(v)\n    {\n        let this->p += v;\n    }"
        );

        $this->assertMatchesRegularExpression(
            '/ZEPHIR_ADD_ASSIGN\([^;]+\);\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{\s*(?:ZEPHIR_MM_RESTORE\(\);\s*)?return;\s*\}\s*zephir_update_property/',
            $generated
        );
    }

    public function testParenthesizedOperationKeepsItsPrecedence(): void
    {
        $generated = $this->generateOk(
            'oeparen',
            "    public function run(a, b)\n    {\n        return !(a + b == 3);\n    }"
        );

        $this->assertMatchesRegularExpression('/RETURN_MM_BOOL\(!\(/', $generated);
    }

    public function testNativeDivisionByANonZeroLiteralStaysInline(): void
    {
        $generated = $this->generateOk(
            'oediv',
            "    public function run(long a) -> double\n    {\n        return a / 2;\n    }"
        );

        $this->assertDoesNotMatchRegularExpression('/EG\(exception\)/', $generated);
    }

    public function testNativeDivisionByAVariableIsChecked(): void
    {
        $generated = $this->generateOk(
            'oedivv',
            "    public function run(long a, long b) -> double\n    {\n        double d;\n        let d = a / b;\n        return d;\n    }"
        );

        $this->assertMatchesRegularExpression('/zephir_safe_div_long_long\(a, b\);\s*if \(UNEXPECTED\(EG\(exception\)\)\)/', $generated);
    }

    /**
     * A returned quotient leaves the method right away, so it needs no check.
     */
    public function testReturnedNativeDivisionStaysInline(): void
    {
        $generated = $this->generateOk(
            'oedivr',
            "    public function run(long a, long b) -> double\n    {\n        return a / b;\n    }"
        );

        $this->assertStringContainsString('RETURN_DOUBLE(zephir_safe_div_long_long(a, b));', $generated);
    }

    public function testLooseComparisonOfTwoZvalsIsChecked(): void
    {
        $generated = $this->generateOk(
            'oecmp',
            "    public function run(a, b)\n    {\n        if a == b {\n            return 1;\n        }\n        return 2;\n    }"
        );

        $this->assertMatchesRegularExpression('/= ZEPHIR_IS_EQUAL\(a, b\);\s*if \(UNEXPECTED\(EG\(exception\)\)\)/', $generated);
    }

    public function testIdenticalComparisonIsNotChecked(): void
    {
        $generated = $this->generateOk(
            'oeident',
            "    public function run(a, b)\n    {\n        if a === b {\n            return 1;\n        }\n        return 2;\n    }"
        );

        $this->assertDoesNotMatchRegularExpression('/EG\(exception\)/', $generated);
    }

    public function testPropertyInitializerHasNoBareReturn(): void
    {
        $generated = $this->generateOk(
            'oeinit',
            "    protected p = [1, 2];\n\n    public function run()\n    {\n        return this->p;\n    }"
        );

        $this->assertDoesNotMatchRegularExpression('/zend_object \*zephir_init_properties[^}]*\breturn;/s', $generated);
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
