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

use function addcslashes;
use function file_get_contents;

use const PHP_EOL;

/**
 * A class constant, `X::class` or PHP constant is resolved in every
 * declaration PHP allows it in: array constant elements and keys, typed and
 * trait property defaults, as PHP evaluates them at compile time.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2825
 */
final class Issue2825DeclarationTest extends TestCase
{
    use GeneratesStubProject;

    protected function setUp(): void
    {
        $this->setUpStubProject();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testArrayConstantElementsAndKeys(): void
    {
        $generated = $this->generateOk('decl.zep', <<<'ZEP'
            namespace Stub;

            class Decl
            {
                const A = 1;
                const BASE = [1, 2];
                const LIST = [self::A, self::class, \ArrayObject::STD_PROP_LIST, [self::A]];
                const MAP = [self::class: 1, self::A: "one"];
                const NESTED = [self::BASE, 3];
            }
            ZEP);

        $this->assertStringContainsString('add_next_index_long(&', $generated);
        $this->assertStringContainsString('add_next_index_stringl(&_zc0, SL("Stub\\\\Decl"));', $generated);
        $this->assertStringContainsString('add_assoc_long_ex(&', $generated);
        $this->assertStringContainsString('SL("Stub\\\\Decl"), 1);', $generated);
        $this->assertStringContainsString('add_index_stringl(&', $generated);
        $this->assertStringContainsString(', 1, SL("one"));', $generated);
    }

    public function testNativeConstantAsArrayKeyKeepsItsMacro(): void
    {
        $generated = $this->generateOk('decl.zep', <<<'ZEP'
            namespace Stub;

            class Decl
            {
                const FLAGS = [\Attribute::TARGET_CLASS: "class"];
            }
            ZEP);

        $this->assertStringContainsString('(zend_long) (ZEND_ATTRIBUTE_TARGET_CLASS), SL("class"));', $generated);
    }

    public function testTypedAndTraitPropertyDefaults(): void
    {
        $result = $this->generate([
            'decl.zep' => <<<'ZEP'
                namespace Stub;

                class Decl
                {
                    const A = 7;

                    public int number = self::A;
                    public string name = Decl::class;
                    public array list = [self::A, Decl::class];
                    public eol = PHP_EOL;
                }
                ZEP,
            'tr.zep' => <<<'ZEP'
                namespace Stub;

                trait Tr
                {
                    public list = [Decl::A, Decl::class];
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $decl = (string) file_get_contents($this->projectDir . '/ext/stub/decl.zep.c');
        $this->assertStringNotContainsString('static-constant-access', $decl);
        $this->assertStringContainsString('"Stub\\\\Decl"', $decl);
        // PHP_EOL folds to the value of the PHP running Zephir: "\r\n" on Windows.
        $this->assertStringContainsString('SL("eol"), "' . addcslashes(PHP_EOL, "\r\n") . '"', $decl);
    }

    public function testCharConstantAsPropertyDefault(): void
    {
        $generated = $this->generateOk('decl.zep', <<<'ZEP'
            namespace Stub;

            class Decl
            {
                const PATTERN = '[a-z]+';

                public pattern = self::PATTERN;
            }
            ZEP);

        $this->assertStringContainsString('zend_declare_property_string(stub_decl_ce, SL("pattern"), "[a-z]+"', $generated);
    }

    public function testSelfInAnotherClassConstantResolvesInItsOwnClass(): void
    {
        $result = $this->generate([
            'zed.zep' => <<<'ZEP'
                namespace Stub;

                class Zed
                {
                    const X = "zed";
                    const Y = self::X;
                }
                ZEP,
            'ord.zep' => <<<'ZEP'
                namespace Stub;

                class Ord
                {
                    public static function y() -> string { return Zed::Y; }
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertStringContainsString(
            'RETURN_STRING("zed");',
            (string) file_get_contents($this->projectDir . '/ext/stub/ord.zep.c')
        );
    }

    /**
     * @return iterable<string, array{0: string, 1: string}>
     */
    public static function staticProvider(): iterable
    {
        yield 'constant' => ['static::A', '"static::" is not allowed in compile-time constants'];
        yield 'class name' => ['static::class', 'static::class cannot be used for compile-time class name resolution'];
    }

    /**
     * @dataProvider staticProvider
     */
    public function testStaticIsRejectedInAConstant(string $value, string $error): void
    {
        $result = $this->generate(['decl.zep' => <<<ZEP
            namespace Stub;

            class Decl
            {
                const A = 1;
                const B = {$value};
            }
            ZEP]);

        $this->assertNotSame(0, $result['exitCode']);
        $this->assertStringContainsString($error, $result['stdout'] . $result['stderr']);
    }
}
