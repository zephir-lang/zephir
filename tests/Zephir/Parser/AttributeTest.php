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

/**
 * Pure-PHP parser support for PHP attributes (issue #2466).
 *
 * The positive IR shapes are asserted byte-for-byte against the C extension by
 * PhpParserParityTest over the `phpt__*_attributes_*` golden fixtures. What
 * cannot live there is everything whose source is not a heredoc in a .phpt:
 * generate-golden.php only lifts `$var = <<<LABEL ... LABEL;` snippets, so the
 * rejection and the "must not be recognised here" cases have no golden. They
 * are covered here instead, with the expectations taken from what the C
 * extension actually produces.
 */
final class AttributeTest extends TestCase
{
    private function parse(string $code): array
    {
        return (new PhpParser())->parse($code, '(eval code)');
    }

    /** @return list<array{0: string, 1: int, 2: int}> */
    public static function rejectedProvider(): array
    {
        return [
            'empty group'     => ["#[]\nclass A {}", 1, 4],
            'lone comma'      => ["#[,]\nclass A {}", 1, 4],
            'unclosed parens' => ["#[A(\nclass A {}", 2, 5],
            'unclosed group'  => ["#[A\nclass A {}", 2, 5],
            // A docblock is its own top-level statement, so it cannot sit
            // between the attributes and the declaration they belong to.
            'docblock after'  => ["#[A]\n/**\n * d\n */\nclass A {}", 4, 2],
            'inside a body'   => ['class A { public function m() { #[X] let y = 1; } }', 1, 35],
        ];
    }

    /**
     * @dataProvider rejectedProvider
     */
    public function testMalformedAttributeIsASyntaxError(string $code, int $line, int $char): void
    {
        $ir = $this->parse($code);

        $this->assertSame(
            ['type' => 'error', 'message' => 'Syntax error', 'file' => '(eval code)', 'line' => $line, 'char' => $char],
            $ir
        );
    }

    /**
     * A bare `#` is not an attribute opener and keeps falling through to the
     * scanner's catch-all error, as it did before `#[` existed.
     */
    public function testBareHashIsAScannerError(): void
    {
        $ir = $this->parse('class A {} #');

        $this->assertSame('error', $ir['type']);
        $this->assertStringStartsWith('Scanner error: -2', $ir['message']);
    }

    /** @return list<array{0: string, 1: list<string>}> */
    public static function notRecognisedProvider(): array
    {
        return [
            'in a string'        => ['class A { public function m() { let s = "x #[not] y"; } }', ['class']],
            'in a line comment'  => ["// #[nope]\nclass A {}", ['class']],
            'in a docblock'      => ["/**\n * #[nope]\n */\nclass A {}", ['comment', 'class']],
            'in a block comment' => ["/* #[nope] */\nclass A {}", ['class']],
            'in a cblock'        => ["%{ #[nope] }%\nclass A {}", ['cblock', 'class']],
        ];
    }

    /**
     * `#[` inside a literal is part of that literal, never a token. Both
     * backends rely on the whole literal being matched in one go.
     *
     * @dataProvider notRecognisedProvider
     *
     * @param list<string> $expected
     */
    public function testAttributeOpenerInsideALiteralIsNotAToken(string $code, array $expected): void
    {
        $ir = $this->parse($code);

        $this->assertSame($expected, array_column($ir, 'type'));
    }

    public function testConsecutiveAndGroupedAttributesFlattenIdentically(): void
    {
        $stacked = $this->parse("#[A]\n#[B(1)]\nclass X {}")[0]['attributes'];
        $grouped = $this->parse("#[A, B(1)]\nclass X {}")[0]['attributes'];

        $this->assertSame(['A', 'B'], array_column($stacked, 'name'));
        $this->assertSame(['A', 'B'], array_column($grouped, 'name'));
    }

    public function testDeclarationWithoutAttributesCarriesNoAttributesKey(): void
    {
        $ir = $this->parse("class X {\n\tpublic p = 1;\n}");

        $this->assertArrayNotHasKey('attributes', $ir[0]);
        $this->assertArrayNotHasKey('attributes', $ir[0]['definition']['properties'][0]);
    }

    /**
     * A keyword token carries no text, so an attribute named after one is
     * recorded under its canonical spelling. PHP resolves class names
     * case-insensitively, so the normalization is lossless.
     */
    public function testKeywordAttributeNameNormalizesToItsCanonicalSpelling(): void
    {
        foreach (['#[Deprecated]', '#[DEPRECATED]', '#[deprecated]'] as $written) {
            $ir = $this->parse($written . "\nclass X {}");

            $this->assertSame('Deprecated', $ir[0]['attributes'][0]['name'], $written);
        }
    }

    /**
     * `float` shares its token with `double`, so it is deliberately absent from
     * the keyword list: accepting it would record `#[Float]` as `#[Double]`.
     */
    public function testAmbiguousKeywordAttributeNameIsRejectedRatherThanRenamed(): void
    {
        $this->assertSame('error', $this->parse("#[Float]\nclass X {}")['type']);
    }

    public function testAttributeArgumentsCarryNamedAndPositionalForms(): void
    {
        $arguments = $this->parse("#[A(1, key: \"v\")]\nclass X {}")[0]['attributes'][0]['arguments'];

        $this->assertArrayNotHasKey('name', $arguments[0]);
        $this->assertSame('1', $arguments[0]['parameter']['value']);
        $this->assertSame('key', $arguments[1]['name']);
        $this->assertSame('v', $arguments[1]['parameter']['value']);
    }

    /**
     * The parameter list is shared, so one wrapper covers methods, interface
     * methods, free functions and closures; and an attribute on a variadic is
     * appended after the `variadic` flag.
     */
    public function testParameterAttributesAttachAtTheDeclaredOffset(): void
    {
        $ir = $this->parse("class X {\n\tpublic function m(a, #[S] b, #[C] ...rest) {}\n}");

        $parameters = $ir[0]['definition']['methods'][0]['parameters'];

        $this->assertArrayNotHasKey('attributes', $parameters[0]);
        $this->assertSame('S', $parameters[1]['attributes'][0]['name']);
        $this->assertSame(
            ['type', 'name', 'const', 'data-type', 'mandatory', 'reference', 'file', 'line', 'char', 'variadic', 'attributes'],
            array_keys($parameters[2])
        );
    }
}
