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
use Zephir\Exception\CompilerException;

/**
 * Regression coverage for redeclaring a parameter as a local `var`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2009
 */
final class ParameterCollisionTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_paramcollision_test_', ['stub/issue2009']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    public function testThrowsWhenLocalVarReusesParameterName(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2009;

class Collision
{
    public static function decodes(b32)
    {
        var b32;
        let b32 = strtoupper(b32);
        return strlen(b32);
    }
}
ZEP;
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessageMatches(
            "/Variable 'b32' was already declared as a parameter of method 'decodes'/"
        );

        $this->compileSource('Stub\\Issue2009\\Collision', 'stub/issue2009/collision.zep', $zep);
    }

    public function testAllowsLocalVarWithDifferentName(): void
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2009;

class NoCollision
{
    public static function decodes(b32)
    {
        var local;
        let local = strtoupper(b32);
        return strlen(local);
    }
}
ZEP;
        $this->compileSource('Stub\\Issue2009\\NoCollision', 'stub/issue2009/nocollision.zep', $zep);

        $c = $this->generatedC('stub/issue2009/nocollision.zep');

        // Parameter must be declared as zval pointer.
        $this->assertStringContainsString('zval *b32', $c);
        // The local must be a stack zval.
        $this->assertMatchesRegularExpression('/zval [^;]*\\blocal\\b/', $c);
        // No bogus ZEPHIR_SEPARATE_PARAM(b32) — the param is not mutated here.
        $this->assertStringNotContainsString('ZEPHIR_SEPARATE_PARAM(b32)', $c);
    }
}
