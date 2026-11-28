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

use FilesystemIterator;
use PDO;
use PHPUnit\Framework\TestCase;

/**
 * A constant of a PHP class used to be replaced with its value in the PHP
 * that runs `zephir generate`. One generated `ext/` is compiled for many PHP
 * versions, and PHP 8.5 renumbered the `PDO::FETCH_*` flags (PHP 8.1 the
 * `FilesystemIterator` ones), so `fetchAll()` threw a ValueError.
 *
 * A constant that has an engine macro is emitted as that macro, in every
 * context, with the header that defines it. Any other constant of a class
 * this project does not compile is read when the code runs, as PHP does.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2869
 *
 * @requires extension pdo
 */
final class Issue2869Test extends TestCase
{
    use RunsZephirCommands;

    private const PROJECT = 'phpconsts';

    private const PDO_HEADER = '#include "ext/pdo/php_pdo_driver.h"';

    private const SPL_HEADER = '#include "ext/spl/spl_directory.h"';

    /** @var array<string, string> generated `.zep.c` and `.zep.h` contents, keyed by file name */
    private static array $generated = [];

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        if ([] !== self::$generated) {
            return;
        }

        self::$generated = $this->generate(self::PROJECT, [
            // Sorts before `flags.zep`, so it may read Flags::GROUP before Flags is compiled.
            'early.zep' => <<<'ZEP'
                namespace Phpconsts;

                class Early
                {
                    public function group() -> int
                    {
                        return Flags::GROUP;
                    }
                }
                ZEP,
            'flags.zep' => <<<'ZEP'
                namespace Phpconsts;

                class Flags
                {
                    const GROUP = \PDO::FETCH_GROUP;
                    const UNIQUE_ASSOC = \PDO::FETCH_ASSOC | \PDO::FETCH_UNIQUE;
                    const LIST = [\PDO::FETCH_CLASSTYPE, "late": \PDO::FETCH_PROPS_LATE];
                    const SYMLINKS = \RecursiveDirectoryIterator::FOLLOW_SYMLINKS;
                    const ASSOC = \PDO::FETCH_ASSOC;

                    protected serialize = \PDO::FETCH_SERIALIZE;
                    protected int mask = \FilesystemIterator::OTHER_MODE_MASK;

                    public function withDefault(int flags = \PDO::FETCH_GROUP) -> int
                    {
                        return flags;
                    }

                    public function grouped(int flags) -> array
                    {
                        return [\PDO::FETCH_GROUP | flags];
                    }

                    public function inverted() -> int
                    {
                        return ~\PDO::FETCH_GROUP;
                    }

                    public function assoc() -> int
                    {
                        return \PDO::FETCH_ASSOC;
                    }

                    public function props() -> var
                    {
                        var flags;

                        let flags = \ArrayObject::ARRAY_AS_PROPS;

                        return flags;
                    }
                }
                ZEP,
            'connection.zep' => <<<'ZEP'
                namespace Phpconsts;

                class Connection extends \PDO
                {
                    public static function unique() -> int
                    {
                        return self::FETCH_UNIQUE;
                    }

                    public static function group() -> int
                    {
                        return parent::FETCH_GROUP;
                    }

                    public static function assoc() -> int
                    {
                        return self::FETCH_ASSOC;
                    }

                    public static function late() -> int
                    {
                        return static::FETCH_GROUP;
                    }
                }
                ZEP,
            'reader.zep' => <<<'ZEP'
                namespace Phpconsts;

                class Reader
                {
                    public function group() -> int
                    {
                        return Flags::GROUP;
                    }

                    public function listed() -> array
                    {
                        return Flags::LIST;
                    }
                }
                ZEP,
        ]);
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    /** @return array<string, array{0: string, 1: string}> */
    public static function emittedLineProvider(): array
    {
        return [
            'class constant' => [
                'flags.zep.c',
                'zephir_declare_class_constant_long(phpconsts_flags_ce, SL("GROUP"), ((zend_long) PDO_FETCH_GROUP));',
            ],
            'class constant expression' => [
                'flags.zep.c',
                'zephir_declare_class_constant_long(phpconsts_flags_ce, SL("UNIQUE_ASSOC"), 2 | ((zend_long) PDO_FETCH_UNIQUE));',
            ],
            'array class constant element' => ['flags.zep.c', 'add_next_index_long(&_zc0, ((zend_long) PDO_FETCH_CLASSTYPE));'],
            'array class constant keyed element' => [
                'flags.zep.c',
                'add_assoc_long_ex(&_zc0, SL("late"), ((zend_long) PDO_FETCH_PROPS_LATE));',
            ],
            'constant inherited by a subclass of the declaring class' => [
                'flags.zep.c',
                'zephir_declare_class_constant_long(phpconsts_flags_ce, SL("SYMLINKS"), SPL_FILE_DIR_FOLLOW_SYMLINKS);',
            ],
            'untyped property default' => [
                'flags.zep.c',
                'zend_declare_property_long(phpconsts_flags_ce, SL("serialize"), ((zend_long) PDO_FETCH_SERIALIZE),',
            ],
            'typed property default' => ['flags.zep.c', 'SPL_FILE_DIR_OTHERS_MASK'],
            'int parameter default in the body' => ['flags.zep.c', 'flags = ((zend_long) PDO_FETCH_GROUP);'],
            'the issue expression' => ['flags.zep.c', '(flags | ((zend_long) PDO_FETCH_GROUP))'],
            'bitwise not of an unsigned macro' => ['flags.zep.c', 'RETURN_LONG(~(((zend_long) PDO_FETCH_GROUP)));'],
            'self:: in a subclass' => ['connection.zep.c', 'RETURN_LONG(((zend_long) PDO_FETCH_UNIQUE));'],
            'parent:: in a subclass' => ['connection.zep.c', 'RETURN_LONG(((zend_long) PDO_FETCH_GROUP));'],
            'Zephir constant read from a file compiled first' => ['early.zep.c', '((zend_long) PDO_FETCH_GROUP)'],
            'Zephir constant read from a file compiled later' => ['reader.zep.c', '((zend_long) PDO_FETCH_GROUP)'],
            'Zephir array constant taken in place' => ['reader.zep.c', '((zend_long) PDO_FETCH_CLASSTYPE)'],
        ];
    }

