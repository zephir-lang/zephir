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
 * PHP 8.5 renumbered `Attribute::TARGET_ALL` and `Attribute::IS_REPEATABLE`
 * and added `Attribute::TARGET_CONSTANT`. A value read from the PHP that runs
 * `zephir generate` is therefore wrong wherever the C is compiled against a
 * different PHP, which is the normal case for a committed `ext/` or a PECL
 * package. Every place such a constant can reach C has to write the engine's
 * `ZEND_ATTRIBUTE_*` macro instead, the way php-src's own arginfo does.
 *
 * These assertions inspect the generated C, so they catch the bug on a single
 * PHP version: generating and compiling on the same version hides it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2738
 */
final class Issue2738Test extends TestCase
{
    use RunsZephirCommands;

    private static string $generated = '';

    private static string $header = '';

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        if ('' === self::$generated) {
            self::$generated = $this->generate('attrflags', 'flags.zep', <<<'ZEP'
                namespace Attrflags;

                use Attribute as A;

                class Flags
                {
                    const TARGETS = \Attribute::TARGET_METHOD | \Attribute::IS_REPEATABLE;
                    const SINGLE = A::TARGET_ALL;
                    const LIST = [\Attribute::TARGET_CLASS, "all": \Attribute::TARGET_ALL];

                    protected untyped = \Attribute::IS_REPEATABLE;
                    protected int typed = \Attribute::TARGET_ALL;
                    protected int typedExpression = \Attribute::TARGET_CLASS | \Attribute::IS_REPEATABLE;

                    public function withDefault(int flags = \Attribute::TARGET_CLASS) -> int
                    {
                        return flags;
                    }

                    public function withUntypedDefault(flags = \Attribute::IS_REPEATABLE)
                    {
                        return flags;
                    }

                    public function returned() -> int
                    {
                        return \Attribute::TARGET_ALL;
                    }

                    public function combined() -> int
                    {
                        return \Attribute::TARGET_METHOD | \Attribute::IS_REPEATABLE;
                    }

                    public function assigned() -> var
                    {
                        int native;
                        var dynamic;

                        let native = \Attribute::TARGET_PROPERTY;
                        let dynamic = \Attribute::TARGET_PARAMETER;

                        return [native, dynamic];
                    }

                    public function compared(int flags) -> bool
                    {
                        return (flags & \Attribute::IS_REPEATABLE) == \Attribute::IS_REPEATABLE;
                    }

                    public function described() -> string
                    {
                        return "flags=" . \Attribute::TARGET_ALL;
                    }
                }
                ZEP);
        }
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    /** @return array<string, array{0: string}> */
    public static function emittedLineProvider(): array
    {
        return [
            'folded class constant' => [
                'zephir_declare_class_constant_long(attrflags_flags_ce, SL("TARGETS"), ZEND_ATTRIBUTE_TARGET_METHOD | ZEND_ATTRIBUTE_IS_REPEATABLE);',
            ],
            'aliased class constant' => [
                'zephir_declare_class_constant_long(attrflags_flags_ce, SL("SINGLE"), ZEND_ATTRIBUTE_TARGET_ALL);',
            ],
            'untyped property default' => [
                'zend_declare_property_long(attrflags_flags_ce, SL("untyped"), ZEND_ATTRIBUTE_IS_REPEATABLE,',
            ],
            'typed property default' => ['ZEND_ATTRIBUTE_TARGET_ALL);'],
            'typed property expression' => ['ZEND_ATTRIBUTE_TARGET_CLASS | ZEND_ATTRIBUTE_IS_REPEATABLE);'],
            'returned constant' => ['RETURN_LONG(ZEND_ATTRIBUTE_TARGET_ALL);'],
            'returned expression' => ['(ZEND_ATTRIBUTE_TARGET_METHOD | ZEND_ATTRIBUTE_IS_REPEATABLE)'],
            'assigned to an int' => ['native = ZEND_ATTRIBUTE_TARGET_PROPERTY;'],
            'assigned to a var' => ['ZVAL_LONG(&dynamic, ZEND_ATTRIBUTE_TARGET_PARAMETER);'],
            'int parameter default in the body' => ['flags = ZEND_ATTRIBUTE_TARGET_CLASS;'],
            'untyped parameter default in the body' => ['ZVAL_LONG(flags, ZEND_ATTRIBUTE_IS_REPEATABLE);'],
        ];
    }

    /**
     * @dataProvider emittedLineProvider
     */
    public function testAttributeConstantReachesCAsAnEngineMacro(string $line): void
    {
        $this->assertStringContainsString($line, self::$generated);
    }

    /**
     * No emitter may fall back to the number the generating PHP reported.
     */
    public function testNoFrozenFlagValueIsEmitted(): void
    {
        $frozen = [
            \Attribute::TARGET_ALL,
            \Attribute::IS_REPEATABLE,
            \Attribute::TARGET_METHOD | \Attribute::IS_REPEATABLE,
            \Attribute::TARGET_CLASS | \Attribute::IS_REPEATABLE,
        ];

        foreach ($frozen as $value) {
            $this->assertDoesNotMatchRegularExpression('/[(, ]' . $value . '[);,]/', self::$generated);
        }
    }

    /**
     * An internal function's default is a PHP expression string the engine
     * evaluates when reflection asks for it, so it follows the running PHP
     * exactly, `TARGET_CONSTANT` included.
     */
    public function testArgInfoDefaultIsTheQualifiedPhpExpression(): void
    {
        $this->assertStringContainsString(
            'ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, flags, IS_LONG, 0, "\\\\Attribute::TARGET_CLASS")',
            self::$header
        );
    }

    /**
     * Concatenation turns the int into its decimal string at run time; it
     * must never quote the macro name into a C string literal.
     */
    public function testConcatenatedConstantIsNotQuotedIntoAStringLiteral(): void
    {
        $this->assertStringNotContainsString('"ZEND_ATTRIBUTE_', self::$generated);
        $this->assertStringNotContainsString('flags=ZEND_ATTRIBUTE_', self::$generated);
    }

    /**
     * Scaffolds a throwaway project, drops a single `.zep` in it, runs
     * `zephir generate` and returns the generated C source.
     */
    private function generate(string $project, string $fileName, string $source): string
    {
        $projectDir = $this->outputDir() . '/' . $project;
        $this->cleanupPath($projectDir);

        $this->assertSame(
            0,
            $this->runZephir('init ' . $project, $this->outputDir())['exitCode'],
        );

        file_put_contents($projectDir . '/' . $project . '/' . $fileName, $source . "\n");

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr'] . $result['stdout']);

        $generatedFile = sprintf(
            '%s/ext/%s/%s.c',
            $projectDir,
            $project,
            basename($fileName, '.zep') . '.zep',
        );
        $this->assertFileExists($generatedFile);
        self::$header = (string) file_get_contents(substr($generatedFile, 0, -1) . 'h');

        return (string) file_get_contents($generatedFile);
    }
}
