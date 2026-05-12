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

namespace Zephir\Stubs;

use PHPStan\PhpDocParser\Lexer\Lexer;
use PHPStan\PhpDocParser\Parser\ConstExprParser;
use PHPStan\PhpDocParser\Parser\ParserException;
use PHPStan\PhpDocParser\Parser\TokenIterator;
use PHPStan\PhpDocParser\Parser\TypeParser;
use PHPStan\PhpDocParser\ParserConfig;

use function class_exists;
use function in_array;
use function str_ends_with;
use function trim;

/**
 * Validates the value portion of type-bearing PHPStan/Psalm docblock tags
 * (e.g. @phpstan-return, @phpstan-param). Delegates parsing to
 * phpstan/phpdoc-parser so the accepted grammar matches PHPStan exactly.
 *
 * Tags listed in TYPE_BEARING_TAGS are validated. Other tags (templates,
 * extends/implements, plain @param/@return, etc.) are out of scope for
 * Phase 2 and continue to pass through verbatim via Phase 1's TagRegistry.
 */
final class PhpStanTagValidator
{
    /**
     * Tag names (without leading @) whose value is a type expression.
     */
    public const TYPE_BEARING_TAGS = [
        'phpstan-import-type',
        'phpstan-param',
        'phpstan-return',
        'phpstan-type',
        'phpstan-var',
        'psalm-param',
        'psalm-return',
        'psalm-type',
        'psalm-var',
    ];

    private Lexer $lexer;

    private TypeParser $typeParser;

    public function __construct(?Lexer $lexer = null, ?TypeParser $typeParser = null)
    {
        if ($lexer !== null && $typeParser !== null) {
            $this->lexer      = $lexer;
            $this->typeParser = $typeParser;
            return;
        }

        // phpstan/phpdoc-parser >=2.0 uses ParserConfig; <2.0 uses no-arg ctors.
        // In 2.x, TypeParser requires a ConstExprParser as its second argument.
        if (class_exists(ParserConfig::class)) {
            $config           = new ParserConfig([]);
            $this->lexer      = $lexer ?? new Lexer($config);
            $this->typeParser = $typeParser ?? new TypeParser($config, new ConstExprParser($config));
        } else {
            $this->lexer      = $lexer ?? new Lexer();
            $this->typeParser = $typeParser ?? new TypeParser();
        }
    }

    /**
     * @param string $tagName Tag name without the leading @.
     *
     * @return bool
     */
    public static function isTypeBearingTag(string $tagName): bool
    {
        return in_array($tagName, self::TYPE_BEARING_TAGS, true);
    }

    /**
     * Validates a tag value (the portion after "@phpstan-return ", etc.).
     *
     * @param string $tagName  Tag name without the leading @.
     * @param string $tagValue The raw value string from the docblock.
     *
     * @return ValidationError|null Null on success; ValidationError on failure.
     */
    public function validateTagValue(string $tagName, string $tagValue): ?ValidationError
    {
        $value = trim($tagValue);

        if ('' === $value) {
            return new ValidationError('Empty type expression', 0);
        }

        try {
            if (
                str_ends_with($tagName, '-return')
                || str_ends_with($tagName, '-var')
            ) {
                $this->parseTypeOnly($value);
                return null;
            }

            if (str_ends_with($tagName, '-type') && !str_ends_with($tagName, '-import-type')) {
                $this->parseTypeAlias($value);
                return null;
            }

            if (str_ends_with($tagName, '-param')) {
                $this->parseParam($value);
                return null;
            }

            if (str_ends_with($tagName, '-import-type')) {
                $this->parseImportType($value);
                return null;
            }

            return null;
        } catch (ParserException $e) {
            return new ValidationError($e->getMessage(), $e->getCurrentOffset());
        }
    }

