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
 * Expression initializers must reach the class-entry emitters already folded to
 * a literal, so the declaration is a plain constant in the generated C.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2061
 */
final class ConstantExpressionCodeGenTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_constexpr_test_', ['stub/issue2061']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    public static function declarationProvider(): iterable
    {
        yield 'issue expression'  => ['/zephir_declare_class_constant_long\([^,]+, SL\("INT8_MIN"\), -128\);/'];
        yield 'bitmask'           => ['/zephir_declare_class_constant_long\([^,]+, SL\("MASK"\), 65295\);/'];
        yield 'grouped division'  => ['/zephir_declare_class_constant_double\([^,]+, SL\("RATIO"\), 2\.5\);/'];
        yield 'concat'            => ['/zephir_declare_class_constant_string\([^,]+, SL\("NAME"\), "ab"\);/'];
        yield 'comparison'        => ['/zephir_declare_class_constant_bool\([^,]+, SL\("FLAG"\), 1\);/'];
        yield 'reads own const'   => ['/zephir_declare_class_constant_long\([^,]+, SL\("DOUBLED"\), 16384\);/'];
        yield 'property default'  => ['/SL\("size"\), 8192/'];
    }

    /**
     * @dataProvider declarationProvider
     */
    public function testFoldedDeclarationIsEmittedAsALiteral(string $expected): void
    {
        $this->assertMatchesRegularExpression($expected, $this->compileFixture());
    }

    public function testNoOperatorNodeLeaksIntoTheEmittedSymbolName(): void
    {
        $c = $this->compileFixture();

        foreach (['_sub(', '_add(', '_mul(', '_bitwise_or(', '_concat(', '_list('] as $leaked) {
            $this->assertStringNotContainsString(
                'zephir_declare_class_constant' . $leaked,
                $c,
                'An unfolded operator node reached the constant emitter.'
            );
        }
    }

    private function compileFixture(): string
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2061;

final class Limits
{
    const INT8_MIN = -0x7f - 1;
    const MASK = 0xff << 8 | 0x0f;
    const RATIO = (1 + 4) / 2;
    const NAME = "a" . "b";
    const FLAG = 1 < 2;
    const SIZE = 1024 * 8;
    const DOUBLED = self::SIZE * 2;

    public size = 1024 * 8;
}
ZEP;
        $this->compileSource('Stub\\Issue2061\\Limits', 'stub/issue2061/limits.zep', $zep);

        return $this->generatedC('stub/issue2061/limits.zep');
    }
}
