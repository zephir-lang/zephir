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

namespace Zephir\Test\Stubs;

use PHPUnit\Framework\TestCase;
use Zephir\Stubs\PhpStanTagValidator;

final class PhpStanTagValidatorTest extends TestCase
{
    public function typeBearingTagProvider(): array
    {
        return [
            'phpstan-import-type' => ['phpstan-import-type'],
            'phpstan-param'       => ['phpstan-param'],
            'phpstan-return'      => ['phpstan-return'],
            'phpstan-type'        => ['phpstan-type'],
            'phpstan-var'         => ['phpstan-var'],
            'psalm-param'         => ['psalm-param'],
            'psalm-return'        => ['psalm-return'],
            'psalm-type'          => ['psalm-type'],
            'psalm-var'           => ['psalm-var'],
        ];
    }

    /**
     * @dataProvider typeBearingTagProvider
     */
    public function testIsTypeBearingTagAcceptsWhitelisted(string $tag): void
    {
        $this->assertTrue(PhpStanTagValidator::isTypeBearingTag($tag));
    }

    public function nonTypeBearingTagProvider(): array
    {
        return [
            'param'              => ['param'],
            'return'             => ['return'],
            'var'                => ['var'],
            'throws'             => ['throws'],
            'template'           => ['template'],
            'template-covariant' => ['template-covariant'],
            'extends'            => ['extends'],
            'implements'         => ['implements'],
            'phpstan-template'   => ['phpstan-template'],
            'psalm-template'     => ['psalm-template'],
            'phpstan-assert'     => ['phpstan-assert'],
            'phan-return'        => ['phan-return'],
            'empty string'       => [''],
            'gibberish'          => ['totally-made-up-tag'],
        ];
    }

    /**
     * @dataProvider nonTypeBearingTagProvider
     */
    public function testIsTypeBearingTagRejectsOthers(string $tag): void
    {
        $this->assertFalse(PhpStanTagValidator::isTypeBearingTag($tag));
    }

    public function testPhpStanPhpDocParserIsLoadable(): void
    {
        $this->assertTrue(
            class_exists(\PHPStan\PhpDocParser\Parser\TypeParser::class),
            'phpstan/phpdoc-parser dependency must be installed and autoloadable.'
        );
    }

    public function validReturnLikeProvider(): array
    {
        return [
            'phpstan-return simple'       => ['phpstan-return', 'int'],
            'phpstan-return class'        => ['phpstan-return', '\Foo\Bar'],
            'phpstan-return union'        => ['phpstan-return', 'int|string|null'],
            'phpstan-return generic'      => ['phpstan-return', 'array<int, string>'],
            'phpstan-return shape'        => ['phpstan-return', 'array{key: int, other?: string}'],
            'phpstan-return nested'       => ['phpstan-return', 'array<int, array<string, Foo>>'],
            'phpstan-return special'      => ['phpstan-return', 'non-empty-list<int>'],
            'phpstan-return class-string' => ['phpstan-return', 'class-string<\Throwable>'],
            'phpstan-return positive-int' => ['phpstan-return', 'positive-int'],
            'phpstan-return static-null'  => ['phpstan-return', 'static|null'],
            'psalm-return generic'        => ['psalm-return', 'array<int, string>'],
            'phpstan-var simple'          => ['phpstan-var', 'int'],
            'phpstan-var generic'         => ['phpstan-var', 'array<string, mixed>'],
            'psalm-var non-empty-string'  => ['psalm-var', 'non-empty-string'],
            'phpstan-type basic'          => ['phpstan-type', 'FindParams array{conditions?: string, limit?: int}'],
            'phpstan-type simple-alias'   => ['phpstan-type', 'UserId int'],
            'psalm-type generic'          => ['psalm-type', 'List array<int, string>'],
        ];
    }

    /**
     * @dataProvider validReturnLikeProvider
     */
    public function testValidateTagValueAcceptsValid(string $tagName, string $value): void
    {
        $validator = new PhpStanTagValidator();
        $this->assertNull($validator->validateTagValue($tagName, $value));
    }