    /**
     * @dataProvider emittedLineProvider
     */
    public function testVersionDependentConstantReachesCAsAnEngineMacro(string $file, string $line): void
    {
        $this->assertStringContainsString($line, self::$generated[$file]);
    }

    /** @return array<string, array{0: string, 1: string}> */
    public static function headerProvider(): array
    {
        return [
            'PDO header where the class declares the flags' => ['flags.zep.c', self::PDO_HEADER],
            'SPL header where the class declares the flags' => ['flags.zep.c', self::SPL_HEADER],
            'PDO header in a subclass' => ['connection.zep.c', self::PDO_HEADER],
            'PDO header in a file compiled first' => ['early.zep.c', self::PDO_HEADER],
            'PDO header in a file compiled later' => ['reader.zep.c', self::PDO_HEADER],
        ];
    }

    /**
     * Each translation unit that names a macro includes the header that
     * defines it, never relying on another file having compiled first.
     *
     * @dataProvider headerProvider
     */
    public function testFileThatNamesAMacroIncludesItsHeader(string $file, string $include): void
    {
        $this->assertStringContainsString($include, self::$generated[$file]);
    }

    public function testFileWithoutAMacroDoesNotIncludeTheHeader(): void
    {
        $this->assertStringNotContainsString(self::SPL_HEADER, self::$generated['connection.zep.c']);
    }

    /**
     * No emitter may fall back to the number the generating PHP reported.
     * Small numbers are skipped, as they would also match unrelated code.
     */
    public function testNoFrozenFlagValueIsEmitted(): void
    {
        $frozen = [
            PDO::FETCH_GROUP,
            PDO::FETCH_UNIQUE,
            PDO::FETCH_CLASSTYPE,
            PDO::FETCH_PROPS_LATE,
            PDO::FETCH_SERIALIZE,
            PDO::FETCH_ASSOC | PDO::FETCH_UNIQUE,
            ~PDO::FETCH_GROUP,
            FilesystemIterator::FOLLOW_SYMLINKS,
            FilesystemIterator::OTHER_MODE_MASK,
        ];
        $code = implode("\n", self::$generated);

        foreach ($frozen as $value) {
            if (abs($value) < 100) {
                continue;
            }

            $this->assertDoesNotMatchRegularExpression('/[(, ]' . $value . '[);,]/', $code);
        }
    }

