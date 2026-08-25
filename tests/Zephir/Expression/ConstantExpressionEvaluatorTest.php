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

namespace Zephir\Test\Expression;

use PHPUnit\Framework\TestCase;
use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;
use Zephir\Expression\ConstantExpressionEvaluator;
use Zephir\Parser\Php\PhpParser;

/**
 * Folding of constant expressions in declaration position (zephir#2061).
 *
 * Nodes are built by parsing real Zephir source with the built-in parser, so a
 * case reads as the source a user writes and the operator precedence under test
 * is the parser's, not the test's.
 */
final class ConstantExpressionEvaluatorTest extends TestCase
{
    public static function foldProvider(): iterable
    {
        // arithmetic, including the issue's own expression
        yield '-0x7f - 1'    => ['-0x7f - 1', 'int', '-128'];
        yield '1024 * 8'     => ['1024 * 8', 'int', '8192'];
        yield '0xff + 1'     => ['0xff + 1', 'int', '256'];
        yield '7 - 10'       => ['7 - 10', 'int', '-3'];
        yield '4 / 2'        => ['4 / 2', 'int', '2'];
        yield '5 / 2'        => ['5 / 2', 'double', '2.5'];
        yield '7 % 3'        => ['7 % 3', 'int', '1'];
        yield '2.5 * 2'      => ['2.5 * 2', 'double', '5.0'];
        yield '0.1 + 0.2'    => ['0.1 + 0.2', 'double', '0.30000000000000004'];

        // precedence and grouping — `(expr)` parses to a `list` node
        yield '1 + 2 * 3'    => ['1 + 2 * 3', 'int', '7'];
        yield '(1 + 2) * 3'  => ['(1 + 2) * 3', 'int', '9'];
        yield '-(2 + 3)'     => ['-(2 + 3)', 'int', '-5'];

        // bitwise
        yield '0xff << 8 | 0x0f' => ['0xff << 8 | 0x0f', 'int', '65295'];
        yield '6 & 3'            => ['6 & 3', 'int', '2'];
        yield '6 ^ 3'            => ['6 ^ 3', 'int', '5'];
        yield '256 >> 4'         => ['256 >> 4', 'int', '16'];
        yield '~0'               => ['~0', 'int', '-1'];

        // concat
        yield '"a" . "b"' => ['"a" . "b"', 'string', 'ab'];
        yield '"a" . 1'   => ['"a" . 1', 'string', 'a1'];

        // comparison and logical — PHP semantics, including loose equality
        yield '1 < 2'          => ['1 < 2', 'bool', 'true'];
        yield '1 == "1"'       => ['1 == "1"', 'bool', 'true'];
        yield '1 === "1"'      => ['1 === "1"', 'bool', 'false'];
        yield '2 != 2'         => ['2 != 2', 'bool', 'false'];
        yield '2 !== 2'        => ['2 !== 2', 'bool', 'false'];
        yield '3 >= 3'         => ['3 >= 3', 'bool', 'true'];
        yield '3 <= 2'         => ['3 <= 2', 'bool', 'false'];
        yield '4 > 5'          => ['4 > 5', 'bool', 'false'];
        yield 'true && false'  => ['true && false', 'bool', 'false'];
        yield 'false || true'  => ['false || true', 'bool', 'true'];
        yield '!false'         => ['!false', 'bool', 'true'];

        // ternary
        yield '1 < 2 ? 10 : 20' => ['1 < 2 ? 10 : 20', 'int', '10'];
        yield 'false ? 1 : 2'   => ['false ? 1 : 2', 'int', '2'];
        yield '0 ?: 5'          => ['0 ?: 5', 'int', '5'];
        yield '9 ?: 5'          => ['9 ?: 5', 'int', '9'];

        // a char literal is its byte value, as everywhere else in Zephir
        yield "'a' + 1" => ["'a' + 1", 'int', '98'];

        // null and bool coerce the way PHP coerces them
        yield 'null + 1'  => ['null + 1', 'int', '1'];
        yield 'true + 1'  => ['true + 1', 'int', '2'];
    }

    /**
     * @dataProvider foldProvider
     */
    public function testFoldsExpressionToLiteral(string $source, string $type, string $value): void
    {
        $folded = (new ConstantExpressionEvaluator())->fold(
            $this->nodeFor($source),
            $this->createMock(CompilationContext::class)
        );

        $this->assertSame(['type' => $type, 'value' => $value], $folded);
    }

    public function testShortCircuitsTernarySoTheDeadBranchIsNeverEvaluated(): void
    {
        $folded = (new ConstantExpressionEvaluator())->fold(
            $this->nodeFor('true ? 1 : 1 / 0'),
            $this->createMock(CompilationContext::class)
        );

        $this->assertSame(['type' => 'int', 'value' => '1'], $folded);
    }

    public function testShortCircuitsLogicalOperators(): void
    {
        $evaluator = new ConstantExpressionEvaluator();
        $context   = $this->createMock(CompilationContext::class);

        $this->assertSame(
            ['type' => 'bool', 'value' => 'false'],
            $evaluator->fold($this->nodeFor('false && 1 / 0'), $context)
        );
        $this->assertSame(
            ['type' => 'bool', 'value' => 'true'],
            $evaluator->fold($this->nodeFor('true || 1 / 0'), $context)
        );
    }

