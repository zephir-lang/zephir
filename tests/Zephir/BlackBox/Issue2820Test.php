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
 * `==` and `!=` with null are PHP's loose comparison, under which 0, 0.0, "",
 * false and [] equal null, while `===` and `!==` stay a type test. Each loose
 * comparison of a variable with null is reported, as it used to be a type test.
 * A nullable `array` or `string` parameter keeps an explicit null argument.
 *
 * The runtime behaviour is covered by tests/zept/issue2820_*.zept.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2820
 */
final class Issue2820Test extends TestCase
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
    public static function looseComparisonProvider(): array
    {
        return [
            'x == null' => ['x == null', 'ZEPHIR_IS_EQUAL_NULL(x)'],
            'null == x' => ['null == x', 'ZEPHIR_IS_EQUAL_NULL(x)'],
            'x != null' => ['x != null', '!ZEPHIR_IS_EQUAL_NULL(x)'],
            'null != x' => ['null != x', '!ZEPHIR_IS_EQUAL_NULL(x)'],
        ];
    }

    /**
     * @dataProvider looseComparisonProvider
     */
    public function testLooseComparisonWithNullIsNotATypeTest(string $comparison, string $expected): void
    {
        [$generated] = $this->generateOk('i2820loose', 'var x', $comparison);

        $this->assertStringContainsString($expected, $generated);
        $this->assertStringNotContainsString('IS_NULL', $generated);
    }

    /**
     * @return array<string, array{0: string, 1: string}>
     */
    public static function strictComparisonProvider(): array
    {
        return [
            'x === null' => ['x === null', 'Z_TYPE_P(x) == IS_NULL'],
            'null === x' => ['null === x', 'Z_TYPE_P(x) == IS_NULL'],
            'x !== null' => ['x !== null', 'Z_TYPE_P(x) != IS_NULL'],
            'null !== x' => ['null !== x', 'Z_TYPE_P(x) != IS_NULL'],
        ];
    }

    /**
     * @dataProvider strictComparisonProvider
     */
    public function testStrictComparisonWithNullStaysATypeTest(string $comparison, string $expected): void
    {
        [$generated, $output] = $this->generateOk('i2820strict', 'var x', $comparison);

        $this->assertStringContainsString($expected, $generated);
        $this->assertStringNotContainsString('loose-null-comparison', $output);
    }

    /**
     * @return array<string, array{0: string}>
     */
    public static function orderingProvider(): array
    {
        return [
            'x < null'  => ['x < null'],
            'x <= null' => ['x <= null'],
            'x > null'  => ['x > null'],
            'x >= null' => ['x >= null'],
            'null < x'  => ['null < x'],
            'null >= x' => ['null >= x'],
        ];
    }

    /**
     * @dataProvider orderingProvider
     */
    public function testOrderingWithNullComparesValuesNotTypes(string $comparison): void
    {
        [$generated] = $this->generateOk('i2820order', 'var x', $comparison);

        $this->assertMatchesRegularExpression('/ZEPHIR_L[TE]\(/', $generated);
        $this->assertStringNotContainsString('IS_NULL', $generated);
    }

    public function testOrderingOfAStringParameterWithNullComparesValues(): void
    {
        [$generated] = $this->generateOk('i2820orderstr', 'string s', 's < null');

        $this->assertMatchesRegularExpression('/ZEPHIR_LT\(/', $generated);
    }

    /**
     * @return array<string, array{0: string, 1: string}>
     */
    public static function warnedProvider(): array
    {
        return [
            'x == null' => ['x == null', "use '===' to test for null"],
            'null != x' => ['null != x', "use '!==' to test for null"],
        ];
    }

    /**
     * @dataProvider warnedProvider
     */
    public function testLooseComparisonOfAVariableWithNullWarns(string $comparison, string $advice): void
    {
        [, $output] = $this->generateOk('i2820warn', 'var x', $comparison);

        $this->assertStringContainsString('loose-null-comparison', $output);
        $this->assertStringContainsString($advice, $output);
    }

    public function testWarningCanBeTurnedOff(): void
    {
        [, $output] = $this->generateOk('i2820off', 'var x', 'x == null', '-Wloose-null-comparison');

        $this->assertStringNotContainsString('loose-null-comparison', $output);
    }

    public function testLooseComparisonOfALiteralWithNullDoesNotWarn(): void
    {
        [, $output] = $this->generateOk('i2820literal', 'var x', 'null == "abc"');

        $this->assertStringNotContainsString('loose-null-comparison', $output);
    }

    /**
     * @return array<string, array{0: string, 1: string}>
     */
    public static function compilesProvider(): array
    {
        return [
            'null == array local'    => ['array a', 'null == a'],
            'null === array local'   => ['array a', 'null === a'],
            'null == string literal' => ['var x', 'null == "abc"'],
            'null == array literal'  => ['var x', 'null == []'],
        ];
    }

    /**
     * @dataProvider compilesProvider
     */
    public function testNullAgainstAnArrayOrALiteralCompiles(string $parameter, string $comparison): void
    {
        [$generated] = $this->generateOk('i2820compiles', $parameter, $comparison);

        $this->assertMatchesRegularExpression('/RETURN_(MM_)?BOOL\(/', $generated);
    }

    /**
     * zephir_get_arrval() and zephir_get_strval() would turn an explicit null
     * into [] or "", so a nullable parameter's null takes the default branch.
     *
     * @return array<string, array{0: string, 1: string}>
     */
    public static function nullableParameterProvider(): array
    {
        return [
            'array x = null'          => ['array x = null', ''],
            'written string x = null' => ['string x = null', "let x = x;\n        "],
        ];
    }

    /**
     * @dataProvider nullableParameterProvider
     */
    public function testNullableParameterKeepsAnExplicitNull(string $parameter, string $preamble): void
    {
        [$generated] = $this->generateOk('i2820nullable', $parameter, 'x === null', '', $preamble);

        $this->assertStringContainsString('if (!x_param || Z_TYPE_P(x_param) == IS_NULL) {', $generated);
    }

    public function testParameterWithANonNullDefaultIsUnchanged(): void
    {
        [$generated] = $this->generateOk('i2820arraydefault', 'array x = []', 'x === null');

        $this->assertStringContainsString('if (!x_param) {', $generated);
    }

    /**
     * An array default is assigned when the object is created, under a
     * compiler-made `this->p === null` check that must stay a type test.
     */
    public function testArrayPropertyDefaultIsInitializedUnderATypeTest(): void
    {
        [$generated, $output] = $this->generateClass('i2820property', "    protected p = [];\n");

        $this->assertMatchesRegularExpression('/if \\(Z_TYPE_P\\(&_\\d+\\) == IS_NULL\\) \\{/', $generated);
        $this->assertStringNotContainsString('ZEPHIR_IS_EQUAL_NULL', $generated);
        $this->assertStringNotContainsString('loose-null-comparison', $output);
    }

    public function testSwitchCaseNullWarnsAtTheCase(): void
    {
        [$generated, $output] = $this->generateClass(
            'i2820switch',
            "    public static function f(var x) -> int\n    {\n"
            . "        switch x {\n            case null:\n                return 1;\n        }\n\n        return 0;\n    }\n"
        );

        $this->assertStringContainsString('ZEPHIR_IS_EQUAL_NULL(', $generated);
        $this->assertStringContainsString('loose-null-comparison', $output);
        $this->assertStringContainsString('sample.zep on line 8', $output);
    }

    /**
     * @return array{0: string, 1: string} the generated C and the compiler output
     */
    private function generateOk(
        string $name,
        string $parameter,
        string $comparison,
        string $flags = '',
        string $preamble = ''
    ): array {
        return $this->generateClass(
            $name,
            '    public static function f(' . $parameter . ") -> bool\n    {\n"
            . '        ' . $preamble . 'return ' . $comparison . ";\n    }\n",
            $flags
        );
    }

    /**
     * @return array{0: string, 1: string} the generated C and the compiler output
     */
    private function generateClass(string $name, string $members, string $flags = ''): array
    {
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        $source = 'namespace ' . ucfirst($name) . ";\n\nclass Sample\n{\n" . $members . "}\n";
        file_put_contents($projectDir . '/' . $name . '/sample.zep', $source);

        $result = $this->runZephir(trim('generate --no-ansi ' . $flags), $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        $generatedFile = sprintf('%s/ext/%s/sample.zep.c', $projectDir, $name);
        $this->assertFileExists($generatedFile);

        return [(string) file_get_contents($generatedFile), $result['stdout'] . $result['stderr']];
    }
}
