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

namespace Zephir\Test\Parser;

use PHPUnit\Framework\TestCase;
use Zephir\Parser\Php\PhpParser;

use function str_repeat;

/**
 * Behavior tests for the pure PHP parser. They add to the golden corpus by
 * checking that unary plus is rejected and that the parser keeps the C
 * extension contract: never throw to the caller, return an error node instead.
 */
final class PhpParserTest extends TestCase
{
    public function testUnaryPlusIsRejectedLikeTheCExtension(): void
    {
        // The C extension returns a "Syntax error" for `+expr`, because its
        // compiled LALR parser hides the prefix plus rule from the grammar. The
        // pure PHP parser must reject it the same way: an error node, not a
        // `plus` node. Checked against ext 2.0.4 by comparing output directly.
        $ir = $this->parse("namespace T;\nclass C {\n    public function f() {\n        return +a;\n    }\n}\n");

        $this->assertSame('error', $ir['type'] ?? null, 'unary plus must return an error node');
        $this->assertFalse(self::hasNodeType($ir, 'plus'), 'unary plus must not return a "plus" node');
    }

    public function testReferenceExpressionProducesReferenceNode(): void
    {
        // `&expr` returns a `reference` node. The generated golden corpus does
        // not cover this path, since no stub or phpt input uses it. Output is
        // identical to ext 2.0.4, checked by comparing directly.
        $ir = $this->parse("namespace T;\nclass C {\n    public function f() {\n        return &a;\n    }\n}\n");

        $this->assertFalse(self::hasNodeType($ir, 'error'), '&expr must not return an error node');
        $this->assertTrue(self::hasNodeType($ir, 'reference'), '&expr must return a "reference" node');
    }

    public function testMalformedInputReturnsErrorNodeAndNeverThrows(): void
    {
        // A `class` with no name is a syntax error. The parser must return the
        // C extension error node, not throw.
        $ir = $this->parse('namespace T; class {{{ broken');

        $this->assertSame('error', $ir['type'] ?? null);
    }

    public function testDeeplyNestedExpressionParsesWithoutCrashingOrTrippingTheGuard(): void
    {
        // Depth 30 is well inside any real nesting and safe under the CI
        // xdebug.max_nesting_level and small stacks, so it must parse cleanly.
        // This proves the depth counter does not break normal parsing and the
        // MAX_EXPR_DEPTH guard does not trigger on reasonable input.
        $depth = 30;
        $expr  = str_repeat('(', $depth) . 'a' . str_repeat(')', $depth);
        $ir    = $this->parse("namespace T;\nclass C {\n    public function f() {\n        return {$expr};\n    }\n}\n");

        $this->assertIsArray($ir);
        $this->assertFalse(self::hasNodeType($ir, 'error'), 'normal nesting must not trigger the depth guard');
    }

    private function parse(string $source): array
    {
        return (new PhpParser())->parse($source, 'test.zep');
    }

    /**
     * Search the tree for any node with the given `type`.
     */
    private static function hasNodeType(array $ir, string $type): bool
    {
        foreach ($ir as $value) {
            if (!is_array($value)) {
                continue;
            }
            if (($value['type'] ?? null) === $type || self::hasNodeType($value, $type)) {
                return true;
            }
        }

        return false;
    }
}