    private function parseTypeOnly(string $value): void
    {
        $tokens = new TokenIterator($this->lexer->tokenize($value));
        $this->typeParser->parse($tokens);

        if (!$tokens->isCurrentTokenType(Lexer::TOKEN_END)) {
            throw new ParserException(
                $tokens->currentTokenValue(),
                $tokens->currentTokenType(),
                $tokens->currentTokenOffset(),
                Lexer::TOKEN_END,
                null,
                $tokens->currentTokenLine(),
            );
        }
    }

    private function parseTypeAlias(string $value): void
    {
        $tokens = new TokenIterator($this->lexer->tokenize($value));

        // Expect a leading identifier (the alias name).
        if (!$tokens->isCurrentTokenType(Lexer::TOKEN_IDENTIFIER)) {
            throw new ParserException(
                $tokens->currentTokenValue(),
                $tokens->currentTokenType(),
                $tokens->currentTokenOffset(),
                Lexer::TOKEN_IDENTIFIER,
                null,
                $tokens->currentTokenLine(),
            );
        }
        $tokens->next();

        // Then a type expression.
        $this->typeParser->parse($tokens);

        if (!$tokens->isCurrentTokenType(Lexer::TOKEN_END)) {
            throw new ParserException(
                $tokens->currentTokenValue(),
                $tokens->currentTokenType(),
                $tokens->currentTokenOffset(),
                Lexer::TOKEN_END,
                null,
                $tokens->currentTokenLine(),
            );
        }
    }

    private function parseParam(string $value): void
    {
        $tokens = new TokenIterator($this->lexer->tokenize($value));
        $this->typeParser->parse($tokens);

        // Expect a $variable token.
        if (!$tokens->isCurrentTokenType(Lexer::TOKEN_VARIABLE)) {
            throw new ParserException(
                $tokens->currentTokenValue(),
                $tokens->currentTokenType(),
                $tokens->currentTokenOffset(),
                Lexer::TOKEN_VARIABLE,
                null,
                $tokens->currentTokenLine(),
            );
        }
        $tokens->next();

        // Anything after the variable is free-form description; accept it all.
    }

    private function parseImportType(string $value): void
    {
        $tokens = new TokenIterator($this->lexer->tokenize($value));

        // Expect IDENT (alias name).
        if (!$tokens->isCurrentTokenType(Lexer::TOKEN_IDENTIFIER)) {
            throw new ParserException(
                $tokens->currentTokenValue(),
                $tokens->currentTokenType(),
                $tokens->currentTokenOffset(),
                Lexer::TOKEN_IDENTIFIER,
                null,
                $tokens->currentTokenLine(),
            );
        }
        $tokens->next();

        // Expect literal "from".
        if (
            !$tokens->isCurrentTokenType(Lexer::TOKEN_IDENTIFIER)
            || $tokens->currentTokenValue() !== 'from'
        ) {
            throw new ParserException(
                $tokens->currentTokenValue(),
                $tokens->currentTokenType(),
                $tokens->currentTokenOffset(),
                Lexer::TOKEN_IDENTIFIER,
                'from',
                $tokens->currentTokenLine(),
            );
        }
        $tokens->next();

        // Expect a type (the source class).
        $this->typeParser->parse($tokens);

        // Optional "as IDENT".
        if (
            $tokens->isCurrentTokenType(Lexer::TOKEN_IDENTIFIER)
            && $tokens->currentTokenValue() === 'as'
        ) {
            $tokens->next();
            if (!$tokens->isCurrentTokenType(Lexer::TOKEN_IDENTIFIER)) {
                throw new ParserException(
                    $tokens->currentTokenValue(),
                    $tokens->currentTokenType(),
                    $tokens->currentTokenOffset(),
                    Lexer::TOKEN_IDENTIFIER,
                    null,
                    $tokens->currentTokenLine(),
                );
            }
            $tokens->next();
        }

        if (!$tokens->isCurrentTokenType(Lexer::TOKEN_END)) {
            throw new ParserException(
                $tokens->currentTokenValue(),
                $tokens->currentTokenType(),
                $tokens->currentTokenOffset(),
                Lexer::TOKEN_END,
                null,
                $tokens->currentTokenLine(),
            );
        }
    }
}