    public function invalidReturnLikeProvider(): array
    {
        return [
            'missing comma'           => ['phpstan-return', 'array<int string>'],
            'unbalanced angle'        => ['phpstan-return', 'array<int, string'],
            'unbalanced brace'        => ['phpstan-return', 'array{key: int'],
            'stray comma'             => ['phpstan-return', 'array<,int>'],
            'bad shape key'           => ['phpstan-return', 'array{: int}'],
            'trailing garbage'        => ['phpstan-return', 'int garbage stuff'],
            'phpstan-var missing >'   => ['phpstan-var',    'array<int, string'],
            'phpstan-type no ident'   => ['phpstan-type',   'array<int, string>'],
            'phpstan-type no type'    => ['phpstan-type',   'AliasName'],
            'empty value'             => ['phpstan-return', ''],
            'whitespace only'         => ['phpstan-return', '   '],
        ];
    }

    /**
     * @dataProvider invalidReturnLikeProvider
     */
    public function testValidateTagValueRejectsInvalid(string $tagName, string $value): void
    {
        $validator = new PhpStanTagValidator();
        $error     = $validator->validateTagValue($tagName, $value);

        $this->assertInstanceOf(\Zephir\Stubs\ValidationError::class, $error);
        $this->assertNotSame('', $error->message);
    }

    public function validParamProvider(): array
    {
        return [
            'simple'                   => ['phpstan-param', 'int $count'],
            'generic with description' => ['phpstan-param', 'array<int, string> $items - the items to process'],
            'shape'                    => ['phpstan-param', 'array{conditions?: string, limit?: int} $params'],
            'union'                    => ['phpstan-param', 'int|string $key'],
            'class type'               => ['phpstan-param', '\Foo\Bar $obj'],
            'psalm-param'              => ['psalm-param', 'positive-int $count'],
        ];
    }

    /**
     * @dataProvider validParamProvider
     */
    public function testValidateTagValueAcceptsValidParam(string $tagName, string $value): void
    {
        $validator = new PhpStanTagValidator();
        $this->assertNull($validator->validateTagValue($tagName, $value));
    }

    public function invalidParamProvider(): array
    {
        return [
            'missing $'      => ['phpstan-param', 'array<int, string> name'],
            'no name'        => ['phpstan-param', 'int'],
            'malformed type' => ['phpstan-param', 'array<int string> $foo'],
            'just dollar'    => ['phpstan-param', 'int $'],
        ];
    }

    /**
     * @dataProvider invalidParamProvider
     */
    public function testValidateTagValueRejectsInvalidParam(string $tagName, string $value): void
    {
        $validator = new PhpStanTagValidator();
        $error     = $validator->validateTagValue($tagName, $value);

        $this->assertInstanceOf(\Zephir\Stubs\ValidationError::class, $error);
        $this->assertNotSame('', $error->message);
    }

    public function validImportTypeProvider(): array
    {
        return [
            'basic'    => ['phpstan-import-type', 'FindParams from \Foo\Repo'],
            'aliased'  => ['phpstan-import-type', 'FindParams from \Foo\Repo as LocalName'],
            'short'    => ['phpstan-import-type', 'Params from Foo'],
        ];
    }

    /**
     * @dataProvider validImportTypeProvider
     */
    public function testValidateTagValueAcceptsValidImportType(string $tagName, string $value): void
    {
        $validator = new PhpStanTagValidator();
        $this->assertNull($validator->validateTagValue($tagName, $value));
    }

    public function invalidImportTypeProvider(): array
    {
        return [
            'missing from'  => ['phpstan-import-type', 'FindParams \Foo\Repo'],
            'wrong keyword' => ['phpstan-import-type', 'FindParams of \Foo\Repo'],
            'no source'     => ['phpstan-import-type', 'FindParams from'],
            'just ident'    => ['phpstan-import-type', 'FindParams'],
        ];
    }

    /**
     * @dataProvider invalidImportTypeProvider
     */
    public function testValidateTagValueRejectsInvalidImportType(string $tagName, string $value): void
    {
        $validator = new PhpStanTagValidator();
        $error     = $validator->validateTagValue($tagName, $value);

        $this->assertInstanceOf(\Zephir\Stubs\ValidationError::class, $error);
        $this->assertNotSame('', $error->message);
    }
}