    public function testIntegerOverflowBecomesADouble(): void
    {
        $folded = (new ConstantExpressionEvaluator())->fold(
            $this->nodeFor('9223372036854775807 + 1'),
            $this->createMock(CompilationContext::class)
        );

        $this->assertSame('double', $folded['type']);
        $this->assertSame(PHP_INT_MAX + 1, (float) $folded['value']);
    }

    public function testFoldsExpressionsInsideAnArray(): void
    {
        $folded = (new ConstantExpressionEvaluator())->fold(
            $this->nodeFor('[1 + 1, "k": 2 * 2, "plain"]'),
            $this->createMock(CompilationContext::class)
        );

        $this->assertSame('array', $folded['type']);
        $this->assertSame(['type' => 'int', 'value' => '2'], $folded['left'][0]['value']);
        $this->assertSame(['type' => 'int', 'value' => '4'], $folded['left'][1]['value']);
        $this->assertSame('k', $folded['left'][1]['key']['value']);
        $this->assertSame('plain', $folded['left'][2]['value']['value']);
    }

    public function testUnwrapsAParenthesisedArray(): void
    {
        $folded = (new ConstantExpressionEvaluator())->fold(
            $this->nodeFor('([1 + 1])'),
            $this->createMock(CompilationContext::class)
        );

        $this->assertSame('array', $folded['type']);
        $this->assertSame(['type' => 'int', 'value' => '2'], $folded['left'][0]['value']);
    }

    public function testKeepsALiteralNodeUntouched(): void
    {
        $node   = $this->nodeFor('10');
        $folded = (new ConstantExpressionEvaluator())->fold(
            $node,
            $this->createMock(CompilationContext::class)
        );

        $this->assertSame($node, $folded);
    }

    public static function needsFoldingProvider(): iterable
    {
        yield 'literal int'            => ['10', false];
        yield 'literal string'         => ['"a"', false];
        yield 'array of literals'      => ['[1, 2]', false];
        yield 'empty array'            => ['[]', false];
        yield 'php constant'           => ['PHP_INT_SIZE', false];
        yield 'static constant access' => ['self::OTHER', false];
        yield 'arithmetic'             => ['1 + 1', true];
        yield 'grouped literal'        => ['(1)', true];
        yield 'array with expression'  => ['[1 + 1]', true];
    }

    /**
     * @dataProvider needsFoldingProvider
     */
    public function testNeedsFoldingIdentifiesWhatCallersMustFold(string $source, bool $expected): void
    {
        $this->assertSame($expected, ConstantExpressionEvaluator::needsFolding($this->nodeFor($source)));
    }

    public static function rejectionProvider(): iterable
    {
        yield 'division by zero'      => ['1 / 0', 'Division by zero'];
        yield 'modulo by zero'        => ['1 % 0', 'Modulo by zero'];
        yield 'negative shift'        => ['1 << -1', 'negative number'];
        yield 'non-numeric string'    => ['"abc" * 2', 'Unsupported operand types'];
        yield 'char in concat'        => ["'a' . \"b\"", 'char'];
        yield 'array as operand'      => ['[1] + 2', 'array cannot be used as an operand'];
        yield 'function call'         => ['strlen("a")', 'not a compile-time constant'];
        yield 'variable'              => ['someVariable', 'not a compile-time constant'];
        yield 'object instantiation'  => ['new stdClass()', 'not a compile-time constant'];
    }

    /**
     * @dataProvider rejectionProvider
     */
    public function testRejectsWhatCannotBeFolded(string $source, string $expectedMessage): void
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessageMatches('/' . preg_quote($expectedMessage, '/') . '/i');

        (new ConstantExpressionEvaluator())->fold(
            $this->nodeFor($source),
            $this->createMock(CompilationContext::class)
        );
    }

    public function testRejectsANonFiniteResult(): void
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessageMatches('/finite/i');

        // 1.0E+308 * 10 overflows to INF, which has no C literal spelling.
        // Built by hand: the lexer has no exponent form for doubles.
        (new ConstantExpressionEvaluator())->fold(
            [
                'type'  => 'mul',
                'left'  => ['type' => 'double', 'value' => '1.0E+308'],
                'right' => ['type' => 'double', 'value' => '10.0'],
                'file'  => 'c.zep',
                'line'  => 1,
            ],
            $this->createMock(CompilationContext::class)
        );
    }

    /**
     * Parses `const X = <source>;` and returns the initializer node.
     */
    private function nodeFor(string $source): array
    {
        $code = "namespace T;\n\nclass C\n{\n\tconst X = {$source};\n}\n";
        $ir   = (new PhpParser())->parse($code, 'c.zep');

        foreach ($ir as $statement) {
            if ('class' === $statement['type']) {
                return $statement['definition']['constants'][0]['default'];
            }
        }

        self::fail('Unable to parse: ' . $source);
    }
}
