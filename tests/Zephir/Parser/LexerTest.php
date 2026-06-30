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
use Zephir\Parser\Php\Lexer;
use Zephir\Parser\Php\TokenType;

/**
 * Unit tests for the pure-PHP lexer port of scanner.re, focused on the
 * position arithmetic (active_line/active_char) that drives node-position
 * parity, and on the trickier rules (negative-literal disambiguation,
 * case-insensitive keywords, docblock value/position).
 */
final class LexerTest extends TestCase
{
    /**
     * Each token records the scanner position immediately after it is consumed
     * — the value the parser stamps as a node's (line, char) lookahead.
     */
    public function testNamespaceClassPositions(): void
    {
        $tokens = (new Lexer('namespace Acme; class Foo {}'))->tokenize();

        $actual = [];
        foreach ($tokens as $t) {
            $actual[] = [$t->opcode, $t->value, $t->line, $t->char];
        }

        $this->assertSame([
            [TokenType::T_NAMESPACE, null, 1, 10],
            [TokenType::T_IDENTIFIER, 'Acme', 1, 15],
            [TokenType::T_DOTCOMMA, null, 1, 16],
            [TokenType::T_CLASS, null, 1, 22],
            [TokenType::T_IDENTIFIER, 'Foo', 1, 26],
            [TokenType::T_BRACKET_OPEN, null, 1, 28],
            [TokenType::T_BRACKET_CLOSE, null, 1, 29],
            [0, null, 1, 29],
        ], $actual);
    }

    public function testActiveCharResetsToZeroOnNewline(): void
    {
        // Second line starts at active_char 0, so a 6-char keyword ends at 6.
        $tokens = (new Lexer("a\npublic"))->tokenize();

        $this->assertSame(TokenType::T_IDENTIFIER, $tokens[0]->opcode);
        $this->assertSame(1, $tokens[0]->line);
        $this->assertSame(TokenType::T_PUBLIC, $tokens[1]->opcode);
        $this->assertSame(2, $tokens[1]->line);
        $this->assertSame(6, $tokens[1]->char);
    }

    public function testNegativeLiteralVsSubtraction(): void
    {
        // After a value (identifier), `-1` glued to a digit is binary SUB.
        $glued = (new Lexer('len-1'))->tokenize();
        $this->assertSame(TokenType::T_IDENTIFIER, $glued[0]->opcode);
        $this->assertSame(TokenType::T_SUB, $glued[1]->opcode);
        $this->assertSame(TokenType::T_INTEGER, $glued[2]->opcode);
        $this->assertSame('1', $glued[2]->value);

        // After a non-value (assign), `-5` is a single negative literal.
        $neg = (new Lexer('a = -5'))->tokenize();
        $this->assertSame(TokenType::T_ASSIGN, $neg[1]->opcode);
        $this->assertSame(TokenType::T_INTEGER, $neg[2]->opcode);
        $this->assertSame('-5', $neg[2]->value);
    }

    public function testKeywordsAreCaseInsensitive(): void
    {
        $tokens = (new Lexer('CLASS NULL Foo'))->tokenize();
        $this->assertSame(TokenType::T_CLASS, $tokens[0]->opcode);
        $this->assertSame(TokenType::T_NULL, $tokens[1]->opcode);
        // Mixed-case non-keyword stays an identifier.
        $this->assertSame(TokenType::T_IDENTIFIER, $tokens[2]->opcode);
        $this->assertSame('Foo', $tokens[2]->value);
    }

    public function testConstantVsIdentifierClassification(): void
    {
        $tokens = (new Lexer('FOO bar _GET ____'))->tokenize();
        $this->assertSame(TokenType::T_CONSTANT, $tokens[0]->opcode);
        $this->assertSame(TokenType::T_IDENTIFIER, $tokens[1]->opcode);
        // Superglobal forced to identifier despite being all-caps.
        $this->assertSame(TokenType::T_IDENTIFIER, $tokens[2]->opcode);
        // All-underscore forced to identifier.
        $this->assertSame(TokenType::T_IDENTIFIER, $tokens[3]->opcode);
    }

    public function testStringStoresRawInnerTextWithoutDecodingEscapes(): void
    {
        $tokens = (new Lexer('"a\\"b"'))->tokenize();
        $this->assertSame(TokenType::T_STRING, $tokens[0]->opcode);
        $this->assertSame('a\\"b', $tokens[0]->value);
    }

    public function testDocblockValueAndComment(): void
    {
        $tokens = (new Lexer("/** hi */"))->tokenize();
        $this->assertSame(TokenType::T_COMMENT, $tokens[0]->opcode);
        // Leading '/' and trailing '/' dropped; inner kept verbatim.
        $this->assertSame('** hi *', $tokens[0]->value);

        // Plain C comments are discarded (IGNORE), leaving only EOF.
        $cComment = (new Lexer('/* x */'))->tokenize();
        $this->assertSame(0, $cComment[0]->opcode);
    }

    public function testDollarPrefixStrippedFromIdentifier(): void
    {
        $tokens = (new Lexer('$foo'))->tokenize();
        $this->assertSame(TokenType::T_IDENTIFIER, $tokens[0]->opcode);
        $this->assertSame('foo', $tokens[0]->value);
    }
}
