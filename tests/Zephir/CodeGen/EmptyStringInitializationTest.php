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

namespace Zephir\Test\CodeGen;

use PHPUnit\Framework\TestCase;

/**
 * Regression coverage for `let s = "";` on string-typed locals.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2393
 */
final class EmptyStringInitializationTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_emptystr_test_', ['stub/issue2393']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    public function testEmptyStringDeclarationInitializerIsEmittedBeforeSwitch(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2393;

class Decl
{
    public function getSpecial(int special) -> string
    {
        string suffix = "";

        switch special {
            case 1:
                let suffix = "alpha";
                break;
        }

        return suffix;
    }
}
ZEP;
        $c = $this->compileZep('Stub\\Issue2393\\Decl', 'stub/issue2393/decl.zep', $zep);

        $beforeSwitch = $this->sliceBetween($c, 'PHP_METHOD(Stub_Issue2393_Decl, getSpecial)', 'zephir_switch_');
        $this->assertNotSame(
            '',
            $beforeSwitch,
            'Could not locate the getSpecial body up to the switch.'
        );

        $this->assertTrue(
            str_contains($beforeSwitch, 'ZVAL_EMPTY_STRING(&suffix)')
                || str_contains($beforeSwitch, 'ZVAL_STRING(&suffix, "")'),
            "The empty-string initializer for 'suffix' must be emitted before the switch.\n"
            . "Pre-switch body:\n" . $beforeSwitch
        );
    }

    public function testExplicitLetEmptyStringIsEmitted(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2393;

class Lett
{
    public function getSpecial(int special) -> string
    {
        string suffix;
        let suffix = "";

        switch special {
            case 1:
                let suffix = "alpha";
                break;
        }

        return suffix;
    }
}
ZEP;
        $c = $this->compileZep('Stub\\Issue2393\\Lett', 'stub/issue2393/lett.zep', $zep);

        $beforeSwitch = $this->sliceBetween($c, 'PHP_METHOD(Stub_Issue2393_Lett, getSpecial)', 'zephir_switch_');
        $this->assertNotSame(
            '',
            $beforeSwitch,
            'Could not locate the getSpecial body up to the switch.'
        );

        $this->assertTrue(
            str_contains($beforeSwitch, 'ZVAL_EMPTY_STRING(&suffix)')
                || str_contains($beforeSwitch, 'ZVAL_STRING(&suffix, "")'),
            "let suffix = \"\" must emit the empty-string assignment before the switch.\n"
            . "Pre-switch body:\n" . $beforeSwitch
        );
    }

    private function compileZep(string $className, string $relPath, string $zep): string
    {
        $this->compileSource($className, $relPath, $zep);

        return $this->generatedC($relPath);
    }

    private function sliceBetween(string $haystack, string $start, string $end): string
    {
        $startPos = strpos($haystack, $start);
        if ($startPos === false) {
            return '';
        }
        $endPos = strpos($haystack, $end, $startPos);
        if ($endPos === false) {
            return '';
        }
        return substr($haystack, $startPos, $endPos - $startPos);
    }
}