    /**
     * An internal function's default is a PHP expression string the engine
     * evaluates when reflection asks for it, so it follows the running PHP.
     */
    public function testArgInfoDefaultIsTheQualifiedPhpExpression(): void
    {
        $this->assertStringContainsString(
            'ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, flags, IS_LONG, 0, "\\\\PDO::FETCH_GROUP")',
            self::$generated['flags.zep.h']
        );
    }

    /** @return array<string, array{0: string, 1: string}> */
    public static function runtimeReadProvider(): array
    {
        return [
            'PDO constant without a macro' => [
                'flags.zep.c',
                'zephir_fetch_class_str_ex(SL("PDO"), ZEND_FETCH_CLASS_DEFAULT | ZEND_FETCH_CLASS_EXCEPTION), SL("FETCH_ASSOC"));',
            ],
            'core class constant' => [
                'flags.zep.c',
                'zephir_fetch_class_str_ex(SL("ArrayObject"), ZEND_FETCH_CLASS_DEFAULT | ZEND_FETCH_CLASS_EXCEPTION), SL("ARRAY_AS_PROPS"));',
            ],
            'constant a Zephir subclass inherits' => [
                'connection.zep.c',
                'phpconsts_connection_ce, SL("FETCH_ASSOC"));',
            ],
            // A PHP subclass may redeclare it, so a macro would be wrong here.
            'static:: on a flag with a macro' => [
                'connection.zep.c',
                'zephir_get_called_scope(execute_data), SL("FETCH_GROUP"));',
            ],
        ];
    }

    /**
     * The value of a constant without a macro may differ between the PHP
     * that runs Zephir and the one that runs the extension (a library
     * version, another extension), so it is read like PHP reads it.
     *
     * @dataProvider runtimeReadProvider
     */
    public function testOtherConstantOfAPhpClassIsReadWhenTheCodeRuns(string $file, string $line): void
    {
        $this->assertStringContainsString('zephir_get_class_constant(', self::$generated[$file]);
        $this->assertStringContainsString($line, self::$generated[$file]);
    }

    /**
     * A declaration has no run time to read in: its value is still folded.
     */
    public function testDeclarationOfAnotherConstantIsStillFolded(): void
    {
        $this->assertStringContainsString(
            'zephir_declare_class_constant_long(phpconsts_flags_ce, SL("ASSOC"), ' . PDO::FETCH_ASSOC . ');',
            self::$generated['flags.zep.c']
        );
    }

    /**
     * Arithmetic on a macro in a declaration would need the value at
     * generate time, the #2738 rule for version-dependent constants.
     */
    public function testArithmeticOnAMacroInADeclarationIsACompileError(): void
    {
        $projectDir = $this->outputDir() . '/phpconstsbad';
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init phpconstsbad', $this->outputDir())['exitCode']);
        file_put_contents($projectDir . '/phpconstsbad/bad.zep', <<<'ZEP'
            namespace Phpconstsbad;

            class Bad
            {
                const NEXT = \PDO::FETCH_GROUP + 1;
            }

            ZEP);

        $result = $this->runZephir('generate --no-ansi', $projectDir);

        $this->assertNotSame(0, $result['exitCode']);
        $this->assertStringContainsString(
            'can only be combined with |, &, ^ and ~',
            $result['stderr'] . $result['stdout']
        );
    }

    /**
     * Scaffolds a throwaway project, drops the `.zep` files in it, runs
     * `zephir generate` and returns the generated C sources and headers.
     *
     * @param array<string, string> $files
     *
     * @return array<string, string>
     */
    private function generate(string $project, array $files): array
    {
        $projectDir = $this->outputDir() . '/' . $project;
        $this->removePath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $project, $this->outputDir())['exitCode']);

        foreach ($files as $fileName => $source) {
            file_put_contents($projectDir . '/' . $project . '/' . $fileName, $source . "\n");
        }

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr'] . $result['stdout']);

        $generated = [];
        foreach (array_keys($files) as $fileName) {
            foreach (['c', 'h'] as $extension) {
                $name = basename($fileName, '.zep') . '.zep.' . $extension;
                $path = sprintf('%s/ext/%s/%s', $projectDir, $project, $name);
                $this->assertFileExists($path);
                $generated[$name] = (string) file_get_contents($path);
            }
        }

        $this->removePath($projectDir);

        return $generated;
    }
}
