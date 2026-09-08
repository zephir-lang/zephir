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
 * Locks the codegen for `for` loops that iterate the `..` / `...` range
 * operators. Before #2433, `for i in 0..n` desugared to `(array) range(0, n)`
 * and was iterated with `ZEND_HASH_FOREACH`, i.e. it materialised an
 * intermediate array — even though the identical `for i in range(0, n)`
 * already compiled to a bare integer counting loop via
 * `ForStatement::compileRange()`. This test pins the operator form to the
 * same counting-loop codegen (no array, no `range()` call, no hash foreach).
 *
 * @see https://github.com/zephir-lang/zephir/issues/2433
 */
final class RangeOperatorLoopTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_range_operator_test_', ['stub/issue2433']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    public function testInclusiveRangeOperatorCompilesToCountingLoop(): void
    {
        $c = $this->compileSingle('InclusiveLoop', 'inclusiveloop.zep', <<<'ZEP'
namespace Stub\Issue2433;

class InclusiveLoop
{
    public function sum() -> int
    {
        int total = 0, i = 0;
        for i in 0..10 {
            let total += i;
        }
        return total;
    }
}
ZEP);

        $this->assertStringContainsString(
            'while (1) {',
            $c,
            'The `..` range loop must emit the integer counting loop (`while (1)`) used by compileRange().'
        );

        $this->assertStringNotContainsString(
            'ZEND_HASH_FOREACH',
            $c,
            'The `..` range loop must not materialise an array and iterate it with ZEND_HASH_FOREACH.'
        );

        $this->assertStringNotContainsString(
            '"range"',
            $c,
            'The `..` range loop must not emit a runtime range() function call.'
        );
    }

    public function testExclusiveRangeOperatorCompilesToCountingLoop(): void
    {
        $c = $this->compileSingle('ExclusiveLoop', 'exclusiveloop.zep', <<<'ZEP'
namespace Stub\Issue2433;

class ExclusiveLoop
{
    public function sum() -> int
    {
        int total = 0, i = 0;
        for i in 0...10 {
            let total += i;
        }
        return total;
    }
}
ZEP);

        $this->assertStringContainsString(
            'while (1) {',
            $c,
            'The `...` range loop must emit the integer counting loop (`while (1)`) used by compileRange().'
        );

        $this->assertStringNotContainsString(
            'ZEND_HASH_FOREACH',
            $c,
            'The `...` range loop must not materialise an array and iterate it with ZEND_HASH_FOREACH.'
        );
    }

    private function compileSingle(string $simpleClassName, string $fileName, string $zep): string
    {
        $srcRel = 'stub/issue2433/' . $fileName;

        $this->compileSource('Stub\\Issue2433\\' . $simpleClassName, $srcRel, $zep);

        return $this->generatedC($srcRel);
    }
}
